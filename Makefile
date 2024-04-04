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

$(builddir)/adtrack2.res: adtrack2.rc Makefile | $(builddir)
	$(WINDRES) -i $< -o $@

$(builddir)/font/track16.inc: font/track16.pbm Makefile | $(builddir)/font
	fontconv -pbmtoinc -s c -o $@ $<

$(builddir)/font/vga16.inc: font/vga16.pbm Makefile | $(builddir)/font
	fontconv -pbmtoinc -s c -o $@ $<

$(builddir)/adtrack2-icon.inc: adtrack2.bmp Makefile | $(builddir)
	bintoinc -s c -o $@ $<

# adt2data.h
adt2data_h_deps=adt2data.h\
 $(defines_h_deps)

# adt2data.c
adt2data_c_deps=adt2data.c $(adt2data_h_deps)\
 $(defines_h_deps)\
 $(builddir)/font/track16.inc\
 $(builddir)/font/vga16.inc

ifneq ($(OS_TARGET),DJGPP)
 adt2data_c_deps+=\
  $(builddir)/adtrack2-icon.inc
endif

$(builddir)/adt2data.o: $(adt2data_c_deps)\
 Makefile | $(builddir)
	$(GCC) $(GCCFLAGS) -c -o $@ $< -I. -I$(builddir) >>$(buildlog)

# adt2ext2.h
adt2ext2_h_deps=adt2ext2.h\
 $(ipattern_h_deps)\
 $(ipattord_h_deps)

# adt2sys.h
adt2sys_h_deps=adt2sys.h\
 $(defines_h_deps)\
 $(pascal_h_deps)

# adt2sys.c
adt2sys_c_deps=adt2sys.c $(adt2sys_h_deps)\
 $(defines_h_deps)

$(builddir)/adt2sys.o: $(adt2sys_c_deps)\
 Makefile | $(builddir)
	$(GCC) $(GCCFLAGS) -c -o $@ $< -I. >>$(buildlog)

# adt2unit.h
adt2unit_h_deps=adt2unit.h\
 $(defines_h_deps)\
 $(pascal_h_deps)\
 $(realtime_h_deps)\
 $(typcons1_h_deps)\
 $(typcons2_h_deps)

# adt2unit.c
adt2unit_c_deps=adt2unit.c $(adt2unit_h_deps)\
 adt2unit/min.c\
 adt2unit/max.c\
 $(realtime_c_deps)
 $(typcons1_c_deps)
 $(typcons2_c_deps)

$(builddir)/adt2unit.o: $(adt2unit_c_deps)\
 Makefile | $(builddir)
	$(GCC) $(GCCFLAGS) -c -o $@ $< -I. >>$(buildlog)

# common.h
common_h_deps=common.h

# common.c
common_c_deps=common.c $(common_h_deps)\
 $(defines_h_deps)\
 $(pascal_h_deps)

$(builddir)/common.o: $(common_c_deps)\
 Makefile | $(builddir)
	$(GCC) $(GCCFLAGS) -c -o $@ $< -I. >>$(buildlog)

# defines.h
defines_h_deps=defines.h

# dialogio.h
dialogio_h_deps=dialogio.h\
 $(pascal_h_deps)\
 $(txtscrio_h_deps)

# dialogio.c
dialogio_c_deps=dialogio.c $(dialogio_h_deps)

$(builddir)/dialogio.o: $(dialogio_c_deps)\
 Makefile | $(builddir)
	$(GCC) $(GCCFLAGS) -c -o $@ $< -I. >>$(buildlog)

# go32/VBIOS.h
go32_VBIOS_h_deps=go32/VBIOS.h\
 go32/VBIOS_get_cursor_pos.c\
 go32/VBIOS_get_video_mode.c\
 go32/VBIOS_is_VGA.c\
 go32/VBIOS_load_ROM_font_8x14.c\
 go32/VBIOS_load_ROM_font_8x16.c\
 go32/VBIOS_load_ROM_font_8x8.c\
 go32/VBIOS_set_cursor_pos.c\
 go32/VBIOS_set_screen_refresh.c\
 go32/VBIOS_set_video_mode.c

# go32/VESA.h
go32_VESA_h_deps=go32/VESA.h\
 go32/VESA_set_video_mode.c

# go32/VGA.h
go32_VGA_h_deps=go32/VGA.h

# ipattern.h
ipattern_h_deps=ipattern.h

# ipattord.h
ipattord_h_deps=ipattord.h

# pascal.h
pascal_h_deps=pascal.h

# pascal/dos.h
pascal_dos_h_deps=pascal/dos.h\
 $(pascal_h_deps)

# pascal/dos.c
pascal_dos_c_deps=pascal/dos.c $(pascal_dos_h_deps)

$(builddir)/pascal/dos.o: $(pascal_dos_c_deps)\
 Makefile | $(builddir)
	$(GCC) $(GCCFLAGS) -c -o $@ $< -I. >>$(buildlog)

# pascal/farptr.h
pascal_farptr_h_deps=pascal/farptr.h

# pascal/go32.h
pascal_go32_h_deps=pascal/go32.h

# pascal/go32.c
pascal_go32_c_deps=pascal/go32.c $(pascal_go32_h_deps)\
 $(pascal_h_deps)

$(builddir)/pascal/go32.o: $(pascal_go32_c_deps)\
 Makefile | $(builddir)/pascal
	$(GCC) $(GCCFLAGS) -c -o $@ $< -I. >>$(buildlog)

# pascal/pc.h
pascal_pc_h_deps=pascal/pc.h

# pascal/pc.c
pascal_pc_c_deps=pascal/pc.c $(pascal_pc_h_deps)

$(builddir)/pascal/pc.o: $(pascal_pc_c_deps)\
 Makefile | $(builddir)/pascal
	$(GCC) $(GCCFLAGS) -c -o $@ $< -I. >>$(buildlog)

# pascal/string.h
pascal_string_h_deps=pascal/string.h

# pascal/string.c
pascal_string_c_deps=pascal/string.c $(pascal_string_h_deps)\
 $(pascal_h_deps)

$(builddir)/pascal/string.o: $(pascal_string_c_deps)\
 Makefile | $(builddir)/pascal
	$(GCC) $(GCCFLAGS) -c -o $@ $< -I. >>$(buildlog)

# realtime.h
realtime_h_deps=realtime.h

# realtime.c
realtime_c_deps=realtime.c

# txtscrio.h
txtscrio_h_deps=txtscrio.h\
 $(defines_h_deps)\
 $(pascal_h_deps)

# txtscrio.c
txtscrio_c_deps=txtscrio.c $(txtscrio_h_deps)\
 $(adt2ext2_h_deps)\
 $(adt2sys_h_deps)\
 $(adt2unit_h_deps)\
 $(common_h_deps)\
 $(dialogio_h_deps)\
 $(defines_h_deps)\
 $(go32_VBIOS_h_deps)\
 $(go32_VESA_h_deps)\
 $(go32_VGA_h_deps)\
 $(pascal_h_deps)\
 $(pascal_dos_h_deps)\
 $(pascal_farptr_h_deps)\
 $(pascal_go32_h_deps)\
 $(pascal_pc_h_deps)\
 $(pascal_string_h_deps)\
 txtscrio/C3StrLen.c\
 txtscrio/CStr2Len.c\
 txtscrio/CStrLen.c\
 txtscrio/DupChar.c\
 txtscrio/Frame.c\
 txtscrio/GetCursor.c\
 txtscrio/GetCursorShape.c\
 txtscrio/go32/GetPalette.c\
 txtscrio/go32/GetRGBitem.c\
 txtscrio/go32/get_VESA_emulated_mode_idx.c\
 txtscrio/go32/initialize.c\
 txtscrio/go32/is_VESA_emulated_mode.c\
 txtscrio/go32/iVGA.c\
 txtscrio/go32/LoadVgaRegisters.c\
 txtscrio/go32/out_reg.c\
 txtscrio/go32/RefreshDisable.c\
 txtscrio/go32/RefreshEnable.c\
 txtscrio/go32/ResetMode.c\
 txtscrio/go32/set_custom_svga_txtmode.c\
 txtscrio/go32/SetCustomVideoMode.c\
 txtscrio/go32/SetPalette.c\
 txtscrio/go32/SetRGBitem.c\
 txtscrio/go32/SetSize.c\
 txtscrio/go32/set_svga_txtmode_100x38.c\
 txtscrio/go32/set_svga_txtmode_128x48.c\
 txtscrio/go32/set_svga_txtmode.c\
 txtscrio/go32/SetTextDisp.c\
 txtscrio/go32/set_vga_txtmode_80x25.c\
 txtscrio/go32/Split2Static.c\
 txtscrio/go32/SplitScr.c\
 txtscrio/go32/VgaFade.c\
 txtscrio/go32/WaitRetrace.c\
 txtscrio/GotoXY.c\
 txtscrio/HideCursor.c\
 txtscrio/is_default_screen_mode.c\
 txtscrio/is_scrollable_screen_mode.c\
 txtscrio/move2screen.c\
 txtscrio/move2screen_alt.c\
 txtscrio/ScreenMemCopy.c\
 txtscrio/SetCursor.c\
 txtscrio/SetCursorShape.c\
 txtscrio/ShowC3Str.c\
 txtscrio/ShowC4Str.c\
 txtscrio/ShowCStr2.c\
 txtscrio/show_cstr_alt.c\
 txtscrio/show_cstr.c\
 txtscrio/ShowCStr.c\
 txtscrio/show_str.c\
 txtscrio/ShowStr.c\
 txtscrio/ShowVC3Str.c\
 txtscrio/ShowVCStr2.c\
 txtscrio/show_vcstr.c\
 txtscrio/ShowVCStr.c\
 txtscrio/show_vstr.c\
 txtscrio/ShowVStr.c\
 txtscrio/ThinCursor.c\
 txtscrio/TxtScrIO_Init.c\
 txtscrio/WhereX.c\
 txtscrio/WhereY.c\
 txtscrio/WideCursor.c

$(builddir)/txtscrio/txtscrio.o: $(txtscrio_c_deps)\
 Makefile | $(builddir)/txtscrio
	$(GCC) $(GCCFLAGS) -c -o $@ $< -I. >>$(buildlog)

# typcons1.h
typcons1_h_deps=typcons1.h

# typcons1.c
typcons1_c_deps=typcons1.c

# typcons2.h
typcons2_h_deps=typcons2.h

# typcons2.c
typcons2_c_deps=typcons2.c

# Ported Pascal to C units

$(builddir)/units/adt2data.ppu: adt2data.pas\
 $(builddir)/adt2data.o\
 Makefile | $(builddir) $(builddir)/units
	$(FPC)\
	 -FU$(builddir)/units\
	 -FE$(builddir)\
	 $(FPCFLAGS)\
	 $<\
	 -o$@\
	 -vnh >>$(buildlog)

$(builddir)/units/common.ppu: common.pas\
 $(builddir)/common.o\
 Makefile | $(builddir) $(builddir)/units
	$(FPC)\
	 -FU$(builddir)/units\
	 -FE$(builddir)\
	 $(FPCFLAGS)\
	 $<\
	 -o$@\
	 -vnh >>$(buildlog)

# Pascal units and target executable

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
 adt2sys.pas\
 $(builddir)/adt2sys.o

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
 typcons2.inc\
 $(builddir)/adt2unit.o

ifeq ($(FPC_OS_TARGET),go32v2)
 adt2unit_deps+=\
  $(iss_tim_deps)
endif

adt2vesa_deps=\
 go32/adt2vesa.pas

depackio_deps=\
 depackio.pas

dialogio_deps=\
 dialogio.pas\
 $(builddir)/dialogio.o

iss_tim_deps=\
 go32/iss_tim.pas

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

ifeq ($(FPC_OS_TARGET),go32v2)
 pascal_deps+=\
  pascal/dos.pas\
  pascal/go32.pas\
  pascal/pc.pas\
  $(builddir)/pascal/dos.o\
  $(builddir)/pascal/go32.o\
  $(builddir)/pascal/pc.o
endif

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
 $(depackio_deps)\
 $(dialogio_deps)\
 $(menulib1_deps)\
 $(menulib2_deps)\
 $(parserio_deps)\
 $(pascal_deps)\
 $(stringio_deps)\
 $(txtscrio_deps)\
 $(builddir)/units/adt2data.ppu\
 $(builddir)/units/common.ppu

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
	 -Fugo32\
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
