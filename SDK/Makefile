#!/bin/make -f
# SPDX-FileType: SOURCE
# SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
# SPDX-License-Identifier: GPL-3.0-or-later

include Common.mak

prefix?=$(AT2SDK_PREFIX)

.PHONY: all
all: install

.PHONY: build
build: | src
	$(MAKE) -w -C $| prefix=$(prefix)

.PHONY: install
install: | src
	$(MAKE) -w -C $| prefix=$(prefix) install

.PHONY: uninstall
uninstall: | src
	$(MAKE) -w -C $| prefix=$(prefix) uninstall

.PHONY: clean
clean: | src
	$(MAKE) -w -C $| prefix=$(prefix) clean

.PHONY: distclean
distclean: | src
	$(MAKE) -w -C $| prefix=$(prefix) distclean
