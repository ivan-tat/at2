// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure init_buffers;

var
  temp: Byte;

begin
  _dbg_enter ({$I %FILE%}, 'init_buffers');

  FillChar(fmpar_table,SizeOf(fmpar_table),0);
  FillChar(pan_lock,SizeOf(pan_lock),BYTE(panlock));
{$IFDEF ADT2PLAY}
  FillChar(volume_table,SizeOf(volume_table),0);
{$ELSE} // NOT DEFINED(ADT2PLAY)
  FillChar(volume_table,SizeOf(volume_table),63);
{$ENDIF} // NOT DEFINED(ADT2PLAY)
  FillChar(vscale_table,SizeOf(vscale_table),0);
  FillChar(modulator_vol,SizeOf(modulator_vol),0);
  FillChar(carrier_vol,SizeOf(carrier_vol),0);
{$IFNDEF ADT2PLAY}
  FillChar(decay_bar,SizeOf(decay_bar),0);
  FillChar(volum_bar,SizeOf(volum_bar),0);
{$ENDIF} // NOT DEFINED(ADT2PLAY)
  FillChar(event_table,SizeOf(event_table),0);
  FillChar(freq_table,SizeOf(freq_table),0);
  FillChar(zero_fq_table,SizeOf(zero_fq_table),0);
  FillChar(effect_table,SizeOf(effect_table),0);
  FillChar(effect_table2,SizeOf(effect_table2),0);
  FillChar(fslide_table,SizeOf(fslide_table),0);
  FillChar(fslide_table2,SizeOf(fslide_table2),0);
  FillChar(glfsld_table,SizeOf(glfsld_table),0);
  FillChar(glfsld_table2,SizeOf(glfsld_table2),0);
  FillChar(porta_table,SizeOf(porta_table),0);
  FillChar(porta_table2,SizeOf(porta_table2),0);
  FillChar(portaFK_table,SizeOf(portaFK_table),BYTE(FALSE));
  FillChar(arpgg_table,SizeOf(arpgg_table),0);
  FillChar(arpgg_table2,SizeOf(arpgg_table2),0);
  FillChar(vibr_table,SizeOf(vibr_table),0);
  FillChar(vibr_table2,SizeOf(vibr_table2),0);
  FillChar(trem_table,SizeOf(trem_table),0);
  FillChar(trem_table2,SizeOf(trem_table2),0);
  FillChar(retrig_table,SizeOf(retrig_table),0);
  FillChar(retrig_table2,SizeOf(retrig_table2),0);
  FillChar(tremor_table,SizeOf(tremor_table),0);
  FillChar(tremor_table2,SizeOf(tremor_table2),0);
  FillChar(last_effect,SizeOf(last_effect),0);
  FillChar(last_effect2,SizeOf(last_effect2),0);
  FillChar(voice_table,SizeOf(voice_table),0);
  FillChar(event_new,SizeOf(event_new),0);
{$IFNDEF ADT2PLAY}
  FillChar(freqtable2,SizeOf(freqtable2),0);
{$ENDIF} // NOT DEFINED(ADT2PLAY)
  FillChar(notedel_table,SizeOf(notedel_table),BYTE_NULL);
  FillChar(notecut_table,SizeOf(notecut_table),BYTE_NULL);
  FillChar(ftune_table,SizeOf(ftune_table),0);
  FillChar(loopbck_table,SizeOf(loopbck_table),BYTE_NULL);
  FillChar(loop_table,SizeOf(loop_table),BYTE_NULL);
  FillChar(reset_chan,SizeOf(reset_chan),BYTE(FALSE));
{$IFNDEF ADT2PLAY}
  FillChar(reset_adsrw,SizeOf(reset_adsrw),BYTE(FALSE));
{$ENDIF} // NOT DEFINED(ADT2PLAY)
  FillChar(keyoff_loop,SizeOf(keyoff_loop),BYTE(FALSE));
  FillChar(macro_table,SizeOf(macro_table),0);
{$IFNDEF ADT2PLAY}
  FillChar(ignore_note_once,SizeOf(ignore_note_once),FALSE);
{$ENDIF} // NOT DEFINED(ADT2PLAY)

  If NOT lockvol then FillChar(volume_lock,SizeOf(volume_lock),0)
  else For temp := 1 to 20 do volume_lock[temp] := BOOLEAN(songdata.lock_flags[temp] SHR 4 AND 1);

  If NOT panlock then FillChar(panning_table,SizeOf(panning_table),0)
  else For temp := 1 to 20 do panning_table[temp] := songdata.lock_flags[temp] AND 3;

  If NOT lockVP then FillChar(peak_lock,SizeOf(peak_lock),0)
  else For temp := 1 to 20 do peak_lock[temp] := BOOLEAN(songdata.lock_flags[temp] SHR 5 AND 1);

  FillChar(vol4op_lock,SizeOf(vol4op_lock),BYTE(FALSE));
  For temp := 1 to 6 do
    begin
      vol4op_lock[_4op_main_chan[temp]] :=
        (songdata.lock_flags[_4op_main_chan[temp]] OR $40 = songdata.lock_flags[_4op_main_chan[temp]]);
      vol4op_lock[PRED(_4op_main_chan[temp])] :=
        (songdata.lock_flags[PRED(_4op_main_chan[temp])] OR $40 = songdata.lock_flags[PRED(_4op_main_chan[temp])]);
    end;

  For temp := 1 to 20 do
    volslide_type[temp] := songdata.lock_flags[temp] SHR 2 AND 3;

  _dbg_leave; //EXIT //init_buffers
end;
