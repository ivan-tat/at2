// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure init_player;

var
  temp: Byte;

begin
  _dbg_enter ({$I %FILE%}, 'init_player');

{$IFNDEF ADT2PLAY}
{$IFNDEF GO32V2}
  opl3_init;
{$ENDIF} // NOT DEFINED(GO32V2)

  FillChar(ai_table,SizeOf(ai_table),0);
{$ENDIF} // NOT DEFINED(ADT2PLAY)
  opl2out($01,0);

  For temp := 1 to 18 do opl2out($0b0+_chan_n[temp],0);
  For temp := $080 to $08d do opl2out(temp,BYTE_NULL);
  For temp := $090 to $095 do opl2out(temp,BYTE_NULL);

{$IFDEF ADT2PLAY}
  speed_update    := BOOLEAN(songdata.common_flag AND 1);
  lockvol         := BOOLEAN(songdata.common_flag SHR 1 AND 1);
  lockVP          := BOOLEAN(songdata.common_flag SHR 2 AND 1);
  tremolo_depth   :=         songdata.common_flag SHR 3 AND 1;
  vibrato_depth   :=         songdata.common_flag SHR 4 AND 1;
  panlock         := BOOLEAN(songdata.common_flag SHR 5 AND 1);
  percussion_mode := BOOLEAN(songdata.common_flag SHR 6 AND 1);
  volume_scaling  := BOOLEAN(songdata.common_flag SHR 7 AND 1);
{$ENDIF} // DEFINED(ADT2PLAY)

  current_tremolo_depth := tremolo_depth;
  current_vibrato_depth := vibrato_depth;
{$IFDEF ADT2PLAY}
  init_buffers;

  If NOT percussion_mode then
    begin
      _chan_n := _chmm_n;
      _chan_m := _chmm_m;
      _chan_c := _chmm_c;
    end
  else
    begin
      _chan_n := _chpm_n;
      _chan_m := _chpm_m;
      _chan_c := _chpm_c;
    end;
{$ENDIF} // DEFINED(ADT2PLAY)

  misc_register := tremolo_depth SHL 7+
                   vibrato_depth SHL 6+
                   BYTE(percussion_mode) SHL 5;

  opl2out($01,$20);
  opl2out($08,$40);
  opl3exp($0105);
  opl3exp($04+songdata.flag_4op SHL 8);

  key_off(17);
  key_off(18);
  opl2out(_instr[11],misc_register);
{$IFNDEF ADT2PLAY}
  init_buffers;
{$ENDIF} // NOT DEFINED(ADT2PLAY)

  global_volume := 63;
{$IFNDEF ADT2PLAY}
  macro_ticklooper := 0;
{$ENDIF} // NOT DEFINED(ADT2PLAY)
  vibtrem_speed_factor := def_vibtrem_speed_factor;
  vibtrem_table_size := def_vibtrem_table_size;
  Move(def_vibtrem_table,vibtrem_table,SizeOf(vibtrem_table));

  For temp := 1 to 20 do
    begin
      arpgg_table[temp].state := 1;
{$IFNDEF ADT2PLAY}
      arpgg_table2[temp].state := 1;
{$ENDIF} // NOT DEFINED(ADT2PLAY)
      voice_table[temp] := temp;
    end;

  _dbg_leave; //EXIT //init_player
end;
