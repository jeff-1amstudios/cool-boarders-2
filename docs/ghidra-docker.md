# Ghidra in Docker

The container build uses the same checksum-pinned Ghidra 12.1.4 and PSX extension 2026.10.03 as the original local installation. It builds Linux ARM64 native components for the existing Apple Silicon Docker Desktop engine. No Ghidra distribution or JDK is installed on the host by this workflow.

Docker keeps the image, build cache, and named state volume in Docker Desktop's own storage. This removes the unpacked application and downloads from the checkout; it does not eliminate their disk usage. Analysis projects and reports remain ordinary files in the repository so they can be backed up independently.

## Build

From the project root, with Docker Desktop running:

```sh
./tools/ghidra-docker.sh build
```

The image is `coolboarders2/ghidra-psx:12.1.4-2026.10.03-arm64`. The build downloads and checksum-verifies the two official archives *inside Docker*. The Java base image is pinned by digest in the Dockerfile. Ubuntu build/runtime packages come from its package repositories.

The build context permits only the small Docker recipe, entrypoint and release lock file. The disc image, game files, existing Ghidra projects and the rest of the workspace are not copied into the image.

## Run headlessly

```sh
./tools/ghidra-docker.sh headless
```

No arguments prints Ghidra's CLI usage. The existing project can be checked without re-running its full analysis:

```sh
./tools/ghidra-docker.sh headless /projects CB2_USA \
  -process SCUS_943.58 -noanalysis -readOnly \
  -scriptPath /scripts \
  -postScript VerifyPsx.java /reports/ghidra-docker-validation.txt \
  -log /home/ghidra/container-analysis.log
```

To import a fresh boot-only analysis project:

```sh
./tools/ghidra-docker.sh headless /projects CB2_USA_New \
  -import /inputs/files/SCUS_943.58 \
  -scriptPath /scripts \
  -postScript VerifyPsx.java /reports/ghidra-docker-validation.txt \
  -analysisTimeoutPerFile 120 -max-cpu 4
```

Close any other Ghidra process using the same project before opening it in the container. A generated project contains game-derived data and remains under ignored `orig/`.

## Container paths

| Container path | Storage / access |
| --- | --- |
| `/opt/ghidra` | Ghidra and PSX extension in the Docker image |
| `/home/ghidra` | Docker volume `coolboarders2-ghidra-state-v1`; settings and caches |
| `/inputs` | Host `orig/disc/`, read-only |
| `/projects` | Host `orig/ghidra/`, writable |
| `/reports` | Host `reports/`, writable |
| `/scripts` | Host `tools/ghidra_scripts/`, read-only |

Runs are temporary containers (`--rm`), execute as a non-root user matched to the host UID/GID, and use no network. Project and report writes persist through their bind mounts; application state persists in the named volume.

The launcher also supplies the host username through Ghidra's Java options. Ghidra records a project-owner name separately from filesystem UID/GID; preserving that name lets the existing macOS-created project open in the Linux container without changing its ownership metadata.

## GUI scope

This launcher provides headless analysis. Ghidra's [official Docker documentation](https://github.com/NationalSecurityAgency/ghidra/blob/Ghidra_12.1.4_build/docker/README.md) describes GUI operation using X11 forwarding and discourages that setup. A browser desktop using Xvfb/noVNC inside the container is another possible route and would avoid installing XQuartz on the work computer. That desktop is not part of this headless image.

## Remove Docker artifacts

When no longer needed, remove this project's image and, separately, its settings/cache volume:

```sh
docker image rm coolboarders2/ghidra-psx:12.1.4-2026.10.03-arm64
docker volume rm coolboarders2-ghidra-state-v1
```

Volume removal deletes container settings and cache, including any migrated settings backup. The host analysis projects under `orig/ghidra/` and reports are separate and remain intact. Do not use global Docker prune commands for this project; other work may share Docker Desktop.
