PYTHON := tools/.venv/bin/python

.PHONY: all verify split toolchain compiler-matrix
all: verify

verify:
	$(PYTHON) tools/rebuild.py

split:
	$(PYTHON) -m splat split scus_943.58.yaml

toolchain:
	$(PYTHON) tools/install_mips.py

compiler-matrix:
	$(PYTHON) tools/compiler_matrix.py
