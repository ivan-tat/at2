// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure import_old_songdata(old_songdata: pOLD_FIXED_SONGDATA);

var
  temp: Byte;

begin
  songdata.songname := old_songdata^.songname;
  songdata.composer := old_songdata^.composer;

  For temp := 1 to 250 do
    begin
      songdata.instr_names[temp] := old_songdata^.instr_names[temp];
      songdata.instr_data[temp].fm_data := old_songdata^.instr_data[temp].fm_data;
      songdata.instr_data[temp].panning := old_songdata^.instr_data[temp].panning;
      songdata.instr_data[temp].fine_tune := old_songdata^.instr_data[temp].fine_tune;
      songdata.instr_data[temp].perc_voice := 0;
    end;

  Move(old_songdata^.pattern_order,
       songdata.pattern_order,
       SizeOf(old_songdata^.pattern_order));

  songdata.tempo := old_songdata^.tempo;
  songdata.speed := old_songdata^.speed;
  songdata.common_flag := old_songdata^.common_flag;
  import_old_flags;
end;
