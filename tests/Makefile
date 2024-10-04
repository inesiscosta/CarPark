# Copyright (C) 2021, Pedro Reis dos Santos
.SUFFIXES: .in .out .diff
MAKEFLAGS += --no-print-directory # No entering and leaving messages
SHELL := /bin/bash # Execute command with bash
OK="\e[1;32mtest $< PASSED\e[0m"
KO="\e[1;31mtest $< FAILED\e[0m"
EXE=../proj1
LOG=tests.log

all:: clean # run regression tests
	@rm -f $(LOG)
	@for i in `ls test*.in | sed -e "s/in/diff/"`; do $(MAKE) $(MFLAGS) $$i; done
	@echo "`wc -l < $(LOG)` tests passed"

.in.diff:
	@-$(EXE) < $< | diff - $*.out > $@
#	@-(ulimit -d 780 -t 1 && $(EXE) < $<) | diff - $*.out > $@
	@if [ `wc -l < $@` -eq 0 ]; then echo -e $(OK); echo $* >> $(LOG); else echo -e $(KO); fi;

.in.out:
	$(EXE) < $< > $@

out::
	@for i in `ls test*.in | sed -e "s/in/out/"`; do $(MAKE) $(MFLAGS) $$i; done

#tests:: cleanall
#	python3 mktest.py tests.txt
#	$(MAKE) $(MFLAGS) out
#	$(MAKE) $(MFLAGS)

clean::
	@rm -f *.diff $(LOG)

cleanall:: clean
	@rm -f *.out *.in
