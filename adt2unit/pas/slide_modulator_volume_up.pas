// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure slide_modulator_volume_up(chan: Byte; slide,limit: Byte);

var
  vol: Word;
  vLo,vHi: Byte;

begin
  vLo := LO(volume_table[chan]);
  vHi := HI(volume_table[chan]);
  If (vLo-slide >= limit) then vol := concw(vLo-slide,vHi)
  else vol := concw(limit,vHi);
  set_ins_volume(LO(vol),BYTE_NULL,chan);
  volume_table[chan] := vol;
end;
