#!/bin/make -f
# SPDX-FileType: SOURCE
# SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
# SPDX-License-Identifier: GPL-3.0-or-later

BUILD?=go32v2-i386

include ../SDK/Common.mak

FPCFLAGS+=-O2

# Setup version

ifeq ($(OS_TARGET),DJGPP)
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
 builddir:=$(buildroot)/$(AT2SDK_TARGET_SUBDIR)
endif
buildlog:=$(builddir)/log

FPCFLAGS+=\
 -Fi..\
 -Fi$(builddir)

# Setup target

AT2PLAY_BIN=adt2play$E

$(info ---)
$(info Goals:              $(MAKECMDGOALS))
$(info Build:              $(BUILD))
$(info Host:               $(OS_HOST) ($(CPU_HOST)))
$(info Target:             $(OS_TARGET) ($(CPU_TARGET)))
$(info GNU C Host:         $(GCC_OS_HOST) ($(GCC_CPU_HOST)))
$(info GNU C Target:       $(GCC_OS_TARGET) ($(GCC_CPU_TARGET)))
$(info Free Pascal Host:   $(FPC_OS_HOST) ($(FPC_CPU_HOST)))
$(info Free Pascal Target: $(FPC_OS_TARGET) ($(FPC_CPU_TARGET)))
$(info Program version:    $(AT2PLAY_VERSION))
$(info Debug symbols:      $(if $(findstring $(DEBUG),0),no,yes))
$(info Output binary:      $(AT2PLAY_BIN))
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

$(builddir)/font/play8.inc: ../font/play8.pbm Makefile | $(builddir)/font
	fontconv -pbmtoinc -s pascal -o $@ $<

$(builddir)/font/play16.inc: ../font/play16.pbm Makefile | $(builddir)/font
	fontconv -pbmtoinc -s pascal -o $@ $<

a2data_deps=\
 a2data.pas\
 $(builddir)/font/play8.inc\
 $(builddir)/font/play16.inc

a2depack_deps=\
 a2depack.pas

a2fileio_deps=\
 a2fileio.pas\
 ../iloaders.inc

a2player_deps=\
 a2player.pas\
 typconst.inc

a2scrio_deps=\
 a2scrio.pas

iss_tim_deps=\
 ../go32v2/iss_tim.pas

parserio_deps=\
 parserio.pas

stringio_deps=\
 stringio.pas

txtscrio_deps=\
 txtscrio.pas\
 ../txtscrio/i386/DupChar.pas\
 ../txtscrio/i386/ShowC3Str.pas\
 ../txtscrio/i386/ScreenMemCopy.pas

$(builddir)/units/a2data.ppu: $(a2data_deps) Makefile | $(builddir)/units
	$(FPC)\
	 -FU$(builddir)/units\
	 $(FPCFLAGS)\
	 $<\
	 -o$@\
	 -vnh >$(buildlog)

$(builddir)/units/iss_tim.ppu: $(iss_tim_deps) Makefile | $(builddir)/units
	$(FPC)\
	 -Ccpascal -Mtp\
	 -FU$(builddir)/units\
	 $(FPCFLAGS)\
	 $<\
	 -o$@\
	 -vnh >$(buildlog)

# Setup target dependencies

adt2play_deps=build-sdk

adt2play_deps+=\
 adt2play.pas\
 $(builddir)/adt2play.inc\
 $(a2depack_deps)\
 $(a2fileio_deps)\
 $(a2player_deps)\
 $(a2scrio_deps)\
 $(parserio_deps)\
 $(stringio_deps)\
 $(txtscrio_deps)\
 $(builddir)/units/a2data.ppu

ifeq ($(FPC_OS_TARGET),go32v2)
 adt2play_deps+=\
  $(builddir)/units/iss_tim.ppu
endif

$(builddir)/$(AT2PLAY_BIN): $(adt2play_deps)\
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
