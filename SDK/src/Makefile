#!/bin/make -f
# SPDX-FileType: SOURCE
# SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
# SPDX-License-Identifier: GPL-3.0-or-later

include ../Common.mak

prefix?=/usr/local

TARGETS=\
 bintoinc\
 fontconv

.PHONY: all
all: install

# bintoinc

.PHONY: build-bintoinc
build-bintoinc: | bintoinc
	$(MAKE) -w -C $| prefix=$(prefix)

.PHONY: install-bintoinc
install-bintoinc: | bintoinc
	$(MAKE) -w -C $| prefix=$(prefix) install

.PHONY: uninstall-bintoinc
uninstall-bintoinc: | bintoinc
	$(MAKE) -w -C $| prefix=$(prefix) uninstall

.PHONY: clean-bintoinc
clean-bintoinc: | bintoinc
	$(MAKE) -w -C $| prefix=$(prefix) clean

.PHONY: distclean-bintoinc
distclean-bintoinc: | bintoinc
	$(MAKE) -w -C $| prefix=$(prefix) distclean

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
