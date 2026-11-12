"""Qualify an existing private Windows build without rebuilding its binaries."""
import argparse
import base64
import datetime
import hashlib
import io
import json
import lzma
import os
from pathlib import Path
import re
import struct
import subprocess
import tempfile
import time
import zipfile


def require(condition, message):
    if not condition:
        raise ValueError(message)


def gh_json(endpoint):
    return json.loads(subprocess.check_output(['gh', 'api', endpoint]))


def verify_checkout_log(repository, run_id, source_sha):
    jobs = gh_json(f'repos/{repository}/actions/runs/{run_id}/jobs?per_page=100')['jobs']
    release_jobs = [job for job in jobs if job['name'] == 'Release Windows x64' and job['conclusion'] == 'success']
    require(len(release_jobs) == 1, 'Expected one successful Windows release job')
    steps = [step for step in release_jobs[0]['steps'] if step['name'] == 'Clone.' and step['conclusion'] == 'success']
    require(len(steps) == 1, 'Successful source checkout step missing')
    start = datetime.datetime.fromisoformat(steps[0]['started_at'].replace('Z', '+00:00'))
    end = datetime.datetime.fromisoformat(steps[0]['completed_at'].replace('Z', '+00:00')) + datetime.timedelta(seconds=1)
    archive = subprocess.check_output(['gh', 'api', f'repos/{repository}/actions/runs/{run_id}/logs'])
    with zipfile.ZipFile(io.BytesIO(archive)) as logs:
        names = [name for name in logs.namelist() if re.fullmatch(r'\d+_Release Windows x64\.txt', name)]
        require(len(names) == 1, 'Windows release checkout log missing')
        lines = logs.read(names[0]).decode('utf-8-sig', errors='replace').splitlines()
    checked_out = []
    for index, line in enumerate(lines):
        if not re.search(r'\[command\].*git\.exe"? log -1 --format=%H$', line):
            continue
        timestamp = datetime.datetime.fromisoformat(line.split(' ', 1)[0].replace('Z', '+00:00'))
        if start <= timestamp <= end and index + 1 < len(lines):
            match = re.search(r'Z ([0-9a-f]{40})$', lines[index + 1])
            if match:
                checked_out.append(match.group(1))
    require(checked_out == [source_sha], 'Actual build checkout differs from the qualified source SHA')


def verify_signature(key, signature, message, der=False):
    with tempfile.TemporaryDirectory() as directory:
        root = Path(directory)
        for name, data in [('key', key), ('signature', signature), ('message', message)]:
            (root / name).write_bytes(data)
        command = ['openssl', 'pkeyutl', '-verify', '-pubin', '-inkey', str(root / 'key'),
                   '-rawin', '-in', str(root / 'message'), '-sigfile', str(root / 'signature')]
        if der:
            command.extend(['-keyform', 'DER'])
        result = subprocess.run(command, capture_output=True)
        require(result.returncode == 0, 'Invalid Ed25519 release signature')


def verify_package(data, directory, trust):
    offset = 0

    def take(size):
        nonlocal offset
        require(0 <= size <= len(data) - offset, 'Truncated signed package')
        value = data[offset:offset + size]
        offset += size
        return value

    def number(format):
        return struct.unpack(format, take(struct.calcsize(format)))[0]

    def chunk(limit):
        size = number('<I')
        require(0 < size <= limit, 'Invalid signed chunk size')
        return take(size)

    require(take(4) == b'TDUP' and number('<I') == 2, 'Expected TDUP v2')
    require(take(3) == bytes([0, 0, 1]), 'Expected stable Windows x64')
    require(number('<Q') == 3004003 << 32, 'Expected release 0.4.3, counter zero')
    created = number('<Q')
    require(-300 <= time.time() - created < 86400, 'Release package is stale or from the future')
    manifest = chunk(256 * 1024)
    signed_header = data[:offset]
    manifest_signature = chunk(1024)
    require(manifest == (trust / 'manifest.min.json').read_bytes(), 'Unexpected trust manifest')
    verify_signature((trust / 'root-public.pem').read_bytes(), manifest_signature, manifest)
    auth = json.loads(manifest)
    require(auth['format'] == 1 and auth['manifest_version'] == 1, 'Unsupported trust manifest')
    require(auth['issued'] <= created < auth['expires'] and time.time() < auth['expires'], 'Expired trust manifest')
    count = number('<I')
    require(0 < count <= 16, 'Invalid signature count')
    signatures = {}
    for _ in range(count):
        key_id = chunk(256).decode('ascii')
        require(key_id not in signatures, 'Duplicate signing key')
        signatures[key_id] = chunk(1024)
    payload = chunk(256 * 1024 * 1024)
    require(offset == len(data), 'Trailing package data')
    signed = signed_header + hashlib.sha256(payload).digest()
    valid = set()
    for item in auth['keys']:
        if item['id'] not in signatures:
            continue
        require(item['alg'] == 'Ed25519' and item['id'] not in auth['revoked'], 'Revoked or unsupported key')
        require(not item.get('expires') or item['expires'] > time.time(), 'Expired channel key')
        raw = base64.urlsafe_b64decode(item['x'] + '=' * (-len(item['x']) % 4))
        require(len(raw) == 32, 'Invalid channel public key')
        der = bytes.fromhex('302a300506032b6570032100') + raw
        verify_signature(der, signatures[item['id']], signed, der=True)
        valid.add(item['id'])
    require(all(valid.intersection(group) for group in auth['channels']['stable']), 'Stable signature quorum missing')
    require(len(payload) >= 9, 'Truncated LZMA payload')
    property_byte = payload[0]
    lc, remainder = property_byte % 9, property_byte // 9
    lp, pb = remainder % 5, remainder // 5
    dictionary, original_size = struct.unpack('<II', payload[1:9])
    require(lc <= 8 and lp <= 4 and pb <= 4 and dictionary <= 128 * 1024 * 1024, 'Invalid LZMA settings')
    require(0 < original_size < 1024 ** 3, 'Payload exceeds updater budget')
    # LzmaCompress omits the end marker. Supply its authenticated output size
    # using the LZMA-alone header; raw decoding beyond that size can interpret
    # range-coder padding as another symbol. Require complete input consumption.
    decoder = lzma.LZMADecompressor(format=lzma.FORMAT_ALONE, memlimit=192 * 1024 * 1024)
    stream = payload[:5] + struct.pack('<Q', original_size) + payload[9:]
    plain = decoder.decompress(stream, max_length=original_size + 1)
    require(len(plain) == original_size and decoder.eof and not decoder.unused_data,
            'Unpacked payload size or stream boundary mismatch')
    cursor = 0

    def read(size):
        nonlocal cursor
        require(0 <= size <= len(plain) - cursor, 'Truncated updater payload')
        value = plain[cursor:cursor + size]
        cursor += size
        return value

    def be32():
        return struct.unpack('>I', read(4))[0]

    require(be32() == 3004003 and be32() == 2, 'Unexpected updater version or file count')
    files = {}
    for _ in range(2):
        name_size = be32()
        require(0 < name_size < 512 and name_size % 2 == 0, 'Invalid updater file name')
        name = read(name_size).decode('utf-16-be')
        require(name in ('Ansible.exe', 'Updater.exe') and name not in files, 'Unexpected updater file')
        expected, actual = be32(), be32()
        require(expected == actual, 'Updater file size mismatch')
        digest = hashlib.sha256(read(actual)).hexdigest()
        require(digest == hashlib.sha256((directory / name).read_bytes()).hexdigest(), 'Updater file bytes mismatch')
        files[name] = {'sha256': digest, 'bytes': actual}
    require(cursor == len(plain), 'Trailing updater payload data')
    return files


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--run-id', required=True)
    parser.add_argument('--source-sha', required=True)
    parser.add_argument('--workflow-sha')
    parser.add_argument('--artifact-sha256', required=True)
    parser.add_argument('--source', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    require(re.fullmatch(r'[0-9]+', args.run_id), 'Invalid build run')
    require(re.fullmatch(r'[0-9a-f]{40}', args.source_sha), 'An immutable source SHA is required')
    workflow_sha = args.workflow_sha or args.source_sha
    require(re.fullmatch(r'[0-9a-f]{40}', workflow_sha), 'An immutable reviewed workflow SHA is required')
    require(re.fullmatch(r'[0-9a-f]{64}', args.artifact_sha256), 'An independently reviewed artifact digest is required')
    repository = os.environ['GITHUB_REPOSITORY']
    run = gh_json(f'repos/{repository}/actions/runs/{args.run_id}')
    require(run['status'] == 'completed' and run['conclusion'] == 'success', 'Release build is not successful')
    require(run['path'] == '.github/workflows/release_win.yml' and run['event'] == 'workflow_dispatch', 'Unexpected build workflow')
    require(run['head_repository']['full_name'] == repository and run['head_sha'] == workflow_sha, 'Build workflow source mismatch')
    verify_checkout_log(repository, args.run_id, args.source_sha)
    checkout = subprocess.check_output(['git', '-C', str(args.source), 'rev-parse', 'HEAD'], text=True).strip()
    require(checkout == args.source_sha, 'Trust source checkout mismatch')
    version = (args.source / 'Ansible/build/version').read_text()
    require(re.search(r'^AppVersion\s+3004003$', version, re.M)
            and re.search(r'^AppVersionStr\s+0\.4\.3$', version, re.M), 'Source version mismatch')
    listing = gh_json(f'repos/{repository}/actions/runs/{args.run_id}/artifacts?per_page=100')
    artifacts = [a for a in listing['artifacts'] if a['name'] == 'Ansible-release-x64-0.4.3' and not a['expired']]
    require(len(artifacts) == 1, 'Expected one private release artifact')
    artifact = artifacts[0]
    require(artifact['digest'] == 'sha256:' + args.artifact_sha256, 'Artifact API digest mismatch')
    archive = subprocess.check_output(['gh', 'api', f'repos/{repository}/actions/artifacts/{artifact["id"]}/zip'])
    require(len(archive) == artifact['size_in_bytes'] and hashlib.sha256(archive).hexdigest() == args.artifact_sha256,
            'Downloaded artifact digest mismatch')
    expected = {'Ansible.exe', 'Updater.exe', 'ansible-update-win-x64-3004003'}
    args.output.mkdir()
    with zipfile.ZipFile(io.BytesIO(archive)) as bundle:
        require(set(bundle.namelist()) == expected and len(bundle.infolist()) == 3, 'Unexpected artifact contents')
        require(sum(item.file_size for item in bundle.infolist()) < 1024 ** 3, 'Artifact exceeds extraction budget')
        for name in expected:
            (args.output / name).write_bytes(bundle.read(name))
    package = args.output / 'ansible-update-win-x64-3004003'
    files = verify_package(package.read_bytes(), args.output, args.source / 'Ansible/Resources/update')
    portable = args.output / 'ansible-portable-win-x64-0.4.3.zip'
    with zipfile.ZipFile(portable, 'x', compression=zipfile.ZIP_DEFLATED) as bundle:
        for name in sorted(files):
            bundle.write(args.output / name, name)
    proof = {'source': args.source_sha, 'workflowSource': workflow_sha, 'run': args.run_id, 'artifact': artifact['id'],
             'artifactSha256': args.artifact_sha256, 'packageSha256': hashlib.sha256(package.read_bytes()).hexdigest(),
             'actualCheckoutVerified': True, 'rootSignatureValid': True, 'stableSignaturesValid': True, 'payloadFiles': files,
             'guiAnimationsQualified': False, 'published': False}
    (args.output / 'windows-provenance.json').write_text(json.dumps(proof, indent=2) + '\n')
    print(json.dumps(proof))


if __name__ == '__main__':
    main()
