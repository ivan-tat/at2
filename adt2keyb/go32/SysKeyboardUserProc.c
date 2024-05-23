// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static __LOCK_FUNC void SysKeyboardUserProc (void) {
  uint8_t x;

  process_global_keys ();

  seconds_counter = 0;

  x = inportb (0x60); // Read keyboard
  // Bit  7   - 0 = key is pressed, 1 = key is released
  // Bits 6-0 - scan code

  if (!(x & 0x80)) { // Key is pressed
    keydown[x] = true;
#if USE_LCTRL_RCTRL
    if ((x != SC_LCTRL) && (x != SC_RCTRL)) {
#else // !USE_LCTRL_RCTRL
    if (x != SC_CTRL) {
#endif // !USE_LCTRL_RCTRL
      _ctrl_pressed = false;
      _2x_ctrl_pressed = false;
    }
  } else { // Key is released
    x &= 0x7F; // Scan code only
    keydown[x] = false;
#if USE_LCTRL_RCTRL
    if ((x == SC_LCTRL) || (x == SC_RCTRL)) {
#else // !USE_LCTRL_RCTRL
    if (x == SC_CTRL) {
#endif // !USE_LCTRL_RCTRL
      if (_ctrl_pressed)
        _2x_ctrl_pressed = true;
      _ctrl_pressed = true;
    }
  }

  if (keyboard_sleep)
    goto _acknowledge;

#if USE_LCTRL_RCTRL
  if (keydown[SC_LCTRL] || keydown[SC_RCTRL]) {
#else // !USE_LCTRL_RCTRL
  if (keydown[SC_CTRL]) {
#endif // !USE_LCTRL_RCTRL
#if USE_LALT_RALT
    if (keydown[SC_LALT] || keydown[SC_RALT]) {
#else // !USE_LALT_RALT
    if (keydown[SC_ALT]) {
#endif // !USE_LALT_RALT
      if (keydown[SC_NP_MINUS] || keydown[SC_NP_PLUS])
        goto _acknowledge;
      if (keydown[SC_DELETE]) {
        CTRL_ALT_DEL_pressed = true;
        goto _acknowledge;
      }
    }
    if (keydown[SC_1]
    ||  keydown[SC_2]
    ||  keydown[SC_3]
    ||  keydown[SC_4]
    ||  keydown[SC_5]
    ||  keydown[SC_6]
    ||  keydown[SC_7]
    ||  keydown[SC_8])
      goto _acknowledge;
  }

  return;

_acknowledge:
  // Acknowledge the key: set and clear bit 7
  x = inportb (0x61);
  outportb (0x61, x | 0x80);
  outportb (0x61, x & 0x7F);
  outportb (0x20, 0x20); // Normal EOI
}

END_OF_STATIC_FUNCTION (SysKeyboardUserProc)

void lock_SysKeyboardUserProc (bool lock) {
  LOCK_DECL (lock, v, f);

  LOCK_FUNCTION (f, SysKeyboardUserProc);
}
