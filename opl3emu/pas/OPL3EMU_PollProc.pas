// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure OPL3EMU_PollProc(p_data: pDword; var ch_table);

var
  chan_num: Byte;
  temp: Dword;

begin
  // assign main output
  p_data^ := chip_generate(@opl3);

  // assign per-channel output
  For chan_num := 0 to 17 do
    CHAN_PTR_TABLE(ch_table)[chan_num]^:=
      (WORD(opl3.out_r[CH_MAPPING[chan_num]]) SHL 16) OR
       WORD(opl3.out_l[CH_MAPPING[chan_num]]);

  // sort rhythm channels as last
  If (opl3.rhy_flag AND $20 <> 0) then
    begin
      temp := CHAN_PTR_TABLE(ch_table)[15]^;
      CHAN_PTR_TABLE(ch_table)[15]^ := CHAN_PTR_TABLE(ch_table)[6]^;
      CHAN_PTR_TABLE(ch_table)[6]^ := temp;
      temp := CHAN_PTR_TABLE(ch_table)[16]^;
      CHAN_PTR_TABLE(ch_table)[16]^ := CHAN_PTR_TABLE(ch_table)[7]^;
      CHAN_PTR_TABLE(ch_table)[7]^ := temp;
      temp := CHAN_PTR_TABLE(ch_table)[17]^;
      CHAN_PTR_TABLE(ch_table)[17]^ := CHAN_PTR_TABLE(ch_table)[8]^;
      CHAN_PTR_TABLE(ch_table)[8]^ := temp;
    end;
end;
