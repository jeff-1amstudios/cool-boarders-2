#!/bin/sh
set -eu
task_tools=$(CDPATH= cd "$(dirname "$0")" && pwd)
task_root=$(CDPATH= cd "$task_tools/.." && pwd)
task_image=coolboarders2/ghidra-psx:12.1.4-2026.10.03-arm64
task_volume=coolboarders2-ghidra-state-v1
task_web_image=coolboarders2/ghidra-psx-web:12.1.4-2026.10.03-arm64
task_container=coolboarders2-ghidra-web
task_mode=${1:-headless}
if [ "$#" -gt 0 ]; then shift; fi
case "$task_mode" in
    web-build)
        exec docker build --platform linux/arm64 \
            -t "$task_web_image" -f "$task_tools/ghidra-docker/Dockerfile.web" "$task_tools" "$@"
        ;;
    web)
        task_port=${GHIDRA_WEB_PORT:-6080}
        case "$task_port" in
            ''|*[!0-9]*) echo 'GHIDRA_WEB_PORT must be a port number.' >&2; exit 2 ;;
        esac
        if [ "$task_port" -lt 1 ] || [ "$task_port" -gt 65535 ]; then
            echo 'GHIDRA_WEB_PORT must be between 1 and 65535.' >&2; exit 2
        fi
        if [ "$#" -eq 0 ]; then set -- /projects/CB2_USA.gpr; fi
        mkdir -p "$task_root/orig/ghidra" "$task_root/reports"
        docker run --detach --rm --init --name "$task_container" \
            --publish "127.0.0.1:$task_port:6080" --shm-size 256m \
            --env "GHIDRA_GUI_JAVA_OPTIONS=-Duser.name=$(id -un)" \
            --mount "type=volume,source=$task_volume,target=/home/ghidra" \
            --mount "type=bind,source=$task_root/orig/disc,target=/inputs,readonly" \
            --mount "type=bind,source=$task_root/orig/ghidra,target=/projects" \
            --mount "type=bind,source=$task_root/reports,target=/reports" \
            --mount "type=bind,source=$task_tools/ghidra_scripts,target=/scripts,readonly" \
            "$task_web_image" "$@"
        echo "Ghidra: http://127.0.0.1:$task_port/vnc.html?autoconnect=true&resize=scale"
        echo "Stop: ./tools/ghidra.sh web-stop (close Ghidra first to save your work)"
        ;;
    web-stop)
        exec docker stop --time 30 "$task_container"
        ;;
    web-logs)
        exec docker logs "$task_container" "$@"
        ;;
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
    *) echo 'Usage: tools/ghidra-docker.sh [build | headless <Ghidra arguments...> | web-build | web [project.gpr] | web-stop | web-logs]' >&2; exit 2 ;;
esac
