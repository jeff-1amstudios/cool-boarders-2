PYTHON := tools/.venv/bin/python

.PHONY: all verify validate split toolchain compiler-matrix boundary-audit target-list overlays overlay-report overlay-validate objdiff-install objdiff-prepare objdiff objdiff-report FORCE
all: verify

verify validate: objdiff-install
	$(PYTHON) tools/validate.py $(if $(IMAGE),--image $(IMAGE)) $(if $(UNIT),--unit $(UNIT))

split:
	$(PYTHON) -m splat split scus_943.58.yaml

toolchain:
	$(PYTHON) tools/install_mips.py

compiler-matrix:
	$(PYTHON) tools/compiler_matrix.py

target-list:
	$(PYTHON) tools/target_queue.py

overlays:
	$(PYTHON) tools/overlays.py analyze

overlay-report:
	$(PYTHON) tools/overlays.py report

# Use OVERLAY=init (or INIT.BIN) for an isolated full-file check.
overlay-validate:
	$(PYTHON) tools/overlay_rebuild.py $(if $(OVERLAY),--overlay $(OVERLAY))

boundary-audit: objdiff-install
	$(PYTHON) tools/boundary_audit.py --prepare-ghidra-export
	./tools/ghidra-docker.sh headless /projects CB2_USA -process SCUS_943.58 -noanalysis -readOnly -scriptPath /scripts -postScript ExportBoundaryEvidence.java /reports/boundary-audit/ghidra -log /home/ghidra/boundary-export.log
	$(PYTHON) tools/reconstruct.py inventory
	$(PYTHON) tools/boundary_audit.py --require-ghidra

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
build/src/overlays/%.o: FORCE
	$(PYTHON) tools/overlay_rebuild.py --object $@

build/src/%.o: FORCE
	$(PYTHON) tools/rebuild.py --object $@

FORCE:
