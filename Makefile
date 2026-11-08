PYTHON := tools/.venv/bin/python

.PHONY: all verify validate split toolchain compiler-matrix objdiff-install objdiff-prepare objdiff objdiff-report FORCE
all: verify

verify validate: objdiff-install
	$(PYTHON) tools/validate.py

split:
	$(PYTHON) -m splat split scus_943.58.yaml

toolchain:
	$(PYTHON) tools/install_mips.py

compiler-matrix:
	$(PYTHON) tools/compiler_matrix.py

objdiff-install:
	$(PYTHON) tools/install_objdiff.py

objdiff-prepare:
	$(PYTHON) tools/objdiff.py

objdiff: objdiff-report

objdiff-report: objdiff-install objdiff-prepare
	tools/objdiff/bin/objdiff-cli report generate -o reports/objdiff.json
	$(PYTHON) tools/objdiff.py --summary

# Allow batch work on a single object without linking the executable.
# Always rebuild so flags, headers and the shared compiler recipe take effect.
build/src/%.o: FORCE
	$(PYTHON) tools/rebuild.py --object $@

FORCE:
