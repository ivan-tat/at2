// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function _4op_data_flag(chan: Byte): Dword;

var
  _4op_conn: Byte;
  _4op_mode: Boolean;
  _4op_ch1,_4op_ch2: Byte;
  _4op_ins1,_4op_ins2: Byte;

begin
  _4op_mode := FALSE;
  If is_4op_chan(chan) then
    begin
      _4op_mode := TRUE;
      If (chan in _4op_tracks_hi) then
        begin
          _4op_ch1 := chan;
          _4op_ch2 := SUCC(chan);
        end
      else
        begin
          _4op_ch1 := PRED(chan);
          _4op_ch2 := chan;
        end;
      _4op_ins1 := event_table[_4op_ch1].instr_def;
      If (_4op_ins1 = 0) then _4op_ins1 := voice_table[_4op_ch1];
      _4op_ins2 := event_table[_4op_ch2].instr_def;
      If (_4op_ins2 = 0) then _4op_ins2 := voice_table[_4op_ch2];
      If (_4op_ins1 <> 0) and (_4op_ins2 <> 0) then
        begin
          _4op_mode := TRUE;
          _4op_conn := (pBYTE(@Addr(songdata.instr_data[_4op_ins1])^)[_instr_data_ofs[11]] AND 1) SHL 1 +
                       (pBYTE(@Addr(songdata.instr_data[_4op_ins2])^)[_instr_data_ofs[11]] AND 1);
        end;
    end;

  {------+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+---}
  { BIT  |31|30|29|28|27|26|25|24|23|22|21|20|19|18|17|16|15|14|13|12|11|10| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0 }
  {------+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+---}
  { DATA |..|..|..|..|..|F7|F6|F5|F4|F3|F2|F1|F0|E7|E6|E5|E4|E3|E2|E1|E0|D3|D2|D1|D0|C3|C2|C1|C0|B1|B0|A0 }
  {------+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+---}
  _4op_data_flag := ORD(_4op_mode) +           {1-bit: A0}
                    (_4op_conn AND 3) SHL 1 +  {2-bit: B1-B0}
                    (_4op_ch1 AND 15) SHL 3 +  {4-bit: C3-C0}
                    (_4op_ch2 AND 15) SHL 7 +  {4-bit: D3-D0}
                    _4op_ins1 SHL 11 +         {8-bit: E7-E0}
                    _4op_ins2 SHL 19;          {8-bit: F7-F0}
end;
