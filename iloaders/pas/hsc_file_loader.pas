// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

type
  tHSC_PATTERNS = array[0..$31] of
                  array[0..$3f] of array[1..9] of Word;
type
  tHSC_DATA = Record
                instr: array[0..$7f] of array[0..$0b] of Byte;
                order: array[0..$31] of Byte;
                patts: tHSC_PATTERNS;
              end;

procedure import_hsc_event(patt,line,chan: Byte; event: Word);

var
  chunk: tCHUNK;

begin
  FillChar(chunk,SizeOf(chunk),0);
  Case HI(event) of
  { REGULAR NOTE }
    1..12*8+1: If NOT fix_c_note_bug then chunk.note := HI(event)
               else begin
                      chunk.note := HI(event)+1;
                      If (chunk.note > 12*8+1) then
                        chunk.note := 12*8+1;
                    end;
  { PAUSE }
    $7f: chunk.note := BYTE_NULL;

  { INSTRUMENT }
    $80: begin
           chunk.effect_def := ef_Extended;
           chunk.effect := ef_ex_ExtendedCmd2*16+ef_ex_cmd2_ResetVol;
           chunk.instr_def := LO(event)+1;
           chunk.note := BYTE_NULL;
         end;
  end;

  If (HI(event) <> $80) then
    Case (LO(event) AND $0f0) of
    { PATTERNBREAK }
      $00: If (LO(event) AND $0f = 1) then
             chunk.effect_def := ef_PatternBreak;

    { MANUAL SLIDE UP }
      $10: begin
             chunk.effect_def := ef_Extended2;
             chunk.effect := ef_ex2_FineTuneUp*16+
                             max(LO(event) AND $0f +1,15);
           end;

    { MANUAL SLIDE DOWN }
      $20: begin
             chunk.effect_def := ef_Extended2;
             chunk.effect := ef_ex2_FineTuneDown*16+
                             max(LO(event) AND $0f +1,15);
           end;

    { SET CARRIER VOLUME }
      $a0: begin
             chunk.effect_def := ef_SetCarrierVol;
             chunk.effect := 63-(LO(event) AND $0f)*4;
             chunk.instr_def := LO(event)+1;
           end;

    { SET MODULATOR VOLUME }
      $b0: begin
             chunk.effect_def := ef_SetModulatorVol;
             chunk.effect := 63-(LO(event) AND $0f)*4;
           end;

    { SET INSTRUMENT VOLUME }
      $c0: begin
             chunk.effect_def := ef_SetInsVolume;
             chunk.effect := 63-(LO(event) AND $0f)*4;
           end;

    { SET SPEED }
      $f0: begin
             chunk.effect_def := ef_SetSpeed;
             chunk.effect := (LO(event) AND $0f)+1;
           end;
    end;
  put_chunk(patt,line,chan,chunk);
end;

procedure import_hsc_patterns(var data; patterns: Byte);

var
  voice: array[1..9] of Byte;
  event: Word;
  chunk: tCHUNK;
  temp,temp2,temp3: Byte;
  order,patt: Byte;
  patt_break: Byte;
  patts: String;

function _hsc_event(patt,line,chan: Byte): Word;
begin
  _hsc_event := LO(tHSC_PATTERNS(data)[patt][line][chan+1])+
                HI(tHSC_PATTERNS(data)[patt][line][chan]) SHL 8;
end;

begin { import_hsc_patterns }
  patts := '';
  If NOT accurate_conv then
       For temp := 1 to 9 do voice[temp] := temp
  else For temp := 1 to 9 do voice[temp] := 0;

  For temp := 0 to $31 do
    For temp2 := 0 to $3f do
      For temp3 := 1 to 9 do
        If (_hsc_event(temp,temp2,temp3) <> 0) then
          import_hsc_event(temp,temp2,temp3,_hsc_event(temp,temp2,temp3));

  order := 0;
  patt := BYTE_NULL;

  Repeat
    If (songdata.pattern_order[order] > $31) then Inc(order)
    else
      begin
        patt := songdata.pattern_order[order];
        patt_break := BYTE_NULL;
        For temp2 := 0 to $3f do
          For temp3 := 1 to 9 do
            begin
              get_chunk(patt,temp2,temp3,chunk);
              event := _hsc_event(patt,temp2,temp3);

              Case HI(event) of
              { REGULAR NOTE }
                1..12*8+1: begin
                             If accurate_conv then
                               If (voice[temp3] = 0) then
                                 begin
                                   voice[temp3] := temp3;
                                   chunk.instr_def := voice[temp3];
                                 end;

                             If NOT accurate_conv then
                               chunk.instr_def := voice[temp3];
                           end;

              { INSTRUMENT }
                $80: If (temp2 <> patt_break) then
                       begin
                         voice[temp3] := LO(event)+1;
                         If NOT accurate_conv then
                           begin
                             chunk.instr_def := voice[temp3];
                             chunk.note := BYTE_NULL;
                           end;
                       end;
              end;

              If (HI(event) <> $80) then
                Case (LO(event) AND $0f0) of
                { PATTERNBREAK }
                  $00: If (LO(event) AND $0f = 1) then
                         patt_break := temp2+1;

                { SET CARRIER VOLUME }
                  $a0: If (chunk.instr_def = 0) and NOT accurate_conv then
                         chunk.instr_def := voice[temp3]
                       else If (chunk.instr_def = 0) and
                               (voice[temp3] = 0) then chunk.instr_def := temp3;

                { SET MODULATOR VOLUME }
                  $b0: If (chunk.instr_def = 0) and NOT accurate_conv then
                         chunk.instr_def := voice[temp3]
                       else If (chunk.instr_def = 0) and
                               (voice[temp3] = 0) then chunk.instr_def := temp3;

                { SET INSTRUMENT VOLUME }
                  $c0: If (chunk.instr_def = 0) and NOT accurate_conv then
                         chunk.instr_def := voice[temp3]
                       else If (chunk.instr_def = 0) and
                               (voice[temp3] = 0) then chunk.instr_def := temp3;
                end;

              If (Pos(CHR(songdata.pattern_order[order]),patts) = 0) then
                put_chunk(patt,temp2,temp3,chunk);
            end;
        Inc(order);
        patts := patts+CHR(patt);
      end;
  until (patt >= patterns) or (order > $7f);
end;

var
  hscbuf: tHSC_DATA;

procedure hsc_file_loader;

const
  HSC_KSL: array[0..3] of Byte = (0,3,2,1);

var
  f: File;
  temp,temp2,temp3: Longint;

begin
  _dbg_enter ({$I %FILE%}, 'hsc_file_loader');

{$IFDEF ADT2PLAY}
  If (Lower(ExtOnly(songdata_source)) <> 'hsc') then
    begin
      load_flag := $7f;
      _dbg_leave; EXIT; //hsc_file_loader
    end;
{$ENDIF}

  {$i-}
  Assign(f,songdata_source);
  ResetF(f);
  {$i+}
  If (IOresult <> 0) then
    begin
      CloseF(f);
{$IFNDEF ADT2PLAY}
      Dialog('ERROR READiNG DATA - DiSK ERROR?$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' HSC LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //hsc_file_loader
    end;

{$IFNDEF ADT2PLAY}
  If (play_status <> isStopped) then
    begin
      fade_out_playback(FALSE);
      stop_playing;
    end;
{$ENDIF}

  FillChar(hscbuf,SizeOf(hscbuf),0);
  BlockReadF(f,hscbuf,SizeOf(hscbuf),temp);
  If (temp < SizeOf(hscbuf.instr)+SizeOf(hscbuf.order)) then
    begin
      CloseF(f);
{$IFNDEF ADT2PLAY}
      Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' HSC LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //hsc_file_loader
    end;

  For temp2 := 0 to $31 do
    If (hscbuf.order[temp2] > $b0) then hscbuf.order[temp2] := $080;

  temp3 := 0;
  While (temp3 < temp-SizeOf(hscbuf.instr)-SizeOf(hscbuf.order)) do
    begin
      If NOT (pBYTE(@Addr(hscbuf.patts)^)[temp3+1] in
             [1..12*8+1,$00,$7f,$80]) or
         NOT (pBYTE(@Addr(hscbuf.patts)^)[temp3] AND $0f0 in
             [$00,$10,$20,$a0,$b0,$c0,$f0]) then
        begin
          If NOT (pBYTE(@Addr(hscbuf.patts)^)[temp3+1] in
                 [1..12*8+1,$00,$7f,$80]) then
            pBYTE(@Addr(hscbuf.patts)^)[temp3+1] := $00;

          If NOT (pBYTE(@Addr(hscbuf.patts)^)[temp3] AND $0f0 in
                 [$00,$10,$20,$a0,$b0,$c0,$f0]) then
            pBYTE(@Addr(hscbuf.patts)^)[temp3] := 0;
        end;
      Inc(temp3,2);
    end;

  init_songdata;
  load_flag := 0;

  songdata.patt_len := 64;
  If adjust_tracks then songdata.nm_tracks := 9
  else If (songdata.nm_tracks < 9) then songdata.nm_tracks := 9;

  tempo := 18;
  speed := 2;

  songdata.common_flag := songdata.common_flag OR 2;
  songdata.tempo := tempo;
  songdata.speed := speed;
  import_old_flags;

  For temp2 := 0 to $31 do
    songdata.pattern_order[temp2] := hscbuf.order[temp2];

  import_hsc_patterns(hscbuf.patts,(temp-SizeOf(hscbuf.instr)
                                        -SizeOf(hscbuf.order)-1) DIV $480);

// specific corrections for HSC-Tracker instrument
  For temp2 := 0 to $7f do
    begin
      import_hsc_instrument_alt (songdata.instr_data[temp2+1], hscbuf.instr[temp2]);
      With songdata.instr_data[temp2+1].fm_data do
        begin
          KSL_VOLUM_modulator := KSL_VOLUM_modulator AND $3f+
                                 HSC_KSL[KSL_VOLUM_modulator SHR 6] SHL 6;
          KSL_VOLUM_carrier   := KSL_VOLUM_carrier AND $3f+
                                 HSC_KSL[KSL_VOLUM_carrier SHR 6] SHL 6;
        end;
    end;

  CloseF(f);
  songdata_title := NameOnly(songdata_source);
{$IFDEF ADT2PLAY}
  load_flag := 9;
{$ELSE}
  load_flag := 1;
{$ENDIF}

  _dbg_leave; //EXIT //hsc_file_loader
end;
