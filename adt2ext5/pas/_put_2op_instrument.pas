// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Saved from "adt2ext5.pas"
// Local to `a2w_file_loader' procedure

procedure _put_2op_instrument;
begin
  songdata.instr_data[current_inst] := temp_songdata.instr_data[index];
  songdata.instr_macros[current_inst] := temp_songdata.instr_macros[index];
  songdata.dis_fmreg_col[current_inst] := temp_songdata.dis_fmreg_col[index];
  songdata.instr_names[current_inst] := Copy(songdata.instr_names[current_inst],1,9)+
                                        Copy(temp_songdata.instr_names[index],10,32);
  idx1 := -1;
  idx2 := -1;
  If (songdata.instr_macros[current_inst].arpeggio_table <> 0) then
    idx1 := get_free_arpeggio_table_idx(temp_songdata.macro_table[
                                        songdata.instr_macros[current_inst].arpeggio_table].arpeggio);
  If (songdata.instr_macros[current_inst].vibrato_table <> 0) then
    idx2 := get_free_vibrato_table_idx(temp_songdata.macro_table[
                                       songdata.instr_macros[current_inst].vibrato_table].vibrato);
  temp_str := '';
  If (idx1 = 0) then
    If (idx2 = 0) then
      temp_str := '~ARPEGGiO/ViBRATO'
    else temp_str := '~ARPEGGiO'
  else If (idx2 = 0) then
         temp_str := '~ViBRATO';

  If NOT (temp_str <> '') then
    begin
      If (idx1 > 0) then
        begin
          songdata.macro_table[idx1].arpeggio :=
          temp_songdata.macro_table[songdata.instr_macros[current_inst].arpeggio_table].arpeggio;
          songdata.instr_macros[current_inst].arpeggio_table := idx1;
        end;
      If (idx2 > 0) then
        begin
          songdata.macro_table[idx2].vibrato :=
          temp_songdata.macro_table[songdata.instr_macros[current_inst].vibrato_table].vibrato;
          songdata.instr_macros[current_inst].vibrato_table := idx2;
        end
    end
  else Dialog('RELATED '+temp_str+' DATA~ WAS NOT LOADED!$'+
              'FREE SOME SPACE iN MACRO TABLES AND ~REPEAT THiS ACTiON~$',
              '~O~K$',' A2W LOADER ',1);
end;
