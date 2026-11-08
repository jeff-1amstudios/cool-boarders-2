#!/bin/sh
set -eu
cd "$(dirname "$0")/.."
mkdir -p tools/tmp tools/cache
export TMPDIR="$PWD/tools/tmp"
export PIP_CACHE_DIR="$PWD/tools/cache/pip"
export PIP_DISABLE_PIP_VERSION_CHECK=1
export PYTHONPYCACHEPREFIX="$PWD/tools/cache/pycache"
task_python="${PSX_PYTHON:-python3}"
"$task_python" -c 'import sys; assert sys.version_info >= (3,10), "Python 3.10+ required; set PSX_PYTHON"'
if [ ! -d tools/.venv ]; then
    "$task_python" -m venv tools/.venv
fi
tools/.venv/bin/python -m pip install --no-cache-dir -r tools/requirements.lock
task_mkpsxiso_commit=a6b11ea86e67c189137ac50a4066044b3fdb0525
if [ ! -d tools/vendor/mkpsxiso/.git ]; then
    mkdir -p tools/vendor
    git clone --no-checkout https://github.com/Lameguy64/mkpsxiso.git tools/vendor/mkpsxiso
    git -C tools/vendor/mkpsxiso checkout "$task_mkpsxiso_commit"
fi
test "$(git -C tools/vendor/mkpsxiso rev-parse HEAD)" = "$task_mkpsxiso_commit" || {
    echo 'mkpsxiso revision differs from the project pin; inspect it before proceeding.' >&2
    exit 1
}
git -C tools/vendor/mkpsxiso submodule update --init --recursive
cmake -S tools/vendor/mkpsxiso -B tools/build/mkpsxiso \
    -DCMAKE_BUILD_TYPE=Release -DMKPSXISO_NO_LIBFLAC=1
cmake --build tools/build/mkpsxiso -j 4
tools/.venv/bin/python -m splat --version
tools/build/mkpsxiso/dumpsxiso --help
