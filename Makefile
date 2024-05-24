#!/bin/make -f
# SPDX-FileType: SOURCE
# SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
# SPDX-License-Identifier: GPL-3.0-or-later

makefile:=$(realpath $(lastword $(MAKEFILE_LIST)))
srcdir:=$(patsubst %/,%,$(dir $(makefile)))
builddir:=$(patsubst %/,%,$(realpath .))

ifeq ($(srcdir),$(builddir))
 $(error Do not run "make" from sources' directory: use build directory instead)
endif

include $(srcdir)/SDK/Common.mak

buildlog=log

# Check version

ifeq ($(AT2_VERSION),)
 $(error AT2_VERSION variable is not set)
endif

# Check or setup default package

ifneq ($(PACKAGE),)
 ifeq ($(findstring $(PACKAGE),dos linux windows),)
  $(error Unknown package "$(PACKAGE)")
 endif
else
 ifeq ($(OS_TARGET),DJGPP)
  PACKAGE=dos
 else ifeq ($(OS_TARGET),Linux)
  PACKAGE=linux
 else ifeq ($(OS_TARGET),Windows)
  PACKAGE=windows
 else
  $(warning No package is defined for target OS: $(OS_TARGET))
 endif
endif

# Setup GCC

# The reason for `gnu99' instead of `c99' is to use `ssize_t' when compiling
# in DJGPP environment
GCCFLAGS+=-std=gnu99 -Wall -Wextra -pedantic

GCCFLAGS_DIRS+=-I$(srcdir) -I.

# Setup FPC

FPCFLAGS_DIRS+=-Fi$(srcdir) -Fi. -Fuunits

# Setup files

SRCS=\
 adt2data.c\
 adt2sys.c\
 adt2keyb.c\
 adt2unit.c\
 common.c\
 dialogio.c\
 go32/adt2vesa.c\
 go32/dpmi.c\
 go32/iss_tim.c\
 go32/VGA.c\
 opl3emu.c\
 pascal/dos.c\
 pascal/dpmi.c\
 pascal/go32.c\
 pascal/pc.c\
 pascal/stdio.c\
 pascal/stdlib.c\
 pascal/string.c\
 stringio.c\
 txtscrio.c

DEPS=$(patsubst %.c,_deps_/%.d,$(SRCS))

OBJS=$(SRCS:.c=.o)

AT2_PAS_SRC=adtrack2.pas
AT2_BIN=adtrack2$E

RELEASE_BIN?=adtrack-$(AT2_VERSION)-$(FILE_OS_TARGET)-bin-$(FILE_CPU_TARGET)

$(info ---)
$(info Goals:              $(MAKECMDGOALS))
$(info Build:              $(BUILD))
$(info Host:               $(OS_HOST) ($(CPU_HOST)))
$(info Target:             $(OS_TARGET) ($(CPU_TARGET)))
$(info GNU C Host:         $(GCC_OS_HOST) ($(GCC_CPU_HOST)))
$(info GNU C Target:       $(GCC_OS_TARGET) ($(GCC_CPU_TARGET)))
$(info Free Pascal Host:   $(FPC_OS_HOST) ($(FPC_CPU_HOST)))
$(info Free Pascal Target: $(FPC_OS_TARGET) ($(FPC_CPU_TARGET)))
$(info Program version:    $(AT2_VERSION))
$(info Debug symbols:      $(if $(findstring $(DEBUG),0),no,yes))
$(info Output binary:      $(AT2_BIN))
$(info ---)
$(info SDK prefix:      $(AT2SDK_PREFIX))
$(info Build directory: $(builddir))
$(info Build log:       $(buildlog))
$(info Package:         $(if $(PACKAGE),$(PACKAGE),<none>))
$(info Release:         $(RELEASE_BIN).tar.gz)
$(info ---)

.PHONY: all
all: clean-log clean-at2-version build-at2-bin

font \
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

.PHONY: build-sdl
build-sdl: | $(srcdir)/SDL/Makefile
	@mkdir -p SDL
	FPCFLAGS='$(FPCFLAGS_CPU) $(FPCFLAGS) -Ccpascal -Mtp' \
	$(MAKE) -w -C SDL -f $|

.PHONY: clean-sdl
clean-sdl: | $(srcdir)/SDL/Makefile
	if [ -d SDL ]; then $(MAKE) -w -C SDL -f $| clean; fi

#-----------------------------------------------------------------------------

.PHONY: clean-at2-version
clean-at2-version:
	rm -f adt2ver.inc

adt2ver.inc:
	{ echo "const";\
	 echo "at2ver = '$(AT2_VERSION)';";\
	 echo "at2date = '$(shell $(DATEPROG) +%m-%d-%Y)';";\
	 echo "at2link = '$(strip $(shell LC_ALL=C $(DATEPROG) +%-l:%M%P))';";\
	 } >$@

adtrack2.inc:
	{ echo "{\$$IFDEF WINDOWS}";\
	 echo "{\$$APPTYPE GUI}";\
	 echo "{\$$R $(builddir)/adtrack2.res}";\
	 echo "{\$$ENDIF}";\
	 } >$@

adtrack2.res: $(srcdir)/adtrack2.rc $(makefile)
	@echo "  RC     $(patsubst $(srcdir)/%,%,$<)"; \
	$(WINDRES) -i $< -o $@

font/track16.inc: $(srcdir)/font/track16.pbm $(makefile) | font
	fontconv -pbmtoinc -s c -o $@ $<

font/vga16.inc: $(srcdir)/font/vga16.pbm $(makefile) | font
	fontconv -pbmtoinc -s c -o $@ $<

adtrack2-icon.inc: $(srcdir)/adtrack2.bmp $(makefile)
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
ifneq ($(DEBUG),0)
	@$$(GCC) $$(GCCFLAGS_CPU) $$(GCCFLAGS) $$(GCCFLAGS_DIRS)\
	 -S $$< -o $$(@:.o=.s) >>$$(buildlog)
endif

endef

$(foreach f,$(SRCS),$(eval $(call make_c_obj,$f,$(srcdir))))

# Pascal units with known dependencies

units/adt2data.ppu: $(srcdir)/adt2data.pas adt2data.o $(makefile) | units
	@echo "  PC     $(patsubst $(srcdir)/%,%,$<)"; \
	$(FPC) $(FPCFLAGS_CPU) $(FPCFLAGS) $(FPCFLAGS_DIRS) -FUunits\
	 $< -o$@ -vnh >>$(buildlog)

adt2keyb_ppu_deps=\
 $(srcdir)/adt2keyb.pas\
 adt2keyb.o\
 units/pascal.ppu

ifneq ($(findstring $(OS_TARGET),Linux Windows),)
 adt2keyb_ppu_deps+=\
  SDL/sdl_events.ppu\
  SDL/sdl_keyboard.ppu
endif

units/adt2keyb.ppu: $(adt2keyb_ppu_deps) $(makefile) | units
	@echo "  PC     $(patsubst $(srcdir)/%,%,$<)"; \
	$(FPC) $(FPCFLAGS_CPU) $(FPCFLAGS) $(FPCFLAGS_DIRS) -FUunits\
	 $< -o$@ -vnh >>$(buildlog)

units/adt2vesa.ppu: $(srcdir)/go32/adt2vesa.pas go32/adt2vesa.o $(makefile) | units
	@echo "  PC     $(patsubst $(srcdir)/%,%,$<)"; \
	$(FPC) $(FPCFLAGS_CPU) $(FPCFLAGS) $(FPCFLAGS_DIRS) -FUunits\
	 $< -o$@ -vnh >>$(buildlog)

units/common.ppu: $(srcdir)/common.pas common.o $(makefile) | units
	@echo "  PC     $(patsubst $(srcdir)/%,%,$<)"; \
	$(FPC) $(FPCFLAGS_CPU) $(FPCFLAGS) $(FPCFLAGS_DIRS) -FUunits\
	 $< -o$@ -vnh >>$(buildlog)

dpmi_ppu_deps=\
 $(srcdir)/go32/dpmi.pas\
 go32/dpmi.o\
 units/pascal.ppu

units/dpmi.ppu: $(dpmi_ppu_deps) $(makefile) | units
	@echo "  PC     $(patsubst $(srcdir)/%,%,$<)"; \
	$(FPC) $(FPCFLAGS_CPU) $(FPCFLAGS) $(FPCFLAGS_DIRS) -FUunits\
	 $< -o$@ -vnh >>$(buildlog)

iss_tim_ppu_deps=\
 $(srcdir)/go32/iss_tim.pas\
 go32/iss_tim.o\
 units/dpmi.ppu\
 units/pascal.ppu\
 units/PIT.ppu

units/iss_tim.ppu: $(iss_tim_ppu_deps) $(makefile) | units
	@echo "  PC     $(patsubst $(srcdir)/%,%,$<)"; \
	$(FPC) $(FPCFLAGS_CPU) $(FPCFLAGS) $(FPCFLAGS_DIRS) -FUunits\
	 $< -o$@ -vnh >>$(buildlog)

opl3emu_ppu_deps=\
 $(srcdir)/opl3emu.pas\
 opl3emu.o\
 units/pascal.ppu

units/opl3emu.ppu: $(opl3emu_ppu_deps) $(makefile) | units
	@echo "  PC     $(patsubst $(srcdir)/%,%,$<)"; \
	$(FPC) $(FPCFLAGS_CPU) $(FPCFLAGS) $(FPCFLAGS_DIRS) -FUunits\
	 $< -o$@ -vnh >>$(buildlog)

pascal_ppu_deps=\
 $(srcdir)/pascal.pas\
 $(srcdir)/pascal/stdio.pas\
 $(srcdir)/pascal/stdlib.pas\
 $(srcdir)/pascal/string.pas\
 pascal/stdio.o\
 pascal/stdlib.o\
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

units/pascal.ppu: $(pascal_ppu_deps) $(makefile) | units
	@echo "  PC     $(patsubst $(srcdir)/%,%,$<)"; \
	$(FPC) $(FPCFLAGS_CPU) $(FPCFLAGS) $(FPCFLAGS_DIRS) -FUunits\
	 $< -o$@ -vnh >>$(buildlog)

PIT_ppu_deps=\
 $(srcdir)/go32/PIT.pas\
 $(srcdir)/go32/PIT/pas/PIT_consts.inc

units/PIT.ppu: $(PIT_ppu_deps) $(makefile) | units
	@echo "  PC     $(patsubst $(srcdir)/%,%,$<)"; \
	$(FPC) $(FPCFLAGS_CPU) $(FPCFLAGS) $(FPCFLAGS_DIRS) -FUunits\
	 $< -o$@ -vnh >>$(buildlog)

stringio_ppu_deps=\
 $(srcdir)/stringio.pas\
 stringio.o

units/stringio.ppu: $(stringio_ppu_deps) $(makefile) | units
	@echo "  PC     $(patsubst $(srcdir)/%,%,$<)"; \
	$(FPC) $(FPCFLAGS_CPU) $(FPCFLAGS) $(FPCFLAGS_DIRS) -FUunits\
	 $< -o$@ -vnh >>$(buildlog)

VGA_ppu_deps=\
 $(srcdir)/go32/VGA.pas\
 go32/VGA.o

units/VGA.ppu: $(VGA_ppu_deps) $(makefile) | units
	@echo "  PC     $(patsubst $(srcdir)/%,%,$<)"; \
	$(FPC) $(FPCFLAGS_CPU) $(FPCFLAGS) $(FPCFLAGS_DIRS) -FUunits\
	 $< -o$@ -vnh >>$(buildlog)

# Pascal units with complex dependencies and target executable

adt2ext2_ppu_deps=\
 $(srcdir)/adt2ext2.pas\
 $(srcdir)/instedit.inc\
 $(srcdir)/ipattern.inc\
 $(srcdir)/ipattord.inc

adt2ext3_ppu_deps=\
 $(srcdir)/adt2ext3.pas\
 $(srcdir)/go32/PIT/pas/PIT_consts.inc\
 $(srcdir)/iloaders.inc\
 $(srcdir)/iloadins.inc

adt2ext4_ppu_deps=\
 $(srcdir)/adt2ext4.pas

adt2ext5_ppu_deps=\
 $(srcdir)/adt2ext5.pas

adt2extn_ppu_deps=\
 $(srcdir)/adt2extn.pas

adt2opl3_ppu_deps=\
 $(srcdir)/adt2opl3.pas\
 units/opl3emu.ppu

adt2pack_ppu_deps=\
 $(srcdir)/adt2pack.pas

adt2sys_ppu_deps=\
 $(srcdir)/adt2sys.pas\
 adt2sys.o

ifeq ($(FPC_OS_TARGET),go32v2)
 adt2sys_ppu_deps+=\
  units/adt2vesa.ppu
else
 adt2sys_ppu_deps+=\
  $(adt2opl3_ppu_deps)
endif

adt2text_ppu_deps=\
 $(srcdir)/adt2text.pas\
 adt2ver.inc

adt2unit_ppu_deps=\
 $(srcdir)/adt2unit.pas\
 $(srcdir)/realtime.inc\
 $(srcdir)/typcons1.inc\
 $(srcdir)/typcons2.inc\
 adt2unit.o

ifeq ($(FPC_OS_TARGET),go32v2)
 adt2unit_ppu_deps+=\
  units/iss_tim.ppu\
  units/PIT.ppu
endif

depackio_ppu_deps=\
 $(srcdir)/depackio.pas

dialogio_ppu_deps=\
 $(srcdir)/dialogio.pas\
 dialogio.o

menulib1_ppu_deps=\
 $(srcdir)/menulib1.pas

menulib2_ppu_deps=\
 $(srcdir)/menulib2.pas

parserio_ppu_deps=\
 $(srcdir)/parserio.pas

txtscrio_ppu_deps=\
 $(srcdir)/txtscrio.pas\
 txtscrio.o

ifeq ($(FPC_OS_TARGET),go32v2)
 txtscrio_ppu_deps+=\
  units/VGA.ppu
endif

# Setup target dependencies

adtrack2_bin_deps=build-sdk build-deps

ifeq ($(FPC_OS_TARGET),go32v2)
 adtrack2_bin_deps+=\
  units/iss_tim.ppu\
  units/adt2vesa.ppu
endif

ifneq ($(findstring $(OS_TARGET),Linux Windows),)
 adtrack2_bin_deps+=build-sdl
 FPCFLAGS_DIRS+=-FuSDL
endif

adtrack2_bin_deps+=\
 $(srcdir)/$(AT2_PAS_SRC)\
 adtrack2.inc\
 $(adt2ext2_ppu_deps)\
 $(adt2ext3_ppu_deps)\
 $(adt2ext4_ppu_deps)\
 $(adt2ext5_ppu_deps)\
 $(adt2extn_ppu_deps)\
 $(adt2pack_ppu_deps)\
 $(adt2sys_ppu_deps)\
 $(adt2text_ppu_deps)\
 $(adt2unit_ppu_deps)\
 $(depackio_ppu_deps)\
 $(dialogio_ppu_deps)\
 $(menulib1_ppu_deps)\
 $(menulib2_ppu_deps)\
 $(parserio_ppu_deps)\
 $(txtscrio_ppu_deps)\
 units/adt2data.ppu\
 units/adt2keyb.ppu\
 units/common.ppu\
 units/pascal.ppu\
 units/stringio.ppu

ifeq ($(OS_TARGET),Windows)
 adtrack2_bin_deps+=\
  adtrack2.res
 FPCFLAGS_AT2_BIN+=-WC
endif

$(AT2_BIN): $(adtrack2_bin_deps) $(makefile) | units
	@echo "  PC     $(AT2_PAS_SRC)"; \
	$(FPC) $(FPCFLAGS_CPU) $(FPCFLAGS) -Ccpascal -Mtp -Rintel\
	 $(FPCFLAGS_DIRS) -FUunits -FE.\
	 $(FPCFLAGS_AT2_BIN)\
	 $(srcdir)/$(AT2_PAS_SRC) -o$@ -vnh >>$(buildlog)

.PHONY: build-at2-bin
build-at2-bin: $(AT2_BIN)

.PHONY: clean-at2-bin
clean-at2-bin: clean-log clean-at2-version
	rm -f\
	 font/*\
	 units/*\
	 adtrack2.inc\
	 adtrack2-icon.inc\
	 adtrack2.res\
	 $(OBJS)\
	 $(AT2_BIN)

#-----------------------------------------------------------------------------

.PHONY: release-at2-bin
release-at2-bin:
	rm -rf $(RELEASE_BIN)
	mkdir -p $(RELEASE_BIN)
	$(INSTALL) -vp $(AT2_BIN) $(RELEASE_BIN)
ifeq ($(PACKAGE),dos)
	cp -var\
	 $(srcdir)/package/dos/*\
	 $(RELEASE_BIN)
else ifeq ($(PACKAGE),linux)
	cp -var\
	 $(srcdir)/package/sdl/linux/*\
	 $(srcdir)/package/sdl/adtrack2.ini\
	 $(srcdir)/package/sdl/adtrack2.mht\
	 $(srcdir)/package/sdl/revision.txt\
	 $(RELEASE_BIN)
else ifeq ($(PACKAGE),windows)
	cp -var\
	 $(srcdir)/3rdparty\
	 $(srcdir)/package/sdl/windows/*\
	 $(srcdir)/package/sdl/adtrack2.ini\
	 $(srcdir)/package/sdl/adtrack2.mht\
	 $(srcdir)/package/sdl/revision.txt\
	 $(RELEASE_BIN)
endif
	echo "Creating $(RELEASE_BIN).tar.gz..."
	tar cvzf $(RELEASE_BIN).tar.gz $(RELEASE_BIN)
	rm -rf $(RELEASE_BIN)

.PHONY: clean-release-at2-bin
clean-release-at2-bin:
	rm -rf $(RELEASE_BIN)
	rm -f $(RELEASE_BIN).tar.gz

#-----------------------------------------------------------------------------

.PHONY: release
release: clean-at2-version build-at2-bin release-at2-bin

.PHONY: clean-release
clean-release: clean-release-at2-bin

#-----------------------------------------------------------------------------

.PHONY: clean
clean: clean-deps clean-sdl clean-at2-bin clean-release

.PHONY: distclean
distclean: clean

#-----------------------------------------------------------------------------

_CleanDepsGoals=clean-deps clean distclean
ifeq ($(foreach t,$(MAKECMDGOALS),$(findstring $t,$(_CleanDepsGoals))),)
 -include $(DEPS)
endif
