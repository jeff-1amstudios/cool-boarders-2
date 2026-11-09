#!/bin/sh
set -eu
mkdir -p /home/ghidra/config /home/ghidra/cache /home/ghidra/tmp
export JAVA_TOOL_OPTIONS="-Dapplication.settingsdir=/home/ghidra/config -Dapplication.cachedir=/home/ghidra/cache -Dapplication.tempdir=/home/ghidra/tmp -Djava.io.tmpdir=/home/ghidra/tmp -XX:-UsePerfData"
if [ "${GHIDRA_GUI:-0}" = 1 ]; then
    exec /opt/ghidra/support/launch.sh fg jdk Ghidra "${GHIDRA_GUI_MAXMEM:-2G}" \
        "${GHIDRA_GUI_JAVA_OPTIONS:-}" ghidra.GhidraRun "$@"
fi
exec /opt/ghidra/support/analyzeHeadless "$@"
