// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure reset_chan_data(chan: Byte);
begin
  If (percussion_mode and (chan in [17..20])) then
    If channel_flag[chan] then
      reset_ins_volume(chan)
    else set_ins_volume(0,0,chan)
  else
    begin
      opl3out(_instr[02]+_chan_m[chan],63);
      opl3out(_instr[03]+_chan_c[chan],63);
      key_on(chan);

      opl3out(_instr[04]+_chan_m[chan],BYTE_NULL);
      opl3out(_instr[05]+_chan_c[chan],BYTE_NULL);
      opl3out(_instr[06]+_chan_m[chan],BYTE_NULL);
      opl3out(_instr[07]+_chan_c[chan],BYTE_NULL);
      key_off(chan);
      update_fmpar(chan);
    end;

  reset_adsrw[chan] := TRUE;
  If (play_status <> isStopped) then
    If (event_table[chan].note AND $7f in [1..12*8+1]) then
      init_macro_table(chan,event_table[chan].note AND $7f,voice_table[chan],freq_table[chan])
    else init_macro_table(chan,0,voice_table[chan],freq_table[chan]);
end;
