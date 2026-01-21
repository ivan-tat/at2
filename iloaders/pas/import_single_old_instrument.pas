// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure import_single_old_instrument(old_songdata: pOLD_FIXED_SONGDATA;
                                       pos,instr: Byte);
begin
  songdata.instr_names[pos] := Copy(songdata.instr_names[pos],1,9)+
                               Copy(old_songdata^.instr_names[instr],10,22);
  songdata.instr_data[pos].fm_data := old_songdata^.instr_data[instr].fm_data;
  songdata.instr_data[pos].panning := old_songdata^.instr_data[instr].panning;
  songdata.instr_data[pos].fine_tune := old_songdata^.instr_data[instr].fine_tune;
  songdata.instr_data[pos].perc_voice := 0;
end;
