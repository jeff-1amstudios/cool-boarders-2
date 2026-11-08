FROM eclipse-temurin:21-jdk-jammy@sha256:4b1a74fbffbe6b4e7b6f0171a472478282fb463790a51edf10e465d578f22b69 AS build
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential ca-certificates curl python3 unzip zlib1g-dev \
    && rm -rf /var/lib/apt/lists/*
COPY ghidra-install.lock.json /tmp/releases.json
RUN python3 - <<'PY'
import hashlib, json, pathlib, subprocess, urllib.request
root = pathlib.Path('/opt')
for asset in json.loads(pathlib.Path('/tmp/releases.json').read_text())['assets']:
    archive = pathlib.Path('/tmp') / asset['name']
    request = urllib.request.Request(asset['browser_download_url'], headers={'User-Agent': 'coolboarders2-ghidra-docker'})
    digest = hashlib.sha256()
    with urllib.request.urlopen(request, timeout=120) as source, archive.open('wb') as target:
        for block in iter(lambda: source.read(1024*1024), b''):
            target.write(block)
            digest.update(block)
    assert archive.stat().st_size == asset['size']
    assert 'sha256:' + digest.hexdigest() == asset['digest'], 'Archive checksum mismatch'
    if asset['name'].endswith('_ghidra_psx_ldr.zip'):
        destination = root/'ghidra/Ghidra/Extensions'
    else:
        destination = root
    destination.mkdir(parents=True, exist_ok=True)
    subprocess.run(['unzip', '-q', str(archive), '-d', str(destination)], check=True)
    if not asset['name'].endswith('_ghidra_psx_ldr.zip'):
        (root/'ghidra_12.1.4_PUBLIC').rename(root/'ghidra')
    archive.unlink()
PY
WORKDIR /opt/ghidra/support/gradle
RUN ./gradlew --no-daemon --max-workers=4 buildNatives
# Keep built native binaries without carrying compiler object/cache files forward.
RUN python3 - <<'PY'
import pathlib, shutil
for area in ('Ghidra', 'GPL'):
    root = pathlib.Path('/opt/ghidra')/area
    for build in sorted(root.rglob('build'), key=lambda p: len(p.parts), reverse=True):
        if not build.is_dir(): continue
        native = build/'os/linux_arm_64'
        if native.is_dir():
            shutil.copytree(native, build.parent/'os/linux_arm_64', dirs_exist_ok=True)
        shutil.rmtree(build)
PY

FROM eclipse-temurin:21-jdk-jammy@sha256:4b1a74fbffbe6b4e7b6f0171a472478282fb463790a51edf10e465d578f22b69 AS runtime
ARG USER_UID=1000
ARG USER_GID=1000
RUN apt-get update && apt-get install -y --no-install-recommends \
    fontconfig libxrender1 libxtst6 libxi6 \
    && rm -rf /var/lib/apt/lists/* \
    && (getent group "$USER_GID" >/dev/null || groupadd -g "$USER_GID" ghidra) \
    && useradd -m -u "$USER_UID" -g "$USER_GID" -s /bin/bash ghidra
COPY --from=build /opt/ghidra /opt/ghidra
COPY ghidra-docker/entrypoint.sh /usr/local/bin/ghidra-project
RUN chmod +x /usr/local/bin/ghidra-project \
    && chown -R ghidra /opt/ghidra/Ghidra/Extensions/ghidra_psx_ldr
USER ghidra
WORKDIR /projects
ENV GHIDRA_HEADLESS_MAXMEM=2G
ENTRYPOINT ["/usr/local/bin/ghidra-project"]
