# Ghidra setup

Ghidra now runs in Docker. The earlier native macOS installation, downloaded archives, and local Gradle/application caches have been removed from this checkout after a successful container decompiler check. About 1.63 GiB of local Ghidra files was removed; Docker Desktop still stores the image and its build cache separately.

Use the [Docker setup guide](ghidra-docker.md) for building, container paths, headless commands and the interactive browser desktop.

```sh
./tools/ghidra.sh headless
```

`tools/ghidra.sh` delegates to `tools/ghidra-docker.sh`. Its default mode is now headless CLI usage. No host Java installation is used by this launcher.

The image contains Ghidra 12.1.4, matching PSX extension 2026.10.03, JDK 21, and Linux ARM64 native components. Their archive checksums remain pinned in `tools/ghidra-install.lock.json`; the Java base image is pinned by digest in the Dockerfile.

The saved analysis project remains at `orig/ghidra/CB2_USA.gpr`. In the container, its project directory is `/projects`. The game input is mounted read-only at `/inputs`, and generated reports are writable at `/reports`.

Container settings and caches live in the Docker volume `coolboarders2-ghidra-state-v1`. The previous macOS settings were preserved there under `legacy-macos-config/` as a backup; they are not loaded as Linux settings automatically.

Validation reopened the existing boot project and decompiled entrypoint `0x800120FC` through the PSX language. Its program database files retained their hashes. Ghidra refreshed some project index/journal housekeeping files while opening the project, even with the program processed read-only. See `reports/ghidra-docker-validation.txt` and `reports/ghidra-docker-migration.json`.

For interactive analysis, build the optional browser image with `./tools/ghidra.sh web-build`, then run `./tools/ghidra.sh web` and open [the desktop](http://127.0.0.1:6080/vnc.html?autoconnect=true&resize=scale). It opens the existing project. Save and close Ghidra before returning to headless analysis on that project. No host X11 application is required.
