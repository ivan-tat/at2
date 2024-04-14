// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

int VESA_SetMode (uint16_t Mode) {
  uint_least16_t i;

  if (!VESA_GraphicsSysInited)
    return -1;

  VESA_ModeIndex = VESA_NumberOfModes + 1;

  for (i = 0; i <= VESA_NumberOfModes; i++)
    if (VESA_ModeList[i].ModeNumber == Mode) {
      printf ("%u,", VESA_ModeList[i].ModeNumber);
      VESA_ModeIndex = i;
      break;
    }

  if (VESA_ModeIndex == VESA_NumberOfModes + 1)
    return -1;

  if (Mode != 19) // Standard VGA mode 320x200x8
    Mode |= 0x4000; // Use linear/flat frame buffer model

  if (VESA_set_video_mode (Mode))
    return -1;

  VESA_Mode = Mode;
  VESA_XResolution = VESA_ModeList[VESA_ModeIndex].XResolution;
  VESA_YResolution = VESA_ModeList[VESA_ModeIndex].YResolution;
  VESA_FrameBuffer = VESA_ModeList[VESA_ModeIndex].BufferAddress;
  return 0;
}
