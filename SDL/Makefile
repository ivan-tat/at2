#!/bin/make -f
# SPDX-FileType: SOURCE
# SPDX-FileCopyrightText: 2024 Ivan Tatarinov
# SPDX-License-Identifier: GPL-3.0-or-later

srcdir:=$(patsubst %/,%,$(realpath $(dir $(lastword $(MAKEFILE_LIST)))))
builddir:=$(patsubst %/,%,$(realpath .))

ifeq ($(srcdir),$(builddir))
 $(error Do not run "make" from sources' directory: use build directory instead)
endif

FPC?=fpc
FPCFLAGS+=-Fi$(srcdir) -Fu. -FU.

UNITS_SRCS=\
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
all: $(UNITS_SRCS:.pp=.ppu)

sdl.ppu: $(srcdir)/sdl.pp\
 sdl_types.ppu

sdl__rwops.ppu: $(srcdir)/sdl__rwops.pp\
 sdl_types.ppu\
 stdio.ppu

sdl_audio.ppu: $(srcdir)/sdl_audio.pp\
 sdl_types.ppu\
 sdl__rwops.ppu

sdl_cdrom.ppu: $(srcdir)/sdl_cdrom.pp\
 sdl_types.ppu

sdl_events.ppu: $(srcdir)/sdl_events.pp\
 sdl_types.ppu\
 sdl_syswm.ppu\
 sdl_keyboard.ppu

sdl_keyboard.ppu: $(srcdir)/sdl_keyboard.pp\
 sdl_types.ppu

sdl_mouse.ppu: $(srcdir)/sdl_mouse.pp\
 sdl_types.ppu\
 sdl_video.ppu

sdl_syswm.ppu: $(srcdir)/sdl_syswm.pp

sdl_timer.ppu: $(srcdir)/sdl_timer.pp\
 sdl_types.ppu

sdl_types.ppu: $(srcdir)/sdl_types.pp

sdl_video.ppu: $(srcdir)/sdl_video.pp\
 sdl_types.ppu\
 sdl__rwops.ppu

stdio.ppu: $(srcdir)/stdio.pp

%.ppu: $(srcdir)/%.pp
	@echo "  PC     $(patsubst $(srcdir)/%,%,$<)"; \
	$(FPC) $(FPCFLAGS) $< > /dev/null

.PHONY: clean
clean:
	rm -f $(UNITS_SRCS:.pp=.ppu) $(UNITS_SRCS:.pp=.o) *.a

.PHONY: distclean
distclean: clean
