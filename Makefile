#!/bin/make -f
# SPDX-FileType: SOURCE
# SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
# SPDX-License-Identifier: GPL-3.0-or-later

include SDK/Common.mak

# The reason for `gnu99' instead of `c99' is to use `ssize_t' when compiling
# in DJGPP environment
GCCFLAGS+=-std=gnu99 -Wall -Wextra -pedantic -O2

FPCFLAGS+=-O2

# Check package

ifneq ($(PACKAGE),)
 ifeq ($(findstring $(PACKAGE),dos linux windows),)
  $(error Unknown package "$(PACKAGE)". Use "make usage" to get help)
 endif
endif

# Setup version and package

ifeq ($(OS_TARGET),DJGPP)
 AT2_VERSION_FILE?=adtrack2.dos
 PACKAGE?=dos
else ifeq ($(OS_TARGET),Linux)
 AT2_VERSION_FILE?=adtrack2.linux
 PACKAGE?=linux
else ifeq ($(OS_TARGET),Windows)
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
 builddir:=$(buildroot)/$(AT2SDK_TARGET_SUBDIR)
endif
buildlog:=$(builddir)/log

FPCFLAGS+=-Fi$(builddir)

# Setup target

AT2_BIN=adtrack2$E

# Setup release targets

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

.PHONY: usage
usage:
	@fmt Makefile.txt

$(builddir) \
$(builddir)/SDL \
$(builddir)/font \
$(builddir)/pascal \
$(builddir)/txtscrio \
$(builddir)/units:
	mkdir -p "$@"

#-----------------------------------------------------------------------------

.PHONY: clean-log
clean-log: | $(builddir)
	rm -f $(buildlog)

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
	rm -f $(builddir)/adt2ver.inc

$(builddir)/adt2ver.inc: | $(builddir)
	{ echo "const";\
	 echo "at2ver = '$(AT2_VERSION)';";\
	 echo "at2date = '$(shell $(DATEPROG) +%m-%d-%Y)';";\
	 echo "at2link = '$(strip $(shell LC_ALL=C $(DATEPROG) +%-l:%M%P))';";\
	 } >$@

$(builddir)/adtrack2.inc: | $(builddir)
	{ echo "{\$$IFDEF WINDOWS}";\
	 echo "{\$$APPTYPE GUI}";\
	 echo "{\$$R $(builddir)/adtrack2.res}";\
	 echo "{\$$ENDIF}";\
	 } >$@

$(builddir)/adtrack2-icon.inc: adtrack2.bmp Makefile | $(builddir)
	bintoinc -s pascal -o $@ $<

$(builddir)/adtrack2.res: adtrack2.rc Makefile | $(builddir)
	$(WINDRES) -i $< -o $@

$(builddir)/common.o: common.c\
 common.h\
 defines.h\
 pascal.h\
 Makefile | $(builddir)
	$(GCC) $(GCCFLAGS) -c -o $@ $< -I. >>$(buildlog)

$(builddir)/font/track16.inc: font/track16.pbm Makefile | $(builddir)/font
	fontconv -pbmtoinc -s pascal -o $@ $<

$(builddir)/font/vga16.inc: font/vga16.pbm Makefile | $(builddir)/font
	fontconv -pbmtoinc -s pascal -o $@ $<

$(builddir)/pascal/string.o: pascal/string.c\
 pascal.h\
 pascal/string.h\
 Makefile | $(builddir)/pascal
	$(GCC) $(GCCFLAGS) -c -o $@ $< -I. >>$(buildlog)

$(builddir)/txtscrio/txtscrio.o: txtscrio.c\
 defines.h\
 pascal.h\
 txtscrio.h\
 txtscrio/show_str.c\
 txtscrio/show_vstr.c\
 txtscrio/show_cstr.c\
 txtscrio/show_vcstr.c\
 txtscrio/show_cstr_alt.c\
 txtscrio/DupChar.c\
 txtscrio/ShowStr.c\
 txtscrio/ShowVStr.c\
 txtscrio/ShowCStr.c\
 txtscrio/ShowVCStr.c\
 txtscrio/ShowCStr2.c\
 txtscrio/ShowVCStr2.c\
 txtscrio/ShowC3Str.c\
 txtscrio/ShowVC3Str.c\
 txtscrio/ShowC4Str.c\
 txtscrio/CStrLen.c\
 txtscrio/CStr2Len.c\
 txtscrio/C3StrLen.c\
 txtscrio/Frame.c\
 txtscrio/GetCursor.c\
 txtscrio/ScreenMemCopy.c\
 txtscrio/move2screen_alt.c\
 Makefile | $(builddir)/txtscrio
	$(GCC) $(GCCFLAGS) -c -o $@ $< -I. >>$(buildlog)

adt2data_deps=\
 adt2data.pas\
 $(builddir)/font/track16.inc\
 $(builddir)/font/vga16.inc

ifneq ($(OS_TARGET),DJGPP)
 adt2data_deps+=\
  $(builddir)/adtrack2-icon.inc
endif

adt2ext2_deps=\
 adt2ext2.pas\
 instedit.inc\
 ipattern.inc\
 ipattord.inc

adt2ext3_deps=\
 adt2ext3.pas\
 iloaders.inc\
 iloadins.inc

adt2ext4_deps=\
 adt2ext4.pas

adt2ext5_deps=\
 adt2ext5.pas

adt2extn_deps=\
 adt2extn.pas

adt2keyb_deps=\
 adt2keyb.pas

adt2opl3_deps=\
 adt2opl3.pas\
 $(opl3emu_deps)

adt2pack_deps=\
 adt2pack.pas

adt2sys_deps=\
 adt2sys.pas

ifeq ($(FPC_OS_TARGET),go32v2)
 adt2sys_deps+=\
  $(adt2vesa_deps)
else
 adt2sys_deps+=\
  $(adt2opl3_deps)
endif

adt2text_deps=\
 adt2text.pas\
 $(builddir)/adt2ver.inc

adt2unit_deps=\
 adt2unit.pas\
 realtime.inc\
 typcons1.inc\
 typcons2.inc

ifeq ($(FPC_OS_TARGET),go32v2)
 adt2unit_deps+=\
  $(iss_tim_deps)
endif

adt2vesa_deps=\
 go32v2/adt2vesa.pas

common_deps=\
 common.pas\
 $(builddir)/common.o

depackio_deps=\
 depackio.pas

dialogio_deps=\
 dialogio.pas

iss_tim_deps=\
 go32v2/iss_tim.pas

menulib1_deps=\
 menulib1.pas

menulib2_deps=\
 menulib2.pas

opl3emu_deps=\
 opl3emu.pas

parserio_deps=\
 parserio.pas

pascal_deps=\
 pascal.pas\
 pascal/string.pas\
 $(builddir)/pascal/string.o

stringio_deps=\
 stringio.pas

txtscrio_deps=\
 txtscrio.pas\
 $(builddir)/txtscrio/txtscrio.o

# Setup target dependencies

adtrack2_deps=build-sdk
ifneq ($(findstring $(OS_TARGET),Linux Windows),)
 adtrack2_deps+=build-sdl
endif

adtrack2_deps+=\
 adtrack2.pas\
 $(builddir)/adtrack2.inc\
 $(adt2data_deps)\
 $(adt2ext2_deps)\
 $(adt2ext3_deps)\
 $(adt2ext4_deps)\
 $(adt2ext5_deps)\
 $(adt2extn_deps)\
 $(adt2keyb_deps)\
 $(adt2opl3_deps)\
 $(adt2pack_deps)\
 $(adt2sys_deps)\
 $(adt2text_deps)\
 $(adt2unit_deps)\
 $(common_deps)\
 $(depackio_deps)\
 $(dialogio_deps)\
 $(menulib1_deps)\
 $(menulib2_deps)\
 $(parserio_deps)\
 $(pascal_deps)\
 $(stringio_deps)\
 $(txtscrio_deps)

# Add platform-specific source code dependencies
ifeq ($(FPC_OS_TARGET),go32v2)
 adtrack2_deps+=\
  $(iss_tim_deps)\
  $(adt2vesa_deps)
endif

ifeq ($(OS_TARGET),Windows)
 FPCFLAGS+=-WC
 adtrack2_deps+=\
  $(builddir)/adtrack2.res
endif

$(builddir)/$(AT2_BIN): $(adtrack2_deps)\
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
	 -vnh >>$(buildlog)

.PHONY: build-at2-bin
build-at2-bin: $(builddir)/$(AT2_BIN)

.PHONY: clean-at2-bin
clean-at2-bin: clean-log clean-at2-version
	rm -f\
	 $(builddir)/font/*\
	 $(builddir)/pascal/*\
	 $(builddir)/txtscrio/*\
	 $(builddir)/units/*\
	 $(builddir)/adtrack2.inc\
	 $(builddir)/adtrack2-icon.inc\
	 $(builddir)/adtrack2.res\
	 $(builddir)/*.o\
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
