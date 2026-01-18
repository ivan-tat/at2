// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Saved from "adt2ext5.pas"
// Local to `a2w_file_loader' procedure

procedure _put_4op_instrument;
begin
  songdata.instr_data[_4op_idx11] := temp_songdata.instr_data[_4op_idx21];
  songdata.instr_macros[_4op_idx11] := temp_songdata.instr_macros[_4op_idx21];
  songdata.dis_fmreg_col[_4op_idx11] := temp_songdata.dis_fmreg_col[_4op_idx21];
  songdata.instr_names[_4op_idx11] := Copy(songdata.instr_names[_4op_idx11],1,9)+
                                        Copy(temp_songdata.instr_names[_4op_idx21],10,32);
  idx1 := -1;
  idx2 := -1;
  If (songdata.instr_macros[_4op_idx11].arpeggio_table <> 0) then
    idx1 := get_free_arpeggio_table_idx(temp_songdata.macro_table[
                                        songdata.instr_macros[_4op_idx11].arpeggio_table].arpeggio);
  If (songdata.instr_macros[_4op_idx11].vibrato_table <> 0) then
    idx2 := get_free_vibrato_table_idx(temp_songdata.macro_table[
                                       songdata.instr_macros[_4op_idx11].vibrato_table].vibrato);
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
          temp_songdata.macro_table[songdata.instr_macros[_4op_idx11].arpeggio_table].arpeggio;
          songdata.instr_macros[_4op_idx11].arpeggio_table := idx1;
        end;
      If (idx2 > 0) then
        begin
          songdata.macro_table[idx2].vibrato :=
          temp_songdata.macro_table[songdata.instr_macros[_4op_idx11].vibrato_table].vibrato;
          songdata.instr_macros[_4op_idx11].vibrato_table := idx2;
        end
    end
  else Dialog('RELATED '+temp_str+' DATA~ WAS NOT LOADED!$'+
              'FREE SOME SPACE iN MACRO TABLES AND ~REPEAT THiS ACTiON~$',
              '~O~K$',' A2W LOADER ',1);

  songdata.instr_data[_4op_idx12] := temp_songdata.instr_data[_4op_idx22];
  songdata.instr_macros[_4op_idx12] := temp_songdata.instr_macros[_4op_idx22];
  songdata.dis_fmreg_col[_4op_idx12] := temp_songdata.dis_fmreg_col[_4op_idx22];
  songdata.instr_names[_4op_idx12] := Copy(songdata.instr_names[_4op_idx12],1,9)+
                                        Copy(temp_songdata.instr_names[_4op_idx22],10,32);
  idx1 := -1;
  idx2 := -1;
  If (songdata.instr_macros[_4op_idx12].arpeggio_table <> 0) then
    idx1 := get_free_arpeggio_table_idx(temp_songdata.macro_table[
                                        songdata.instr_macros[_4op_idx12].arpeggio_table].arpeggio);
  If (songdata.instr_macros[_4op_idx12].vibrato_table <> 0) then
    idx2 := get_free_vibrato_table_idx(temp_songdata.macro_table[
                                       songdata.instr_macros[_4op_idx12].vibrato_table].vibrato);
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
          temp_songdata.macro_table[songdata.instr_macros[_4op_idx12].arpeggio_table].arpeggio;
          songdata.instr_macros[_4op_idx12].arpeggio_table := idx1;
        end;
      If (idx2 > 0) then
        begin
          songdata.macro_table[idx2].vibrato :=
          temp_songdata.macro_table[songdata.instr_macros[_4op_idx12].vibrato_table].vibrato;
          songdata.instr_macros[_4op_idx12].vibrato_table := idx2;
        end
    end
  else Dialog('RELATED '+temp_str+' DATA~ WAS NOT LOADED!$'+
              'FREE SOME SPACE iN MACRO TABLES AND ~REPEAT THiS ACTiON~$',
              '~O~K$',' A2W LOADER ',1);
end;
