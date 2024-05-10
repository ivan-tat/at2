// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function Bpm2str(bpm: Real): String;
begin
  If (bpm < 1000) then
    Bpm2str := Num2str(Trunc(bpm),10)+'.'+Num2str(Trunc((bpm-Trunc(bpm))*10),10)
  else
    Bpm2str := Num2str(Round(bpm),10);
end;
