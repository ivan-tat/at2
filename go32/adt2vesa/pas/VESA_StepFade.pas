// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure VESA_StepFade;

var
  idx: Word;

begin
  For idx := 0 to 767 do
    begin
      StepRealPal[idx] := StepRealPal[idx]+StepDelta[idx];
      StepWorkPalette[idx] := ROUND(StepRealPal[idx]);
    end;
  VESA_SetPalette(StepWorkPalette);
end;
