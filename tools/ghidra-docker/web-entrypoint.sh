#!/bin/bash
set -euo pipefail
pids=()
cleanup() {
    trap - EXIT
    if ((${#pids[@]})); then
        kill "${pids[@]}" 2>/dev/null || true
        wait "${pids[@]}" 2>/dev/null || true
    fi
}
trap cleanup EXIT
trap 'exit 0' TERM INT

Xvfb "$DISPLAY" -screen 0 1600x1000x24 -nolisten tcp -ac &
pids+=("$!")
for ((attempt=0; attempt<100; attempt++)); do
    if xdpyinfo -display "$DISPLAY" >/dev/null 2>&1; then break; fi
    kill -0 "${pids[0]}"
    sleep 0.1
done
xdpyinfo -display "$DISPLAY" >/dev/null
openbox &
pids+=("$!")
# The raw VNC endpoint is container-local. Only the browser proxy is published.
x11vnc -display "$DISPLAY" -localhost -rfbport 5900 -forever -shared -nopw -xkb &
pids+=("$!")
websockify --web=/usr/share/novnc 6080 localhost:5900 &
pids+=("$!")
/usr/local/bin/ghidra-project "$@" &
pids+=("$!")
# Stop the session if Ghidra closes or a desktop service fails.
wait -n "${pids[@]}"
