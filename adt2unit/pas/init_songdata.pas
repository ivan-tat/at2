// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure init_songdata;

{$IFNDEF ADT2PLAY}
var
  temp: Byte;
{$ENDIF} // NOT DEFINED(ADT2PLAY)

begin
  _dbg_enter ({$I %FILE%}, 'init_songdata');

  If (play_status <> isStopped) then
    begin
{$IFNDEF ADT2PLAY}
      fade_out_playback(FALSE);
{$ENDIF} // NOT DEFINED(ADT2PLAY)
      stop_playing;
    end
  else init_buffers;

  FillChar(songdata,SizeOf(songdata),0);
  FillChar(songdata.pattern_order,SizeOf(songdata.pattern_order),$080);
{$IFDEF ADT2PLAY}
  FillChar(pattdata^,PATTERN_SIZE*max_patterns,0);
{$ELSE} // NOT DEFINED(ADT2PLAY)
  For temp := 1 to max_patterns DIV 8 do
    FillChar(pattdata^[PRED(temp)],8*PATTERN_SIZE,0);
{$ENDIF} // NOT DEFINED(ADT2PLAY)

{$IFDEF ADT2PLAY}
  songdata.patt_len := 64;
  songdata.nm_tracks := 9;
{$ELSE} // NOT DEFINED(ADT2PLAY)
  songdata.patt_len := patt_len;
  songdata.nm_tracks := nm_tracks;
{$ENDIF} // NOT DEFINED(ADT2PLAY)
  songdata.tempo := tempo;
  songdata.speed := speed;
{$IFDEF ADT2PLAY}
  songdata.macro_speedup := 1;
{$ELSE} // NOT DEFINED(ADT2PLAY)
  songdata.macro_speedup := init_macro_speedup;
{$ENDIF} // NOT DEFINED(ADT2PLAY)
  speed_update := FALSE;
{$IFNDEF ADT2PLAY}
  IRQ_freq_shift := 0;
  playback_speed_shift := 0;
  songdata.bpm_data.rows_per_beat := mark_line;
{$ENDIF} // NOT DEFINED(ADT2PLAY)
  songdata.bpm_data.tempo_finetune := IRQ_freq_shift;
  lockvol := FALSE;
  panlock := FALSE;
  lockVP  := FALSE;
  tremolo_depth := 0;
  vibrato_depth := 0;
  volume_scaling := FALSE;

{$IFNDEF ADT2PLAY}
  old_chan_pos := 1;
  old_hpos := 1;
  old_page := 0;
  old_block_chan_pos := 1;
  old_block_patt_hpos := 1;
  old_block_patt_page := 0;
  marking := FALSE;
{$ENDIF} // NOT DEFINED(ADT2PLAY)

{$IFDEF ADT2PLAY}
  If (songdata.nm_tracks <= 18) then
{$ELSE} // NOT DEFINED(ADT2PLAY)
  If (nm_tracks <= 18) then
{$ENDIF} // NOT DEFINED(ADT2PLAY)
    begin
      percussion_mode := FALSE;
      _chan_n := _chmm_n;
      _chan_m := _chmm_m;
      _chan_c := _chmm_c;
    end
  else
    begin
      percussion_mode := TRUE;
      _chan_n := _chpm_n;
      _chan_m := _chpm_m;
      _chan_c := _chpm_c;
    end;

{$IFNDEF ADT2PLAY}
  For temp := 1 to 255 do
    songdata.instr_names[temp] := ' iNS_'+byte2hex(temp)+#247' ';

  For temp := 0 to $7f do
    songdata.pattern_names[temp] := ' PAT_'+byte2hex(temp)+'  '#247' ';

  songdata_crc_ord := Update32(songdata.pattern_order,
                               SizeOf(songdata.pattern_order),0);
  module_archived := TRUE;
{$ENDIF} // NOT DEFINED(ADT2PLAY)

  _dbg_leave; //EXIT //init_songdata
end;
