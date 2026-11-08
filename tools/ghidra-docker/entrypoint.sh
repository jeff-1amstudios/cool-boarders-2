#!/bin/sh
set -eu
mkdir -p /home/ghidra/config /home/ghidra/cache /home/ghidra/tmp
export JAVA_TOOL_OPTIONS="-Dapplication.settingsdir=/home/ghidra/config -Dapplication.cachedir=/home/ghidra/cache -Dapplication.tempdir=/home/ghidra/tmp -Djava.io.tmpdir=/home/ghidra/tmp -XX:-UsePerfData"
exec /opt/ghidra/support/analyzeHeadless "$@"
