// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef GO32_ADT2VESA_H
#define GO32_ADT2VESA_H

#include "pascal.h"

// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)

// VESA graphics modes
#define VESA_640x400x8    0x100
#define VESA_640x480x8    0x101
#define VESA_800x600x4    0x102
#define VESA_800x600x8    0x103
#define VESA_1024x768x4   0x104
#define VESA_1024x768x8   0x105
#define VESA_1280x1024x4  0x106
#define VESA_1280x1024x8  0x107

// VESA text modes
#define VESA_80x60        0x108
#define VESA_132x25       0x109
#define VESA_132x43       0x10A
#define VESA_132x50       0x10B
#define VESA_132x60       0x10C

// VESA Super VGA Standard VS911022-8
#define VESA_320x200x15   0x10D // X1:R5:G5:B5
#define VESA_320x200x16   0x10E // R5:G6:B5
#define VESA_320x200x24   0x10F // R8:G8:B8
#define VESA_640x480x15   0x110 // X1:R5:G5:B5
#define VESA_640x480x16   0x111 // R5:G6:B5
#define VESA_640x480x24   0x112 // R8:G8:B8
#define VESA_800x600x15   0x113 // X1:R5:G5:B5
#define VESA_800x600x16   0x114 // R5:G6:B5
#define VESA_800x600x24   0x115 // R8:G8:B8
#define VESA_1024x768x15  0x116 // X1:R5:G5:B5
#define VESA_1024x768x16  0x117 // R5:G6:B5
#define VESA_1024x768x24  0x118 // R8:G8:B8
#define VESA_1280x1024x15 0x119 // X1:R5:G5:B5
#define VESA_1280x1024x16 0x11A // R5:G6:B5
#define VESA_1280x1024x24 0x11B // R8:G8:B8

// Aliases
#define VESA_640x480  VESA_640x480x8
#define VESA_800x600  VESA_800x600x8
#define VESA_1024x768 VESA_1024x768x8

extern bool VESA_GraphicsSysInited;

// VESA call status
#define VESACS_OK 0 // Success
#define VESACS_NS 1 // Function call is not supported
#define VESACS_DE 2 // DPMI error

extern uint8_t VESA_CallStatus;
extern uint8_t VESA_Error;

#pragma pack(push, 1)
typedef struct {
  uint16_t ModeNumber;
  uint16_t XResolution;
  uint16_t YResolution;
  void    *BufferAddress;
} tModeInfo;
#pragma pack(pop)

extern uint8_t   VESA_NumberOfModes;              // Total LFB videomodes supported
extern void     *VESA_FrameBuffer;                // LFB address for current mode
extern tModeInfo VESA_ModeList[256];              // HINT: (FPC) start index 0
extern uint16_t  VESA_Version;                    // VESA version
extern String    VESA_OEM_String[255+1];          // VESA OEM string
extern uint32_t  VESA_Capabilities;               // Hardware capabilities
extern uint16_t  VESA_VideoMemory;                // Videomemory amount in Kb
extern uint16_t  VESA_OEM_SoftwareRevision;       // VESA implementation revision
extern String    VESA_OEM_VendorName[255+1];      // VESA vendor name
extern String    VESA_OEM_ProductName[255+1];     // VESA OEM product name
extern String    VESA_OEM_ProductRevision[255+1]; // VESA OEM product revision
extern uint16_t  VESA_Mode;                       // Current videomode
extern uint8_t   VESA_ModeIndex;                  // Current mode index in VESA_ModeList
extern uint16_t  VESA_XResolution;                // Current X resolution
extern uint16_t  VESA_YResolution;                // Current Y resolution

typedef uint8_t tPaletteArray[256*3]; // HINT: (FPC) start index 0

extern uint16_t VESA_SegLFB;
extern tPaletteArray StepWorkPalette;
extern float StepRealPal[256*3], StepDelta[256*3];
extern uint16_t FadeSteps;

int _VESA_CheckStatus (uint16_t status);

#include "go32/adt2vesa/VESA_set_video_mode.c"

void VESA_Init (void);

// Returns 0 on success, -1 on error.
int VESA_GetModeInfo (uint16_t Mode, uint32_t DOSAddr);

// Returns 0 on success, -1 on error.
int VESA_SetMode (uint16_t Mode);

void VESA_GetPalette (void *Palette);
void VESA_SetPalette (const void *Palette);
void VESA_InitStepFade (const tPaletteArray StartPalette,
                        tPaletteArray EndPalette, float Speed);
void VESA_StepFade (void);
void VESA_SwitchBank (uint8_t Bank);

#endif // !defined(GO32_ADT2VESA_H)
