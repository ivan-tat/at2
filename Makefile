#!/bin/make -f
# SPDX-FileType: SOURCE
# SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
# SPDX-License-Identifier: GPL-3.0-or-later

include SDK/Common.mak

# Check package

ifneq ($(PACKAGE),)
 ifeq ($(findstring $(PACKAGE),dos linux windows),)
  $(error Unknown package "$(PACKAGE)". Use "make usage" to get help)
 endif
endif

# Setup version and package

ifeq ($(OS_TARGET),go32v2)
 AT2_VERSION_FILE?=adtrack2.dos
 PACKAGE?=dos
else ifeq ($(OS_TARGET),linux)
 AT2_VERSION_FILE?=adtrack2.linux
 PACKAGE?=linux
else ifneq ($(findstring $(OS_TARGET),win32 win64),)
 AT2_VERSION_FILE?=adtrack2.windows
 PACKAGE?=windows
else
 $(warning AT2_VERSION_FILE variable is not defined for target OS: $(OS_TARGET))
endif

ifeq ($(AT2_VERSION),)
 ifneq ($(AT2_VERSION_FILE),)
  AT2_VERSION=$(shell cat $(AT2_VERSION_FILE))
 else
  $(error AT2_VERSION variable is not set)
 endif
endif

# Setup paths

ifeq ($(builddir),)
 builddir:=$(buildroot)/$(FULL_TARGET)
endif
buildlog:=$(builddir)/log

FPCFLAGS+=-Fi$(builddir)

# Setup target

AT2_BIN=adtrack2$E

# Setup target dependencies

A=build-sdk
ifneq ($(findstring $(OS_TARGET),linux win32 win64 windows),)
 A+=build-sdl
endif
ifneq ($(findstring $(OS_TARGET),win32 win64 windows),)
 FPCFLAGS+=-WC
 A+=$(builddir)/adtrack2.res
endif

# Setup release targets

RELEASE_BIN?=adtrack-$(AT2_VERSION)-$(OS_TARGET)-bin-$(CPU_TARGET)

$(info ---)
$(info Goals:   $(MAKECMDGOALS))
$(info Host:    $(OS_HOST)-$(CPU_HOST))
$(info Build:   $(BUILD))
$(info Source:  $(OS_SOURCE)-$(CPU_SOURCE))
$(info Target:  $(OS_TARGET)-$(CPU_TARGET))
$(info Version: $(AT2_VERSION))
$(info Debug:   $(if $(findstring $(DEBUG),0),no,yes))
$(info Binary:  $(AT2_BIN))
$(info ---)
$(info SDK prefix:      $(AT2SDK_PREFIX))
$(info Build directory: $(builddir))
$(info Build log:       $(buildlog))
$(info Package:         $(if $(PACKAGE),$(PACKAGE),<none>))
$(info Release:         $(RELEASE_BIN).tar.gz)
$(info ---)

.PHONY: all
all: clean-at2-version build-at2-bin

.PHONY: usage
usage:
	@fmt Makefile.txt

$(builddir) \
$(builddir)/SDL \
$(builddir)/font \
$(builddir)/units:
	echo "$(PATH)" && mkdir -p "$@"

#-----------------------------------------------------------------------------

.PHONY: build-sdk
build-sdk: | $(AT2SDK)
	$(MAKE) -w -C $|

#-----------------------------------------------------------------------------

.PHONY: build-sdl
build-sdl: | SDL
	$(MAKE) -w -C $|\
	 builddir=$(builddir)/SDL\
	 FPC=$(FPC)\
	 FPCFLAGS='$(FPCFLAGS) -Ccpascal -Mtp'

.PHONY: clean-sdl
clean-sdl: | SDL
	$(MAKE) -w -C $|\
	 builddir=$(builddir)/SDL\
	 clean

#-----------------------------------------------------------------------------

.PHONY: clean-at2-version
clean-at2-version:
	rm -f $(builddir)/adtrack2.inc

$(builddir)/adtrack2.inc: | $(builddir)
	{ echo "const";\
	 echo "at2ver = '$(AT2_VERSION)';";\
	 echo "at2date = '$(shell $(DATEPROG) +%m-%d-%Y)';";\
	 echo "at2link = '$(strip $(shell LC_ALL=C $(DATEPROG) +%-l:%M%P))';";\
	 } >$@

$(builddir)/units/iss_tim.ppu: go32v2/iss_tim.pas Makefile | $(builddir)/units
	$(FPC)\
	 -Ccpascal -Mtp\
	 -FU$(builddir)/units\
	 $(FPCFLAGS)\
	 $<\
	 -o$@\
	 -vnh >$(buildlog)

$(builddir)/units/adt2vesa.ppu: go32v2/adt2vesa.pas Makefile | $(builddir)/units
	$(FPC)\
	 -Ccpascal -Mtp\
	 -FU$(builddir)/units\
	 $(FPCFLAGS)\
	 $<\
	 -o$@\
	 -vnh >$(buildlog)

$(builddir)/font/track16.inc: font/track16.pbm Makefile | $(builddir)/font
	fontconv -pbmtoinc -s pascal -o $@ $<

$(builddir)/font/vga16.inc: font/vga16.pbm Makefile | $(builddir)/font
	fontconv -pbmtoinc -s pascal -o $@ $<

$(builddir)/units/adt2data.ppu: adt2data.pas\
 $(builddir)/font/track16.inc\
 $(builddir)/font/vga16.inc\
 Makefile | $(builddir)/units
	$(FPC)\
	 -FU$(builddir)/units\
	 $(FPCFLAGS)\
	 $<\
	 -o$@\
	 -vnh >$(buildlog)

$(builddir)/adtrack2.res: adtrack2.rc Makefile | $(builddir)
	$(WINDRES) -i $< -o $@

# Add platform-specific source code dependencies
ifeq ($(OS_TARGET),go32v2)
 A+=\
  $(builddir)/units/iss_tim.ppu\
  $(builddir)/units/adt2vesa.ppu
endif

$(builddir)/$(AT2_BIN): $A\
 $(builddir)/units/adt2data.ppu\
 $(builddir)/adtrack2.inc\
 adtrack2.pas\
 adt2ext2.pas\
 adt2ext3.pas\
 adt2ext4.pas\
 adt2ext5.pas\
 adt2extn.pas\
 adt2keyb.pas\
 adt2opl3.pas\
 adt2pack.pas\
 adt2sys.pas\
 adt2text.pas\
 adt2unit.pas\
 depackio.pas\
 dialogio.pas\
 iloaders.inc\
 iloadins.inc\
 instedit.inc\
 ipattern.inc\
 ipattord.inc\
 menulib1.pas\
 menulib2.pas\
 opl3emu.pas\
 parserio.pas\
 realtime.inc\
 stringio.pas\
 txtscrio.pas\
 typcons1.inc\
 typcons2.inc\
 Makefile | $(builddir) $(builddir)/units
	$(FPC)\
	 -Ccpascal -Mtp -Rintel\
	 -Fugo32v2\
	 -Fu$(builddir)/SDL\
	 -FU$(builddir)/units\
	 -FE$(builddir)\
	 $(FPCFLAGS)\
	 adtrack2.pas\
	 -o$@\
	 -vnh >$(buildlog)

.PHONY: build-at2-bin
build-at2-bin: $(builddir)/$(AT2_BIN)

.PHONY: clean-at2-bin
clean-at2-bin: clean-at2-version
	rm -f\
	 $(buildlog)\
	 $(builddir)/font/*\
	 $(builddir)/units/*\
	 $(builddir)/adtrack2.res\
	 $(builddir)/$(AT2_BIN)

#-----------------------------------------------------------------------------

.PHONY: release-at2-bin
release-at2-bin:
	rm -rf $(builddir)/$(RELEASE_BIN)
	mkdir -p $(builddir)/$(RELEASE_BIN)
	$(INSTALL) -vp $(builddir)/$(AT2_BIN) $(builddir)/$(RELEASE_BIN)
ifeq ($(PACKAGE),dos)
	cp -var\
	 package/dos/*\
	 $(builddir)/$(RELEASE_BIN)
else ifeq ($(PACKAGE),linux)
	cp -var\
	 package/sdl/linux/*\
	 package/sdl/adtrack2.ini\
	 package/sdl/adtrack2.mht\
	 package/sdl/revision.txt\
	 $(builddir)/$(RELEASE_BIN)
else ifeq ($(PACKAGE),windows)
	cp -var\
	 3rdparty\
	 package/sdl/windows/*\
	 package/sdl/adtrack2.ini\
	 package/sdl/adtrack2.mht\
	 package/sdl/revision.txt\
	 $(builddir)/$(RELEASE_BIN)
endif
	echo "Creating $(RELEASE_BIN).tar.gz..."
	cd $(builddir) && tar cvzf $(RELEASE_BIN).tar.gz $(RELEASE_BIN)
	rm -rf $(builddir)/$(RELEASE_BIN)

.PHONY: clean-release-at2-bin
clean-release-at2-bin:
	rm -rf $(builddir)/$(RELEASE_BIN)
	rm -f $(builddir)/$(RELEASE_BIN).tar.gz

#-----------------------------------------------------------------------------

.PHONY: release
release: clean-at2-version build-at2-bin release-at2-bin
	@echo "DONE. Your files are ready at \"$(builddir)/\""

.PHONY: clean-release
clean-release: clean-release-at2-bin

#-----------------------------------------------------------------------------

.PHONY: clean
clean: clean-sdl clean-at2-bin clean-release

.PHONY: distclean
distclean:
	rm -rf build
