#!/bin/make -f
# SPDX-FileType: SOURCE
# SPDX-FileCopyrightText: 2024 Ivan Tatarinov
# SPDX-License-Identifier: GPL-3.0-or-later

include ../../Common.mak

prefix?=/usr/local
exec_prefix?=$(prefix)
bindir?=$(exec_prefix)/bin

ifeq ($(builddir),)
 builddir:=$(realpath .)/build/$(AT2SDK_HOST_SUBDIR)
endif

# Setup GNU C compiler for host

GCCH?=gcc
GCCFLAGSH+=-std=c99 -Wall -Wextra -pedantic -O2

ifeq ($(OS_HOST),Windows)
 GCCFLAGSH+=-mconsole
endif

BIN=fontconv$(EH)

.PHONY: all
all: $(builddir)/$(BIN)

$(builddir) \
$(DESTDIR)$(bindir):
	mkdir -p $@

$(builddir)/$(BIN): fontconv.c Makefile | $(builddir)
	$(GCCH) $(GCCFLAGSH) -o $@ $<

.PHONY: install
install: $(DESTDIR)$(bindir)/$(BIN)

$(DESTDIR)$(bindir)/$(BIN): $(builddir)/$(BIN) | $(DESTDIR)$(bindir)
	$(INSTALL_PROGRAM) $< $@

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(bindir)/$(BIN)

.PHONT: clean
clean:
	rm -f $(builddir)/$(BIN)

.PHONY: distclean
distclean:
	rm -rf $(builddir)
