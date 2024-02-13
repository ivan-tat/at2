#!/bin/make -f
# SPDX-FileType: SOURCE
# SPDX-FileCopyrightText: 2024 Ivan Tatarinov
# SPDX-License-Identifier: GPL-3.0-or-later

builddir?=build
FPC?=fpc
FPCFLAGS?=-Ccpascal -Mtp

UNITS=\
 sdl.pp\
 sdl__rwops.pp\
 sdl_audio.pp\
 sdl_cdrom.pp\
 sdl_events.pp\
 sdl_keyboard.pp\
 sdl_mouse.pp\
 sdl_syswm.pp\
 sdl_timer.pp\
 sdl_types.pp\
 sdl_video.pp\
 stdio.pp

.PHONY: all
all: $(foreach u,$(UNITS),$(addprefix $(builddir)/,$(u:.pp=.ppu)))

.PHONY: usage
usage:
	@echo "Usage:";\
	 echo "  make [option ...] [TARGET]";\
	 echo "where:";\
	 echo "  options:";\
	 echo "    builddir=<DIR>";\
	 echo "    FPC=<FPC>";\
	 echo "    FPCFLAGS=<FPCFLAGS>";\
	 echo "  TARGET: usage | all | clean | distclean"

$(builddir):
	mkdir -p $@

buildunit=$(FPC) $(FPCFLAGS) -FU$(builddir) -Fu$(builddir) $<

$(builddir)/sdl.ppu: sdl.pp\
 $(builddir)/sdl_types.ppu\
 | $(builddir)
	$(call buildunit)

$(builddir)/sdl__rwops.ppu: sdl__rwops.pp\
 $(builddir)/sdl_types.ppu\
 $(builddir)/stdio.ppu\
 | $(builddir)
	$(call buildunit)

$(builddir)/sdl_audio.ppu: sdl_audio.pp\
 $(builddir)/sdl_types.ppu\
 $(builddir)/sdl__rwops.ppu\
 | $(builddir)
	$(call buildunit)

$(builddir)/sdl_cdrom.ppu: sdl_cdrom.pp\
 $(builddir)/sdl_types.ppu\
 | $(builddir)
	$(call buildunit)

$(builddir)/sdl_events.ppu: sdl_events.pp\
 $(builddir)/sdl_types.ppu\
 $(builddir)/sdl_syswm.ppu\
 $(builddir)/sdl_keyboard.ppu\
 | $(builddir)
	$(call buildunit)

$(builddir)/sdl_keyboard.ppu: sdl_keyboard.pp\
 $(builddir)/sdl_types.ppu\
 | $(builddir)
	$(call buildunit)

$(builddir)/sdl_mouse.ppu: sdl_mouse.pp\
 $(builddir)/sdl_types.ppu\
 $(builddir)/sdl_video.ppu\
 | $(builddir)
	$(call buildunit)

$(builddir)/sdl_syswm.ppu: sdl_syswm.pp | $(builddir)
	$(call buildunit)

$(builddir)/sdl_timer.ppu: sdl_timer.pp\
 $(builddir)/sdl_types.ppu\
 | $(builddir)
	$(call buildunit)

$(builddir)/sdl_types.ppu: sdl_types.pp | $(builddir)
	$(call buildunit)

$(builddir)/sdl_video.ppu: sdl_video.pp\
 $(builddir)/sdl_types.ppu\
 $(builddir)/sdl__rwops.ppu\
 | $(builddir)
	$(call buildunit)

$(builddir)/stdio.ppu: stdio.pp | $(builddir)
	$(call buildunit)

.PHONY: clean
clean:
	rm -f $(foreach u,$(UNITS),$(addprefix $(builddir)/,$(u:.pp=.ppu)))
	rm -f $(foreach u,$(UNITS),$(addprefix $(builddir)/,$(u:.pp=.o)))

.PHONY: distclean
distclean:
	rm -rf $(builddir)
