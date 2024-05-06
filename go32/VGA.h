// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef GO32_VGA_H
#define GO32_VGA_H

#include <stdbool.h>
#include <stdint.h>

#define VGA_ATTR_WRITE_PORT     0x3C0 // Attribute Address/Data Register (write)
#define VGA_ATTR_READ_PORT      0x3C1 // Attribute Data Register (read)
#define VGA_STATUS_0_PORT       0x3C2 // Input Status #0 Register
#define VGA_STATUS_1_PORT       0x3DA // Input Status #1 Register
#define VGA_MISC_WRITE_PORT     0x3C2 // Miscellaneous Output Register (write)
#define VGA_MISC_READ_PORT      0x3CC // Miscellaneous Output Register (read)
#define VGA_ENABLE_PORT         0x3C3 // Video Subsystem Enable Register
#define VGA_SEQ_ADDR_PORT       0x3C4 // Sequencer Address Register
#define VGA_SEQ_DATA_PORT       0x3C5 // Sequencer Data Register
#define VGA_DAC_ADDR_READ_PORT  0x3C7 // DAC Address Read Mode Register
#define VGA_DAC_ADDR_WRITE_PORT 0x3C8 // DAC Address Write Mode Register
#define VGA_DAC_DATA_PORT       0x3C9 // DAC Data Register
#define VGA_DAC_STATE_PORT      0x3C7 // DAC State Register
#define VGA_GRAPH_ADDR_PORT     0x3CE // Graphics Controller Address Register
#define VGA_GRAPH_DATA_PORT     0x3CF // Graphics Controller Data Register
#define VGA_CRTC_ADDR_PORT      0x3D4 // CRT Controller Address Register
#define VGA_CRTC_DATA_PORT      0x3D5 // CRT Controller Data Register

#define VGA_SEG_A000 0xA000
#define VGA_SEG_B000 0xB000
#define VGA_SEG_B800 0xB800

extern uint8_t  v_mode;
extern uint8_t  v_font;
extern uint8_t  v_cols;
extern uint8_t  v_rows;
extern uint8_t  v_page;
extern uint16_t v_regen_size;
extern uint16_t v_ofs;
extern uint16_t v_seg;
extern uint16_t v_curpos;
extern uint16_t v_curshape;

#include "go32/VGA/VGA_wait_while_display_disabled.c"
#include "go32/VGA/VGA_wait_while_vertical_retrace.c"

void VGA_WaitDisplayDisabled (void);
void VGA_WaitVerticalRetrace (void);

void VGA_GetPaletteEntry (uint8_t index, uint8_t *red, uint8_t *green,
                          uint8_t *blue);
void VGA_GetPalette (uint8_t index, uint16_t count, void *palette);
void VGA_SetPaletteEntry (uint8_t red, uint8_t green, uint8_t blue,
                          uint8_t index);
void VGA_SetPalette (const void *palette, uint16_t count, uint8_t index);

uint16_t VGA_GetCursorShape (void);           // CRTC Reg. 0x0A, 0x0B
void VGA_SetCursorShape (uint16_t shape);     // CRTC Reg. 0x0A, 0x0B
void VGA_SetPresetRowScan (uint8_t row);      // CRTC Reg. 0x08
void VGA_SetOffset (uint16_t offset);         // CRTC Reg. 0x13
void VGA_SetStartAddress (uint16_t address);  // CRTC Reg. 0x0C, 0x0D
void VGA_SetLineCompare (uint16_t line);      // CRTC Reg. 0x18, 0x07, 0x09
void VGA_SetPixelPanningMode (bool mode);     // ATTRC Reg. 0x10

void VGA_ReadTextMode (void);
void VGA_SetTextMode (uint8_t mode, uint8_t font, uint8_t page);
void VGA_SetTextMode_80x25 (uint8_t page);
void VGA_SetTextModeDimensions (uint8_t cols, uint8_t rows, uint16_t size);
void VGA_ResetTextMode (void);
void VGA_MakeTextMode (uint8_t font, uint8_t cols, uint8_t rows,
                       uint16_t curshape);

#if ADT2PLAY

typedef struct {
  uint8_t mode;
  uint8_t font;
  uint8_t cols, rows;
  uint8_t page;
  uint16_t regen_size;
  uint16_t ofs, seg;
  uint16_t curpos;
  uint16_t curshape;
  uint16_t data_size;
  void *data;
  void *screen;
} VGAVideoState_t;

int VGA_SaveVideoState (VGAVideoState_t *vs, bool save_screen);
void VGA_RestoreVideoState (VGAVideoState_t *vs, bool restore_screen);
void VGA_FreeVideoState (VGAVideoState_t *vs);

#endif // ADT2PLAY

#endif // !defined(GO32_VGA_H)
