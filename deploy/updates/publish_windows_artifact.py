"""Publish a qualified Windows artifact before advertising its update feed."""
import argparse
import copy
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import time
import urllib.request


def command(arguments):
    return subprocess.check_output(arguments)


def api_optional(endpoint):
    result = subprocess.run(['gh', 'api', endpoint], capture_output=True)
    if result.returncode == 0:
        return json.loads(result.stdout)
    if b'(HTTP 404)' in result.stderr:
        return None
    raise RuntimeError('GitHub release preflight failed')


def fetch(path):
    request = urllib.request.Request('https://ansible.su/ad/' + path, headers={'Cache-Control': 'no-cache'})
    with urllib.request.urlopen(request, timeout=60) as response:
        return response.read(), dict(response.headers.items())


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--source-sha', required=True)
    parser.add_argument('--run-id', required=True)
    parser.add_argument('--directory', required=True, type=Path)
    args = parser.parse_args()
    if not re.fullmatch(r'[0-9a-f]{40}', args.source_sha) or not args.run_id.isdigit():
        raise ValueError('Invalid immutable build reference')
    directory = args.directory.resolve()
    proof = json.loads((directory / 'windows-provenance.json').read_text())
    if proof['source'] != args.source_sha or proof['run'] != args.run_id:
        raise ValueError('Prepared artifact source mismatch')
    package_name = 'ansible-update-win-x64-3004003'
    package = directory / package_name
    if hashlib.sha256(package.read_bytes()).hexdigest() != proof['packageSha256']:
        raise ValueError('Prepared package changed')
    for name in ('AWS_ACCESS_KEY_ID', 'AWS_SECRET_ACCESS_KEY', 'S3_ENDPOINT', 'S3_BUCKET'):
        if not os.environ.get(name):
            raise ValueError('Required publication credential or destination is unavailable: ' + name)
    repository = os.environ['GITHUB_REPOSITORY']
    api_root = 'repos/' + repository
    release_path = api_root + '/releases/tags/v0.4.3'
    if api_optional(release_path) is not None:
        raise ValueError('Release 0.4.3 already exists; review its state before replacing anything')
    tag = api_optional(api_root + '/git/ref/tags/v0.4.3')
    if tag is not None and (tag['object']['sha'] != args.source_sha or tag['object']['type'] != 'commit'):
        raise ValueError('Release tag does not point to the qualified source commit')
    previous_bytes, _ = fetch('current6')
    previous = json.loads(previous_bytes)
    if json.loads(fetch('current2')[0]) != previous:
        raise ValueError('Existing platform feeds disagree')
    if previous.get('win64', {}).get('stable', {}).get('released', 0) >= 3004003:
        raise ValueError('Refusing to replace an advertised release')
    intended = copy.deepcopy(previous)
    intended.setdefault('win64', {})['stable'] = {'released': 3004003, 'link': '/win64/' + package_name}
    (directory / 'previous-current.json').write_bytes(previous_bytes)
    current = directory / 'intended-current.json'
    current.write_text(json.dumps(intended, indent=1, sort_keys=True) + '\n')
    bucket = os.environ['S3_BUCKET']
    endpoint = os.environ['S3_ENDPOINT']
    destination = f's3://{bucket}/win64/{package_name}'
    existing = subprocess.run(['aws', 's3api', 'head-object', '--bucket', bucket,
        '--key', 'win64/' + package_name, '--endpoint-url', endpoint], capture_output=True)
    if existing.returncode == 0 or b'(404)' not in existing.stderr:
        raise ValueError('Package exists or its absence cannot be verified; refusing to overwrite')
    command(['aws', 's3', 'cp', str(package), destination, '--endpoint-url', endpoint,
             '--cache-control', 'public, max-age=31536000, immutable', '--only-show-errors'])
    downloaded, _ = fetch('win64/' + package_name)
    if hashlib.sha256(downloaded).hexdigest() != proof['packageSha256']:
        raise ValueError('Public package digest mismatch; update feed remains unchanged')
    request = urllib.request.Request('https://ansible.su/ad/win64/' + package_name, headers={'Range': 'bytes=0-31'})
    with urllib.request.urlopen(request, timeout=60) as response:
        if response.status != 206 or response.headers.get('Content-Range') != f'bytes 0-31/{package.stat().st_size}' or response.read() != package.read_bytes()[:32]:
            raise ValueError('Public package range request failed; feed remains unchanged')
    if tag is None:
        command(['gh', 'api', '--method', 'POST', api_root + '/git/refs', '-f', 'ref=refs/tags/v0.4.3', '-f', 'sha=' + args.source_sha])
    notes = directory / 'windows-release-notes.md'
    notes.write_text('Ansible Desktop 0.4.3 for Windows x64.\n\n'
        'Updates the widget engine and fixes unsafe script compilation and excessive nesting. '
        'Includes rich messages and widgets, message sending and dissolution animations. '
        'Ansible design, translation, voice transcription and bots are preserved.\n\n'
        f'Source: `{args.source_sha}`. Release build: https://github.com/{repository}/actions/runs/{args.run_id}\n')
    command(['gh', 'release', 'create', 'v0.4.3', '--repo', repository, '--verify-tag', '--draft',
             '--title', 'Ansible Desktop 0.4.3', '--notes-file', str(notes)])
    command(['gh', 'release', 'upload', 'v0.4.3', '--repo', repository,
             str(directory / 'ansible-portable-win-x64-0.4.3.zip'), str(package), str(directory / 'windows-provenance.json')])
    if any(json.loads(fetch(name)[0]) != previous for name in ('current2', 'current6')):
        raise ValueError('Another publisher changed the feed; release remains a draft')
    command(['gh', 'release', 'edit', 'v0.4.3', '--repo', repository, '--draft=false'])
    for name in ('current2', 'current6'):
        command(['aws', 's3', 'cp', str(current), f's3://{bucket}/{name}', '--endpoint-url', endpoint,
                 '--content-type', 'application/json', '--cache-control', 'public, max-age=60', '--only-show-errors'])
    for attempt in range(7):
        if all(json.loads(fetch(name)[0]) == intended for name in ('current2', 'current6')):
            print('Published the exact qualified Windows binaries; both feeds verified; other platforms preserved')
            return
        if attempt != 6:
            time.sleep(10)
    raise ValueError('Publication happened but public feed verification is incomplete; inspect saved manifests')


if __name__ == '__main__':
    main()
