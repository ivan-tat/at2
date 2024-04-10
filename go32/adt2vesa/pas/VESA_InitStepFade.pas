// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure VESA_InitStepFade(var StartPalette,EndPalette; Speed: Single);

var
  EndRealPal: array[0..767] of Single;
  idx: Word;

begin
  For idx := 0 to 767 do
    begin
      StepRealPal[idx] := tPaletteArray(StartPalette)[idx];
      StepWorkPalette[idx] := tPaletteArray(StartPalette)[idx];
      EndRealPal[idx] := tPaletteArray(EndPalette)[idx];
      StepDelta[idx] := (EndRealPal[idx]-StepRealPal[idx])/Speed;
    end;
  VESA_SetPalette(StartPalette);
  FadeSteps := TRUNC(Speed);
end;
