// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#include "defines.h"
#if GO32
#include "platform.h"
#include <pc.h>
#include "go32/BIOS.h"
#include "go32/adt2dpmi.h"
#include "txtscrio.h"
#else // !GO32
#include <SDL/SDL_events.h>
#include <SDL/SDL_keyboard.h>
#if USE_FPC
#include "SDL/SDL_events_fix.h"
#include "SDL/SDL_keyboard_fix.h"
#endif // USE_FPC
#endif // !GO32
#include "debug.h"
#include "adt2ext2.h"
#include "adt2keyb.h"
#include "adt2unit.h"
#include "adt2sys.h"

#if GO32

bool keyboard_sleep = false;
bool CTRL_ALT_DEL_pressed = false;
bool _ctrl_pressed = false;
bool _2x_ctrl_pressed = false;

static bool keydown[128];

static __dpmi_paddr SysKeyboardOldIRQ;
static void (*SysKeyboardCallback) (void);

#include "adt2keyb/go32/SysKeyboardIRQ.c"
#include "adt2keyb/go32/SysKeyboardUserProc.c"

#include "adt2keyb/go32/EASCIIKeymap.c"
#include "adt2keyb/go32/scancodes.c"

#include "adt2keyb/go32/keyboard_init.c"
#include "adt2keyb/go32/keyboard_done.c"
#include "adt2keyb/go32/keyboard_reset_buffer.c"
#include "adt2keyb/go32/keyboard_reset_buffer_alt.c"
#include "adt2keyb/go32/keyboard_toggle_sleep.c"
#include "adt2keyb/go32/wait_until_F11_F12_released.c"
#include "adt2keyb/go32/keyboard_poll_input.c"
#include "adt2keyb/go32/keypressed.c"
#include "adt2keyb/go32/GetKey.c"
#include "adt2keyb/go32/scankey.c"
#include "adt2keyb/go32/CapsLock.c"
#include "adt2keyb/go32/NumLock.c"
#include "adt2keyb/go32/ScrollLock.c"
#include "adt2keyb/go32/shift_pressed.c"
#include "adt2keyb/go32/left_shift_pressed.c"
#include "adt2keyb/go32/right_shift_pressed.c"
#include "adt2keyb/go32/both_shifts_pressed.c"
#include "adt2keyb/go32/alt_pressed.c"
#include "adt2keyb/go32/ctrl_pressed.c"

#else // !GO32

static bool keydown[256];

static bool _numlock = false;
static bool _capslock = false;
static bool *keystate; // Set by `SDL_GetKeyState()'
static int numkeys; // Set by `SDL_GetKeyState()'

#include "adt2keyb/sdl/keyboard_init.c"
#include "adt2keyb/sdl/keyboard_done.c"
#include "adt2keyb/sdl/keyboard_reset_buffer.c"
#include "adt2keyb/sdl/symtab.c"
#include "adt2keyb/sdl/TranslateKeycodes.c"
#include "adt2keyb/sdl/wait_until_F11_F12_released.c"
#include "adt2keyb/sdl/keyboard_poll_input.c"
#include "adt2keyb/sdl/keypressed.c"
#include "adt2keyb/sdl/GetKey.c"
#include "adt2keyb/sdl/scankey.c"
#include "adt2keyb/sdl/CapsLock.c"
#include "adt2keyb/sdl/NumLock.c"
#include "adt2keyb/sdl/shift_pressed.c"
#include "adt2keyb/sdl/left_shift_pressed.c"
#include "adt2keyb/sdl/right_shift_pressed.c"
#include "adt2keyb/sdl/alt_pressed.c"
#include "adt2keyb/sdl/ctrl_pressed.c"
#endif // !GO32
#include "adt2keyb/ctrl_tab_pressed.c"
#include "adt2keyb/LookUpKey.c"
#if GO32
#include "adt2keyb/go32/screen_saver.c"
#else // !GO32
#include "adt2keyb/sdl/screen_saver.c"
#endif // !GO32
