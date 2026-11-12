"""Restore only this release's Windows feed entry; retain other platforms."""
import argparse
import copy
import hashlib
import io
import json
import os
from pathlib import Path
import subprocess
import time
import urllib.request
import zipfile


def gh(endpoint):
    return json.loads(subprocess.check_output(['gh', 'api', endpoint]))


def read_feed(name):
    with urllib.request.urlopen('https://ansible.su/ad/' + name, timeout=30) as response:
        return json.load(response)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--publication-run', required=True)
    parser.add_argument('--evidence-sha256', required=True)
    parser.add_argument('--source-sha', required=True)
    parser.add_argument('--directory', type=Path, required=True)
    parser.add_argument('--apply', action='store_true')
    args = parser.parse_args()
    repo = os.environ.get('GITHUB_REPOSITORY', 'ansible-desktop/app-desktop')
    run = gh(f'repos/{repo}/actions/runs/{args.publication_run}')
    assert run['status'] == 'completed' and run['conclusion'] == 'success'
    assert run['path'] == '.github/workflows/release_win.yml'
    assert run['head_repository']['full_name'] == repo
    artifacts = gh(f'repos/{repo}/actions/runs/{args.publication_run}/artifacts')['artifacts']
    selected = [a for a in artifacts if a['name'] == 'windows-publication-evidence-0.4.2' and not a['expired']]
    assert len(selected) == 1
    artifact = selected[0]
    assert artifact['size_in_bytes'] < 1024 * 1024
    assert artifact['digest'] == 'sha256:' + args.evidence_sha256
    raw = subprocess.check_output(['gh', 'api', f'repos/{repo}/actions/artifacts/{artifact["id"]}/zip'])
    assert len(raw) == artifact['size_in_bytes'] and hashlib.sha256(raw).hexdigest() == args.evidence_sha256
    with zipfile.ZipFile(io.BytesIO(raw)) as archive:
        assert set(archive.namelist()) == {'windows-provenance.json', 'previous-current.json', 'intended-current.json'}
        assert sum(f.file_size for f in archive.infolist()) < 1024 * 1024
        proof = json.loads(archive.read('windows-provenance.json'))
        previous = json.loads(archive.read('previous-current.json'))
        intended = json.loads(archive.read('intended-current.json'))
    assert proof['source'] == args.source_sha
    assert intended['win64']['stable'] == {'released': 3004002, 'link': '/win64/ansible-update-win-x64-3004002'}
    assert previous['win64']['stable']['released'] < 3004002
    tag = gh(f'repos/{repo}/git/ref/tags/v0.4.2')
    assert tag['object']['type'] == 'commit' and tag['object']['sha'] == args.source_sha
    feeds = [read_feed(name) for name in ('current2', 'current6')]
    # An interrupted rollback may have restored one feed. No unrelated edit is
    # accepted, and a newer Windows release must never be rolled back by this job.
    assert all(feed.get('win64') in (previous['win64'], intended['win64']) for feed in feeds)
    other = [{k: v for k, v in feed.items() if k != 'win64'} for feed in feeds]
    assert other[0] == other[1], 'Other platform feeds changed; inspect before rollback'
    restored = copy.deepcopy(feeds[0])
    restored['win64'] = previous['win64']
    args.directory.mkdir(parents=True, exist_ok=True)
    output = args.directory / 'rollback-current.json'
    output.write_text(json.dumps(restored, indent=1, sort_keys=True) + '\n')
    evidence = {'publicationRun': args.publication_run, 'source': args.source_sha,
                'previousWindows': previous['win64'], 'withdrawnWindows': intended['win64'],
                'otherPlatformsPreserved': True, 'applied': False}
    (args.directory / 'rollback-before.json').write_text(json.dumps(feeds, indent=2))
    if args.apply:
        for name in ('current2', 'current6'):
            current = read_feed(name)
            assert current in feeds or current == restored, 'Concurrent feed publication changed'
            subprocess.run(['aws', 's3', 'cp', str(output), f's3://{os.environ["S3_BUCKET"]}/{name}',
                            '--endpoint-url', os.environ['S3_ENDPOINT'], '--content-type', 'application/json',
                            '--cache-control', 'public, max-age=60', '--only-show-errors'], check=True)
        for attempt in range(10):
            if all(read_feed(name) == restored for name in ('current2', 'current6')):
                break
            if attempt == 9:
                raise RuntimeError('Rollback wrote feeds; public convergence needs inspection')
            time.sleep(10)
        # Keep the immutable tag and downloadable evidence; remove it from the
        # stable-release listing while the corrected version is being prepared.
        subprocess.run(['gh', 'release', 'edit', 'v0.4.2', '--repo', repo, '--prerelease'], check=True)
        evidence['applied'] = True
    (args.directory / 'rollback-proof.json').write_text(json.dumps(evidence, indent=2))
    print(json.dumps(evidence))


if __name__ == '__main__':
    main()
