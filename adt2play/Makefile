#!/bin/make -f
# SPDX-FileType: SOURCE
# SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
# SPDX-License-Identifier: GPL-3.0-or-later

# $(srcdir) is root directory, $(srcdir1) is current directory
srcdir1:=$(patsubst %/,%,$(realpath $(dir $(lastword $(MAKEFILE_LIST)))))
srcdir:=$(patsubst %/,%,$(realpath $(srcdir1)/..))
builddir:=$(patsubst %/,%,$(realpath .))

ifeq ($(srcdir1),$(builddir))
 $(error Do not run "make" from sources' directory: use build directory instead)
endif

include $(srcdir)/SDK/Common.mak

buildlog=log

# Check version

ifeq ($(AT2PLAY_VERSION),)
 $(error AT2PLAY_VERSION variable is not set)
endif

# Setup GCC

# The reason for `gnu99' instead of `c99' is to use `ssize_t' when compiling
# in DJGPP environment
GCCFLAGS+=-std=gnu99 -Wall -Wextra -pedantic

GCCFLAGS+=-DADT2PLAY=1

GCCFLAGS_DIRS+=-I$(srcdir1) -I$(srcdir) -I.

# Setup FPC

FPCFLAGS+=-dADT2PLAY

FPCFLAGS_DIRS+=-Fi$(srcdir1) -Fi$(srcdir) -Fi.

# Setup files

SRCS=\
 common.c\
 pascal/dos.c\
 pascal/dpmi.c\
 pascal/go32.c\
 pascal/pc.c\
 pascal/string.c

SRCS1=\
 a2data.c\
 txtscrio.c

DEPS=$(patsubst %.c,_deps_/%.d,$(SRCS) $(SRCS1))

OBJS=$(SRCS:.c=.o) $(SRCS1:.c=.o)

AT2PLAY_PAS_SRC=adt2play.pas
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
all: clean-log clean-at2play-version build-at2play-bin

font \
picture \
units:
	mkdir -p $@

#-----------------------------------------------------------------------------

.PHONY: clean-log
clean-log:
	rm -f $(buildlog)

#-----------------------------------------------------------------------------

# $1=path/file.c (relative to $2), $2=directory
# Keep last line empty: it is needed in $(call ...) expansion
define make_c_dep =
_deps_/$(1:.c=.d): $2/$1
	@if [ \( -n "$$(@D)" \) -a \! -e "$$(@D)" ]; then mkdir -p $$(@D); fi
	@echo "  DEPCC  $1"; \
	$$(GCC) $$(GCCFLAGS_CPU) $$(GCCFLAGS) $$(GCCFLAGS_DIRS)\
	 -MM -MG -MT $$@ -MT $(1:.c=.o) -MF $$@ $$<

endef

$(foreach f,$(SRCS),$(eval $(call make_c_dep,$f,$(srcdir))))
$(foreach f,$(SRCS1),$(eval $(call make_c_dep,$f,$(srcdir1))))

.PHONY: build-deps
build-deps: $(DEPS)

.PHONY: clean-deps
clean-deps:
	rm -rf _deps_

#-----------------------------------------------------------------------------

.PHONY: build-sdk
build-sdk: | $(AT2SDK)
	$(MAKE) -w -C $|

#-----------------------------------------------------------------------------

.PHONY: clean-at2play-version
clean-at2play-version:
	rm -f adt2play.inc

adt2play.inc:
	echo "const VERSION_STR = '$(AT2PLAY_VERSION)';" >$@

font/play8.inc: $(srcdir)/font/play8.pbm\
 $(srcdir1)/Makefile | font
	fontconv -pbmtoinc -s c -o $@ $<

font/play16.inc: $(srcdir)/font/play16.pbm\
 $(srcdir1)/Makefile | font
	fontconv -pbmtoinc -s c -o $@ $<

picture/palette.inc: $(srcdir1)/picture/picture.pal\
 $(srcdir1)/Makefile | picture
	bintoinc -s c -o $@ $<

picture/bitmap.inc: $(srcdir1)/picture/picture.raw\
 $(srcdir1)/Makefile | picture
	bintoinc -s c -o $@ $<

# Object files

# $1=path/file.c (relative to $2), $2=directory
# Keep last line empty: it is needed in $(call ...) expansion
define make_c_obj =
$(1:.c=.o): $2/$1
	@if [ \( -n "$$(@D)" \) -a \! -e "$$(@D)" ]; then mkdir -p $$(@D); fi
	@echo "  CC     $1"; \
	$$(GCC) $$(GCCFLAGS_CPU) $$(GCCFLAGS) $$(GCCFLAGS_DIRS)\
	 -c $$< -o $$@ >>$$(buildlog)

endef

$(foreach f,$(SRCS),$(eval $(call make_c_obj,$f,$(srcdir))))
$(foreach f,$(SRCS1),$(eval $(call make_c_obj,$f,$(srcdir1))))

# Pascal units with known dependencies

units/a2data.ppu: $(srcdir1)/a2data.pas a2data.o $(srcdir1)/Makefile | units
	@echo "  PC     $(patsubst $(srcdir1)/%,%,$<)"; \
	$(FPC) $(FPCFLAGS_CPU) $(FPCFLAGS)\
	 $(FPCFLAGS_DIRS) -FUunits\
	 $< -o$@ -vnh >>$(buildlog)

units/common.ppu: $(srcdir)/common.pas common.o $(srcdir1)/Makefile | units
	@echo "  PC     $(patsubst $(srcdir)/%,%,$<)"; \
	$(FPC) $(FPCFLAGS_CPU) $(FPCFLAGS)\
	 $(FPCFLAGS_DIRS) -FUunits\
	 $< -o$@ -vnh >>$(buildlog)

units/iss_tim.ppu: $(srcdir)/go32/iss_tim.pas $(srcdir1)/Makefile | units
	@echo "  PC     $(patsubst $(srcdir)/%,%,$<)"; \
	$(FPC) $(FPCFLAGS_CPU) $(FPCFLAGS) -Ccpascal -Mtp\
	 $(FPCFLAGS_DIRS) -FUunits\
	 $< -o$@ -vnh >>$(buildlog)

pascal_ppu_deps=\
 $(srcdir)/pascal.pas\
 $(srcdir)/pascal/string.pas\
 pascal/string.o

ifeq ($(FPC_OS_TARGET),go32v2)
 pascal_ppu_deps+=\
  $(srcdir)/pascal/dos.pas\
  $(srcdir)/pascal/dpmi.pas\
  $(srcdir)/pascal/go32.pas\
  $(srcdir)/pascal/pc.pas\
  pascal/dos.o\
  pascal/dpmi.o\
  pascal/go32.o\
  pascal/pc.o
endif

units/pascal.ppu: $(pascal_ppu_deps) $(srcdir1)/Makefile | units
	@echo "  PC     $(patsubst $(srcdir)/%,%,$<)"; \
	$(FPC) $(FPCFLAGS_CPU) $(FPCFLAGS)\
	 $(FPCFLAGS_DIRS) -FUunits\
	 $< -o$@ -vnh >>$(buildlog)

# Pascal units with complex dependencies and target executable

a2depack_ppu_deps=\
 $(srcdir1)/a2depack.pas

a2fileio_ppu_deps=\
 $(srcdir1)/a2fileio.pas\
 $(srcdir)/iloaders.inc

a2player_ppu_deps=\
 $(srcdir1)/a2player.pas\
 $(srcdir1)/typconst.inc

a2scrio_ppu_deps=\
 $(srcdir1)/a2scrio.pas

parserio_ppu_deps=\
 $(srcdir1)/parserio.pas

stringio_ppu_deps=\
 $(srcdir1)/stringio.pas

txtscrio_ppu_deps=\
 $(srcdir1)/txtscrio.pas\
 $(srcdir)/txtscrio/i386/DupChar.pas\
 $(srcdir)/txtscrio/i386/ShowC3Str.pas\
 $(srcdir)/txtscrio/i386/ScreenMemCopy.pas\
 $(srcdir)/txtscrio/go32/pas/fade.pas\
 $(srcdir)/txtscrio/go32/pas/GetPalette.pas\
 $(srcdir)/txtscrio/go32/pas/GetRGBitem.pas\
 $(srcdir)/txtscrio/go32/pas/GetVideoState.pas\
 $(srcdir)/txtscrio/go32/pas/initialize.pas\
 $(srcdir)/txtscrio/go32/pas/iVGA.pas\
 $(srcdir)/txtscrio/go32/pas/SetPalette.pas\
 $(srcdir)/txtscrio/go32/pas/SetRGBitem.pas\
 $(srcdir)/txtscrio/go32/pas/SetVideoState.pas\
 $(srcdir)/txtscrio/go32/pas/VgaFade.pas\
 $(srcdir)/txtscrio/go32/pas/WaitRetrace.pas\
 $(srcdir)/txtscrio/pas/colors.pas\
 $(srcdir)/txtscrio/pas/GetCursor.pas\
 $(srcdir)/txtscrio/pas/GetCursorShape.pas\
 $(srcdir)/txtscrio/pas/GotoXY.pas\
 $(srcdir)/txtscrio/pas/HideCursor.pas\
 $(srcdir)/txtscrio/pas/SetCursor.pas\
 $(srcdir)/txtscrio/pas/SetCursorShape.pas\
 $(srcdir)/txtscrio/pas/ThinCursor.pas\
 $(srcdir)/txtscrio/pas/WhereX.pas\
 $(srcdir)/txtscrio/pas/WhereY.pas\
 $(srcdir)/txtscrio/pas/WideCursor.pas\
 txtscrio.o

# Setup target dependencies

adt2play_bin_deps=build-sdk build-deps

adt2play_bin_deps+=\
 $(srcdir1)/$(AT2PLAY_PAS_SRC)\
 adt2play.inc\
 $(a2depack_ppu_deps)\
 $(a2fileio_ppu_deps)\
 $(a2player_ppu_deps)\
 $(a2scrio_ppu_deps)\
 $(parserio_ppu_deps)\
 $(stringio_ppu_deps)\
 $(txtscrio_ppu_deps)\
 units/a2data.ppu\
 units/common.ppu\
 units/pascal.ppu

ifeq ($(FPC_OS_TARGET),go32v2)
 adt2play_bin_deps+=\
  units/iss_tim.ppu
endif

$(AT2PLAY_BIN): $(adt2play_bin_deps) $(srcdir1)/Makefile | units
	@echo "  PC     $(AT2PLAY_PAS_SRC)"; \
	$(FPC) $(FPCFLAGS_CPU) $(FPCFLAGS) -Ccpascal -Mtp -Rintel\
	 $(FPCFLAGS_DIRS) -FUunits -FE.\
	 $(srcdir1)/$(AT2PLAY_PAS_SRC) -o$@ -vnh >$(buildlog)

.PHONY: build-at2play-bin
build-at2play-bin: $(AT2PLAY_BIN)

.PHONY: clean-at2play-bin
clean-at2play-bin: clean-at2play-version
	rm -f\
	 $(buildlog)\
	 font/*\
	 picture/*\
	 units/*\
	 $(OBJS)\
	 $(AT2PLAY_BIN)

#-----------------------------------------------------------------------------

.PHONY: clean
clean: clean-deps clean-at2play-bin

.PHONY: distclean
distclean: clean

#-----------------------------------------------------------------------------

_CleanDepsGoals=clean-deps clean distclean
ifeq ($(foreach t,$(MAKECMDGOALS),$(findstring $t,$(_CleanDepsGoals))),)
 -include $(DEPS)
endif
