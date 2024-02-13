#!/bin/make -f
# SPDX-FileType: SOURCE
# SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
# SPDX-License-Identifier: GPL-3.0-or-later

BUILD?=go32v2-i386

include ../SDK/Common.mak

# Setup version

ifeq ($(OS_TARGET),go32v2)
 AT2PLAY_VERSION_FILE?=adt2play.dos
else
 $(warning AT2PLAY_VERSION_FILE variable is not defined for target OS: $(OS_TARGET))
endif

ifeq ($(AT2PLAY_VERSION),)
 ifneq ($(AT2PLAY_VERSION_FILE),)
  AT2PLAY_VERSION=$(shell cat $(AT2PLAY_VERSION_FILE))
 else
  $(error AT2PLAY_VERSION variable is not set)
 endif
endif

# Setup paths

ifeq ($(builddir),)
 builddir:=$(buildroot)/$(FULL_TARGET)
endif
buildlog:=$(builddir)/log

FPCFLAGS+=\
 -Fi..\
 -Fi$(builddir)

# Setup target

AT2PLAY_BIN=adt2play$E

# Setup target dependencies

A=build-sdk

$(info ---)
$(info Goals:   $(MAKECMDGOALS))
$(info Host:    $(OS_HOST)-$(CPU_HOST))
$(info Build:   $(BUILD))
$(info Source:  $(OS_SOURCE)-$(CPU_SOURCE))
$(info Target:  $(OS_TARGET)-$(CPU_TARGET))
$(info Version: $(AT2PLAY_VERSION))
$(info Debug:   $(if $(findstring $(DEBUG),0),no,yes))
$(info Binary:  $(AT2PLAY_BIN))
$(info ---)
$(info SDK prefix:      $(AT2SDK_PREFIX))
$(info Build directory: $(builddir))
$(info Build log:       $(buildlog))
$(info ---)

.PHONY: all
all: clean-at2play-version build-at2play-bin

.PHONY: usage
usage:
	@fmt Makefile.txt

$(builddir) \
$(builddir)/font \
$(builddir)/units:
	mkdir -p $@

#-----------------------------------------------------------------------------

.PHONY: build-sdk
build-sdk: | $(AT2SDK)
	$(MAKE) -w -C $|

#-----------------------------------------------------------------------------

.PHONY: clean-at2play-version
clean-at2play-version:
	rm -f $(builddir)/adt2play.inc

$(builddir)/adt2play.inc: | $(builddir)
	echo "const VERSION_STR = '$(AT2PLAY_VERSION)';" >$@

$(builddir)/units/iss_tim.ppu: ../go32v2/iss_tim.pas Makefile | $(builddir)/units
	$(FPC)\
	 -Ccpascal -Mtp\
	 -FU$(builddir)/units\
	 $(FPCFLAGS)\
	 $<\
	 -o$@\
	 -vnh >$(buildlog)

$(builddir)/font/play8.inc: ../font/play8.pbm Makefile | $(builddir)/font
	fontconv -pbmtoinc -s pascal -o $@ $<

$(builddir)/font/play16.inc: ../font/play16.pbm Makefile | $(builddir)/font
	fontconv -pbmtoinc -s pascal -o $@ $<

$(builddir)/units/a2data.ppu: a2data.pas\
 $(builddir)/font/play8.inc\
 $(builddir)/font/play16.inc\
 Makefile | $(builddir)/units
	$(FPC)\
	 -FU$(builddir)/units\
	 $(FPCFLAGS)\
	 $<\
	 -o$@\
	 -vnh >$(buildlog)

# Add platform-specific source code dependencies
ifeq ($(OS_TARGET),go32v2)
 A+=\
  $(builddir)/units/iss_tim.ppu
endif

$(builddir)/$(AT2PLAY_BIN): $A\
 $(builddir)/units/a2data.ppu\
 $(builddir)/adt2play.inc\
 ../iloaders.inc\
 adt2play.pas\
 a2depack.pas\
 a2fileio.pas\
 a2player.pas\
 a2scrio.pas\
 parserio.pas\
 stringio.pas\
 txtscrio.pas\
 typconst.inc\
 Makefile | $(builddir) $(builddir)/units
	$(FPC) -dADT2PLAY\
	 -Ccpascal -Mtp -Rintel\
	 -FU$(builddir)/units\
	 -FE$(builddir)\
	 $(FPCFLAGS)\
	 adt2play.pas\
	 -o$@\
	 -vnh >$(buildlog)

.PHONY: build-at2play-bin
build-at2play-bin: $(builddir)/$(AT2PLAY_BIN)

.PHONY: clean-at2play-bin
clean-at2play-bin: clean-at2play-version
	rm -f\
	 $(buildlog)\
	 $(builddir)/font/*\
	 $(builddir)/units/*\
	 $(builddir)/$(AT2PLAY_BIN)

#-----------------------------------------------------------------------------

.PHONY: clean
clean: clean-at2play-bin

.PHONY: distclean
distclean:
	rm -rf $(builddir)
