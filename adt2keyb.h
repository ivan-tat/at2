// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef ADT2KEYB_H
#define ADT2KEYB_H

#include "defines.h"

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h> // size_t

/* AT2 keys */

typedef enum AT2KeyCode {
  ATK_UNKNOWN,
  ATK_BACKSPACE,    // Backspace (BS)
  ATK_TAB,          // Tab
  ATK_ENTER,        // Enter (Return)
  ATK_PAUSE,        // Pause and Break
  ATK_ESCAPE,       // Escape (Esc)
  ATK_SPACE,        // Space
  ATK_QUOTE,        // ' "
  ATK_COMMA,        // , <
  ATK_MINUS,        // - _
  ATK_PERIOD,       // . >
  ATK_SLASH,        // / ?
  ATK_0,            // 0
  ATK_1,            // 1
  ATK_2,            // 2
  ATK_3,            // 3
  ATK_4,            // 4
  ATK_5,            // 5
  ATK_6,            // 6
  ATK_7,            // 7
  ATK_8,            // 8
  ATK_9,            // 9
  ATK_SEMICOLON,    // ; :
  ATK_EQUALS,       // = +
  ATK_LEFTBRACKET,  // [ {
  ATK_BACKSLASH,    // \ |
  ATK_RIGHTBRACKET, // ] }
  ATK_BACKQUOTE,    // ` ~
  ATK_a,            // a
  ATK_b,            // b
  ATK_c,            // c
  ATK_d,            // d
  ATK_e,            // e
  ATK_f,            // f
  ATK_g,            // g
  ATK_h,            // h
  ATK_i,            // i
  ATK_j,            // j
  ATK_k,            // k
  ATK_l,            // l
  ATK_m,            // m
  ATK_n,            // n
  ATK_o,            // o
  ATK_p,            // p
  ATK_q,            // q
  ATK_r,            // r
  ATK_s,            // s
  ATK_t,            // t
  ATK_u,            // u
  ATK_v,            // v
  ATK_w,            // w
  ATK_x,            // x
  ATK_y,            // y
  ATK_z,            // z
  ATK_DELETE,       // Delete (Del)
  ATK_KP_0,         // Keypad 0 and Insert (Ins)
  ATK_KP_1,         // Keypad 1 and End
  ATK_KP_2,         // Keypad 2 and Down Arrow
  ATK_KP_3,         // Keypad 3 and Page Down (PgDn)
  ATK_KP_4,         // Keypad 4 and Left
  ATK_KP_5,         // Keypad 5
  ATK_KP_6,         // Keypad 6 and Right
  ATK_KP_7,         // Keypad 7 and Home
  ATK_KP_8,         // Keypad 8 and Up Arrow
  ATK_KP_9,         // Keypad 9 and Page Up (PgUp)
  ATK_KP_PERIOD,    // Keypad . and Delete (Del)
  ATK_KP_DIVIDE,    // Keypad /
  ATK_KP_MULTIPLY,  // Keypad *
  ATK_KP_MINUS,     // Keypad -
  ATK_KP_PLUS,      // Keypad +
  ATK_KP_ENTER,     // Keypad Enter
  ATK_UP,           // Up Arrow
  ATK_DOWN,         // Down Arrow
  ATK_RIGHT,        // Right Arrow
  ATK_LEFT,         // Left Arrow
  ATK_INSERT,       // Insert (Ins)
  ATK_HOME,         // Home
  ATK_END,          // End
  ATK_PAGEUP,       // Page Up (PgUp)
  ATK_PAGEDOWN,     // Page Donw (PgDn)
  ATK_F1,           // F1
  ATK_F2,           // F2
  ATK_F3,           // F3
  ATK_F4,           // F4
  ATK_F5,           // F5
  ATK_F6,           // F6
  ATK_F7,           // F7
  ATK_F8,           // F8
  ATK_F9,           // F9
  ATK_F10,          // F10
  ATK_F11,          // F11
  ATK_F12,          // F12
  ATK_F13,          // F13
  ATK_F14,          // F14
  ATK_F15,          // F15
#if GO32
  ATK_F16,          // F16
  ATK_F17,          // F17
  ATK_F18,          // F18
  ATK_F19,          // F19
  ATK_F20,          // F20
  ATK_F21,          // F21
  ATK_F22,          // F22
  ATK_F23,          // F23
  ATK_F24,          // F24
#endif // GO32
  ATK_NUMLOCK,      // Num Lock
  ATK_CAPSLOCK,     // Caps Lock
  ATK_SCROLLLOCK,   // Scroll Lock
  ATK_RSHIFT,       // Right Shift
  ATK_LSHIFT,       // Left Shift
  ATK_RCTRL,        // Right Control (Ctrl)
  ATK_LCTRL,        // Left Control (Ctrl)
  ATK_RALT,         // Right Alt
  ATK_LALT,         // Left Alt
  ATK_LGUI,         // Left GUI (0xE0,0x5B)
  ATK_RGUI,         // Right GUI (0xE0,0x5C)
  ATK_FN,           // Fn
  ATK_MACRO,        // Macro
  ATK_PRTSCR,       // Prt Scr and SysRq
  ATK_SYSRQ,        // Ctrl + Prt Scr (0xE0,0x37)
  ATK_BREAK,        // Ctrl + Pause (0xE0,0x46)
  ATK_MENU,         // Menu (0xE0,0x5D)
  ATK_LAST = ATK_MENU
} AT2KeyCode;

/* AT2 key modifiers */

typedef enum AT2KeyMod {
  ATM_NONE,
  ATM_LCTRL  = 0x01,
  ATM_RCTRL  = 0x02,
  ATM_LSHIFT = 0x04,
  ATM_RSHIFT = 0x08,
  ATM_LALT   = 0x10,
  ATM_RALT   = 0x20,
  ATM_CTRL   = ATM_LCTRL  | ATM_RCTRL,
  ATM_SHIFT  = ATM_LSHIFT | ATM_RSHIFT,
  ATM_ALT    = ATM_LALT   | ATM_RALT
} AT2KeyMod;

/* AT2 extended keys */

typedef uint16_t ExtKeyCode; // TODO: Remove enumeration limit - use 32 bits

typedef enum AT2ExtKeyCode {
  ATEK_ANY       = 0x00FF, // Mask for a key or character
  ATEK_MOD_CTRL  = 0x0800,
  ATEK_MOD_SHIFT = 0x1000,
  ATEK_MOD_ALT   = 0x2000,
  ATEK_MOD_ANY   = ATEK_MOD_CTRL | ATEK_MOD_SHIFT | ATEK_MOD_ALT,
  ATEK_TYPE      = (3<<14), // Mask for a type
  ATEK_TYPE_CHR  = (0<<14), // Character (ASCII)
  ATEK_TYPE_KEY  = (1<<14), // Normal key
  ATEK_TYPE_MOD  = (2<<14), // Modifier
  ATEK_TYPE_UNK  = (3<<14), // Unknown key
  ATEK_NULL      = 0xF000, // As found in `symtab[]' (acts as '\0' character in the code)
  ATEK_UNKNOWN   = 0xFFFF  // As found in `symtab[]'
} AT2ExtKeyCode;

#define ATEK_IsChar(x)     (((x) & ATEK_TYPE) == ATEK_TYPE_CHR)
#define ATEK_IsKey(x)      (((x) & ATEK_TYPE) == ATEK_TYPE_KEY)
#define ATEK_IsModifier(x) (((x) & ATEK_TYPE) == ATEK_TYPE_MOD)
#define ATEK_IsUnknown(x)  (((x) & ATEK_TYPE) == ATEK_TYPE_UNK)

#define ATEK_GetChar(x) ((x) & ATEK_ANY)
#define ATEK_GetKey(x)  ((x) & ATEK_ANY)

#define MK_ECHR(s)   ((s) | ATEK_TYPE_CHR)
#define MK_EKEY(k)   ((k) | ATEK_TYPE_KEY)
#define MK_EMOD(k,m) ((k) | (m) | ATEK_TYPE_MOD)

/* PC keyboard scan codes (0-0x7F) */

#if GO32
#define USE_LCTRL_RCTRL 0
#define USE_LALT_RALT 0
#endif // GO32

#define MK_SC(sc,p) ((sc)+(p)*0x00) // TODO: Use (p)*0x80 and change tables

#define SC_ESCAPE       MK_SC (0x01,0) // Esc
#define SC_1            MK_SC (0x02,0) // 1 !
#define SC_2            MK_SC (0x03,0) // 2 @
#define SC_3            MK_SC (0x04,0) // 3 #
#define SC_4            MK_SC (0x05,0) // 4 $
#define SC_5            MK_SC (0x06,0) // 5 %
#define SC_6            MK_SC (0x07,0) // 6 ^
#define SC_7            MK_SC (0x08,0) // 7 &
#define SC_8            MK_SC (0x09,0) // 8 *
#define SC_9            MK_SC (0x0A,0) // 9 (
#define SC_0            MK_SC (0x0B,0) // 0 )
#define SC_MINUS        MK_SC (0x0C,0) // - _
#define SC_EQUALS       MK_SC (0x0D,0) // = +
#define SC_BACKSPACE    MK_SC (0x0E,0) // BS
#define SC_TAB          MK_SC (0x0F,0) // Tab
#define SC_Q            MK_SC (0x10,0) // Q
#define SC_W            MK_SC (0x11,0) // W
#define SC_E            MK_SC (0x12,0) // E
#define SC_R            MK_SC (0x13,0) // R
#define SC_T            MK_SC (0x14,0) // T
#define SC_Y            MK_SC (0x15,0) // Y
#define SC_U            MK_SC (0x16,0) // U
#define SC_I            MK_SC (0x17,0) // I
#define SC_O            MK_SC (0x18,0) // O
#define SC_P            MK_SC (0x19,0) // P
#define SC_LEFTBRACKET  MK_SC (0x1A,0) // [ {
#define SC_RIGHTBRACKET MK_SC (0x1B,0) // ] }
#define SC_ENTER        MK_SC (0x1C,0) // Enter (0x1C)
#define SC_NP_ENTER     MK_SC (0x1C,1) // Keypad Enter (0xE0,0x1C)
#if GO32
#define SC_CTRL         MK_SC (0x1D,0) // Left or Right Ctrl
#if USE_LCTRL_RCTRL
#define SC_LCTRL        MK_SC (0x1D,0) // Left Ctrl (0x1D)
#define SC_RCTRL        MK_SC (0x1D,1) // Right Ctrl (0xE0,0x1D)
#endif // USE_LCTRL_RCTRL
#endif // GO32
#define SC_A            MK_SC (0x1E,0) // A
#define SC_S            MK_SC (0x1F,0) // S
#define SC_D            MK_SC (0x20,0) // D
#define SC_F            MK_SC (0x21,0) // F
#define SC_G            MK_SC (0x22,0) // G
#define SC_H            MK_SC (0x23,0) // H
#define SC_J            MK_SC (0x24,0) // J
#define SC_K            MK_SC (0x25,0) // K
#define SC_L            MK_SC (0x26,0) // L
#define SC_SEMICOLON    MK_SC (0x27,0) // ; :
#define SC_QUOTE        MK_SC (0x28,0) // ' "
#define SC_BACKQUOTE    MK_SC (0x29,0) // ` ~
#define SC_LSHIFT       MK_SC (0x2A,0) // Left Shift (0x2A)
#define SC_LSHIFT_FAKE  MK_SC (0x2A,1) // Fake Left Shift (0xE0,0x2A)
#define SC_BACKSLASH    MK_SC (0x2B,0) // \ |
#define SC_Z            MK_SC (0x2C,0) // Z
#define SC_X            MK_SC (0x2D,0) // X
#define SC_C            MK_SC (0x2E,0) // C
#define SC_V            MK_SC (0x2F,0) // V
#define SC_B            MK_SC (0x30,0) // B
#define SC_N            MK_SC (0x31,0) // N
#define SC_M            MK_SC (0x32,0) // M
#define SC_COMMA        MK_SC (0x33,0) // , <
#define SC_PERIOD       MK_SC (0x34,0) // . >
#define SC_SLASH        MK_SC (0x35,0) // / ? (0x35)
#define SC_NP_DIVIDE    MK_SC (0x35,1) // Keypad / (0xE0,0x35)
#define SC_RSHIFT       MK_SC (0x36,0) // Right Shift (0x36)
#define SC_RSHIFT_FAKE  MK_SC (0x36,1) // Fake Right Shift (0xE0,0x36)
#define SC_NP_MULTIPLY  MK_SC (0x37,0) // Keypad * (0x37)
#define SC_PRTSCR       MK_SC (0x37,1) // Ctrl + Prt Scr (0xE0,0x37)
#if GO32
#define SC_ALT          MK_SC (0x38,0) // Left or Right Alt
#if USE_LALT_RALT
#define SC_LALT         MK_SC (0x38,0) // Left Alt (0x38)
#define SC_RALT         MK_SC (0x38,1) // Right Alt (0xE0,0x38)
#endif // USE_LALT_RALT
#endif // GO32
#define SC_SPACE        MK_SC (0x39,0) // Space
#define SC_CAPSLOCK     MK_SC (0x3A,0) // Caps Lock
#define SC_F1           MK_SC (0x3B,0) // F1
#define SC_F2           MK_SC (0x3C,0) // F2
#define SC_F3           MK_SC (0x3D,0) // F3
#define SC_F4           MK_SC (0x3E,0) // F4
#define SC_F5           MK_SC (0x3F,0) // F5
#define SC_F6           MK_SC (0x40,0) // F6
#define SC_F7           MK_SC (0x41,0) // F7
#define SC_F8           MK_SC (0x42,0) // F8
#define SC_F9           MK_SC (0x43,0) // F9
#define SC_F10          MK_SC (0x44,0) // F10
#define SC_NUMLOCK      MK_SC (0x45,0) // Num Lock
#define SC_SCROLLLOCK   MK_SC (0x46,0) // Scroll Lock (0x46)
#define SC_BREAK        MK_SC (0x46,1) // Ctrl + Break (0xE0,0x46)
#define SC_NP_7         MK_SC (0x47,0) // Keypad 7 / Home (0x47)
#define SC_HOME         MK_SC (0x47,1) // Home (0xE0, 0x47)
#define SC_NP_8         MK_SC (0x48,0) // Keypad 8 / UpArrow (0x48)
#define SC_UP           MK_SC (0x48,1) // Up (0xE0,0x48)
#define SC_NP_9         MK_SC (0x49,0) // Keypad 9 / PgUp (0x49)
#define SC_PAGEUP       MK_SC (0x49,1) // Page Up (0xE0,0x49)
#define SC_NP_MINUS     MK_SC (0x4A,0) // Keypad -
#define SC_NP_4         MK_SC (0x4B,0) // Keypad 4 / Left (0x4B)
#define SC_LEFT         MK_SC (0x4B,1) // Left (0xE0,0x4B)
#define SC_NP_5         MK_SC (0x4C,0) // Keypad 5
#define SC_NP_6         MK_SC (0x4D,0) // Keypad 6 / Right (0x4D)
#define SC_RIGHT        MK_SC (0x4D,1) // Right (0xE0,0x4D)
#define SC_NP_PLUS      MK_SC (0x4E,0) // Keypad +
#define SC_NP_1         MK_SC (0x4F,0) // Keypad 1 / End (0x4F)
#define SC_END          MK_SC (0x4F,1) // End (0xE0,0x4F)
#define SC_NP_2         MK_SC (0x50,0) // Keypad 2 / DownArrow (0x50)
#define SC_DOWN         MK_SC (0x50,1) // Down (0xE0,0x50)
#define SC_NP_3         MK_SC (0x51,0) // Keypad 3 / PgDn (0x51)
#define SC_PAGEDOWN     MK_SC (0x51,1) // Page Down (0xE0,0x51)
#define SC_NP_0         MK_SC (0x52,0) // Keypad 0 / Ins (0x52)
#define SC_INSERT       MK_SC (0x52,1) // Insert (0xE0,0x52)
#define SC_NP_PERIOD    MK_SC (0x53,0) // Keypad . / Del (0x53)
#define SC_DELETE       MK_SC (0x53,1) // Delete (0xE0,0x53)
#define SC_SYSRQ        MK_SC (0x54,0) // Prt Scr / SysRq
#define SC_FN           MK_SC (0x55,0) // Fn
#define SC_MACRO        MK_SC (0x56,0) // Macro
#define SC_F11          MK_SC (0x57,0) // F11
#define SC_F12          MK_SC (0x58,0) // F12
#define SC_UNK_59       MK_SC (0x59,0) // ?
#define SC_UNK_5A       MK_SC (0x5A,0) // ?
#define SC_F13          MK_SC (0x5B,0) // F13 (0x5B)
#define SC_LGUI         MK_SC (0x5B,1) // Left GUI (0xE0,0x5B)
#define SC_F14          MK_SC (0x5C,0) // F14
#define SC_RGUI         MK_SC (0x5C,1) // Right GUI (0xE0,0x5C)
#define SC_F15          MK_SC (0x5D,0) // F15 (0x5D)
#define SC_MENU         MK_SC (0x5D,1) // Menu (0xE0,0x5D)
#define SC_UNK_5E       MK_SC (0x5E,0) // ?
#define SC_UNK_5F       MK_SC (0x5F,0) // ?
#define SC_UNK_60       MK_SC (0x60,0) // ? (= 0xE0 & 0x7F)
#define SC_UNK_61       MK_SC (0x61,0) // ?
#define SC_UNK_62       MK_SC (0x62,0) // ? (= 0xE2 & 0x7F)
#define SC_F16          MK_SC (0x63,0) // F16
#define SC_F17          MK_SC (0x64,0) // F17
#define SC_F18          MK_SC (0x65,0) // F18
#define SC_F19          MK_SC (0x66,0) // F19
#define SC_F20          MK_SC (0x67,0) // F20
#define SC_F21          MK_SC (0x68,0) // F21
#define SC_F22          MK_SC (0x69,0) // F22
#define SC_F23          MK_SC (0x6A,0) // F23
#define SC_F24          MK_SC (0x6B,0) // F24
#define SC_UNK_6C       MK_SC (0x6C,0) // ?
#define SC_UNK_6D       MK_SC (0x6D,0) // ?
#define SC_UNK_6E       MK_SC (0x6E,0) // ?
#define SC_UNK_6F       MK_SC (0x6F,0) // ?
#define SC_UNK_70       MK_SC (0x70,0) // ?
#define SC_UNK_71       MK_SC (0x71,0) // ?
#define SC_UNK_72       MK_SC (0x72,0) // ?
#define SC_UNK_73       MK_SC (0x73,0) // ?
#define SC_UNK_74       MK_SC (0x74,0) // ?
#define SC_UNK_75       MK_SC (0x75,0) // ?
#define SC_UNK_76       MK_SC (0x76,0) // ?
#define SC_UNK_77       MK_SC (0x77,0) // ?
#define SC_UNK_78       MK_SC (0x78,0) // ?
#define SC_UNK_79       MK_SC (0x79,0) // ?
#define SC_UNK_7A       MK_SC (0x7A,0) // ?
#define SC_UNK_7B       MK_SC (0x7B,0) // ?
#define SC_UNK_7C       MK_SC (0x7C,0) // ?
#define SC_UNK_7D       MK_SC (0x7D,0) // ?
#define SC_UNK_7E       MK_SC (0x7E,0) // ?
#define SC_UNK_7F       MK_SC (0x7F,0) // ?

#if !GO32
// Fictive scan codes
#define SC_LALT         MK_SC (0xFC,0) // Left ALT
#define SC_RALT         MK_SC (0xFD,0) // Right ALT
#define SC_LCTRL        MK_SC (0xFE,0) // Left CTRL
#define SC_RCTRL        MK_SC (0xFF,0) // Right CTRL
#endif // !GO32

/* AT2 extended key aliases based on scan codes */

#define MK_EKEYSC(sc,ch) (((sc)<<8)+(ch)) // Scan code and ASCII character

#define kNULL    ATEK_NULL
#define kUNKN    ATEK_UNKNOWN

#define kESC     MK_EKEYSC (SC_ESCAPE, '\x1B')   // 0x01 Esc
// 0x02 1 ! (SC_1)
// 0x03 2 @ (SC_2)
// 0x04 3 # (SC_3)
// 0x05 4 $ (SC_4)
// 0x06 5 % (SC_5)
// 0x07 6 ^ (SC_6)
// 0x08 7 & (SC_7)
#define kAstrsk  MK_EKEYSC (SC_8, '*')           // 0x09 8 *
#define kCHastr  MK_EKEYSC (SC_8, '*')           // 0x09 8 *
#define kNINE    MK_EKEYSC (SC_9, '9')           // 0x0A 9 (
#define kZERO    MK_EKEYSC (SC_0, '0')           // 0x0B 0 )
#define kCHmins  MK_EKEYSC (SC_MINUS, '-')       // 0x0C - _
#define kCHplus  MK_EKEYSC (SC_EQUALS, '+')      // 0x0D = +
#define kBkSPC   MK_EKEYSC (SC_BACKSPACE, '\b')  // 0x0E BS
#define kCtBkSp  MK_EKEYSC (SC_BACKSPACE, 0x7F)  // 0x0E BS
#define kTAB     MK_EKEYSC (SC_TAB, '\t')        // 0x0F Tab
#define kCtrlQ   MK_EKEYSC (SC_Q, 'Q'-'A'+1)     // 0x10 Ctrl + Q
#define kCtrlW   MK_EKEYSC (SC_W, 'W'-'A'+1)     // 0x11 Ctrl + W
#define kCtrlE   MK_EKEYSC (SC_E, 'E'-'A'+1)     // 0x12 Ctrl + E
#define kCtrlR   MK_EKEYSC (SC_R, 'R'-'A'+1)     // 0x13 Ctrl + R
#define kCtrlT   MK_EKEYSC (SC_T, 'T'-'A'+1)     // 0x14 Ctrl + T
#define kCtrlY   MK_EKEYSC (SC_Y, 'Y'-'A'+1)     // 0x15 Ctrl + Y
#define kCtrlU   MK_EKEYSC (SC_U, 'U'-'A'+1)     // 0x16 Ctrl + U
#define kCtrlI   MK_EKEYSC (SC_I, 'I'-'A'+1)     // 0x17 Ctrl + I
#define kCtrlO   MK_EKEYSC (SC_O, 'O'-'A'+1)     // 0x18 Ctrl + O
#define kCtrlP   MK_EKEYSC (SC_P, 'P'-'A'+1)     // 0x19 Ctrl + P
#define kCHlbrk  MK_EKEYSC (SC_LEFTBRACKET, '[')        // 0x1A Ctrl(?) + [ {
#define kCtLbr   MK_EKEYSC (SC_LEFTBRACKET, '['-'A'+1)  // 0x1A Ctrl + [ {
#define kCHrbrk  MK_EKEYSC (SC_RIGHTBRACKET, ']')       // 0x1B Ctrl(?) + ] }
#define kCtRbr   MK_EKEYSC (SC_RIGHTBRACKET, ']'-'A'+1) // 0x1B Ctrl + ] }
#define kENTER   MK_EKEYSC (SC_ENTER, '\r')      // 0x1C Enter
#define kCtENTR  MK_EKEYSC (SC_ENTER, '\n')      // 0x1C Ctrl + Enter
// 0x1D Left Ctrl (SC_LCTRL_REAL)
// 0xE0,0x1D Right Ctrl (SC_RCTRL_REAL)
#define kCtrlA   MK_EKEYSC (SC_A, 'A'-'A'+1)     // 0x1E Ctrl + A
#define kCtrlS   MK_EKEYSC (SC_S, 'S'-'A'+1)     // 0x1F Ctrl + S
#define kCtrlD   MK_EKEYSC (SC_D, 'D'-'A'+1)     // 0x20 Ctrl + D
#define kCtrlF   MK_EKEYSC (SC_F, 'F'-'A'+1)     // 0x21 Ctrl + F
#define kCtrlG   MK_EKEYSC (SC_G, 'G'-'A'+1)     // 0x22 Ctrl + G
#define kCtrlH   MK_EKEYSC (SC_H, 'H'-'A'+1)     // 0x23 Ctrl + H
#define kCtrlJ   MK_EKEYSC (SC_J, 'J'-'A'+1)     // 0x24 Ctrl + J
#define kCtrlK   MK_EKEYSC (SC_K, 'K'-'A'+1)     // 0x25 Ctrl + K
#define kCtrlL   MK_EKEYSC (SC_L, 'L'-'A'+1)     // 0x26 Ctrl + L
// 0x27 ; : (SC_SEMICOLON)
// 0x28 ' " (SC_QUOTE)
#define kBquote  MK_EKEYSC (SC_BACKQUOTE, '`')   // 0x29 ` ~
// 0x2A Left Shift / SC_LSHIFT
// 0xE0,0x2A Fake Left Shift / SC_LSHIFT_FAKE
#define kSlashR  MK_EKEYSC (SC_BACKSLASH, '\\')  // 0x2B \ |
#define kCtrlZ   MK_EKEYSC (SC_Z, 'Z'-'A'+1)     // 0x2C Ctrl + Z
#define kCtrlX   MK_EKEYSC (SC_X, 'X'-'A'+1)     // 0x2D Ctrl + X
#define kCtrlC   MK_EKEYSC (SC_C, 'C'-'A'+1)     // 0x2E Ctrl + C
#define kCtrlV   MK_EKEYSC (SC_V, 'V'-'A'+1)     // 0x2F Ctrl + V
#define kCtrlB   MK_EKEYSC (SC_B, 'B'-'A'+1)     // 0x30 Ctrl + B
#define kCtrlN   MK_EKEYSC (SC_N, 'N'-'A'+1)     // 0x31 Ctrl + N
#define kCtrlM   MK_EKEYSC (SC_M, 'M'-'A'+1)     // 0x32 Ctrl + M
// 0x33 , < (SC_COMMA)
// 0x34 . > (SC_PERIOD)
#define kSlash   MK_EKEYSC (SC_SLASH, '/')       // 0x35 / ?
// 0x36 Right Shift (SC_RSHIFT)
// 0xE0,0x36 Fake Right Shift (SC_RSHIFT_FAKE)
#define kNPastr  MK_EKEYSC (SC_NP_MULTIPLY, '*') // 0x37 Keypad *
// 0xE0,0x37 Ctrl + PrtScr (SC_PRTSCR)
// 0x38 Left Alt (SC_LALT)
// 0xE0,0x38 Right Alt (SC_RALT)
#define kSPACE   MK_EKEYSC (' ' , SC_SPACE)      // 0x39 Space
// 0x3A Caps Lock (SC_CAPSLOCK)
// 0x3B F1
// 0x3C F2
// 0x3D F3
// 0x3E F4
// 0x3F F5
// 0x40 F6
// 0x41 F7
// 0x42 F8
// 0x43 F9
// 0x44 F10
// 0x45 Num Lock (SC_NUMLOCK)
// 0x46 Scroll Lock (SC_SCROLLLOCK)
#define kNPHOME  MK_EKEYSC (SC_NP_7, '7')        // 0x47 Keypad 7
#define kNPUp    MK_EKEYSC (SC_NP_8, '8')        // 0x48 Keypad 8
#define kNPPgUP  MK_EKEYSC (SC_NP_9, '9')        // 0x49 Keypad 9
#define kNPmins  MK_EKEYSC (SC_NP_MINUS, '-')    // 0x4A Keypad -
#define kShLEFT  MK_EKEYSC (SC_NP_4, '4')        // 0x4B Keypad 4
#define kNP5     MK_EKEYSC (SC_NP_5, 0)          // 0x4C Keypad 5
#define kShRGHT  MK_EKEYSC (SC_NP_6, '6')        // 0x4D Keypad 6
#define kNPplus  MK_EKEYSC (SC_NP_PLUS, '+')     // 0x4E Keypad +
#define kNPEND   MK_EKEYSC (SC_NP_1, '1')        // 0x4F Keypad 1
#define kNPDown  MK_EKEYSC (SC_NP_2, '2')        // 0x50 Keypad 2
#define kNPPgDN  MK_EKEYSC (SC_NP_3, '3')        // 0x51 Keypad 3
#define kNPins   MK_EKEYSC (SC_NP_0, '0')        // 0x52 Keypad 0
#define kNPdel   MK_EKEYSC (SC_NP_PERIOD, '.')   // 0x53 Keypad .
// 0x54 SysRq (SC_SYSRQ)
// 0x55 Fn (SC_FN)
// 0x56 Macro (SC_MACRO)
// 0x57 F11 (SC_F11)
// 0x58 F12 (SC_F12)
// 0x59 ? (SC_UNK_59)
// 0x5A ? (SC_UNK_5A)
// 0x5B F13 (SC_F13)
// 0xE0,0x5B Left GUI (SC_LGUI)
// 0x5C F14 (SC_F14)
// 0xE0,0x5C Right GUI (SC_RGUI)
// 0x5D F15 (SC_F15)
// 0xE0,0x5D Menu (SC_MENU)
// 0x5E ? (SC_UNK_5E)
// 0x5F ? (SC_UNK_5F)
// 0x60 ? (SC_UNK_60)
// 0x61 ? (SC_UNK_61)
// 0x62 ? (SC_UNK_62)
// 0x63 F16 (SC_F16)
// 0x64 F17 (SC_F17)
// 0x65 F18 (SC_F18)
// 0x66 F19 (SC_F19)
// 0x67 F20 (SC_F20)
// 0x68 F21 (SC_F21)
// 0x69 F22 (SC_F22)
// 0x6A F23 (SC_F23)
// 0x6B F24 (SC_F24)
// 0x6C ? (SC_UNK_6C)
// 0x6D ? (SC_UNK_6D)
// 0x6E ? (SC_UNK_6E)
// 0x6F ? (SC_UNK_6F)
// 0x70 ? (SC_UNK_70)
// 0x71 ? (SC_UNK_71)
// 0x72 ? (SC_UNK_72)
// 0x73 ? (SC_UNK_73)
// 0x74 ? (SC_UNK_74)
// 0x75 ? (SC_UNK_75)
// 0x76 ? (SC_UNK_76)
// 0x77 ? (SC_UNK_77)
// 0x78 ? (SC_UNK_78)
// 0x79 ? (SC_UNK_79)
// 0x7A ? (SC_UNK_7A)
// 0x7B ? (SC_UNK_7B)
// 0x7C ? (SC_UNK_7C)
// 0x7D ? (SC_UNK_7D)
// 0x7E ? (SC_UNK_7E)
// 0x7F ? (SC_UNK_7F)

/* AT2 extended keys based on extended ASCII codes */

#define MK_EKEYEA(ea) ((ea)<<8) // Extended ASCII code and '\0' character

#define kAltESC  MK_EKEYEA (0x01) // Alt + Esc
#define kCtrl1   MK_EKEYEA (0x02) // Ctrl + 1
#define kCtrl2   MK_EKEYEA (0x03) // Ctrl + 2
#define kCtrl3   MK_EKEYEA (0x04) // Ctrl + 3
#define kCtrl4   MK_EKEYEA (0x05) // Ctrl + 4
#define kCtrl5   MK_EKEYEA (0x06) // Ctrl + 5
#define kCtrl6   MK_EKEYEA (0x07) // Ctrl + 6
#define kCtrl7   MK_EKEYEA (0x08) // Ctrl + 7
#define kCtrl8   MK_EKEYEA (0x09) // Ctrl + 8
#define kCtrl9   MK_EKEYEA (0x0A) // Ctrl + 9
#define kCtrl0   MK_EKEYEA (0x0B) // Ctrl + 0
#define kCtMins  MK_EKEYEA (0x0C) // Ctrl + -
#define kCtEqu   MK_EKEYEA (0x0D) // Ctrl + =
#define kAlBkSp  MK_EKEYEA (0x0E) // Alt + BS
#define kShTAB   MK_EKEYEA (0x0F) // Shift + Tab
#define kAltQ    MK_EKEYEA (0x10) // Alt + Q
#define kAltW    MK_EKEYEA (0x11) // Alt + W
#define kAltE    MK_EKEYEA (0x12) // Alt + E
#define kAltR    MK_EKEYEA (0x13) // Alt + R
#define kAltT    MK_EKEYEA (0x14) // Alt + T
#define kAltY    MK_EKEYEA (0x15) // Alt + Y
#define kAltU    MK_EKEYEA (0x16) // Alt + U
#define kAltI    MK_EKEYEA (0x17) // Alt + I
#define kAltO    MK_EKEYEA (0x18) // Alt + O
#define kAltP    MK_EKEYEA (0x19) // Alt + P
#define kAlLbr   MK_EKEYEA (0x1A) // Alt + [
#define kAlRbr   MK_EKEYEA (0x1B) // Alt + ]
#define kAlENTR  MK_EKEYEA (0x1C) // Alt + Enter
// 0x1D ? (Alt + Ctrl ?)
#define kAltA    MK_EKEYEA (0x1E) // Alt + A
#define kAltS    MK_EKEYEA (0x1F) // Alt + S
#define kAltD    MK_EKEYEA (0x20) // Alt + D
#define kAltF    MK_EKEYEA (0x21) // Alt + F
#define kAltG    MK_EKEYEA (0x22) // Alt + G
#define kAltH    MK_EKEYEA (0x23) // Alt + H
#define kAltJ    MK_EKEYEA (0x24) // Alt + J
#define kAltK    MK_EKEYEA (0x25) // Alt + K
#define kAltL    MK_EKEYEA (0x26) // Alt + L
// 0x27 Alt + ;
// 0x28 Alt + '
#define kAlBquot MK_EKEYEA (0x29) // Alt + `
// 0x2A ? (Alt + Left Shift ?)
// 0x2B Alt + '\'
#define kAltZ    MK_EKEYEA (0x2C) // Alt + Z
#define kAltX    MK_EKEYEA (0x2D) // Alt + X
#define kAltC    MK_EKEYEA (0x2E) // Alt + C
#define kAltV    MK_EKEYEA (0x2F) // Alt + V
#define kAltB    MK_EKEYEA (0x30) // Alt + B
#define kAltN    MK_EKEYEA (0x31) // Alt + N
#define kAltM    MK_EKEYEA (0x32) // Alt + M
// 0x33 Alt + ,
// 0x34 Alt + .
// 0x35 Alt + /
// 0x36 ? (Alt + Right Shift ?)
// 0x37 Alt + Keypad *
// 0x38 ? (Left Alt + Right Alt ?)
// 0x39 ? (Alt + Space ?)
// 0x3A ? (Alt + Caps Lock ?)
#define kF1      MK_EKEYEA (0x3B) // F1
#define kF2      MK_EKEYEA (0x3C) // F2
#define kF3      MK_EKEYEA (0x3D) // F3
#define kF4      MK_EKEYEA (0x3E) // F4
#define kF5      MK_EKEYEA (0x3F) // F5
#define kF6      MK_EKEYEA (0x40) // F6
#define kF7      MK_EKEYEA (0x41) // F7
#define kF8      MK_EKEYEA (0x42) // F8
#define kF9      MK_EKEYEA (0x43) // F9
#define kF10     MK_EKEYEA (0x44) // F10
// 0x45 ? (Alt + Num Lock ?)
// 0x46 ? (Alt + Scroll Lock ?)
#define kHOME    MK_EKEYEA (0x47) // Home
#define kUP      MK_EKEYEA (0x48) // Up
#define kPgUP    MK_EKEYEA (0x49) // Page Up
// 0x4A Alt + Keypad -
#define kLEFT    MK_EKEYEA (0x4B) // Left
// 0x4C ? (Alt + Keypad 5 ?)
#define kRIGHT   MK_EKEYEA (0x4D) // Right
// 0x4E Alt + Keypad +
#define kEND     MK_EKEYEA (0x4F) // End
#define kDOWN    MK_EKEYEA (0x50) // Down
#define kPgDOWN  MK_EKEYEA (0x51) // Page Down
#define kINSERT  MK_EKEYEA (0x52) // Insert
#define kDELETE  MK_EKEYEA (0x53) // Delete
#define kShF1    MK_EKEYEA (0x54) // Shift + F1
#define kShF2    MK_EKEYEA (0x55) // Shift + F2
#define kShF3    MK_EKEYEA (0x56) // Shift + F3
#define kShF4    MK_EKEYEA (0x57) // Shift + F4
#define kShF5    MK_EKEYEA (0x58) // Shift + F5
#define kShF6    MK_EKEYEA (0x59) // Shift + F6
#define kShF7    MK_EKEYEA (0x5A) // Shift + F7
#define kShF8    MK_EKEYEA (0x5B) // Shift + F8
#define kShF9    MK_EKEYEA (0x5C) // Shift + F9
#define kShF10   MK_EKEYEA (0x5D) // Shift + F10
#define kCtrlF1  MK_EKEYEA (0x5E) // Ctrl + F1
#define kCtrlF2  MK_EKEYEA (0x5F) // Ctrl + F2
#define kCtrlF3  MK_EKEYEA (0x60) // Ctrl + F3
#define kCtrlF4  MK_EKEYEA (0x61) // Ctrl + F4
#define kCtrlF5  MK_EKEYEA (0x62) // Ctrl + F5
#define kCtrlF6  MK_EKEYEA (0x63) // Ctrl + F6
#define kCtrlF7  MK_EKEYEA (0x64) // Ctrl + F7
#define kCtrlF8  MK_EKEYEA (0x65) // Ctrl + F8
#define kCtrlF9  MK_EKEYEA (0x66) // Ctrl + F9
#define kCtrlF10 MK_EKEYEA (0x67) // Ctrl + F10
#define kAltF1   MK_EKEYEA (0x68) // Alt + F1
#define kAltF2   MK_EKEYEA (0x69) // Alt + F2
#define kAltF3   MK_EKEYEA (0x6A) // Alt + F3
#define kAltF4   MK_EKEYEA (0x6B) // Alt + F4
#define kAltF5   MK_EKEYEA (0x6C) // Alt + F5
#define kAltF6   MK_EKEYEA (0x6D) // Alt + F6
#define kAltF7   MK_EKEYEA (0x6E) // Alt + F7
#define kAltF8   MK_EKEYEA (0x6F) // Alt + F8
#define kAltF9   MK_EKEYEA (0x70) // Alt + F9
#define kAltF10  MK_EKEYEA (0x71) // Alt + F10
// 0x72 SysRq
#define kCtLEFT  MK_EKEYEA (0x73) // Ctrl + Left
#define kCtRGHT  MK_EKEYEA (0x74) // Ctrl + Right
#define kCtEND   MK_EKEYEA (0x75) // Ctrl + End
#define kCtPgDN  MK_EKEYEA (0x76) // Ctrl + Page Down
#define kCtHOME  MK_EKEYEA (0x77) // Ctrl + Home
#define kAlt1    MK_EKEYEA (0x78) // Alt + 1
#define kAlt2    MK_EKEYEA (0x79) // Alt + 2
#define kAlt3    MK_EKEYEA (0x7A) // Alt + 3
#define kAlt4    MK_EKEYEA (0x7B) // Alt + 4
#define kAlt5    MK_EKEYEA (0x7C) // Alt + 5
#define kAlt6    MK_EKEYEA (0x7D) // Alt + 6
#define kAlt7    MK_EKEYEA (0x7E) // Alt + 7
#define kAlt8    MK_EKEYEA (0x7F) // Alt + 8
#define kAlt9    MK_EKEYEA (0x80) // Alt + 9
#define kAlt0    MK_EKEYEA (0x81) // Alt + 0
#define kAltMin  MK_EKEYEA (0x82) // Alt + -
#define kAltEqu  MK_EKEYEA (0x83) // Alt + =
#define kCtPgUP  MK_EKEYEA (0x84) // Ctrl + PgUp
#if 1
#define kF11     MK_EKEYEA (0x85) // F11
#define kF12     MK_EKEYEA (0x86) // F12
#else // 0
#define kF11     MK_EKEYEA (0x45) // Num Lock
#define kF12     MK_EKEYEA (0x46) // Scroll Lock
#endif // 0
#define kShF11   MK_EKEYEA (0x87) // Shift + F11
#define kShF12   MK_EKEYEA (0x88) // Shift + F12
#define kCtrlF11 MK_EKEYEA (0x89) // Ctrl + F11
#define kCtrlF12 MK_EKEYEA (0x8A) // Ctrl + F12
#define kAltF11  MK_EKEYEA (0x8B) // Alt + F11
#define kAltF12  MK_EKEYEA (0x8C) // Alt + F12
#define kCtNP8   MK_EKEYEA (0x8D) // Ctrl + Keypad 8
#define kCtNPmin MK_EKEYEA (0x8E) // Ctrl + Keypad -
#define kCtNP5   MK_EKEYEA (0x8F) // Ctrl + Keypad 5
#define kCtNPpls MK_EKEYEA (0x90) // Ctrl + Keypad +
#define kCtNP2   MK_EKEYEA (0x91) // Ctrl + Keypad 2
#define kCtNP0   MK_EKEYEA (0x92) // Ctrl + Keypad 0
#define kCtNPper MK_EKEYEA (0x93) // Ctrl + Keypad .
#define kCtTAB   MK_EKEYEA (0x94) // Ctrl + Tab
#define kCtNPdiv MK_EKEYEA (0x95) // Ctrl + Keypad /
#define kCtNPmul MK_EKEYEA (0x96) // Ctrl + Keypad *
#define kAltHOME MK_EKEYEA (0x97) // Alt + Home
#define kAltUP   MK_EKEYEA (0x98) // Alt + Up
#define kAltPgUP MK_EKEYEA (0x99) // Alt + Page Up
// 0x9A ?
#define kAltLT   MK_EKEYEA (0x9B) // Alt + Left
// 0x9C ?
#define kAltRGHT MK_EKEYEA (0x9D) // Alt + Right
// 0x9E ?
#define kAltEND  MK_EKEYEA (0x9F) // Alt + End
#define kAltDOWN MK_EKEYEA (0xA0) // Alt + Down
#define kAltPgDN MK_EKEYEA (0xA1) // Alt + Page Down
#define kAltINS  MK_EKEYEA (0xA2) // Alt + Insert
#define kAltDEL  MK_EKEYEA (0xA3) // Alt + Delete
#define kAlNPdiv MK_EKEYEA (0xA4) // Alt + Keypad /
#define kAltTAB  MK_EKEYEA (0xA5) // Alt + Tab
#define kAlNPent MK_EKEYEA (0xA6) // Alt + Keypad Enter

#if GO32
extern bool keyboard_sleep;
extern bool CTRL_ALT_DEL_pressed;
extern bool _ctrl_pressed;
extern bool _2x_ctrl_pressed;
#endif // GO32

void keyboard_init (void);
void keyboard_done (void);
void keyboard_reset_buffer (void);
#if GO32
void keyboard_reset_buffer_alt (void);
void keyboard_toggle_sleep (void);
#endif // GO32
void wait_until_F11_F12_released (void);
void keyboard_poll_input (void);
bool keypressed (void);
ExtKeyCode GetKey (void);
bool scankey (uint8_t scancode);
bool CapsLock (void);
bool NumLock (void);
#if GO32
bool ScrollLock (void);
#endif // GO32
bool shift_pressed (void);
bool left_shift_pressed (void);
bool right_shift_pressed (void);
#if GO32
bool both_shifts_pressed (void);
#endif // GO32
bool alt_pressed (void);
bool ctrl_pressed (void);
bool ctrl_tab_pressed (void);
bool LookUpKey (ExtKeyCode key, const ExtKeyCode *table, size_t size);
void screen_saver (void);

#endif // !defined(ADT2KEYB_H)
