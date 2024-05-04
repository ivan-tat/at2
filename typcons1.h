// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#define UINT8_NULL 0xFF
#define UINT16_NULL 0xFFFF
#define UINT32_NULL 0xFFFFFFFF
#define INT8_NULL (-1)
#define INT16_NULL (-1)
#define INT32_NULL (-1)
#define BYTE_NULL UINT8_NULL
#define WORD_NULL UINT16_NULL
#define DWORD_NULL UINT32_NULL

#if GO32

extern uint8_t screen_mode;
extern uint8_t comp_text_mode;

#endif // GO32

extern uint8_t command_typing;
extern bool    use_large_cursor;

extern CharSet_t _valid_characters; // HINT: (FPC) set of char
extern CharSet_t _valid_characters_fname; // HINT: (FPC) set of char

#define kESC     0x011B

#define kAstrsk  0x092A
#define kCHastr  0x092A
#define kCHmins  0x0C2D
#define kCHplus  0x0D2B
#define kCHlbrk  0x1A5B
#define kCHrbrk  0x1B5D
#define kBkSPC   0x0E08
#define kTAB     0x0F09
#define kENTER   0x1C0D
#define kSlashR  0x2B5C
#define kSlash   0x352F
#define kWeird   0x2960
#define kSPACE   0x3920

#define kShTAB   0x0F00

#define kCtBkSp  0x0E7F
#define kCtLbr   0x1A1B
#define kCtRbr   0x1B1D
#define kCtENTR  0x1C0A

#define kF1      0x3B00
#define kF2      0x3C00
#define kF3      0x3D00
#define kF4      0x3E00
#define kF5      0x3F00
#define kF6      0x4000
#define kF7      0x4100
#define kF8      0x4200
#define kF9      0x4300
#define kF10     0x4400

//#define kShF1    0x5400
#define kShF2    0x5500
#define kShF3    0x5600
//#define kShF4    0x5700
#define kShF5    0x5800
#define kShF6    0x5900
//#define kShF7    0x5A00
#define kShF8    0x5B00
#define kShF9    0x5C00
//#define kShF10   0x5D00

//#define kCtrlF1  0x5E00
#define kCtrlF2  0x5F00
#define kCtrlF3  0x6000
//#define kCtrlF4  0x6100
//#define kCtrlF5  0x6200
//#define kCtrlF6  0x6300
//#define kCtrlF7  0x6400
#define kCtrlF8  0x6500
#define kCtrlF9  0x6600
//#define kCtrlF10 0x6700

//#define kAltF1   0x6800
#define kAltF2   0x6900
//#define kAltF3   0x6A00
//#define kAltF4   0x6B00
#define kAltF5   0x6C00
#define kAltF6   0x6D00
//#define kAltF7   0x6E00
#define kAltF8   0x6F00
#define kAltF9   0x7000
//#define kAltF10  0x7100

#define kAlt1    0x7800
#define kAlt2    0x7900
#define kAlt3    0x7A00
#define kAlt4    0x7B00
#define kAlt5    0x7C00
#define kAlt6    0x7D00
#define kAlt7    0x7E00
#define kAlt8    0x7F00
#define kAlt9    0x8000
#define kAlt0    0x8100

#define kAltA    0x1E00
#define kAltB    0x3000
#define kAltC    0x2E00
#define kAltD    0x2000
#define kAltE    0x1200
#define kAltF    0x2100
#define kAltG    0x2200
#define kAltH    0x2300
#define kAltI    0x1700
//#define kAltJ    0x2400
#define kAltK    0x2500
#define kAltL    0x2600
#define kAltM    0x3200
#define kAltN    0x3100
#define kAltO    0x1800
#define kAltP    0x1900
#define kAltQ    0x1000
#define kAltR    0x1300
#define kAltS    0x1F00
#define kAltT    0x1400
#define kAltU    0x1600
#define kAltV    0x2F00
#define kAltW    0x1100
#define kAltX    0x2D00
//#define kAltY    0x1500
#define kAltZ    0x2C00

#define kCtrlA   0x1E01
#define kCtrlB   0x3002
#define kCtrlC   0x2E03
#define kCtrlD   0x2004
#define kCtrlE   0x1205
#define kCtrlF   0x2106
#define kCtrlG   0x2207
#define kCtrlH   0x2308
#define kCtrlI   0x1709
//#define kCtrlJ   0x240A
#define kCtrlK   0x250B
#define kCtrlL   0x260C
#define kCtrlM   0x320D
#define kCtrlN   0x310E
#define kCtrlO   0x180F
#define kCtrlP   0x1910
#define kCtrlQ   0x1011
#define kCtrlR   0x1312
#define kCtrlS   0x1F13
#define kCtrlT   0x1414
//#define kCtrlU   0x1615
#define kCtrlV   0x2F16
#define kCtrlW   0x1117
#define kCtrlX   0x2D18
#define kCtrlY   0x1519
#define kCtrlZ   0x2C1A

#define kINSERT  0x5200
#define kDELETE  0x5300
#define kHOME    0x4700
#define kEND     0x4F00
#define kPgUP    0x4900
#define kPgDOWN  0x5100

#define kCtHOME  0x7700
#define kCtEND   0x7500
#define kCtPgUP  0x8400
#define kCtPgDN  0x7600

#define kUP      0x4800
#define kDOWN    0x5000
#define kLEFT    0x4B00
#define kRIGHT   0x4D00

#define kShLEFT  0x4B34
#define kShRGHT  0x4D36

#define kCtLEFT  0x7300
#define kCtRGHT  0x7400

// Numeric pad
#define kNPastr  0x372A
#define kNPmins  0x4A2D
#define kNPplus  0x4E2B
#define kNPHOME  0x4737
#define kNPEND   0x4F31
#define kNPUp    0x4838
#define kNPDown  0x5032
#define kNPPgUP  0x4939
#define kNPPgDN  0x5133
#define kNPins   0x5230
#define kNPdel   0x532E

#define SC_ESCAPE       0x01
#define SC_1            0x02
#define SC_2            0x03
#define SC_3            0x04
#define SC_4            0x05
#define SC_5            0x06
#define SC_6            0x07
#define SC_7            0x08
#define SC_8            0x09
#define SC_9            0x0A
#define SC_0            0x0B
#define SC_MINUS        0x0C
#define SC_EQUALS       0x0D
#define SC_BACKSPACE    0x0E
#define SC_TAB          0x0F
#define SC_Q            0x10
#define SC_W            0x11
#define SC_E            0x12
#define SC_R            0x13
#define SC_T            0x14
#define SC_Y            0x15
#define SC_U            0x16
#define SC_I            0x17
#define SC_O            0x18
#define SC_P            0x19
#define SC_LEFTBRACKET  0x1A
#define SC_RIGHTBRACKET 0x1B
#define SC_ENTER        0x1C
#if GO32
#define SC_CTRL         0x1D
#endif // GO32
#define SC_A            0x1E
#define SC_S            0x1F
#define SC_D            0x20
#define SC_F            0x21
#define SC_G            0x22
#define SC_H            0x23
#define SC_J            0x24
#define SC_K            0x25
#define SC_L            0x26
#define SC_SEMICOLON    0x27
#define SC_QUOTE        0x28
#define SC_TILDE        0x29
#define SC_LSHIFT       0x2A
#define SC_BACKSLASH    0x2B
#define SC_Z            0x2C
#define SC_X            0x2D
#define SC_C            0x2E
#define SC_V            0x2F
#define SC_B            0x30
#define SC_N            0x31
#define SC_M            0x32
#define SC_COMMA        0x33
#define SC_PERIOD       0x34
#define SC_SLASH        0x35
#define SC_RSHIFT       0x36
#define SC_MULTIPLY     0x37
#if GO32
#define SC_ALT          0x38
#endif // GO32
#define SC_SPACE        0x39
#define SC_CAPSLOCK     0x3A
#define SC_F1           0x3B
#define SC_F2           0x3C
#define SC_F3           0x3D
#define SC_F4           0x3E
#define SC_F5           0x3F
#define SC_F6           0x40
#define SC_F7           0x41
#define SC_F8           0x42
#define SC_F9           0x43
#define SC_F10          0x44
#define SC_NUMLOCK      0x45
#define SC_SCROLLLOCK   0x46
#define SC_HOME         0x47
#define SC_UP           0x48
#define SC_PAGEUP       0x49
#define SC_MINUS2       0x4A
#define SC_LEFT         0x4B
#define SC_RIGHT        0x4D
#define SC_PLUS         0x4E
#define SC_END          0x4F
#define SC_DOWN         0x50
#define SC_PAGEDOWN     0x51
#define SC_INSERT       0x52
#define SC_DELETE       0x53
#define SC_F11          0x57
#define SC_F12          0x58
#define SC_LWIN         0x7D
#define SC_RWIN         0x7E
#define SC_MENU         0x7F
#if !GO32
// Fictive scan codes
#define SC_LALT         0xFC // Left ALT
#define SC_RALT         0xFD // Right ALT
#define SC_LCTRL        0xFE // Left CTRL
#define SC_RCTRL        0xFF // Right CTRL
#endif // !GO32
