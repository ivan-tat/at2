// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void VESA_StepFade (void) {
  uint_least16_t i;

  for (i = 0; i < 256*3; i++) {
      StepRealPal[i] += StepDelta[i];
      StepWorkPalette[i] = (uint8_t) StepRealPal[i];
  }

  VESA_SetPalette (StepWorkPalette);
}
