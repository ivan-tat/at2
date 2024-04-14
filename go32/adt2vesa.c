// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#include "defines.h"

#if USE_FPC
#include "pascal/dpmi.h"
#include "pascal/go32.h"
#include "pascal/math.h"
#include "pascal/pc.h"
#include "pascal/stdio.h"
#include "pascal/string.h"
#else // !USE_FPC
#include "<dpmi.h>"
#include "<go32.h>"
#include "<math.h>"
#include "<pc.h>"
#include "<stdio.h>"
#include "<string.h>"
#endif // !USE_FPC
#include "common.h"
#include "go32/VGA.h"
#include "go32/adt2vesa.h"

// HINT: (FPC) S-: Stack checking (off)
// HINT: (FPC) Q-: Overflow checking (off)
// HINT: (FPC) R-: Range checking (off)
// HINT: (FPC) V-: Var-string checking (off)
// HINT: (FPC) B-: Complete boolean evaluation (off)
// HINT: (FPC) X+: Extended syntax (ON)
// HINT: (FPC) PACKRECORDS 1: Alignment of record elements (1)

bool VESA_GraphicsSysInited = false;

uint8_t VESA_CallStatus = VESACS_OK;
uint8_t VESA_Error = 0;

uint8_t   VESA_NumberOfModes;
void     *VESA_FrameBuffer;
tModeInfo VESA_ModeList[256];
uint16_t  VESA_Version;
String    VESA_OEM_String[255+1];
uint32_t  VESA_Capabilities;
uint16_t  VESA_VideoMemory;
uint16_t  VESA_OEM_SoftwareRevision;
String    VESA_OEM_VendorName[255+1];
String    VESA_OEM_ProductName[255+1];
String    VESA_OEM_ProductRevision[255+1];
uint16_t  VESA_Mode;
uint8_t   VESA_ModeIndex;
uint16_t  VESA_XResolution;
uint16_t  VESA_YResolution;

uint16_t VESA_SegLFB;
tPaletteArray StepWorkPalette;
float StepRealPal[256*3], StepDelta[256*3];
uint16_t FadeSteps;

#pragma pack(push, 1)
typedef struct {
  uint16_t ModeAttributes;
  uint8_t  WinAAttributes;
  uint8_t  WinBAttributes;
  uint16_t WinGranularity;
  uint16_t WinSize;
  uint16_t WinASegment;
  uint16_t WinBSegment;
  void    *WinFuncPtr;
  uint16_t BytesPerScanLine;
  // VESA 1.2+ specific
  uint16_t XResolution;
  uint16_t YResolution;
  uint8_t  XCharSize;
  uint8_t  YCharSize;
  uint8_t  NumberOfPlanes;
  uint8_t  BitsPerPixel;
  uint8_t  NumberOfBanks;
  uint8_t  MemoryModel;
  uint8_t  BankSize;
  uint8_t  NumberOfImagePages;
  uint8_t  Reserved1;
  // direct color data
  uint8_t  RedMaskSize;
  uint8_t  RedFieldPosition;
  uint8_t  GreenMaskSize;
  uint8_t  GreenFieldPosition;
  uint8_t  BlueMaskSize;
  uint8_t  BlueFieldPosition;
  uint8_t  RsvdMaskSize;
  uint8_t  RsvdFieldPosition;
  uint8_t  DirectColorModeInfo;
  // VESA 2.0+ specific
  void    *PhysBasePtr;
  uint32_t OffScreenMemOffset;
  uint16_t OffScreenMemSize;
  uint8_t  Reserved2[206];
} tModeInfoBlock;
#pragma pack(pop)

typedef uint16_t tModeList[128]; // HINT (FPC) start index 0

#pragma pack(push, 1)
typedef struct {
  uint32_t Signature;
  uint16_t Version;
  uint32_t OEM_StringPtr;
  uint32_t Capabilities;
  uint32_t ModeListPtr;
  uint16_t VideoMemory;
  uint16_t OEM_SoftwareRevision;
  uint32_t OEM_VendorNamePtr;
  uint32_t OEM_ProductNamePtr;
  uint32_t OEM_ProductRevisionPtr;
  uint8_t  Reserved[222];
  uint8_t  OEM_Data[256];
} tVESA_Info;
#pragma pack(pop)

static tModeList ModeList;
static tVESA_Info VESA_Info;
static tModeInfoBlock ModeInfoBlock;

#include "go32/adt2vesa/_VESA_CheckStatus.c"

#include "go32/adt2vesa/VESA_Init.c"
#include "go32/adt2vesa/VESA_GetModeInfo.c"
#include "go32/adt2vesa/VESA_SetMode.c"
#include "go32/adt2vesa/VESA_GetPalette.c"
#include "go32/adt2vesa/VESA_SetPalette.c"
#include "go32/adt2vesa/VESA_InitStepFade.c"
#include "go32/adt2vesa/VESA_StepFade.c"
#include "go32/adt2vesa/VESA_SwitchBank.c"
