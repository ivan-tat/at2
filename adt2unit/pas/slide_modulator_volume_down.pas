// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure slide_modulator_volume_down(chan: Byte; slide: Byte);

var
  vol: Word;
  vLo,vHi: Byte;

begin
  vLo := LO(volume_table[chan]);
  vHi := HI(volume_table[chan]);
  If (vLo+slide <= 63) then vol := concw(vLo+slide,vHi)
  else vol := concw(63,vHi);
  set_ins_volume(LO(vol),BYTE_NULL,chan);
  volume_table[chan] := vol;
end;
