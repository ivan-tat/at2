// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

const
  SYMTABSIZE = $65;
  symtab: array[0..PRED(SYMTABSIZE*10)] of word = (

{ Key   Scan    ASCII   Shift   Ctrl    Alt     Num     Caps    Sh+Caps Sh+Num  SDLK_Keycode}
{ Esc } $01,    $1B,    $1B,    $F000,  $F000,  $1B,    $1B,    $1B,    $1B,    SDLK_ESCAPE,
{ 1! }  $02,    $31,    $21,    $0200,  $7800,  $31,    $31,    $21,    $21,    SDLK_1,
{ 2@ }  $03,    $32,    $40,    $0300,  $7900,  $32,    $32,    $40,    $40,    SDLK_2,
{ 3# }  $04,    $33,    $23,    $0400,  $7A00,  $33,    $33,    $23,    $23,    SDLK_3,
{ 4$ }  $05,    $34,    $24,    $0500,  $7B00,  $34,    $34,    $24,    $24,    SDLK_4,
{ 5% }  $06,    $35,    $25,    $0600,  $7C00,  $35,    $35,    $25,    $25,    SDLK_5,
{ 6^ }  $07,    $36,    $5E,    $0700,  $7D00,  $36,    $36,    $5E,    $5E,    SDLK_6,
{ 7& }  $08,    $37,    $26,    $0800,  $7E00,  $37,    $37,    $26,    $26,    SDLK_7,
{ 8* }  $09,    $38,    $2A,    $0900,  $7F00,  $38,    $38,    $2A,    $2A,    SDLK_8,
{ 9( }  $0A,    $39,    $28,    $0A00,  $8000,  $39,    $39,    $28,    $28,    SDLK_9,
{ 0) }  $0B,    $30,    $29,    $0B00,  $8100,  $30,    $30,    $29,    $29,    SDLK_0,
{ -_ }  $0C,    $2D,    $5F,    $1F,    $8200,  $2D,    $2D,    $5F,    $5F,    SDLK_MINUS,
{ =+ }  $0D,    $3D,    $2B,    $F000,  $8300,  $3D,    $3D,    $2B,    $2B,    SDLK_EQUALS,
{Bksp}  $0E,    $08,    $08,    $7F,    $F000,  $08,    $08,    $08,    $08,    SDLK_BACKSPACE,
{ Tab}  $0F,    $09,    $0F00,  $F000,  $F000,  $09,    $09,    $0F00,  $0F00,  SDLK_TAB,
{ Q }   $10,    $71,    $51,    $11,    $1000,  $71,    $51,    $71,    $51,    SDLK_q,
{ W }   $11,    $77,    $57,    $17,    $1100,  $77,    $57,    $77,    $57,    SDLK_w,
{ E }   $12,    $65,    $45,    $05,    $1200,  $65,    $45,    $65,    $45,    SDLK_e,
{ R }   $13,    $72,    $52,    $12,    $1300,  $72,    $52,    $72,    $52,    SDLK_r,
{ T }   $14,    $74,    $54,    $14,    $1400,  $74,    $54,    $74,    $54,    SDLK_t,
{ Y }   $15,    $79,    $59,    $19,    $1500,  $79,    $59,    $79,    $59,    SDLK_y,
{ U }   $16,    $75,    $55,    $15,    $1600,  $75,    $55,    $75,    $55,    SDLK_u,
{ I }   $17,    $69,    $49,    $09,    $1700,  $69,    $49,    $69,    $49,    SDLK_i,
{ O }   $18,    $6F,    $4F,    $0F,    $1800,  $6F,    $4F,    $6F,    $4F,    SDLK_o,
{ P }   $19,    $70,    $50,    $10,    $1900,  $70,    $50,    $70,    $50,    SDLK_p,
{ [ }   $1A,    $5B,    $7B,    $1B,    $F000,  $5B,    $5B,    $7B,    $7B,    SDLK_LEFTBRACKET,
{ ] }   $1B,    $5D,    $7D,    $1D,    $F000,  $5D,    $5D,    $7D,    $7D,    SDLK_RIGHTBRACKET,
{enter} $1C,    $0D,    $0D,    $0A,    $F000,  $0D,    $0D,    $0D,    $0D,    SDLK_RETURN,
{ A }   $1E,    $61,    $41,    $01,    $1E00,  $61,    $41,    $61,    $41,    SDLK_a,
{ S }   $1F,    $73,    $53,    $13,    $1F00,  $73,    $53,    $73,    $53,    SDLK_s,
{ D }   $20,    $64,    $44,    $04,    $2000,  $64,    $44,    $64,    $44,    SDLK_d,
{ F }   $21,    $66,    $46,    $06,    $2100,  $66,    $46,    $66,    $46,    SDLK_f,
{ G }   $22,    $67,    $47,    $07,    $2200,  $67,    $47,    $67,    $47,    SDLK_g,
{ H }   $23,    $68,    $48,    $08,    $2300,  $68,    $48,    $68,    $48,    SDLK_h,
{ J }   $24,    $6A,    $4A,    $0A,    $2400,  $6A,    $4A,    $6A,    $4A,    SDLK_j,
{ K }   $25,    $6B,    $4B,    $0B,    $2500,  $6B,    $4B,    $6B,    $4B,    SDLK_k,
{ L }   $26,    $6C,    $4C,    $0C,    $2600,  $6C,    $4C,    $6C,    $4C,    SDLK_l,
{ ;:}   $27,    $3B,    $3A,    $F000,  $F000,  $3B,    $3B,    $3A,    $3A,    SDLK_SEMICOLON,
{ '"}   $28,    $27,    $22,    $F000,  $F000,  $27,    $27,    $22,    $22,    SDLK_QUOTE,
{ `~}   $29,    $60,    $7E,    $F000,  $F000,  $60,    $60,    $7E,    $7E,    SDLK_BACKQUOTE,
{Lshft} $2A,    $FFFF,  $FFFF,  $FFFF,  $FFFF,  $FFFF,  $FFFF,  $FFFF,  $FFFF,  SDLK_LSHIFT,
{ \|}   $2B,    $5C,    $7C,    $1C,    $F000,  $5C,    $5C,    $7C,    $7C,    SDLK_BACKSLASH,
{ Z }   $2C,    $7A,    $5A,    $1A,    $2C00,  $7A,    $5A,    $7A,    $5A,    SDLK_z,
{ X }   $2D,    $78,    $58,    $18,    $2D00,  $78,    $58,    $78,    $58,    SDLK_x,
{ C }   $2E,    $63,    $43,    $03,    $2E00,  $63,    $43,    $63,    $43,    SDLK_c,
{ V }   $2F,    $76,    $56,    $16,    $2F00,  $76,    $56,    $76,    $56,    SDLK_v,
{ B }   $30,    $62,    $42,    $02,    $3000,  $62,    $42,    $62,    $42,    SDLK_b,
{ Key   Scan    ASCII   Shift   Ctrl    Alt     Num     Caps    Sh+Caps Sh+Num  SDLK_Keycode}
{ N }   $31,    $6E,    $4E,    $0E,    $3100,  $6E,    $4E,    $6E,    $4E,    SDLK_n,
{ M }   $32,    $6D,    $4D,    $0D,    $3200,  $6D,    $4D,    $6D,    $4D,    SDLK_m,
{ ,<}   $33,    $2C,    $3C,    $F000,  $F000,  $2C,    $2C,    $3C,    $3C,    SDLK_COMMA,
{ .>}   $34,    $2E,    $3E,    $F000,  $F000,  $2E,    $2E,    $3E,    $3E,    SDLK_PERIOD,
{ /?}   $35,    $2F,    $3F,    $F000,  $F000,  $2F,    $2F,    $3F,    $3F,    SDLK_SLASH,
{Rshft} $36,    $FFFF,  $FFFF,  $FFFF,  $FFFF,  $FFFF,  $FFFF,  $FFFF,  $FFFF,  SDLK_RSHIFT,
{PrtSc} $37,    $2A,    $F000,  $F000,  $F000,  $2A,    $2A,    $F000,  $F000,  SDLK_PRINT,
{space} $39,    $20,    $20,    $20,    $20,    $20,    $20,    $20,    $20,    SDLK_SPACE,
{caps}  $3A,    $FFFF,  $FFFF,  $FFFF,  $FFFF,  $FFFF,  $FFFF,  $FFFF,  $FFFF,  SDLK_CAPSLOCK,
{ F1 }  $3B,    $3B00,  $5400,  $5E00,  $6800,  $3B00,  $3B00,  $5400,  $5400,  SDLK_F1,
{ F2 }  $3C,    $3C00,  $5500,  $5F00,  $6900,  $3C00,  $3C00,  $5500,  $5500,  SDLK_F2,
{ F3 }  $3D,    $3D00,  $5600,  $6000,  $6A00,  $3D00,  $3D00,  $5600,  $5600,  SDLK_F3,
{ F4 }  $3E,    $3E00,  $5700,  $6100,  $6B00,  $3E00,  $3E00,  $5700,  $5700,  SDLK_F4,
{ F5 }  $3F,    $3F00,  $5800,  $6200,  $6C00,  $3F00,  $3F00,  $5800,  $5800,  SDLK_F5,
{ F6 }  $40,    $4000,  $5900,  $6300,  $6D00,  $4000,  $4000,  $5900,  $5900,  SDLK_F6,
{ F7 }  $41,    $4100,  $5A00,  $6400,  $6E00,  $4100,  $4100,  $5A00,  $5A00,  SDLK_F7,
{ F8 }  $42,    $4200,  $5B00,  $6500,  $6F00,  $4200,  $4200,  $5B00,  $5B00,  SDLK_F8,
{ F9 }  $43,    $4300,  $5C00,  $6600,  $7000,  $4300,  $4300,  $5C00,  $5C00,  SDLK_F9,
{ F10}  $44,    $4400,  $5D00,  $6700,  $7100,  $4400,  $4400,  $5D00,  $5D00,  SDLK_F10,
{ num}  $45,    $FFFF,  $FFFF,  $FFFF,  $FFFF,  $FFFF,  $FFFF,  $FFFF,  $FFFF,  SDLK_NUMLOCK,
{scrl}  $46,    $FFFF,  $FFFF,  $FFFF,  $FFFF,  $FFFF,  $FFFF,  $FFFF,  $FFFF,  SDLK_SCROLLOCK,
{home}  $47,    $4700,  $4700,  $7700,  $F000,  $4700,  $4700,  $4700,  $4700,  SDLK_HOME,
{ up }  $48,    $0000,  $4800,  $F000,  $F000,  $4800,  $4800,  $4800,  $4800,  SDLK_UP,
{pgup}  $49,    $4900,  $4900,  $8400,  $F000,  $4900,  $4900,  $4900,  $4900,  SDLK_PAGEUP,
{ np-}  $4A,    $2D,    $2D,    $F000,  $F000,  $2D,    $2D,    $2D,    $2D,    SDLK_KP_MINUS,
{left}  $4B,    $4B00,  $4B00,  $7300,  $F000,  $4B00,  $4B00,  $4B00,  $4B00,  SDLK_LEFT,
{centr} $4C,    $4C00,  $4C00,  $F000,  $F000,  $4C00,  $4C00,  $4C00,  $4C00,  SDLK_UNKNOWN,
{right} $4D,    $4D00,  $4D00,  $7400,  $F000,  $4D00,  $4D00,  $4D00,  $4D00,  SDLK_RIGHT,
{ np+}  $4E,    $2B,    $2B,    $F000,  $F000,  $2B,    $2B,    $2B,    $2B,    SDLK_KP_PLUS,
{end}   $4F,    $4F00,  $4F00,  $7500,  $F000,  $4F00,  $4F00,  $4F00,  $4F00,  SDLK_END,
{down}  $50,    $0000,  $5000,  $F000,  $F000,  $5000,  $5000,  $5000,  $5000,  SDLK_DOWN,
{pgdn}  $51,    $5100,  $5100,  $7600,  $F000,  $5100,  $5100,  $5100,  $5100,  SDLK_PAGEDOWN,
{ ins}  $52,    $5200,  $5200,  $F000,  $F000,  $5200,  $5200,  $5200,  $5200,  SDLK_INSERT,
{ del}  $53,    $5300,  $5300,  $F000,  $F000,  $5300,  $5300,  $5300,  $5300,  SDLK_DELETE,
{ F11}  $57,    $4500,  $5E00,  $6800,  $F000,  $4500,  $4500,  $5E00,  $5E00,  SDLK_F11,
{ F12}  $58,    $4500,  $F000,  $6900,  $F000,  $4600,  $4600,  $5F00,  $5F00,  SDLK_F12,
{ np0 } $52,    $5200,  $5200,  $F000,  $F000,  $30,    $30,    $29,    $30,    SDLK_KP0,
{ np1 } $4F,    $4F00,  $4F00,  $F000,  $F000,  $31,    $31,    $21,    $31,    SDLK_KP1,
{ np2 } $50,    $5000,  $5000,  $F000,  $F000,  $32,    $32,    $40,    $32,    SDLK_KP2,
{ np3 } $51,    $5100,  $5100,  $F000,  $F000,  $33,    $33,    $23,    $33,    SDLK_KP3,
{ np4 } $4B,    $4B00,  $4B00,  $F000,  $F000,  $34,    $34,    $24,    $34,    SDLK_KP4,
{ np5 } $4C,    $4C00,  $4C00,  $F000,  $F000,  $35,    $35,    $25,    $35,    SDLK_KP5,
{ np6 } $4D,    $4D00,  $4D00,  $F000,  $F000,  $36,    $36,    $5E,    $36,    SDLK_KP6,
{ np7 } $47,    $4700,  $4700,  $F000,  $F000,  $37,    $37,    $26,    $37,    SDLK_KP7,
{ np8 } $48,    $4800,  $4800,  $F000,  $F000,  $38,    $38,    $2A,    $38,    SDLK_KP8,
{ np9 } $49,    $4900,  $4900,  $F000,  $F000,  $39,    $39,    $28,    $39,    SDLK_KP9,
{ np. } $53,    $5300,  $5300,  $F000,  $F000,  $2E,    $2E,    $3E,    $3E,    SDLK_KP_PERIOD,
{ np/ } $35,    $2F,    $2F,    $F000,  $F000,  $2F,    $2F,    $2F,    $2F,    SDLK_KP_DIVIDE,
{ np* } $37,    $2A,    $2A,    $F000,  $F000,  $2A,    $2A,    $2A,    $2A,    SDLK_KP_MULTIPLY,
{NPent} $1C,    $0D,    $0D,    $0A,    $F000,  $0D,    $0D,    $0A,    $0A,    SDLK_KP_ENTER,
{LALT}  $FC,    $FFFF,  $FFFF,  $FFFF,  $FFFF,  $FFFF,  $FFFF,  $FFFF,  $FFFF,  SDLK_LALT,
{RALT}  $FD,    $FFFF,  $FFFF,  $FFFF,  $FFFF,  $FFFF,  $FFFF,  $FFFF,  $FFFF,  SDLK_RALT,
{LCTRL} $FE,    $FFFF,  $FFFF,  $FFFF,  $FFFF,  $FFFF,  $FFFF,  $FFFF,  $FFFF,  SDLK_LCTRL,
{RCTRL} $FF,    $FFFF,  $FFFF,  $FFFF,  $FFFF,  $FFFF,  $FFFF,  $FFFF,  $FFFF,  SDLK_RCTRL
{ Key   Scan    ASCII   Shift   Ctrl    Alt     Num     Caps    Sh+Caps Sh+Num  SDLK_Keycode}
);
