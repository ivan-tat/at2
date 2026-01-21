// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure import_old_a2m_event1(patt,line,chan: Byte; old_chunk: tOLD_CHUNK;
                                processing_whole_song: Boolean);

const
  fx_Arpeggio          = $00;
  fx_FSlideUp          = $01;
  fx_FSlideDown        = $02;
  fx_FSlideUpFine      = $03;
  fx_FSlideDownFine    = $04;
  fx_TonePortamento    = $05;
  fx_TPortamVolSlide   = $06;
  fx_Vibrato           = $07;
  fx_VibratoVolSlide   = $08;
  fx_SetOpIntensity    = $09;
  fx_SetInsVolume      = $0a;
  fx_PatternBreak      = $0b;
  fx_PatternJump       = $0c;
  fx_SetTempo          = $0d;
  fx_SetTimer          = $0e;
  fx_Extended          = $0f;
  fx_ex_DefAMdepth     = $00;
  fx_ex_DefVibDepth    = $01;
  fx_ex_DefWaveform    = $02;
  fx_ex_ManSlideUp     = $03;
  fx_ex_ManSlideDown   = $04;
  fx_ex_VSlideUp       = $05;
  fx_ex_VSlideDown     = $06;
  fx_ex_VSlideUpFine   = $07;
  fx_ex_VSlideDownFine = $08;
  fx_ex_RetrigNote     = $09;
  fx_ex_SetAttckRate   = $0a;
  fx_ex_SetDecayRate   = $0b;
  fx_ex_SetSustnLevel  = $0c;
  fx_ex_SetReleaseRate = $0d;
  fx_ex_SetFeedback    = $0e;
  fx_ex_ExtendedCmd    = $0f;

var
  chunk: tCHUNK;

begin
  FillChar(chunk,SizeOf(chunk),0);
  chunk.note := old_chunk.note;
  chunk.instr_def := old_chunk.instr_def;
  chunk.effect_def := old_chunk.effect_def;
  chunk.effect := old_chunk.effect;

  Case old_chunk.effect_def of
    fx_Arpeggio:        chunk.effect_def := ef_Arpeggio;
    fx_FSlideUp:        chunk.effect_def := ef_FSlideUp;
    fx_FSlideDown:      chunk.effect_def := ef_FSlideDown;
    fx_FSlideUpFine:    chunk.effect_def := ef_FSlideUpFine;
    fx_FSlideDownFine:  chunk.effect_def := ef_FSlideDownFine;
    fx_TonePortamento:  chunk.effect_def := ef_TonePortamento;
    fx_TPortamVolSlide: chunk.effect_def := ef_TPortamVolSlide;
    fx_Vibrato:         chunk.effect_def := ef_Vibrato;
    fx_VibratoVolSlide: chunk.effect_def := ef_VibratoVolSlide;
    fx_SetInsVolume:    chunk.effect_def := ef_SetInsVolume;
    fx_PatternJump:     chunk.effect_def := ef_PositionJump;
    fx_PatternBreak:    chunk.effect_def := ef_PatternBreak;
    fx_SetTempo:        chunk.effect_def := ef_SetSpeed;
    fx_SetTimer:        chunk.effect_def := ef_SetTempo;

    fx_SetOpIntensity:
      If (old_chunk.effect DIV 16 <> 0) then
        begin
          chunk.effect_def := ef_SetCarrierVol;
          chunk.effect := 3+(old_chunk.effect DIV 16)*4;
        end
      else If (old_chunk.effect MOD 16 <> 0) then
             begin
               chunk.effect_def := ef_SetModulatorVol;
               chunk.effect := 3+(old_chunk.effect MOD 16)*4;
             end
           else chunk.effect_def := 0;

    fx_Extended:
      Case old_chunk.effect DIV 16 of
        fx_ex_DefAMdepth:
          begin
            chunk.effect_def := ef_Extended;
            chunk.effect := ef_ex_SetTremDepth*16+old_chunk.effect MOD 16;
          end;

        fx_ex_DefVibDepth:
          begin
            chunk.effect_def := ef_Extended;
            chunk.effect := ef_ex_SetVibDepth*16+old_chunk.effect MOD 16;
          end;


        fx_ex_DefWaveform:
          begin
            chunk.effect_def := ef_SetWaveform;
            Case old_chunk.effect MOD 16 of
              0..3: chunk.effect := (old_chunk.effect MOD 16)*16+$0f;
              4..7: chunk.effect := $0f0+(old_chunk.effect MOD 16)-4;
            end;
          end;

        fx_ex_VSlideUp:
          begin
            chunk.effect_def := ef_VolSlide;
            chunk.effect := (old_chunk.effect MOD 16)*16;
          end;

        fx_ex_VSlideDown:
          begin
            chunk.effect_def := ef_VolSlide;
            chunk.effect := old_chunk.effect MOD 16;
          end;

        fx_ex_VSlideUpFine:
          begin
            chunk.effect_def := ef_VolSlideFine;
            chunk.effect := (old_chunk.effect MOD 16)*16;
          end;

        fx_ex_VSlideDownFine:
          begin
            chunk.effect_def := ef_VolSlideFine;
            chunk.effect := old_chunk.effect MOD 16;
          end;

        fx_ex_ManSlideUp:
          begin
            chunk.effect_def := ef_Extended2;
            chunk.effect := ef_ex2_FineTuneUp*16+old_chunk.effect MOD 16;
          end;

        fx_ex_ManSlideDown:
          begin
            chunk.effect_def := ef_Extended2;
            chunk.effect := ef_ex2_FineTuneDown*16+old_chunk.effect MOD 16;
          end;

        fx_ex_RetrigNote:
          begin
            chunk.effect_def := ef_RetrigNote;
            chunk.effect := SUCC(old_chunk.effect MOD 16);
          end;

        fx_ex_SetAttckRate:
          begin
            chunk.effect_def := ef_Extended;
            chunk.effect := old_chunk.effect MOD 16;
            If NOT adsr_carrier[chan] then
              Inc(chunk.effect,ef_ex_SetAttckRateM*16)
            else Inc(chunk.effect,ef_ex_SetAttckRateC*16);
          end;

        fx_ex_SetDecayRate:
          begin
            chunk.effect_def := ef_Extended;
            chunk.effect := old_chunk.effect MOD 16;
            If NOT adsr_carrier[chan] then
              Inc(chunk.effect,ef_ex_SetDecayRateM*16)
            else Inc(chunk.effect,ef_ex_SetDecayRateC*16);
          end;

        fx_ex_SetSustnLevel:
          begin
            chunk.effect_def := ef_Extended;
            chunk.effect := old_chunk.effect MOD 16;
            If NOT adsr_carrier[chan] then
              Inc(chunk.effect,ef_ex_SetSustnLevelM*16)
            else Inc(chunk.effect,ef_ex_SetSustnLevelC*16);
          end;

        fx_ex_SetReleaseRate:
          begin
            chunk.effect_def := ef_Extended;
            chunk.effect := old_chunk.effect MOD 16;
            If NOT adsr_carrier[chan] then
              Inc(chunk.effect,ef_ex_SetRelRateM*16)
            else Inc(chunk.effect,ef_ex_SetRelRateC*16);
          end;

        fx_ex_SetFeedback:
          begin
            chunk.effect_def := ef_Extended;
            chunk.effect := ef_ex_SetFeedback*16+old_chunk.effect MOD 16;
          end;

        fx_ex_ExtendedCmd:
          If (old_chunk.effect MOD 16 in [0..9]) then
            begin
              chunk.effect_def := ef_Extended;
              chunk.effect := ef_ex_ExtendedCmd2*16;

              Case old_chunk.effect MOD 16 of
                0: Inc(chunk.effect,ef_ex_cmd2_RSS);
                1: Inc(chunk.effect,ef_ex_cmd2_LockVol);
                2: Inc(chunk.effect,ef_ex_cmd2_UnlockVol);
                3: Inc(chunk.effect,ef_ex_cmd2_LockVP);
                4: Inc(chunk.effect,ef_ex_cmd2_UnlockVP);

                5: begin
                     If processing_whole_song then chunk.effect_def := 255
                     else chunk.effect_def := 0;
                     chunk.effect := 0;
                     adsr_carrier[chan] := TRUE;
                   end;

                6: begin
                     If processing_whole_song then chunk.effect_def := 255
                     else chunk.effect_def := 0;
                     If processing_whole_song then chunk.effect := 1
                     else chunk.effect := 0;
                     adsr_carrier[chan] := FALSE;
                   end;

                7: Inc(chunk.effect,ef_ex_cmd2_VSlide_car);
                8: Inc(chunk.effect,ef_ex_cmd2_VSlide_mod);
                9: Inc(chunk.effect,ef_ex_cmd2_VSlide_def);
              end;
            end
          else begin
                 chunk.effect_def := 0;
                 chunk.effect := 0;
               end;
      end;
  end;

  put_chunk(patt,line,chan,chunk);
end;
