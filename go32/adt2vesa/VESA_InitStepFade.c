// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void VESA_InitStepFade (const tPaletteArray StartPalette,
                        tPaletteArray EndPalette, float Speed) {
  float EndRealPal[256*3]; //0
  uint_least16_t i;

  for (i = 0; i < 256*3; i++) {
      StepRealPal[i] = (float) StartPalette[i];
      StepWorkPalette[i] = StartPalette[i];
      EndRealPal[i] = (float) EndPalette[i];
      StepDelta[i] = (EndRealPal[i] - StepRealPal[i]) / Speed;
  }

  VESA_SetPalette (StartPalette);
  FadeSteps = truncf (Speed);
}
