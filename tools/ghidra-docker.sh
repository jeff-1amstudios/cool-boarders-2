#!/bin/sh
set -eu
task_tools=$(CDPATH= cd "$(dirname "$0")" && pwd)
task_root=$(CDPATH= cd "$task_tools/.." && pwd)
task_image=coolboarders2/ghidra-psx:12.1.4-2026.10.03-arm64
task_volume=coolboarders2-ghidra-state-v1
task_mode=${1:-headless}
if [ "$#" -gt 0 ]; then shift; fi
case "$task_mode" in
    build)
        exec docker build --platform linux/arm64 \
            --build-arg "USER_UID=$(id -u)" --build-arg "USER_GID=$(id -g)" \
            -t "$task_image" -f "$task_tools/ghidra-docker/Dockerfile" "$task_tools" "$@"
        ;;
    headless)
        mkdir -p "$task_root/orig/ghidra" "$task_root/reports"
        exec docker run --rm --init --network none \
            --env "GHIDRA_HEADLESS_JAVA_OPTIONS=-Duser.name=$(id -un)" \
            --mount "type=volume,source=$task_volume,target=/home/ghidra" \
            --mount "type=bind,source=$task_root/orig/disc,target=/inputs,readonly" \
            --mount "type=bind,source=$task_root/orig/ghidra,target=/projects" \
            --mount "type=bind,source=$task_root/reports,target=/reports" \
            --mount "type=bind,source=$task_tools/ghidra_scripts,target=/scripts,readonly" \
            "$task_image" "$@"
        ;;
    *) echo 'Usage: tools/ghidra-docker.sh [build | headless <Ghidra arguments...>]' >&2; exit 2 ;;
esac
