// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure play_line; cdecl;
public name PUBLIC_PREFIX + 'play_line';

var
  chan,idx: Byte;
  event: array[1..20] of tCHUNK;
  eLo,eHi,eLo2,eHi2: array[1..20] of Byte;
  tporta_flag: Boolean;

function no_loop(current_chan,current_line: Byte): Boolean;

var
  result: Boolean;
  chan: Byte;

begin
  result := TRUE;
  For chan := 1 to PRED(current_chan) do
    If (loop_table[chan][current_line] <> 0) and
       (loop_table[chan][current_line] <> BYTE_NULL) then
      begin
        result := FALSE;
        BREAK;
      end;
  no_loop := result;
end;

begin
  _dbg_enter ({$I %FILE%}, 'play_line');

{$IFDEF ADT2PLAY}
  If (current_line = 0) and
     (current_order = calc_following_order(0)) then
    time_playing := 0;
{$ENDIF} // DEFINED(ADT2PLAY)

  If NOT (pattern_break and (next_line AND $0f0 = pattern_loop_flag)) and
         (current_order <> last_order) then
    begin
      FillChar(loopbck_table,SizeOf(loopbck_table),BYTE_NULL);
      FillChar(loop_table,SizeOf(loop_table),BYTE_NULL);
      last_order := current_order;
    end;

{$IFNDEF ADT2PLAY}
  For chan := 1 to songdata.nm_tracks do
    If channel_flag[chan] and reset_adsrw[chan] then
      begin
        update_modulator_adsrw(chan);
        update_carrier_adsrw(chan);
        reset_adsrw[chan] := FALSE;
      end;
{$ENDIF} // NOT DEFINED(ADT2PLAY)

  For chan := 1 to songdata.nm_tracks do
    begin
      event[chan] := pattdata^[current_pattern DIV 8][current_pattern MOD 8]
                              [chan][current_line];
      If (effect_table[chan] <> 0) then last_effect[chan] := effect_table[chan];
      If (glfsld_table[chan] <> 0) then effect_table[chan] := glfsld_table[chan]
      else effect_table[chan] := effect_table[chan] AND $0ff00;
      If (effect_table2[chan] <> 0) then last_effect2[chan] := effect_table2[chan];
      If (glfsld_table2[chan] <> 0) then effect_table2[chan] := glfsld_table2[chan]
      else effect_table2[chan] := effect_table2[chan] AND $0ff00;
      ftune_table[chan] := 0;

      If (event[chan].note = BYTE_NULL) then
        event[chan].note := event_table[chan].note OR keyoff_flag
      else If (event[chan].note in [fixed_note_flag+1..fixed_note_flag+12*8+1]) then
             event[chan].note := event[chan].note-fixed_note_flag;

      If (event[chan].note <> 0) or
         (event[chan].effect_def <> 0) or
         (event[chan].effect_def2 <> 0) or
         ((event[chan].effect_def = 0) and (event[chan].effect <> 0)) or
         ((event[chan].effect_def2 = 0) and (event[chan].effect2 <> 0)) then
        event_new[chan] := TRUE
      else event_new[chan] := FALSE;

      If (event[chan].note <> 0) or
         (event[chan].instr_def <> 0) or
         (event[chan].effect_def+event[chan].effect <> 0) or
         (event[chan].effect_def2+event[chan].effect2 <> 0) then
        begin
          event_table[chan].effect_def := event[chan].effect_def;
          event_table[chan].effect := event[chan].effect;
          event_table[chan].effect_def2 := event[chan].effect_def2;
          event_table[chan].effect2 := event[chan].effect2;
        end;

      If (event[chan].instr_def <> 0) then
        If NOT is_data_empty(songdata.instr_data[event[chan].instr_def],
                             INSTRUMENT_SIZE) then
          set_ins_data(event[chan].instr_def,chan)
        else begin
               release_sustaining_sound(chan);
               set_ins_data(event[chan].instr_def,chan);
             end;

      If NOT (event[chan].effect_def in [ef_Vibrato,ef_ExtraFineVibrato,
                                   ef_VibratoVolSlide,ef_VibratoVSlideFine]) then
        FillChar(vibr_table[chan],SizeOf(vibr_table[chan]),0);

      If NOT (event[chan].effect_def2 in [ef_Vibrato,ef_ExtraFineVibrato,
                                    ef_VibratoVolSlide,ef_VibratoVSlideFine]) then
        FillChar(vibr_table2[chan],SizeOf(vibr_table2[chan]),0);

      If NOT (event[chan].effect_def in [ef_RetrigNote,ef_MultiRetrigNote]) then
        FillChar(retrig_table[chan],SizeOf(retrig_table[chan]),0);

      If NOT (event[chan].effect_def2 in [ef_RetrigNote,ef_MultiRetrigNote]) then
        FillChar(retrig_table2[chan],SizeOf(retrig_table2[chan]),0);

      If NOT (event[chan].effect_def in [ef_Tremolo,ef_ExtraFineTremolo]) then
        FillChar(trem_table[chan],SizeOf(trem_table[chan]),0);

      If NOT (event[chan].effect_def2 in [ef_Tremolo,ef_ExtraFineTremolo]) then
        FillChar(trem_table2[chan],SizeOf(trem_table2[chan]),0);

      If NOT (((event[chan].effect_def = ef_Arpeggio) and (event[chan].effect <> 0)) or
               (event[chan].effect_def = ef_ExtraFineArpeggio)) and
               (arpgg_table[chan].note <> 0) and (arpgg_table[chan].state <> 1) then
        begin
          arpgg_table[chan].state := 1;
          change_frequency(chan,nFreq(arpgg_table[chan].note-1)+
            SHORTINT(ins_parameter(event_table[chan].instr_def,12)));
        end
      else If NOT (((event[chan].effect_def2 = ef_Arpeggio) and (event[chan].effect2 <> 0)) or
                  (event[chan].effect_def2 = ef_ExtraFineArpeggio)) and
                  (arpgg_table2[chan].note <> 0) and (arpgg_table2[chan].state <> 1) then
             begin
               arpgg_table2[chan].state := 1;
               change_frequency(chan,nFreq(arpgg_table2[chan].note-1)+
                 SHORTINT(ins_parameter(event_table[chan].instr_def,12)));
             end;

      If (event[chan].effect_def = ef_GlobalFSlideUp) or
         (event[chan].effect_def = ef_GlobalFSlideDown) then
        begin
          If (event[chan].effect_def2 = ef_Extended) and
             (event[chan].effect2 = ef_ex_ExtendedCmd*16+ef_ex_cmd_ForceBpmSld) then
            begin
              If (event[chan].effect_def = ef_GlobalFSlideUp) then
                update_playback_speed(event[chan].effect)
              else update_playback_speed(-event[chan].effect);
            end
          else
            begin
              Case event[chan].effect_def of
                ef_GlobalFSlideUp:
                  If (event[chan].effect_def2 = ef_Extended) and
                     (event[chan].effect2 = ef_ex_ExtendedCmd2*16+ef_ex_cmd2_FTrm_XFGFS) then
                    effect_table[chan] := concw(ef_extended2+ef_fix2+ef_ex2_FreqSlideUpXF,
                                                event[chan].effect)
                  else If (event[chan].effect_def2 = ef_Extended) and
                          (event[chan].effect2 = ef_ex_ExtendedCmd2*16+ef_ex_cmd2_FVib_FGFS) then
                         effect_table[chan] := concw(ef_FSlideUpFine,event[chan].effect)
                       else effect_table[chan] := concw(ef_FSlideUp,event[chan].effect);
                ef_GlobalFSlideDown:
                  If (event[chan].effect_def2 = ef_Extended) and
                     (event[chan].effect2 = ef_ex_ExtendedCmd2*16+ef_ex_cmd2_FTrm_XFGFS) then
                    effect_table[chan] := concw(ef_extended2+ef_fix2+ef_ex2_FreqSlideDnXF,
                                                event[chan].effect)
                  else If (event[chan].effect_def2 = ef_Extended) and
                          (event[chan].effect2 = ef_ex_ExtendedCmd2*16+ef_ex_cmd2_FVib_FGFS) then
                         effect_table[chan] := concw(ef_FSlideDownFine,event[chan].effect)
                       else effect_table[chan] := concw(ef_FSlideDown,event[chan].effect);
              end;
              For idx := chan to songdata.nm_tracks do
                begin
                  fslide_table[idx] := event[chan].effect;
                  glfsld_table[idx] := effect_table[chan];
                end;
            end;
        end;

      If (event[chan].effect_def2 = ef_GlobalFSlideUp) or
         (event[chan].effect_def2 = ef_GlobalFSlideDown) then
        begin
          If (event[chan].effect_def = ef_Extended) and
             (event[chan].effect = ef_ex_ExtendedCmd*16+ef_ex_cmd_ForceBpmSld) then
            begin
              If (event[chan].effect_def2 = ef_GlobalFSlideUp) then
                update_playback_speed(event[chan].effect2)
              else update_playback_speed(-event[chan].effect2);
            end
          else
            begin
              Case event[chan].effect_def2 of
                ef_GlobalFSlideUp:
                  If (event[chan].effect_def = ef_Extended) and
                     (event[chan].effect = ef_ex_ExtendedCmd2*16+ef_ex_cmd2_FTrm_XFGFS) then
                    effect_table2[chan] := concw(ef_extended2+ef_fix2+ef_ex2_FreqSlideUpXF,
                                                 event[chan].effect2)
                  else If (event[chan].effect_def = ef_Extended) and
                          (event[chan].effect = ef_ex_ExtendedCmd2*16+ef_ex_cmd2_FVib_FGFS) then
                         effect_table2[chan] := concw(ef_FSlideUpFine,event[chan].effect2)
                       else effect_table2[chan] := concw(ef_FSlideUp,event[chan].effect2);
                ef_GlobalFSlideDown:
                  If (event[chan].effect_def = ef_Extended) and
                     (event[chan].effect = ef_ex_ExtendedCmd2*16+ef_ex_cmd2_FTrm_XFGFS) then
                    effect_table2[chan] := concw(ef_extended2+ef_fix2+ef_ex2_FreqSlideDnXF,
                                                 event[chan].effect2)
                  else If (event[chan].effect_def = ef_Extended) and
                          (event[chan].effect = ef_ex_ExtendedCmd2*16+ef_ex_cmd2_FVib_FGFS) then
                         effect_table2[chan] := concw(ef_FSlideDownFine,event[chan].effect2)
                       else effect_table2[chan] := concw(ef_FSlideDown,event[chan].effect2);
              end;
              For idx := chan to songdata.nm_tracks do
                begin
                  fslide_table2[idx] := event[chan].effect2;
                  glfsld_table2[idx] := effect_table2[chan];
                end;
            end;
        end;
    end;

  For chan := 1 to songdata.nm_tracks do
    begin
      If event_new[chan] and is_4op_chan(chan) then
        If (chan in _4op_tracks_hi) then
           event_new[SUCC(chan)] := TRUE
        else event_new[PRED(chan)] := TRUE;

      If (tremor_table[chan].pos <> 0) and
         (event[chan].effect_def <> ef_Tremor) then
        begin
          tremor_table[chan].pos := 0;
          set_ins_volume(LO(tremor_table[chan].volume),
                         HI(tremor_table[chan].volume),chan);
        end;

      If (tremor_table2[chan].pos <> 0) and
         (event[chan].effect_def2 <> ef_Tremor) then
        begin
          tremor_table2[chan].pos := 0;
          set_ins_volume(LO(tremor_table2[chan].volume),
                         HI(tremor_table2[chan].volume),chan);
        end;

      eLo[chan]  := LO(last_effect[chan]);
      eHi[chan]  := HI(last_effect[chan]);
      eLo2[chan] := LO(last_effect2[chan]);
      eHi2[chan] := HI(last_effect2[chan]);
    end;

  For chan := 1 to songdata.nm_tracks do
    Case event[chan].effect_def of
      ef_Arpeggio,
      ef_ExtraFineArpeggio,
      ef_ArpggVSlide,
      ef_ArpggVSlideFine:
        If (event[chan].effect_def <> ef_Arpeggio) or
           (event[chan].effect <> 0) then
          begin
            Case event[chan].effect_def of
              ef_Arpeggio:
                effect_table[chan] := concw(ef_Arpeggio+ef_fix1,event[chan].effect);

              ef_ExtraFineArpeggio:
                effect_table[chan] := concw(ef_ExtraFineArpeggio,event[chan].effect);

              ef_ArpggVSlide,
              ef_ArpggVSlideFine:
                If (event[chan].effect <> 0) then
                  effect_table[chan] := concw(event[chan].effect_def,event[chan].effect)
                else If (eLo[chan] in [ef_ArpggVSlide,ef_ArpggVSlideFine]) and
                        (eHi[chan] <> 0) then
                       effect_table[chan] := concw(event[chan].effect_def,eHi[chan])
                     else effect_table[chan] := effect_table[chan] AND $0ff00;
            end;

            If (event[chan].note AND $7f in [1..12*8+1]) then
              begin
                arpgg_table[chan].state := 0;
                arpgg_table[chan].note := event[chan].note AND $7f;
                If (event[chan].effect_def in [ef_Arpeggio,ef_ExtraFineArpeggio]) then
                  begin
                    arpgg_table[chan].add1 := event[chan].effect DIV 16;
                    arpgg_table[chan].add2 := event[chan].effect MOD 16;
                  end;
              end
            else If (event[chan].note = 0) and
                    (event_table[chan].note AND $7f in [1..12*8+1]) then
                   begin
                     If NOT (eLo[chan] in [ef_Arpeggio+ef_fix1,ef_ExtraFineArpeggio,
                                     ef_ArpggVSlide,ef_ArpggVSlideFine]) then
                       arpgg_table[chan].state := 0;

                     arpgg_table[chan].note := event_table[chan].note AND $7f;
                     If (event[chan].effect_def in [ef_Arpeggio,ef_ExtraFineArpeggio]) then
                       begin
                         arpgg_table[chan].add1 := event[chan].effect DIV 16;
                         arpgg_table[chan].add2 := event[chan].effect MOD 16;
                       end;
                   end
                 else effect_table[chan] := 0;
          end;

      ef_FSlideUp,
      ef_FSlideDown,
      ef_FSlideUpFine,
      ef_FSlideDownFine:
        begin
          effect_table[chan] := concw(event[chan].effect_def,event[chan].effect);
          fslide_table[chan] := event[chan].effect;
        end;

      ef_FSlideUpVSlide,
      ef_FSlUpVSlF,
      ef_FSlideDownVSlide,
      ef_FSlDownVSlF,
      ef_FSlUpFineVSlide,
      ef_FSlUpFineVSlF,
      ef_FSlDownFineVSlide,
      ef_FSlDownFineVSlF:
        If (event[chan].effect <> 0) then
          effect_table[chan] := concw(event[chan].effect_def,event[chan].effect)
        else If (eLo[chan] in [ef_FSlideUpVSlide,ef_FSlUpVSlF,ef_FSlideDownVSlide,
                         ef_FSlDownVSlF,ef_FSlUpFineVSlide,ef_FSlUpFineVSlF,
                         ef_FSlDownFineVSlide,ef_FSlDownFineVSlF]) and
                (eHi[chan] <> 0) then
               effect_table[chan] := concw(event[chan].effect_def,eHi[chan])
             else effect_table[chan] := effect_table[chan] AND $0ff00;

      ef_TonePortamento:
        If (event[chan].note in [1..12*8+1]) then
          begin
            If (event[chan].effect <> 0) then
              effect_table[chan] := concw(ef_TonePortamento,event[chan].effect)
            else If (eLo[chan] = ef_TonePortamento) and
                    (eHi[chan] <> 0) then
                   effect_table[chan] := concw(ef_TonePortamento,eHi[chan])
                 else effect_table[chan] := ef_TonePortamento;

            porta_table[chan].speed := HI(effect_table[chan]);
            porta_table[chan].freq := nFreq(event[chan].note-1)+
              SHORTINT(ins_parameter(event_table[chan].instr_def,12));
          end
        else If (eLo[chan] = ef_TonePortamento) then
               begin
                 If (event[chan].effect <> 0) then
                   effect_table[chan] := concw(ef_TonePortamento,event[chan].effect)
                 else If (eLo[chan] = ef_TonePortamento) and
                         (eHi[chan] <> 0) then
                        effect_table[chan] := concw(ef_TonePortamento,eHi[chan])
                      else effect_table[chan] := ef_TonePortamento;
                 porta_table[chan].speed := HI(effect_table[chan]);
               end;

      ef_TPortamVolSlide,
      ef_TPortamVSlideFine:
        If (event[chan].effect <> 0) then
          effect_table[chan] := concw(event[chan].effect_def,event[chan].effect)
        else If (eLo[chan] in [ef_TPortamVolSlide,ef_TPortamVSlideFine]) and
                (eHi[chan] <> 0) then
               effect_table[chan] := concw(event[chan].effect_def,eHi[chan])
             else effect_table[chan] := effect_table[chan] AND $0ff00;

      ef_Vibrato,
      ef_ExtraFineVibrato:
        begin
          If (event[chan].effect <> 0) then
            effect_table[chan] := concw(event[chan].effect_def,event[chan].effect)
          else If (eLo[chan] in [ef_Vibrato,ef_ExtraFineVibrato]) and
                  (eHi[chan] <> 0) then
                 effect_table[chan] := concw(event[chan].effect_def,eHi[chan])
               else effect_table[chan] := event[chan].effect_def;

          If (event[chan].effect_def2 = ef_Extended) and
             (event[chan].effect2 = ef_ex_ExtendedCmd2*16+ef_ex_cmd2_FVib_FGFS) then
            vibr_table[chan].fine := TRUE;

          vibr_table[chan].speed := HI(effect_table[chan]) DIV 16;
          vibr_table[chan].depth := HI(effect_table[chan]) MOD 16;
        end;

      ef_Tremolo,
      ef_ExtraFineTremolo:
        begin
          If (event[chan].effect <> 0) then
            effect_table[chan] := concw(event[chan].effect_def,event[chan].effect)
          else If (eLo[chan] in [ef_Tremolo,ef_ExtraFineTremolo]) and
                  (eHi[chan] <> 0) then
                 effect_table[chan] := concw(event[chan].effect_def,eHi[chan])
               else effect_table[chan] := event[chan].effect_def;

          If (event[chan].effect_def2 = ef_Extended) and
             (event[chan].effect2 = ef_ex_ExtendedCmd2*16+ef_ex_cmd2_FTrm_XFGFS) then
            trem_table[chan].fine := TRUE;

          trem_table[chan].speed := HI(effect_table[chan]) DIV 16;
          trem_table[chan].depth := HI(effect_table[chan]) MOD 16;
        end;

      ef_VibratoVolSlide,
      ef_VibratoVSlideFine:
        begin
          If (event[chan].effect <> 0) then
            effect_table[chan] := concw(event[chan].effect_def,event[chan].effect)
          else If (eLo[chan] in [ef_VibratoVolSlide,ef_VibratoVSlideFine]) and
                  (HI(effect_table[chan]) <> 0) then
                 effect_table[chan] := concw(event[chan].effect_def,HI(effect_table[chan]))
               else effect_table[chan] := effect_table[chan] AND $0ff00;

          If (event[chan].effect_def2 = ef_Extended) and
             (event[chan].effect2 = ef_ex_ExtendedCmd2*16+ef_ex_cmd2_FVib_FGFS) then
            vibr_table[chan].fine := TRUE;
        end;

      ef_SetCarrierVol:
        set_ins_volume(BYTE_NULL,63-event[chan].effect,chan);

      ef_SetModulatorVol:
        set_ins_volume(63-event[chan].effect,BYTE_NULL,chan);

      ef_SetInsVolume:
        If _4op_vol_valid_chan(chan) then
          set_ins_volume_4op(63-event[chan].effect,chan)
        else If percussion_mode and (chan in [17..20]) then
               set_ins_volume(63-event[chan].effect,BYTE_NULL,chan)
             else If (ins_parameter(voice_table[chan],10) AND 1 = 0) then
                    set_ins_volume(BYTE_NULL,63-event[chan].effect,chan)
                  else set_ins_volume(63-event[chan].effect,63-event[chan].effect,chan);

      ef_ForceInsVolume:
        If percussion_mode and (chan in [17..20]) then
          set_ins_volume(63-event[chan].effect,BYTE_NULL,chan)
        else If (ins_parameter(voice_table[chan],10) AND 1 = 0) then
               set_ins_volume(scale_volume(ins_parameter(voice_table[chan],2) AND $3f,63-event[chan].effect),63-event[chan].effect,chan)
             else set_ins_volume(63-event[chan].effect,63-event[chan].effect,chan);

      ef_PositionJump:
        If no_loop(chan,current_line) then
          begin
            pattern_break := TRUE;
            next_line := pattern_break_flag+chan;
          end;

      ef_PatternBreak:
        If no_loop(chan,current_line) then
          begin
            pattern_break := TRUE;
{$IFNDEF ADT2PLAY}
            seek_pattern_break := TRUE;
{$ENDIF} // NOT DEFINED(ADT2PLAY)
            next_line := max(event[chan].effect,PRED(songdata.patt_len));
          end;

      ef_SetSpeed:
        speed := event[chan].effect;

      ef_SetTempo:
        update_timer(event[chan].effect);

      ef_SetWaveform:
        begin
          If (event[chan].effect DIV 16 in [0..7]) then
            begin
              fmpar_table[chan].adsrw_car.wform := event[chan].effect DIV 16;
              update_carrier_adsrw(chan);
            end;

          If (event[chan].effect MOD 16 in [0..7]) then
            begin
              fmpar_table[chan].adsrw_mod.wform := event[chan].effect MOD 16;
              update_modulator_adsrw(chan);
            end;
        end;

      ef_VolSlide:
        effect_table[chan] := concw(ef_VolSlide,event[chan].effect);

      ef_VolSlideFine:
        effect_table[chan] := concw(ef_VolSlideFine,event[chan].effect);

      ef_RetrigNote:
        If (event[chan].effect <> 0) then
          begin
            If NOT (eLo[chan] in [ef_RetrigNote,ef_MultiRetrigNote]) then
              retrig_table[chan] := 1;
            effect_table[chan] := concw(ef_RetrigNote,event[chan].effect);
          end;

      ef_SetGlobalVolume:
        begin
          global_volume := event[chan].effect;
          set_global_volume;
        end;

      ef_MultiRetrigNote:
        If (event[chan].effect DIV 16 <> 0) then
          begin
            If NOT (eLo[chan] in [ef_RetrigNote,ef_MultiRetrigNote]) then
              retrig_table[chan] := 1;
            effect_table[chan] := concw(ef_MultiRetrigNote,event[chan].effect);
          end;

      ef_Tremor:
        If (event[chan].effect DIV 16 <> 0) and
           (event[chan].effect MOD 16 <> 0) then
        begin
          If (eLo[chan] <> ef_Tremor) then
            begin
              tremor_table[chan].pos := 0;
              tremor_table[chan].volume := volume_table[chan];
            end;
          effect_table[chan] := concw(ef_Tremor,event[chan].effect);
        end;

      ef_Extended:
        Case (event[chan].effect DIV 16) of
          ef_ex_SetTremDepth:
            Case (event[chan].effect MOD 16) of
              0: begin
                   opl3out(_instr[11],misc_register AND $07f);
                   current_tremolo_depth := 0;
                 end;

              1: begin
                   opl3out(_instr[11],misc_register OR $080);
                   current_tremolo_depth := 1;
                 end;
            end;

          ef_ex_SetVibDepth:
            Case (event[chan].effect MOD 16) of
              0: begin
                   opl3out(_instr[11],misc_register AND $0bf);
                   current_vibrato_depth := 0;
                 end;

              1: begin
                   opl3out(_instr[11],misc_register OR $040);
                   current_vibrato_depth := 1;
                 end;
            end;

          ef_ex_SetAttckRateM:
            begin
              fmpar_table[chan].adsrw_mod.attck := event[chan].effect MOD 16;
              update_modulator_adsrw(chan);
            end;

          ef_ex_SetDecayRateM:
            begin
              fmpar_table[chan].adsrw_mod.dec := event[chan].effect MOD 16;
              update_modulator_adsrw(chan);
            end;

          ef_ex_SetSustnLevelM:
            begin
              fmpar_table[chan].adsrw_mod.sustn := event[chan].effect MOD 16;
              update_modulator_adsrw(chan);
            end;

          ef_ex_SetRelRateM:
            begin
              fmpar_table[chan].adsrw_mod.rel := event[chan].effect MOD 16;
              update_modulator_adsrw(chan);
            end;

          ef_ex_SetAttckRateC:
            begin
              fmpar_table[chan].adsrw_car.attck := event[chan].effect MOD 16;
              update_carrier_adsrw(chan);
            end;

          ef_ex_SetDecayRateC:
            begin
              fmpar_table[chan].adsrw_car.dec := event[chan].effect MOD 16;
              update_carrier_adsrw(chan);
            end;

          ef_ex_SetSustnLevelC:
            begin
              fmpar_table[chan].adsrw_car.sustn := event[chan].effect MOD 16;
              update_carrier_adsrw(chan);
            end;

          ef_ex_SetRelRateC:
            begin
              fmpar_table[chan].adsrw_car.rel := event[chan].effect MOD 16;
              update_carrier_adsrw(chan);
            end;

          ef_ex_SetFeedback:
            begin
              fmpar_table[chan].feedb := event[chan].effect MOD 16;
              update_fmpar(chan);
            end;

          ef_ex_SetPanningPos:
            begin
              panning_table[chan] := event[chan].effect MOD 16;
              update_fmpar(chan);
            end;

          ef_ex_PatternLoop,
          ef_ex_PatternLoopRec:
            If (event[chan].effect MOD 16 = 0) then
              loopbck_table[chan] := current_line
            else If (loopbck_table[chan] <> BYTE_NULL) then
                   begin
                     If (loop_table[chan][current_line] = BYTE_NULL) then
                       loop_table[chan][current_line] := event[chan].effect MOD 16;
                     If (loop_table[chan][current_line] <> 0) then
                       begin
                         pattern_break := TRUE;
                         next_line := pattern_loop_flag+chan;
                       end
                     else If (event[chan].effect DIV 16 = ef_ex_PatternLoopRec) then
                            loop_table[chan][current_line] := BYTE_NULL;
                   end;

          ef_ex_ExtendedCmd:
            Case (event[chan].effect MOD 16) of
              ef_ex_cmd_MKOffLoopDi: keyoff_loop[chan] := FALSE;
              ef_ex_cmd_MKOffLoopEn: keyoff_loop[chan] := TRUE;
              ef_ex_cmd_TPortaFKdis: portaFK_table[chan] := FALSE;
              ef_ex_cmd_TPortaFKenb: portaFK_table[chan] := TRUE;

              ef_ex_cmd_RestartEnv:
                begin
                  key_on(chan);
                  change_freq(chan,freq_table[chan]);
                end;

              ef_ex_cmd_4opVlockOff:
                If is_4op_chan(chan) then
                  begin
                    vol4op_lock[chan] := FALSE;
                    If (chan in _4op_tracks_hi) then
                      vol4op_lock[SUCC(chan)] := FALSE
                    else vol4op_lock[PRED(chan)] := FALSE;
                  end;

              ef_ex_cmd_4opVlockOn:
                If is_4op_chan(chan) then
                  begin
                    vol4op_lock[chan] := TRUE;
                    If (chan in _4op_tracks_hi) then
                      vol4op_lock[SUCC(chan)] := TRUE
                    else vol4op_lock[PRED(chan)] := TRUE;
                  end;
            end;

          ef_ex_ExtendedCmd2:
            Case (event[chan].effect MOD 16) of
              ef_ex_cmd2_RSS:        release_sustaining_sound(chan);
              ef_ex_cmd2_ResetVol:   reset_ins_volume(chan);
              ef_ex_cmd2_LockVol:    volume_lock  [chan] := TRUE;
              ef_ex_cmd2_UnlockVol:  volume_lock  [chan] := FALSE;
              ef_ex_cmd2_LockVP:     peak_lock    [chan] := TRUE;
              ef_ex_cmd2_UnlockVP:   peak_lock    [chan] := FALSE;
              ef_ex_cmd2_VSlide_def: volslide_type[chan] := 0;
              ef_ex_cmd2_LockPan:    pan_lock     [chan] := TRUE;
              ef_ex_cmd2_UnlockPan:  pan_lock     [chan] := FALSE;
              ef_ex_cmd2_VibrOff:    change_frequency(chan,freq_table[chan]);

              ef_ex_cmd2_TremOff:
                If is_4op_chan(chan) then
                  set_ins_volume_4op(BYTE_NULL,chan)
                else set_ins_volume(LO(volume_table[chan]),
                                    HI(volume_table[chan]),chan);

              ef_ex_cmd2_VSlide_car:
                If (event[chan].effect_def2 = ef_Extended) and
                   (event[chan].effect2 = ef_ex_ExtendedCmd2*16+
                                    ef_ex_cmd2_VSlide_mod) then
                  volslide_type[chan] := 3
                else volslide_type[chan] := 1;

              ef_ex_cmd2_VSlide_mod:
                If (event[chan].effect_def2 = ef_Extended) and
                   (event[chan].effect2 = ef_ex_ExtendedCmd2*16+
                                    ef_ex_cmd2_VSlide_car) then
                  volslide_type[chan] := 3
                else volslide_type[chan] := 2;
            end;
        end;

      ef_Extended2:
        Case (event[chan].effect DIV 16) of
          ef_ex2_PatDelayFrame,
          ef_ex2_PatDelayRow:
            begin
              pattern_delay := TRUE;
              If (event[chan].effect DIV 16 = ef_ex2_PatDelayFrame) then
                tickD := (event[chan].effect MOD 16)
              else tickD := speed*(event[chan].effect MOD 16);
            end;

          ef_ex2_NoteDelay:
            begin
              effect_table[chan] := concw(ef_extended2+ef_fix2+ef_ex2_NoteDelay,0);
              notedel_table[chan] := event[chan].effect MOD 16;
            end;

          ef_ex2_NoteCut:
            begin
              effect_table[chan] := concw(ef_extended2+ef_fix2+ef_ex2_NoteCut,0);
              notecut_table[chan] := event[chan].effect MOD 16;
            end;

          ef_ex2_FineTuneUp:
            Inc(ftune_table[chan],event[chan].effect MOD 16);

          ef_ex2_FineTuneDown:
            Dec(ftune_table[chan],event[chan].effect MOD 16);

          ef_ex2_GlVolSlideUp:
            effect_table[chan] := concw(ef_extended2+ef_fix2+ef_ex2_GlVolSlideUp,
                                        event[chan].effect MOD 16);
          ef_ex2_GlVolSlideDn:
            effect_table[chan] := concw(ef_extended2+ef_fix2+ef_ex2_GlVolSlideDn,
                                        event[chan].effect MOD 16);
          ef_ex2_GlVolSlideUpF:
            effect_table[chan] := concw(ef_extended2+ef_fix2+ef_ex2_GlVolSlideUpF,
                                        event[chan].effect MOD 16);
          ef_ex2_GlVolSlideDnF:
            effect_table[chan] := concw(ef_extended2+ef_fix2+ef_ex2_GlVolSlideDnF,
                                        event[chan].effect MOD 16);
          ef_ex2_GlVolSldUpXF:
            effect_table[chan] := concw(ef_extended2+ef_fix2+ef_ex2_GlVolSldUpXF,
                                        event[chan].effect MOD 16);
          ef_ex2_GlVolSldDnXF:
            effect_table[chan] := concw(ef_extended2+ef_fix2+ef_ex2_GlVolSldDnXF,
                                        event[chan].effect MOD 16);
          ef_ex2_VolSlideUpXF:
            effect_table[chan] := concw(ef_extended2+ef_fix2+ef_ex2_VolSlideUpXF,
                                        event[chan].effect MOD 16);
          ef_ex2_VolSlideDnXF:
            effect_table[chan] := concw(ef_extended2+ef_fix2+ef_ex2_VolSlideDnXF,
                                        event[chan].effect MOD 16);
          ef_ex2_FreqSlideUpXF:
            effect_table[chan] := concw(ef_extended2+ef_fix2+ef_ex2_FreqSlideUpXF,
                                        event[chan].effect MOD 16);
          ef_ex2_FreqSlideDnXF:
            effect_table[chan] := concw(ef_extended2+ef_fix2+ef_ex2_FreqSlideDnXF,
                                        event[chan].effect MOD 16);
        end;

      ef_Extended3:
        Case (event[chan].effect DIV 16) of
          ef_ex3_SetConnection:
            begin
              fmpar_table[chan].connect := event[chan].effect MOD 16;
              update_fmpar(chan);
            end;

          ef_ex3_SetMultipM:
            begin
              fmpar_table[chan].multipM := event[chan].effect MOD 16;
              update_fmpar(chan);
            end;

          ef_ex3_SetKslM:
            begin
              fmpar_table[chan].kslM := event[chan].effect MOD 16;
              update_fmpar(chan);
            end;

          ef_ex3_SetTremoloM:
            begin
              fmpar_table[chan].tremM := event[chan].effect MOD 16;
              update_fmpar(chan);
            end;

          ef_ex3_SetVibratoM:
            begin
              fmpar_table[chan].vibrM := event[chan].effect MOD 16;
              update_fmpar(chan);
            end;

          ef_ex3_SetKsrM:
            begin
              fmpar_table[chan].ksrM := event[chan].effect MOD 16;
              update_fmpar(chan);
            end;

          ef_ex3_SetSustainM:
            begin
              fmpar_table[chan].sustM := event[chan].effect MOD 16;
              update_fmpar(chan);
            end;

          ef_ex3_SetMultipC:
            begin
              fmpar_table[chan].multipC := event[chan].effect MOD 16;
              update_fmpar(chan);
            end;

          ef_ex3_SetKslC:
            begin
              fmpar_table[chan].kslC := event[chan].effect MOD 16;
              update_fmpar(chan);
            end;

          ef_ex3_SetTremoloC:
            begin
              fmpar_table[chan].tremC := event[chan].effect MOD 16;
              update_fmpar(chan);
            end;

          ef_ex3_SetVibratoC:
            begin
              fmpar_table[chan].vibrC := event[chan].effect MOD 16;
              update_fmpar(chan);
            end;

          ef_ex3_SetKsrC:
            begin
              fmpar_table[chan].ksrC := event[chan].effect MOD 16;
              update_fmpar(chan);
            end;

          ef_ex3_SetSustainC:
            begin
              fmpar_table[chan].sustC := event[chan].effect MOD 16;
              update_fmpar(chan);
            end;
        end;
    end;

  For chan := 1 to songdata.nm_tracks do
    Case event[chan].effect_def2 of
      ef_Arpeggio,
      ef_ExtraFineArpeggio,
      ef_ArpggVSlide,
      ef_ArpggVSlideFine:
        If (event[chan].effect_def2 <> ef_Arpeggio) or
           (event[chan].effect2 <> 0) then
          begin
            Case event[chan].effect_def2 of
              ef_Arpeggio:
                effect_table2[chan] := concw(ef_Arpeggio+ef_fix1,event[chan].effect2);

              ef_ExtraFineArpeggio:
                effect_table2[chan] := concw(ef_ExtraFineArpeggio,event[chan].effect2);

              ef_ArpggVSlide,
              ef_ArpggVSlideFine:
                If (event[chan].effect2 <> 0) then
                  effect_table2[chan] := concw(event[chan].effect_def2,event[chan].effect2)
                else If (eLo2[chan] in [ef_ArpggVSlide,ef_ArpggVSlideFine]) and
                        (eHi2[chan] <> 0) then
                       effect_table2[chan] := concw(event[chan].effect_def2,eHi2[chan])
                     else effect_table2[chan] := effect_table2[chan] AND $0ff00;
            end;

            If (event[chan].note AND $7f in [1..12*8+1]) then
              begin
                arpgg_table2[chan].state := 0;
                arpgg_table2[chan].note := event[chan].note AND $7f;
                If (event[chan].effect_def2 in [ef_Arpeggio,ef_ExtraFineArpeggio]) then
                  begin
                    arpgg_table2[chan].add1 := event[chan].effect2 DIV 16;
                    arpgg_table2[chan].add2 := event[chan].effect2 MOD 16;
                  end;
              end
            else If (event[chan].note = 0) and
                    (event_table[chan].note AND $7f in [1..12*8+1]) then
                   begin
                     If NOT (eLo2[chan] in [ef_Arpeggio+ef_fix1,ef_ExtraFineArpeggio,
                                     ef_ArpggVSlide,ef_ArpggVSlideFine]) then
                       arpgg_table2[chan].state := 0;

                     arpgg_table2[chan].note := event_table[chan].note AND $7f;
                     If (event[chan].effect_def2 in [ef_Arpeggio,ef_ExtraFineArpeggio]) then
                       begin
                         arpgg_table2[chan].add1 := event[chan].effect2 DIV 16;
                         arpgg_table2[chan].add2 := event[chan].effect2 MOD 16;
                       end;
                   end
                 else effect_table2[chan] := 0;
          end;

      ef_FSlideUp,
      ef_FSlideDown,
      ef_FSlideUpFine,
      ef_FSlideDownFine:
        begin
          effect_table2[chan] := concw(event[chan].effect_def2,event[chan].effect2);
          fslide_table2[chan] := event[chan].effect2;
        end;

      ef_FSlideUpVSlide,
      ef_FSlUpVSlF,
      ef_FSlideDownVSlide,
      ef_FSlDownVSlF,
      ef_FSlUpFineVSlide,
      ef_FSlUpFineVSlF,
      ef_FSlDownFineVSlide,
      ef_FSlDownFineVSlF:
        If (event[chan].effect2 <> 0) then
          effect_table2[chan] := concw(event[chan].effect_def2,event[chan].effect2)
        else If (eLo2[chan] in [ef_FSlideUpVSlide,ef_FSlUpVSlF,ef_FSlideDownVSlide,
                         ef_FSlDownVSlF,ef_FSlUpFineVSlide,ef_FSlUpFineVSlF,
                         ef_FSlDownFineVSlide,ef_FSlDownFineVSlF]) and
                (eHi2[chan] <> 0) then
               effect_table2[chan] := concw(event[chan].effect_def2,eHi2[chan])
             else effect_table2[chan] := effect_table2[chan] AND $0ff00;

      ef_TonePortamento:
        If (event[chan].note in [1..12*8+1]) then
          begin
            If (event[chan].effect2 <> 0) then
              effect_table2[chan] := concw(ef_TonePortamento,event[chan].effect2)
            else If (eLo2[chan] = ef_TonePortamento) and
                    (eHi2[chan] <> 0) then
                   effect_table2[chan] := concw(ef_TonePortamento,eHi2[chan])
                 else effect_table2[chan] := ef_TonePortamento;

            porta_table2[chan].speed := HI(effect_table2[chan]);
            porta_table2[chan].freq := nFreq(event[chan].note-1)+
              SHORTINT(ins_parameter(event_table[chan].instr_def,12));
          end
        else If (eLo2[chan] = ef_TonePortamento) then
               begin
                 If (event[chan].effect2 <> 0) then
                   effect_table2[chan] := concw(ef_TonePortamento,event[chan].effect2)
                 else If (eLo2[chan] = ef_TonePortamento) and
                         (eHi2[chan] <> 0) then
                        effect_table2[chan] := concw(ef_TonePortamento,eHi2[chan])
                      else effect_table2[chan] := ef_TonePortamento;
                 porta_table2[chan].speed := HI(effect_table2[chan]);
               end;

      ef_TPortamVolSlide,
      ef_TPortamVSlideFine:
        If (event[chan].effect2 <> 0) then
          effect_table2[chan] := concw(event[chan].effect_def2,event[chan].effect2)
        else If (eLo2[chan] in [ef_TPortamVolSlide,ef_TPortamVSlideFine]) and
                (eHi2[chan] <> 0) then
               effect_table2[chan] := concw(event[chan].effect_def2,eHi2[chan])
             else effect_table2[chan] := effect_table2[chan] AND $0ff00;

      ef_Vibrato,
      ef_ExtraFineVibrato:
        begin
          If (event[chan].effect2 <> 0) then
            effect_table2[chan] := concw(event[chan].effect_def2,event[chan].effect2)
          else If (eLo2[chan] in [ef_Vibrato,ef_ExtraFineVibrato]) and
                  (eHi2[chan] <> 0) then
                 effect_table2[chan] := concw(event[chan].effect_def2,eHi2[chan])
               else effect_table2[chan] := event[chan].effect_def2;

          If (event[chan].effect_def = ef_Extended) and
             (event[chan].effect = ef_ex_ExtendedCmd2*16+ef_ex_cmd2_FVib_FGFS) then
            vibr_table2[chan].fine := TRUE;

          vibr_table2[chan].speed := HI(effect_table2[chan]) DIV 16;
          vibr_table2[chan].depth := HI(effect_table2[chan]) MOD 16;
        end;

      ef_Tremolo,
      ef_ExtraFineTremolo:
        begin
          If (event[chan].effect2 <> 0) then
            effect_table2[chan] := concw(event[chan].effect_def2,event[chan].effect2)
          else If (eLo2[chan] in [ef_Tremolo,ef_ExtraFineTremolo]) and
                  (eHi2[chan] <> 0) then
                 effect_table2[chan] := concw(event[chan].effect_def2,eHi2[chan])
               else effect_table2[chan] := event[chan].effect_def2;

          If (event[chan].effect_def = ef_Extended) and
             (event[chan].effect = ef_ex_ExtendedCmd2*16+ef_ex_cmd2_FTrm_XFGFS) then
            trem_table2[chan].fine := TRUE;

          trem_table2[chan].speed := HI(effect_table2[chan]) DIV 16;
          trem_table2[chan].depth := HI(effect_table2[chan]) MOD 16;
        end;

      ef_VibratoVolSlide,
      ef_VibratoVSlideFine:
        begin
          If (event[chan].effect2 <> 0) then
            effect_table2[chan] := concw(event[chan].effect_def2,event[chan].effect2)
          else If (eLo2[chan] in [ef_VibratoVolSlide,ef_VibratoVSlideFine]) and
                  (HI(effect_table2[chan]) <> 0) then
                 effect_table2[chan] := concw(event[chan].effect_def2,HI(effect_table2[chan]))
               else effect_table2[chan] := effect_table2[chan] AND $0ff00;

          If (event[chan].effect_def = ef_Extended) and
             (event[chan].effect = ef_ex_ExtendedCmd2*16+ef_ex_cmd2_FVib_FGFS) then
            vibr_table2[chan].fine := TRUE;
        end;

      ef_SetCarrierVol:
        set_ins_volume(BYTE_NULL,63-event[chan].effect2,chan);

      ef_SetModulatorVol:
        set_ins_volume(63-event[chan].effect2,BYTE_NULL,chan);

      ef_SetInsVolume:
        If _4op_vol_valid_chan(chan) then
          set_ins_volume_4op(63-event[chan].effect2,chan)
        else If percussion_mode and (chan in [17..20]) then
               set_ins_volume(63-event[chan].effect2,BYTE_NULL,chan)
             else If (ins_parameter(voice_table[chan],10) AND 1 = 0) then
                    set_ins_volume(BYTE_NULL,63-event[chan].effect2,chan)
                  else set_ins_volume(63-event[chan].effect2,63-event[chan].effect2,chan);

      ef_ForceInsVolume:
        If percussion_mode and (chan in [17..20]) then
          set_ins_volume(63-event[chan].effect2,BYTE_NULL,chan)
        else If (ins_parameter(voice_table[chan],10) AND 1 = 0) then
               set_ins_volume(scale_volume(ins_parameter(voice_table[chan],2) AND $3f,63-event[chan].effect2),63-event[chan].effect2,chan)
             else set_ins_volume(63-event[chan].effect2,63-event[chan].effect2,chan);

      ef_PositionJump:
        If no_loop(chan,current_line) then
          begin
            pattern_break := TRUE;
            next_line := pattern_break_flag+chan;
          end;

      ef_PatternBreak:
        If no_loop(chan,current_line) then
          begin
            pattern_break := TRUE;
{$IFNDEF ADT2PLAY}
            seek_pattern_break := TRUE;
{$ENDIF} // NOT DEFINED(ADT2PLAY)
            next_line := max(event[chan].effect2,PRED(songdata.patt_len));
          end;

      ef_SetSpeed:
        speed := event[chan].effect2;

      ef_SetTempo:
        update_timer(event[chan].effect2);

      ef_SetWaveform:
        begin
          If (event[chan].effect2 DIV 16 in [0..7]) then
            begin
              fmpar_table[chan].adsrw_car.wform := event[chan].effect2 DIV 16;
              update_carrier_adsrw(chan);
            end;

          If (event[chan].effect2 MOD 16 in [0..7]) then
            begin
              fmpar_table[chan].adsrw_mod.wform := event[chan].effect2 MOD 16;
              update_modulator_adsrw(chan);
            end;
        end;

      ef_VolSlide:
        effect_table2[chan] := concw(ef_VolSlide,event[chan].effect2);

      ef_VolSlideFine:
        effect_table2[chan] := concw(ef_VolSlideFine,event[chan].effect2);

      ef_RetrigNote:
        If (event[chan].effect2 <> 0) then
          begin
            If NOT (eLo2[chan] in [ef_RetrigNote,ef_MultiRetrigNote]) then
              retrig_table2[chan] := 1;
            effect_table2[chan] := concw(ef_RetrigNote,event[chan].effect2);
          end;

      ef_SetGlobalVolume:
        begin
          global_volume := event[chan].effect2;
          set_global_volume;
        end;

      ef_MultiRetrigNote:
        If (event[chan].effect2 DIV 16 <> 0) then
          begin
            If NOT (eLo2[chan] in [ef_RetrigNote,ef_MultiRetrigNote]) then
              retrig_table2[chan] := 1;
            effect_table2[chan] := concw(ef_MultiRetrigNote,event[chan].effect2);
          end;

      ef_Tremor:
        If (event[chan].effect2 DIV 16 <> 0) and
           (event[chan].effect2 MOD 16 <> 0) then
        begin
          If (eLo2[chan] <> ef_Tremor) then
            begin
              tremor_table2[chan].pos := 0;
              tremor_table2[chan].volume := volume_table[chan];
            end;
          effect_table2[chan] := concw(ef_Tremor,event[chan].effect2);
        end;

      ef_Extended:
        Case (event[chan].effect2 DIV 16) of
          ef_ex_SetTremDepth:
            Case (event[chan].effect2 MOD 16) of
              0: begin
                   opl3out(_instr[11],misc_register AND $07f);
                   current_tremolo_depth := 0;
                 end;

              1: begin
                   opl3out(_instr[11],misc_register OR $080);
                   current_tremolo_depth := 1;
                 end;
            end;

          ef_ex_SetVibDepth:
            Case (event[chan].effect2 MOD 16) of
              0: begin
                   opl3out(_instr[11],misc_register AND $0bf);
                   current_vibrato_depth := 0;
                 end;

              1: begin
                   opl3out(_instr[11],misc_register OR $040);
                   current_vibrato_depth := 1;
                 end;
            end;

          ef_ex_SetAttckRateM:
            begin
              fmpar_table[chan].adsrw_mod.attck := event[chan].effect2 MOD 16;
              update_modulator_adsrw(chan);
            end;

          ef_ex_SetDecayRateM:
            begin
              fmpar_table[chan].adsrw_mod.dec := event[chan].effect2 MOD 16;
              update_modulator_adsrw(chan);
            end;

          ef_ex_SetSustnLevelM:
            begin
              fmpar_table[chan].adsrw_mod.sustn := event[chan].effect2 MOD 16;
              update_modulator_adsrw(chan);
            end;

          ef_ex_SetRelRateM:
            begin
              fmpar_table[chan].adsrw_mod.rel := event[chan].effect2 MOD 16;
              update_modulator_adsrw(chan);
            end;

          ef_ex_SetAttckRateC:
            begin
              fmpar_table[chan].adsrw_car.attck := event[chan].effect2 MOD 16;
              update_carrier_adsrw(chan);
            end;

          ef_ex_SetDecayRateC:
            begin
              fmpar_table[chan].adsrw_car.dec := event[chan].effect2 MOD 16;
              update_carrier_adsrw(chan);
            end;

          ef_ex_SetSustnLevelC:
            begin
              fmpar_table[chan].adsrw_car.sustn := event[chan].effect2 MOD 16;
              update_carrier_adsrw(chan);
            end;

          ef_ex_SetRelRateC:
            begin
              fmpar_table[chan].adsrw_car.rel := event[chan].effect2 MOD 16;
              update_carrier_adsrw(chan);
            end;

          ef_ex_SetFeedback:
            begin
              fmpar_table[chan].feedb := event[chan].effect2 MOD 16;
              update_fmpar(chan);
            end;

          ef_ex_SetPanningPos:
            begin
              panning_table[chan] := event[chan].effect2 MOD 16;
              update_fmpar(chan);
            end;

          ef_ex_PatternLoop,
          ef_ex_PatternLoopRec:
            If (event[chan].effect2 MOD 16 = 0) then
              loopbck_table[chan] := current_line
            else If (loopbck_table[chan] <> BYTE_NULL) then
                   begin
                     If (loop_table[chan][current_line] = BYTE_NULL) then
                       loop_table[chan][current_line] := event[chan].effect2 MOD 16;
                     If (loop_table[chan][current_line] <> 0) then
                       begin
                         pattern_break := TRUE;
                         next_line := pattern_loop_flag+chan;
                       end
                     else If (event[chan].effect2 DIV 16 = ef_ex_PatternLoopRec) then
                            loop_table[chan][current_line] := BYTE_NULL;
                   end;

          ef_ex_ExtendedCmd:
            Case (event[chan].effect2 MOD 16) of
              ef_ex_cmd_MKOffLoopDi: keyoff_loop[chan] := FALSE;
              ef_ex_cmd_MKOffLoopEn: keyoff_loop[chan] := TRUE;
              ef_ex_cmd_TPortaFKdis: portaFK_table[chan] := FALSE;
              ef_ex_cmd_TPortaFKenb: portaFK_table[chan] := TRUE;

              ef_ex_cmd_RestartEnv:
                begin
                  key_on(chan);
                  change_freq(chan,freq_table[chan]);
                end;

              ef_ex_cmd_4opVlockOff:
                If is_4op_chan(chan) then
                  begin
                    vol4op_lock[chan] := FALSE;
                    If (chan in _4op_tracks_hi) then
                      vol4op_lock[SUCC(chan)] := FALSE
                    else vol4op_lock[PRED(chan)] := FALSE;
                  end;

              ef_ex_cmd_4opVlockOn:
                If is_4op_chan(chan) then
                  begin
                    vol4op_lock[chan] := TRUE;
                    If (chan in _4op_tracks_hi) then
                      vol4op_lock[SUCC(chan)] := TRUE
                    else vol4op_lock[PRED(chan)] := TRUE;
                  end;
            end;

          ef_ex_ExtendedCmd2:
            Case (event[chan].effect2 MOD 16) of
              ef_ex_cmd2_RSS:        release_sustaining_sound(chan);
              ef_ex_cmd2_ResetVol:   reset_ins_volume(chan);
              ef_ex_cmd2_LockVol:    volume_lock  [chan] := TRUE;
              ef_ex_cmd2_UnlockVol:  volume_lock  [chan] := FALSE;
              ef_ex_cmd2_LockVP:     peak_lock    [chan] := TRUE;
              ef_ex_cmd2_UnlockVP:   peak_lock    [chan] := FALSE;
              ef_ex_cmd2_VSlide_def: volslide_type[chan] := 0;
              ef_ex_cmd2_LockPan:    pan_lock     [chan] := TRUE;
              ef_ex_cmd2_UnlockPan:  pan_lock     [chan] := FALSE;
              ef_ex_cmd2_VibrOff:    change_frequency(chan,freq_table[chan]);

              ef_ex_cmd2_TremOff:
                If _4op_vol_valid_chan(chan) then
                  set_ins_volume_4op(BYTE_NULL,chan)
                else set_ins_volume(LO(volume_table[chan]),
                                    HI(volume_table[chan]),chan);

              ef_ex_cmd2_VSlide_car:
                If NOT ((event[chan].effect_def = ef_Extended) and
                        (event[chan].effect = ef_ex_ExtendedCmd2*16+
                                        ef_ex_cmd2_VSlide_mod)) then
                  volslide_type[chan] := 1;

              ef_ex_cmd2_VSlide_mod:
                If NOT ((event[chan].effect_def = ef_Extended) and
                        (event[chan].effect = ef_ex_ExtendedCmd2*16+
                                        ef_ex_cmd2_VSlide_car)) then
                  volslide_type[chan] := 2;
            end;
        end;

      ef_Extended2:
        Case (event[chan].effect2 DIV 16) of
          ef_ex2_PatDelayFrame,
          ef_ex2_PatDelayRow:
            begin
              pattern_delay := TRUE;
              If (event[chan].effect2 DIV 16 = ef_ex2_PatDelayFrame) then
                tickD := (event[chan].effect2 MOD 16)
              else tickD := speed*(event[chan].effect2 MOD 16);
            end;

          ef_ex2_NoteDelay:
            begin
              effect_table2[chan] := concw(ef_extended2+ef_fix2+ef_ex2_NoteDelay,0);
              notedel_table[chan] := event[chan].effect2 MOD 16;
            end;

          ef_ex2_NoteCut:
            begin
              effect_table2[chan] := concw(ef_extended2+ef_fix2+ef_ex2_NoteCut,0);
              notecut_table[chan] := event[chan].effect2 MOD 16;
            end;

          ef_ex2_FineTuneUp:
            Inc(ftune_table[chan],event[chan].effect2 MOD 16);

          ef_ex2_FineTuneDown:
            Dec(ftune_table[chan],event[chan].effect2 MOD 16);

          ef_ex2_GlVolSlideUp:
            effect_table2[chan] := concw(ef_extended2+ef_fix2+ef_ex2_GlVolSlideUp,
                                         event[chan].effect2 MOD 16);
          ef_ex2_GlVolSlideDn:
            effect_table2[chan] := concw(ef_extended2+ef_fix2+ef_ex2_GlVolSlideDn,
                                         event[chan].effect2 MOD 16);
          ef_ex2_GlVolSlideUpF:
            effect_table2[chan] := concw(ef_extended2+ef_fix2+ef_ex2_GlVolSlideUpF,
                                         event[chan].effect2 MOD 16);
          ef_ex2_GlVolSlideDnF:
            effect_table2[chan] := concw(ef_extended2+ef_fix2+ef_ex2_GlVolSlideDnF,
                                         event[chan].effect2 MOD 16);
          ef_ex2_GlVolSldUpXF:
            effect_table2[chan] := concw(ef_extended2+ef_fix2+ef_ex2_GlVolSldUpXF,
                                         event[chan].effect2 MOD 16);
          ef_ex2_GlVolSldDnXF:
            effect_table2[chan] := concw(ef_extended2+ef_fix2+ef_ex2_GlVolSldDnXF,
                                         event[chan].effect2 MOD 16);
          ef_ex2_VolSlideUpXF:
            effect_table2[chan] := concw(ef_extended2+ef_fix2+ef_ex2_VolSlideUpXF,
                                         event[chan].effect2 MOD 16);
          ef_ex2_VolSlideDnXF:
            effect_table2[chan] := concw(ef_extended2+ef_fix2+ef_ex2_VolSlideDnXF,
                                         event[chan].effect2 MOD 16);
          ef_ex2_FreqSlideUpXF:
            effect_table2[chan] := concw(ef_extended2+ef_fix2+ef_ex2_FreqSlideUpXF,
                                         event[chan].effect2 MOD 16);
          ef_ex2_FreqSlideDnXF:
            effect_table2[chan] := concw(ef_extended2+ef_fix2+ef_ex2_FreqSlideDnXF,
                                         event[chan].effect2 MOD 16);
        end;

      ef_Extended3:
        Case (event[chan].effect2 DIV 16) of
          ef_ex3_SetConnection:
            begin
              fmpar_table[chan].connect := event[chan].effect2 MOD 16;
              update_fmpar(chan);
            end;

          ef_ex3_SetMultipM:
            begin
              fmpar_table[chan].multipM := event[chan].effect2 MOD 16;
              update_fmpar(chan);
            end;

          ef_ex3_SetKslM:
            begin
              fmpar_table[chan].kslM := event[chan].effect2 MOD 16;
              update_fmpar(chan);
            end;

          ef_ex3_SetTremoloM:
            begin
              fmpar_table[chan].tremM := event[chan].effect2 MOD 16;
              update_fmpar(chan);
            end;

          ef_ex3_SetVibratoM:
            begin
              fmpar_table[chan].vibrM := event[chan].effect2 MOD 16;
              update_fmpar(chan);
            end;

          ef_ex3_SetKsrM:
            begin
              fmpar_table[chan].ksrM := event[chan].effect2 MOD 16;
              update_fmpar(chan);
            end;

          ef_ex3_SetSustainM:
            begin
              fmpar_table[chan].sustM := event[chan].effect2 MOD 16;
              update_fmpar(chan);
            end;

          ef_ex3_SetMultipC:
            begin
              fmpar_table[chan].multipC := event[chan].effect2 MOD 16;
              update_fmpar(chan);
            end;

          ef_ex3_SetKslC:
            begin
              fmpar_table[chan].kslC := event[chan].effect2 MOD 16;
              update_fmpar(chan);
            end;

          ef_ex3_SetTremoloC:
            begin
              fmpar_table[chan].tremC := event[chan].effect2 MOD 16;
              update_fmpar(chan);
            end;

          ef_ex3_SetVibratoC:
            begin
              fmpar_table[chan].vibrC := event[chan].effect2 MOD 16;
              update_fmpar(chan);
            end;

          ef_ex3_SetKsrC:
            begin
              fmpar_table[chan].ksrC := event[chan].effect2 MOD 16;
              update_fmpar(chan);
            end;

          ef_ex3_SetSustainC:
            begin
              fmpar_table[chan].sustC := event[chan].effect2 MOD 16;
              update_fmpar(chan);
            end;
        end;
    end;

  For chan := 1 to songdata.nm_tracks do
    begin
      tporta_flag := (event[chan].effect_def in [ef_TonePortamento,
                                                 ef_TPortamVolSlide,
                                                 ef_TPortamVSlideFine]) or
                     (event[chan].effect_def2 in [ef_TonePortamento,
                                                  ef_TPortamVolSlide,
                                                  ef_TPortamVSlideFine]);

      If (event[chan].effect_def+event[chan].effect = 0) then
        If (glfsld_table[chan] = 0) then effect_table[chan] := 0
{$IFNDEF ADT2PLAY}
        else
{$ENDIF} // NOT DEFINED(ADT2PLAY)
      else begin
             event_table[chan].effect_def := event[chan].effect_def;
             event_table[chan].effect := event[chan].effect;
           end;

      If (event[chan].effect_def2+event[chan].effect2 = 0) then
        If (glfsld_table2[chan] = 0) then effect_table2[chan] := 0
{$IFNDEF ADT2PLAY}
        else
{$ENDIF} // NOT DEFINED(ADT2PLAY)
      else begin
             event_table[chan].effect_def2 := event[chan].effect_def2;
             event_table[chan].effect2 := event[chan].effect2;
           end;

      // treat Tone Portamento based effects
      //   vs. note Key-Off's
      //   vs. step-playing with Spacebar in Pattern Editor window
      If (event[chan].note = event[chan].note OR keyoff_flag) then key_off(chan)
      else If NOT (LO(effect_table[chan]) in  [ef_TonePortamento,
                                               ef_TPortamVolSlide,
                                               ef_TPortamVSlideFine,
                                               ef_extended2+ef_fix2+ef_ex2_NoteDelay]) and
              NOT (LO(effect_table2[chan]) in [ef_TonePortamento,
                                               ef_TPortamVolSlide,
                                               ef_TPortamVSlideFine,
                                               ef_extended2+ef_fix2+ef_ex2_NoteDelay]) then
             If NOT (((event[chan].effect_def2 = ef_SwapArpeggio) or
                      (event[chan].effect_def2 = ef_SwapVibrato)) and
                     (event[chan].effect_def = ef_Extended) and
                     (event[chan].effect DIV 16 = ef_ex_ExtendedCmd2) and
                     (event[chan].effect MOD 16 = ef_ex_cmd2_NoRestart)) and
                NOT (((event[chan].effect_def = ef_SwapArpeggio) or
                      (event[chan].effect_def = ef_SwapVibrato)) and
                     (event[chan].effect_def2 = ef_Extended) and
                     (event[chan].effect2 DIV 16 = ef_ex_ExtendedCmd2) and
                     (event[chan].effect2 MOD 16 = ef_ex_cmd2_NoRestart)) then
{$IFNDEF ADT2PLAY}
               If NOT ignore_note_once[chan] then
{$ENDIF} // NOT DEFINED(ADT2PLAY)
                 output_note(event[chan].note,voice_table[chan],chan,TRUE,TRUE)
{$IFNDEF ADT2PLAY}
               else
{$ENDIF} // NOT DEFINED(ADT2PLAY)
             else output_note(event[chan].note,voice_table[chan],chan,TRUE,FALSE)
{$IFDEF ADT2PLAY}
          else If (event[chan].note <> 0) and tporta_flag and
                  (event_table[chan].note = event_table[chan].note OR keyoff_flag) then
                 output_note(event_table[chan].note AND NOT keyoff_flag,voice_table[chan],chan,FALSE,TRUE)
               else If (event[chan].note <> 0) then
                      If portaFK_table[chan] and tporta_flag then
                        output_note(event[chan].note,event[chan].instr_def,chan,FALSE,TRUE)
                      else event_table[chan].note := event[chan].note;
{$ELSE} // NOT DEFINED(ADT2PLAY)
          else If NOT single_play and
                  (event[chan].note <> 0) and tporta_flag then
                 If (event_table[chan].note = event_table[chan].note OR keyoff_flag) then
                   If NOT ignore_note_once[chan] then
                     output_note(event_table[chan].note AND NOT keyoff_flag,voice_table[chan],chan,FALSE,TRUE)
                   else
                 else If portaFK_table[chan] then
                        output_note(event_table[chan].note AND NOT keyoff_flag,voice_table[chan],chan,FALSE,TRUE)
                      else If (event[chan].note <> 0) then
                             event_table[chan].note := event[chan].note
                           else
               else If single_play and
                       NOT (event[chan].note = event[chan].note OR keyoff_flag) and
                       NOT (event_table[chan].note <> 0) and
                       (event[chan].instr_def <> 0) and tporta_flag then
                      output_note(event[chan].note,event[chan].instr_def,chan,FALSE,TRUE)
                    else If (event[chan].note <> 0) then
                           If portaFK_table[chan] and tporta_flag then
                             output_note(event[chan].note,event[chan].instr_def,chan,FALSE,TRUE)
                           else event_table[chan].note := event[chan].note;
{$ENDIF} // NOT DEFINED(ADT2PLAY)

      Case event[chan].effect_def of
        ef_SwapArpeggio:
          begin
            If (event[chan].effect_def2 = ef_Extended) and
               (event[chan].effect2 DIV 16 = ef_ex_ExtendedCmd2) and
               (event[chan].effect2 MOD 16 = ef_ex_cmd2_NoRestart) then
              begin
                If (macro_table[chan].arpg_pos >
                    songdata.macro_table[event[chan].effect].arpeggio.length) then
                  macro_table[chan].arpg_pos :=
                    songdata.macro_table[event[chan].effect].arpeggio.length;
                macro_table[chan].arpg_table := event[chan].effect;
              end
            else begin
                   macro_table[chan].arpg_count := 1;
                   macro_table[chan].arpg_pos := 0;
                   macro_table[chan].arpg_table := event[chan].effect;
                   macro_table[chan].arpg_note := event_table[chan].note;
                 end;
          end;

        ef_SwapVibrato:
          begin
            If (event[chan].effect_def2 = ef_Extended) and
               (event[chan].effect2 DIV 16 = ef_ex_ExtendedCmd2) and
               (event[chan].effect2 MOD 16 = ef_ex_cmd2_NoRestart) then
              begin
                If (macro_table[chan].vib_table >
                    songdata.macro_table[event[chan].effect].vibrato.length) then
                  macro_table[chan].vib_pos :=
                    songdata.macro_table[event[chan].effect].vibrato.length;
                macro_table[chan].vib_table := event[chan].effect;
              end
            else begin
                   macro_table[chan].vib_count := 1;
                   macro_table[chan].vib_pos := 0;
                   macro_table[chan].vib_table := event[chan].effect;
                   macro_table[chan].vib_delay := songdata.macro_table[macro_table[chan].vib_table].vibrato.delay;
                 end;
          end;

        ef_SetCustomSpeedTab:
          generate_custom_vibrato(event[chan].effect);
      end;

      Case event[chan].effect_def2 of
        ef_SwapArpeggio:
          begin
            If (event[chan].effect_def = ef_Extended) and
               (event[chan].effect DIV 16 = ef_ex_ExtendedCmd2) and
               (event[chan].effect MOD 16 = ef_ex_cmd2_NoRestart) then
              begin
                If (macro_table[chan].arpg_pos >
                    songdata.macro_table[event[chan].effect2].arpeggio.length) then
                  macro_table[chan].arpg_pos :=
                    songdata.macro_table[event[chan].effect2].arpeggio.length;
                macro_table[chan].arpg_table := event[chan].effect2;
              end
            else begin
                   macro_table[chan].arpg_count := 1;
                   macro_table[chan].arpg_pos := 0;
                   macro_table[chan].arpg_table := event[chan].effect2;
                   macro_table[chan].arpg_note := event_table[chan].note;
                 end;
          end;

        ef_SwapVibrato:
          begin
            If (event[chan].effect_def = ef_Extended) and
               (event[chan].effect DIV 16 = ef_ex_ExtendedCmd2) and
               (event[chan].effect MOD 16 = ef_ex_cmd2_NoRestart) then
              begin
                If (macro_table[chan].vib_table >
                    songdata.macro_table[event[chan].effect2].vibrato.length) then
                  macro_table[chan].vib_pos :=
                    songdata.macro_table[event[chan].effect2].vibrato.length;
                macro_table[chan].vib_table := event[chan].effect2;
              end
            else begin
                   macro_table[chan].vib_count := 1;
                   macro_table[chan].vib_pos := 0;
                   macro_table[chan].vib_table := event[chan].effect2;
                   macro_table[chan].vib_delay := songdata.macro_table[macro_table[chan].vib_table].vibrato.delay;
                 end;
          end;

        ef_SetCustomSpeedTab:
          generate_custom_vibrato(event[chan].effect2);
      end;

      update_fine_effects(chan);
    end;

  If pattern_delay then
    begin
      time_playing := time_playing+1/tempo*tickD;
      If (time_playing > 3600-1) then time_playing := 0;
    end
  else begin
         time_playing := time_playing+1/tempo*speed;
         If (time_playing > 3600-1) then time_playing := 0;
       end;

  _dbg_leave; //EXIT //play_line
end;
