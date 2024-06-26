// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static const AT2ExtKeyCode scancodes[2][0x80] = {
  { // no prefix
    /*0x00*/ ATEK_UNKNOWN,
    /*0x01*/ MK_EKEY (ATK_ESCAPE),
    /*0x02*/ MK_EKEY (ATK_1),
    /*0x03*/ MK_EKEY (ATK_2),
    /*0x04*/ MK_EKEY (ATK_3),
    /*0x05*/ MK_EKEY (ATK_4),
    /*0x06*/ MK_EKEY (ATK_5),
    /*0x07*/ MK_EKEY (ATK_6),
    /*0x08*/ MK_EKEY (ATK_7),
    /*0x09*/ MK_EKEY (ATK_8),
    /*0x0A*/ MK_EKEY (ATK_9),
    /*0x0B*/ MK_EKEY (ATK_0),
    /*0x0C*/ MK_EKEY (ATK_MINUS),
    /*0x0D*/ MK_EKEY (ATK_EQUALS),
    /*0x0E*/ MK_EKEY (ATK_BACKSPACE),
    /*0x0F*/ MK_EKEY (ATK_TAB),
    /*0x10*/ MK_EKEY (ATK_q),
    /*0x11*/ MK_EKEY (ATK_w),
    /*0x12*/ MK_EKEY (ATK_e),
    /*0x13*/ MK_EKEY (ATK_r),
    /*0x14*/ MK_EKEY (ATK_t),
    /*0x15*/ MK_EKEY (ATK_y),
    /*0x16*/ MK_EKEY (ATK_u),
    /*0x17*/ MK_EKEY (ATK_i),
    /*0x18*/ MK_EKEY (ATK_o),
    /*0x19*/ MK_EKEY (ATK_p),
    /*0x1A*/ MK_EKEY (ATK_LEFTBRACKET),
    /*0x1B*/ MK_EKEY (ATK_RIGHTBRACKET),
    /*0x1C*/ MK_EKEY (ATK_ENTER),
    /*0x1D*/ MK_EMOD (ATK_LCTRL, ATEK_MOD_CTRL),
    /*0x1E*/ MK_EKEY (ATK_a),
    /*0x1F*/ MK_EKEY (ATK_s),
    /*0x20*/ MK_EKEY (ATK_d),
    /*0x21*/ MK_EKEY (ATK_f),
    /*0x22*/ MK_EKEY (ATK_g),
    /*0x23*/ MK_EKEY (ATK_h),
    /*0x24*/ MK_EKEY (ATK_j),
    /*0x25*/ MK_EKEY (ATK_k),
    /*0x26*/ MK_EKEY (ATK_l),
    /*0x27*/ MK_EKEY (ATK_SEMICOLON),
    /*0x28*/ MK_EKEY (ATK_QUOTE),
    /*0x29*/ MK_EKEY (ATK_BACKQUOTE),
    /*0x2A*/ MK_EMOD (ATK_LSHIFT, ATEK_MOD_SHIFT),
    /*0x2B*/ MK_EKEY (ATK_BACKSLASH),
    /*0x2C*/ MK_EKEY (ATK_z),
    /*0x2D*/ MK_EKEY (ATK_x),
    /*0x2E*/ MK_EKEY (ATK_c),
    /*0x2F*/ MK_EKEY (ATK_v),
    /*0x30*/ MK_EKEY (ATK_b),
    /*0x31*/ MK_EKEY (ATK_n),
    /*0x32*/ MK_EKEY (ATK_m),
    /*0x33*/ MK_EKEY (ATK_COMMA),
    /*0x34*/ MK_EKEY (ATK_PERIOD),
    /*0x35*/ MK_EKEY (ATK_SLASH),
    /*0x36*/ MK_EMOD (ATK_RSHIFT, ATEK_MOD_SHIFT),
    /*0x37*/ MK_EKEY (ATK_KP_MULTIPLY),
    /*0x38*/ MK_EMOD (ATK_LALT, ATEK_MOD_ALT),
    /*0x39*/ MK_EKEY (ATK_SPACE),
    /*0x3A*/ MK_EKEY (ATK_CAPSLOCK),
    /*0x3B*/ MK_EKEY (ATK_F1),
    /*0x3C*/ MK_EKEY (ATK_F2),
    /*0x3D*/ MK_EKEY (ATK_F3),
    /*0x3E*/ MK_EKEY (ATK_F4),
    /*0x3F*/ MK_EKEY (ATK_F5),
    /*0x40*/ MK_EKEY (ATK_F6),
    /*0x41*/ MK_EKEY (ATK_F7),
    /*0x42*/ MK_EKEY (ATK_F8),
    /*0x43*/ MK_EKEY (ATK_F9),
    /*0x44*/ MK_EKEY (ATK_F10),
    /*0x45*/ MK_EKEY (ATK_NUMLOCK),
    /*0x46*/ MK_EKEY (ATK_SCROLLLOCK),
    /*0x47*/ MK_EKEY (ATK_KP_7),
    /*0x48*/ MK_EKEY (ATK_KP_8),
    /*0x49*/ MK_EKEY (ATK_KP_9),
    /*0x4A*/ MK_EKEY (ATK_KP_MINUS),
    /*0x4B*/ MK_EKEY (ATK_KP_4),
    /*0x4C*/ MK_EKEY (ATK_KP_5),
    /*0x4D*/ MK_EKEY (ATK_KP_6),
    /*0x4E*/ MK_EKEY (ATK_KP_PLUS),
    /*0x4F*/ MK_EKEY (ATK_KP_1),
    /*0x50*/ MK_EKEY (ATK_KP_2),
    /*0x51*/ MK_EKEY (ATK_KP_3),
    /*0x52*/ MK_EKEY (ATK_KP_0),
    /*0x53*/ MK_EKEY (ATK_KP_PERIOD),
    /*0x54*/ MK_EKEY (ATK_SYSRQ),
    /*0x55*/ MK_EKEY (ATK_FN),
    /*0x56*/ MK_EKEY (ATK_MACRO),
    /*0x57*/ MK_EKEY (ATK_F11),
    /*0x58*/ MK_EKEY (ATK_F12),
    /*0x59*/ ATEK_UNKNOWN,
    /*0x5A*/ ATEK_UNKNOWN,
    /*0x5B*/ MK_EKEY (ATK_F13),
    /*0x5C*/ MK_EKEY (ATK_F14),
    /*0x5D*/ MK_EKEY (ATK_F15),
    /*0x5E*/ ATEK_UNKNOWN,
    /*0x5F*/ ATEK_UNKNOWN,
    /*0x60*/ ATEK_UNKNOWN,
    /*0x61*/ ATEK_UNKNOWN,
    /*0x62*/ ATEK_UNKNOWN,
    /*0x63*/ MK_EKEY (ATK_F16),
    /*0x64*/ MK_EKEY (ATK_F17),
    /*0x65*/ MK_EKEY (ATK_F18),
    /*0x66*/ MK_EKEY (ATK_F19),
    /*0x67*/ MK_EKEY (ATK_F20),
    /*0x68*/ MK_EKEY (ATK_F21),
    /*0x69*/ MK_EKEY (ATK_F22),
    /*0x6A*/ MK_EKEY (ATK_F23),
    /*0x6B*/ MK_EKEY (ATK_F24),
    /*0x6C*/ ATEK_UNKNOWN,
    /*0x6D*/ ATEK_UNKNOWN,
    /*0x6E*/ ATEK_UNKNOWN,
    /*0x6F*/ ATEK_UNKNOWN,
    /*0x70*/ ATEK_UNKNOWN,
    /*0x71*/ ATEK_UNKNOWN,
    /*0x72*/ ATEK_UNKNOWN,
    /*0x73*/ ATEK_UNKNOWN,
    /*0x74*/ ATEK_UNKNOWN,
    /*0x75*/ ATEK_UNKNOWN,
    /*0x76*/ ATEK_UNKNOWN,
    /*0x77*/ ATEK_UNKNOWN,
    /*0x78*/ ATEK_UNKNOWN,
    /*0x79*/ ATEK_UNKNOWN,
    /*0x7A*/ ATEK_UNKNOWN,
    /*0x7B*/ ATEK_UNKNOWN,
    /*0x7C*/ ATEK_UNKNOWN,
    /*0x7D*/ ATEK_UNKNOWN,
    /*0x7E*/ ATEK_UNKNOWN,
    /*0x7F*/ ATEK_UNKNOWN
  }, { // 0xE0 prefix
    /*0xE0 0x00*/ ATEK_UNKNOWN,
    /*0xE0 0x01*/ ATEK_UNKNOWN,
    /*0xE0 0x02*/ ATEK_UNKNOWN,
    /*0xE0 0x03*/ ATEK_UNKNOWN,
    /*0xE0 0x04*/ ATEK_UNKNOWN,
    /*0xE0 0x05*/ ATEK_UNKNOWN,
    /*0xE0 0x06*/ ATEK_UNKNOWN,
    /*0xE0 0x07*/ ATEK_UNKNOWN,
    /*0xE0 0x08*/ ATEK_UNKNOWN,
    /*0xE0 0x09*/ ATEK_UNKNOWN,
    /*0xE0 0x0A*/ ATEK_UNKNOWN,
    /*0xE0 0x0B*/ ATEK_UNKNOWN,
    /*0xE0 0x0C*/ ATEK_UNKNOWN,
    /*0xE0 0x0D*/ ATEK_UNKNOWN,
    /*0xE0 0x0E*/ ATEK_UNKNOWN,
    /*0xE0 0x0F*/ ATEK_UNKNOWN,
    /*0xE0 0x10*/ ATEK_UNKNOWN,
    /*0xE0 0x11*/ ATEK_UNKNOWN,
    /*0xE0 0x12*/ ATEK_UNKNOWN,
    /*0xE0 0x13*/ ATEK_UNKNOWN,
    /*0xE0 0x14*/ ATEK_UNKNOWN,
    /*0xE0 0x15*/ ATEK_UNKNOWN,
    /*0xE0 0x16*/ ATEK_UNKNOWN,
    /*0xE0 0x17*/ ATEK_UNKNOWN,
    /*0xE0 0x18*/ ATEK_UNKNOWN,
    /*0xE0 0x19*/ ATEK_UNKNOWN,
    /*0xE0 0x1A*/ ATEK_UNKNOWN,
    /*0xE0 0x1B*/ ATEK_UNKNOWN,
    /*0xE0 0x1C*/ MK_EKEY (ATK_KP_ENTER),
    /*0xE0 0x1D*/ MK_EMOD (ATK_RCTRL, ATEK_MOD_CTRL),
    /*0xE0 0x1E*/ ATEK_UNKNOWN,
    /*0xE0 0x1F*/ ATEK_UNKNOWN,
    /*0xE0 0x20*/ ATEK_UNKNOWN,
    /*0xE0 0x21*/ ATEK_UNKNOWN,
    /*0xE0 0x22*/ ATEK_UNKNOWN,
    /*0xE0 0x23*/ ATEK_UNKNOWN,
    /*0xE0 0x24*/ ATEK_UNKNOWN,
    /*0xE0 0x25*/ ATEK_UNKNOWN,
    /*0xE0 0x26*/ ATEK_UNKNOWN,
    /*0xE0 0x27*/ ATEK_UNKNOWN,
    /*0xE0 0x28*/ ATEK_UNKNOWN,
    /*0xE0 0x29*/ ATEK_UNKNOWN,
    /*0xE0 0x2A*/ MK_EMOD (ATK_LSHIFT, ATEK_MOD_SHIFT), // Fake Left Shift
    /*0xE0 0x2B*/ ATEK_UNKNOWN,
    /*0xE0 0x2C*/ ATEK_UNKNOWN,
    /*0xE0 0x2D*/ ATEK_UNKNOWN,
    /*0xE0 0x2E*/ ATEK_UNKNOWN,
    /*0xE0 0x2F*/ ATEK_UNKNOWN,
    /*0xE0 0x30*/ ATEK_UNKNOWN,
    /*0xE0 0x31*/ ATEK_UNKNOWN,
    /*0xE0 0x32*/ ATEK_UNKNOWN,
    /*0xE0 0x33*/ ATEK_UNKNOWN,
    /*0xE0 0x34*/ ATEK_UNKNOWN,
    /*0xE0 0x35*/ MK_EKEY (ATK_KP_DIVIDE),
    /*0xE0 0x36*/ MK_EMOD (ATK_RSHIFT, ATEK_MOD_SHIFT), // Fake Right Shift
    /*0xE0 0x37*/ MK_EKEY (ATK_SYSRQ), // Ctrl + Prt Scr
    /*0xE0 0x38*/ MK_EMOD (ATK_RALT, ATEK_MOD_ALT),
    /*0xE0 0x39*/ ATEK_UNKNOWN,
    /*0xE0 0x3A*/ ATEK_UNKNOWN,
    /*0xE0 0x3B*/ ATEK_UNKNOWN,
    /*0xE0 0x3C*/ ATEK_UNKNOWN,
    /*0xE0 0x3D*/ ATEK_UNKNOWN,
    /*0xE0 0x3E*/ ATEK_UNKNOWN,
    /*0xE0 0x3F*/ ATEK_UNKNOWN,
    /*0xE0 0x40*/ ATEK_UNKNOWN,
    /*0xE0 0x41*/ ATEK_UNKNOWN,
    /*0xE0 0x42*/ ATEK_UNKNOWN,
    /*0xE0 0x43*/ ATEK_UNKNOWN,
    /*0xE0 0x44*/ ATEK_UNKNOWN,
    /*0xE0 0x45*/ ATEK_UNKNOWN,
    /*0xE0 0x46*/ MK_EKEY (ATK_BREAK),
    /*0xE0 0x47*/ MK_EKEY (ATK_HOME),
    /*0xE0 0x48*/ MK_EKEY (ATK_UP),
    /*0xE0 0x49*/ MK_EKEY (ATK_PAGEUP),
    /*0xE0 0x4A*/ ATEK_UNKNOWN,
    /*0xE0 0x4B*/ MK_EKEY (ATK_LEFT),
    /*0xE0 0x4C*/ ATEK_UNKNOWN,
    /*0xE0 0x4D*/ MK_EKEY (ATK_RIGHT),
    /*0xE0 0x4E*/ ATEK_UNKNOWN,
    /*0xE0 0x4F*/ MK_EKEY (ATK_END),
    /*0xE0 0x50*/ MK_EKEY (ATK_DOWN),
    /*0xE0 0x51*/ MK_EKEY (ATK_PAGEDOWN),
    /*0xE0 0x52*/ MK_EKEY (ATK_INSERT),
    /*0xE0 0x53*/ MK_EKEY (ATK_DELETE),
    /*0xE0 0x54*/ ATEK_UNKNOWN,
    /*0xE0 0x55*/ ATEK_UNKNOWN,
    /*0xE0 0x56*/ ATEK_UNKNOWN,
    /*0xE0 0x57*/ ATEK_UNKNOWN,
    /*0xE0 0x58*/ ATEK_UNKNOWN,
    /*0xE0 0x59*/ ATEK_UNKNOWN,
    /*0xE0 0x5A*/ ATEK_UNKNOWN,
    /*0xE0 0x5B*/ MK_EKEY (ATK_LGUI),
    /*0xE0 0x5C*/ MK_EKEY (ATK_RGUI),
    /*0xE0 0x5D*/ MK_EKEY (ATK_MENU),
    /*0xE0 0x5E*/ ATEK_UNKNOWN,
    /*0xE0 0x5F*/ ATEK_UNKNOWN,
    /*0xE0 0x60*/ ATEK_UNKNOWN,
    /*0xE0 0x61*/ ATEK_UNKNOWN,
    /*0xE0 0x62*/ ATEK_UNKNOWN,
    /*0xE0 0x63*/ ATEK_UNKNOWN,
    /*0xE0 0x64*/ ATEK_UNKNOWN,
    /*0xE0 0x65*/ ATEK_UNKNOWN,
    /*0xE0 0x66*/ ATEK_UNKNOWN,
    /*0xE0 0x67*/ ATEK_UNKNOWN,
    /*0xE0 0x68*/ ATEK_UNKNOWN,
    /*0xE0 0x69*/ ATEK_UNKNOWN,
    /*0xE0 0x6A*/ ATEK_UNKNOWN,
    /*0xE0 0x6B*/ ATEK_UNKNOWN,
    /*0xE0 0x6C*/ ATEK_UNKNOWN,
    /*0xE0 0x6D*/ ATEK_UNKNOWN,
    /*0xE0 0x6E*/ ATEK_UNKNOWN,
    /*0xE0 0x6F*/ ATEK_UNKNOWN,
    /*0xE0 0x70*/ ATEK_UNKNOWN,
    /*0xE0 0x71*/ ATEK_UNKNOWN,
    /*0xE0 0x72*/ ATEK_UNKNOWN,
    /*0xE0 0x73*/ ATEK_UNKNOWN,
    /*0xE0 0x74*/ ATEK_UNKNOWN,
    /*0xE0 0x75*/ ATEK_UNKNOWN,
    /*0xE0 0x76*/ ATEK_UNKNOWN,
    /*0xE0 0x77*/ ATEK_UNKNOWN,
    /*0xE0 0x78*/ ATEK_UNKNOWN,
    /*0xE0 0x79*/ ATEK_UNKNOWN,
    /*0xE0 0x7A*/ ATEK_UNKNOWN,
    /*0xE0 0x7B*/ ATEK_UNKNOWN,
    /*0xE0 0x7C*/ ATEK_UNKNOWN,
    /*0xE0 0x7D*/ ATEK_UNKNOWN,
    /*0xE0 0x7E*/ ATEK_UNKNOWN,
    /*0xE0 0x7F*/ ATEK_UNKNOWN
  }
};
