#!/bin/sh
# Ghidra now lives in Docker-managed storage, not in this checkout.
set -eu
task_tools=$(CDPATH= cd "$(dirname "$0")" && pwd)
exec "$task_tools/ghidra-docker.sh" "$@"
