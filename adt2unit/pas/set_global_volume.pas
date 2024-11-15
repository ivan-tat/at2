// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure set_global_volume;

var
  chan: Byte;

begin
  For chan := 1 to songdata.nm_tracks do
    If _4op_vol_valid_chan(chan) then
      set_ins_volume_4op(BYTE_NULL,chan)
    else If NOT ((carrier_vol[chan] = 0) and
                 (modulator_vol[chan] = 0)) then
           If (ins_parameter(voice_table[chan],10) AND 1 = 0) then
             set_ins_volume(BYTE_NULL,HI(volume_table[chan]),chan)
           else set_ins_volume(LO(volume_table[chan]),HI(volume_table[chan]),chan);
end;
