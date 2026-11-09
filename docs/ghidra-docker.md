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

## Interactive browser desktop

Build the optional desktop image once, then start it:

```sh
./tools/ghidra.sh web-build
./tools/ghidra.sh web
```

Open [Ghidra in your browser](http://127.0.0.1:6080/vnc.html?autoconnect=true&resize=scale). This streams the normal Ghidra desktop through [noVNC](https://novnc.com/noVNC/), with Xvfb, Openbox and x11vnc running inside Docker. No host Java, XQuartz or other host packages are required. The browser image extends the existing headless image; build that first with `./tools/ghidra.sh build` if it is missing. Desktop packages come from Ubuntu's repositories, as with the base image's runtime packages.

On first GUI launch, Ghidra displays its User Agreement. Review and accept it yourself in the browser to continue; this choice persists in the settings volume. Dismiss any welcome/tips windows that appear afterward.

The default project is `/projects/CB2_USA.gpr`. Double-click `SCUS_943.58` in the project window to open the CodeBrowser, listing and decompiler. To choose another project or port:

```sh
GHIDRA_WEB_PORT=6081 ./tools/ghidra.sh web /projects/Other.gpr
```

The container runs in the background, so closing the browser tab leaves Ghidra running. Reopen the same URL to reconnect. Only one browser container can run at a time. Inspect startup errors with:

```sh
./tools/ghidra.sh web-logs
```

Save and close Ghidra through its File menu when finished; the container then exits automatically. To stop a remaining session:

```sh
./tools/ghidra.sh web-stop
```

This opens the real project with write access. Close the interactive session before running headless analysis on the same project, and close headless processes before starting the GUI. Do not bypass Ghidra's project lock. The browser desktop is for interactive analysis; matching validation continues to use the headless workflow and full-executable byte comparison.

The HTTP endpoint binds to `127.0.0.1` only and has no password; other users/processes on this computer can access it. Raw VNC listens only on the container's loopback interface and is not published. Unlike headless runs, the desktop uses Docker's default bridge network so the browser can reach the published port. The project, input, report and settings mounts are the same as headless mode. The desktop image is `coolboarders2/ghidra-psx-web:12.1.4-2026.10.03-arm64`; its container is `coolboarders2-ghidra-web`.

## Remove Docker artifacts

When no longer needed, remove this project's image and, separately, its settings/cache volume:

```sh
docker image rm coolboarders2/ghidra-psx-web:12.1.4-2026.10.03-arm64
docker image rm coolboarders2/ghidra-psx:12.1.4-2026.10.03-arm64
docker volume rm coolboarders2-ghidra-state-v1
```

Volume removal deletes container settings and cache, including any migrated settings backup. The host analysis projects under `orig/ghidra/` and reports are separate and remain intact. Do not use global Docker prune commands for this project; other work may share Docker Desktop.
