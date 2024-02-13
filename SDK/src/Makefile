#!/bin/make -f
# SPDX-FileType: SOURCE
# SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
# SPDX-License-Identifier: GPL-3.0-or-later

include ../Common.mak

prefix?=/usr/local

TARGETS=\
 fontconv

.PHONY: all
all: install

# fontconv

.PHONY: build-fontconv
build-fontconv: | fontconv
	$(MAKE) -w -C $| prefix=$(prefix)

.PHONY: install-fontconv
install-fontconv: | fontconv
	$(MAKE) -w -C $| prefix=$(prefix) install

.PHONY: uninstall-fontconv
uninstall-fontconv: | fontconv
	$(MAKE) -w -C $| prefix=$(prefix) uninstall

.PHONY: clean-fontconv
clean-fontconv: | fontconv
	$(MAKE) -w -C $| prefix=$(prefix) clean

.PHONY: distclean-fontconv
distclean-fontconv: | fontconv
	$(MAKE) -w -C $| prefix=$(prefix) distclean

# All targets

.PHONY: build
build: $(foreach t,$(TARGETS),build-$t)

.PHONY: install
install: $(foreach t,$(TARGETS),install-$t)

.PHONY: uninstall
uninstall: $(foreach t,$(TARGETS),uninstall-$t)

.PHONY: clean
clean: $(foreach t,$(TARGETS),clean-$t)

.PHONY: distclean
distclean: $(foreach t,$(TARGETS),distclean-$t)
