// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

var
  ins_c4factor: array[1..99] of Shortint;

procedure fix_s3m_commands(patterns: Byte);

var
  chunk,chunk2: tCHUNK;
  temp,temp4: Byte;
  patt_break: Byte;
  order,patt: Byte;
  patts: String;
  ins_cache,
  misc_cache,
  arpg_cache,
  volsld_cache,
  slide_cache,
  note_cache,
  patloop_cache: array[1..20] of Byte;
  prev_cache: array[1..20] of Record
                                effect_def,
                                effect,
                                effect_def2,
                                effect2: Byte;
                              end;

procedure fix_single_pattern(patt: Byte);

var
  temp2,temp3: Byte;

begin
  FillChar(prev_cache,SizeOf(prev_cache),0);
  FillChar(patloop_cache,SizeOf(patloop_cache),BYTE_NULL);
  patt_break := BYTE_NULL;

  For temp2 := 0 to $3f do
    For temp3 := 1 to 20 do
      begin
        get_chunk(patt,temp2,temp3,chunk);
        If (chunk.effect_def in [ef_PositionJump,ef_PatternBreak]) then
          patt_break := temp2;

        If (chunk.instr_def <> 0) and (temp2 <= patt_break) then
          ins_cache[temp3] := chunk.instr_def;

        If (chunk.note in [1..12*8+1]) and (temp2 <= patt_break) then
          note_cache[temp3] := chunk.note;

        If (chunk.instr_def <> 0) or ((chunk.instr_def = 0) and
                                      (chunk.note in [1..12*8+1]) and
                                      (ins_cache[temp3] <> 0)) then
          begin
            If (chunk.instr_def <> 0) then temp4 := chunk.instr_def
            else temp4 := ins_cache[temp3];
            If (ins_c4factor[temp4] <> 0) and
               NOT (Pos(CHR(songdata.pattern_order[order]),patts) <> 0) then
              begin
                If (ins_c4factor[temp4] <> -127) then
                  chunk.note := min(max(chunk.note+ins_c4factor[temp4],12*8+1),1)
                else chunk.note := 1;
                put_chunk(patt,temp2,temp3,chunk);
              end;
          end;

        If (chunk.effect_def = ef_Extended) and
           (chunk.effect DIV 16 = ef_ex_PatternLoop) and
           (chunk.effect MOD 16 <> 0) then
          If NOT (patloop_cache[temp3] in [0,BYTE_NULL]) and (temp2 <> 0) then
            begin
              If (prev_cache[temp3].effect_def = 0) and
                 (prev_cache[temp3].effect = 0) then
                begin
                  get_chunk(patt,PRED(temp2),temp3,chunk2);
                  chunk2.effect_def := ef_Extended;
                  chunk2.effect := ef_ex_PatternLoop*16;
                  If NOT ((chunk2.effect_def = chunk2.effect_def2) and
                          (chunk2.effect = chunk2.effect2)) then
                    begin
                      put_chunk(patt,PRED(temp2),temp3,chunk2);
                      prev_cache[temp3].effect_def := chunk.effect_def;
                      prev_cache[temp3].effect := chunk.effect;
                    end;
                end
              else If (prev_cache[temp3].effect_def2 = 0) and
                      (prev_cache[temp3].effect2 = 0) then
                     begin
                       get_chunk(patt,PRED(temp2),temp3,chunk2);
                       chunk2.effect_def2 := ef_Extended;
                       chunk2.effect2 := ef_ex_PatternLoop*16;
                       If NOT ((chunk2.effect_def2 = chunk2.effect_def) and
                               (chunk2.effect2 = chunk2.effect)) then
                         begin
                           put_chunk(patt,PRED(temp2),temp3,chunk2);
                           prev_cache[temp3].effect_def2 := chunk.effect_def2;
                           prev_cache[temp3].effect2 := chunk.effect2;
                         end;
                     end;
            end
          else If (patloop_cache[temp3] <> 0) and (temp2 <> 0) then
                 begin
                   get_chunk(patt,0,temp3,chunk2);
                   If (chunk2.effect_def = 0) and
                      (chunk2.effect = 0) then
                     begin
                       chunk2.effect_def := ef_Extended;
                       chunk2.effect := ef_ex_PatternLoop*16;
                       If NOT ((chunk2.effect_def = chunk2.effect_def2) and
                               (chunk2.effect = chunk2.effect2)) then
                         put_chunk(patt,0,temp3,chunk2);
                     end
                   else If (chunk2.effect_def2 = 0) and
                           (chunk2.effect2 = 0) then
                          begin
                            chunk2.effect_def2 := ef_Extended;
                            chunk2.effect2 := ef_ex_PatternLoop*16;
                            If NOT ((chunk2.effect_def2 = chunk2.effect_def) and
                                    (chunk2.effect2 = chunk2.effect)) then
                              put_chunk(patt,0,temp3,chunk2);
                          end;
                 end;

        If (temp2 <= patt_break) then
          begin
            If (chunk.effect DIV 16 <> 0) then
              misc_cache[temp3] := chunk.effect AND $0f0+
                                   misc_cache[temp3] AND $0f
            else If (chunk.effect_def in [ef_Vibrato,
                                          ef_ExtraFineVibrato,
                                          ef_Tremolo,
                                          ef_Tremor,
                                          ef_MultiRetrigNote]) then
                   begin
                     chunk.effect := misc_cache[temp3] AND $0f0+
                                     chunk.effect AND $0f;
                     put_chunk(patt,temp2,temp3,chunk);
                   end;

            If (chunk.effect MOD 16 <> 0) then
              misc_cache[temp3] := misc_cache[temp3] AND $0f0+
                                   chunk.effect AND $0f
            else If (chunk.effect_def in [ef_Vibrato,
                                          ef_ExtraFineVibrato,
                                          ef_Tremolo,
                                          ef_Tremor,
                                          ef_MultiRetrigNote]) then
                   begin
                     chunk.effect := chunk.effect AND $0f0+
                                     misc_cache[temp3] AND $0f;
                     put_chunk(patt,temp2,temp3,chunk);
                   end;

            If (chunk.effect_def = temp_ef_Arpeggio) then
              If (chunk.effect <> 0) then arpg_cache[temp3] := chunk.effect
              else begin
                     chunk.effect := arpg_cache[temp3];
                     put_chunk(patt,temp2,temp3,chunk);
                   end;

            If (chunk.effect_def in [ef_FSlideDown,ef_FSlideDownFine,
                                     ef_FSlideUp,ef_FSlideUpFine,
                                     ef_TonePortamento]) then
              If (chunk.effect <> 0) then slide_cache[temp3] := chunk.effect
              else begin
                     chunk.effect := slide_cache[temp3];
                     put_chunk(patt,temp2,temp3,chunk);
                   end;

         // experimental method to fix up frequency slide
            If (chunk.effect_def in [ef_FSlideDown,ef_FSlideDownFine,
                                     ef_FSlideUp,ef_FSlideUpFine,
                                     ef_Vibrato,
                                     ef_ExtraFineVibrato,
                                     ef_TonePortamento]) then
              If (note_cache[temp3] <> 0) then
                begin
                  If (chunk.effect_def in [ef_Vibrato,ef_ExtraFineVibrato]) then
                    begin
                      temp := chunk.effect AND $0f0;
                      chunk.effect := chunk.effect MOD 16;
                    end;

                  Case SUCC(PRED(note_cache[temp3]) DIV 12) of
                    1: chunk.effect := max(Round(chunk.effect*0.55),255);
                    2: chunk.effect := max(Round(chunk.effect*0.75),255);
                    3: chunk.effect := max(Round(chunk.effect*0.95),255);
                    4: chunk.effect := max(Round(chunk.effect*1.15),255);
                    5: chunk.effect := max(Round(chunk.effect*1.35),255);
                    6: chunk.effect := max(Round(chunk.effect*1.55),255);
                    7: chunk.effect := max(Round(chunk.effect*1.75),255);
                    8: chunk.effect := max(Round(chunk.effect*1.95),255);
                  end;

                  If (chunk.effect_def in [ef_Vibrato,ef_ExtraFineVibrato]) then
                    chunk.effect := max(chunk.effect,15)+temp;

                  put_chunk(patt,temp2,temp3,chunk);
                end;

            If (chunk.effect_def = ef_Extended2) and
               (chunk.effect DIV 16 in [ef_ex2_FreqSlideDnXF,ef_ex2_FreqSlideUpXF]) then
              If (chunk.effect MOD 16 <> 0) then slide_cache[temp3] := chunk.effect MOD 16
              else begin
                     chunk.effect := chunk.effect AND $0f0+slide_cache[temp3] AND $0f;
                     put_chunk(patt,temp2,temp3,chunk);
                   end;

            If (chunk.effect_def in [ef_TPortamVolSlide,ef_VibratoVolSlide,
                                     ef_VolSlide,ef_VolSlideFine]) and
               (temp2 <= patt_break) then
              begin
                If (chunk.effect <> 0) then volsld_cache[temp3] := chunk.effect
                else begin
                       chunk.effect := volsld_cache[temp3];;
                       put_chunk(patt,temp2,temp3,chunk);
                     end;
              end;

            If (chunk.effect_def = ef_Extended2) and
               (chunk.effect DIV 16 in [ef_ex2_VolSlideDnXF,ef_ex2_VolSlideUpXF]) then
              If (chunk.effect MOD 16 <> 0) then
                Case chunk.effect DIV 16 of
                  ef_ex2_VolSlideDnXF:
                    volsld_cache[temp3] := chunk.effect MOD 16;
                  ef_ex2_VolSlideUpXF:
                    volsld_cache[temp3] := chunk.effect MOD 16 SHL 4;
                end
              else begin
                     Case chunk.effect DIV 16 of
                       ef_ex2_VolSlideDnXF:
                         chunk.effect := chunk.effect AND $0f0+volsld_cache[temp3] AND $0f;
                       ef_ex2_VolSlideUpXF:
                         chunk.effect := volsld_cache[temp3] AND $0f0+chunk.effect AND $0f;
                     end;
                     put_chunk(patt,temp2,temp3,chunk);
                   end;
          end;

        If (prev_cache[temp3].effect_def in [ef_Vibrato,ef_ExtraFineVibrato,ef_VibratoVolSlide]) and
           NOT (chunk.effect_def in [ef_Vibrato,ef_ExtraFineVibrato,ef_VibratoVolSlide]) then
          If (chunk.effect_def = 0) and (chunk.effect = 0) then
            begin
              chunk2 := chunk;
              chunk2.effect_def := ef_Extended;
              chunk2.effect := ef_ex_ExtendedCmd2*16+ef_ex_cmd2_VibrOff;
              If NOT ((chunk2.effect_def = chunk2.effect_def2) and
                      (chunk2.effect = chunk2.effect2)) then
                begin
                  put_chunk(patt,temp2,temp3,chunk2);
                  chunk := chunk2;
                end;
            end
          else If (chunk.effect_def2 = 0) and (chunk.effect2 = 0) then
                 begin
                   chunk2 := chunk;
                   chunk2.effect_def2 := ef_Extended;
                   chunk2.effect2 := ef_ex_ExtendedCmd2*16+ef_ex_cmd2_VibrOff;
                   If NOT ((chunk2.effect_def2 = chunk2.effect_def) and
                           (chunk2.effect2 = chunk2.effect)) then
                     begin
                       put_chunk(patt,temp2,temp3,chunk2);
                       chunk := chunk2;
                     end;
                 end;

        If (chunk.effect_def = ef_Extended) and
           (chunk.effect DIV 16 = ef_ex_PatternLoop) then
          patloop_cache[temp3] := chunk.effect MOD 16;

        prev_cache[temp3].effect_def := chunk.effect_def;
        prev_cache[temp3].effect := chunk.effect;
        prev_cache[temp3].effect_def2 := chunk.effect_def2;
        prev_cache[temp3].effect2 := chunk.effect2;

        If (chunk.effect_def = temp_ef_Arpeggio) then
          begin
            chunk2 := chunk;
            chunk2.effect_def := ef_Arpeggio;
            put_chunk(patt,temp2,temp3,chunk2);
          end;
      end;
end;

begin { fix_s3m_commands }
  FillChar(ins_cache,SizeOf(ins_cache),0);
  FillChar(note_cache,SizeOf(note_cache),0);
  FillChar(volsld_cache,SizeOf(volsld_cache),0);
  FillChar(slide_cache,SizeOf(slide_cache),0);
  FillChar(misc_cache,SizeOf(misc_cache),0);
  FillChar(arpg_cache,SizeOf(arpg_cache),0);

  patts := '';
  order := 0; patt := BYTE_NULL;

  Repeat
    If (songdata.pattern_order[order] >= $80) then Inc(order)
    else
      begin
        patt := songdata.pattern_order[order];
        If NOT (Pos(CHR(patt),patts) <> 0) then
          fix_single_pattern(patt);
        Inc(order);
        patts := patts+CHR(patt);
      end;
  until (patt >= patterns) or (order > $7f);

  For patt := 0 to PRED(patterns) do
    If NOT (Pos(CHR(patt),patts) <> 0) then
      fix_single_pattern(patt);
end;

procedure s3m_file_loader;

type
  tS3M_HEADER = Record
                  songname: array[1..28] of Char; { ASCIIZ }
                  byte1a:   Byte; { 1Ah }
                  ftype:    Byte; { File type: 16=ST3 module }
                  resrvd1:  array[0..1] of Byte;
                  ordnum:   Word; { Number of orders in file (should be even!) }
                  insnum:   Word; { Number of instruments in file }
                  patnum:   Word; { Number of patterns in file }
                  flags:    Word; {  [ These are old flags for Ffv1. Not supported in ST3.01 }
                                  {  |  +1:st2vibrato }
                                  {  |  +2:st2tempo }
                                  {  |  +4:amigaslides }
                                  {  | +32:enable filter/sfx with sb }
                                  {  ] }
                                  {   +8: 0vol optimizations }
                                  {         Automatically turn off looping notes whose volume }
                                  {         is zero for >2 note rows. }
                                  {  +16: amiga limits }
                                  {         Disallow any notes that go beyond the amiga hardware }
                                  {         limits (like amiga does). This means that sliding }
                                  {         up stops at B#5 etc. Also affects some minor amiga }
                                  {         compatibility issues. }
                                  {  +64: st3.00 volumeslides }
                                  {         Normally volumeslide is NOT performed on first }
                                  {         frame of each row (this is according to amiga }
                                  {         playing). If this is set, volumeslide is performed }
                                  {         ALSO on the first row. This is set by default }
                                  {         if the Cwt/v files is 0x1300 }
                                  { +128: special custom data in file (see below) }
                  cwt_v:    Word; { Created with tracker / version: &0xfff=version, >>12=tracker }
                                  {     ST3.00:0x1300 (NOTE: volumeslides on EVERY frame) }
                                  {     ST3.01:0x1301 }
                                  {     ST3.03:0x1303 }
                                  {     ST3.20:0x1320 }
                  ffi:      Word; { File format information }
                                  {   1=[VERY OLD] signed samples }
                                  {   2=unsigned samples }
                  id:       array[1..4] of Char; { "SCRM" }
                  g_v:      Byte; { global volume (see next section) }
                  i_s:      Byte; { initial speed (command A) }

                  i_t:      Byte; { initial tempo (command T) }
                  m_v:      Byte; { master volume (see next section) 7 lower bits }
                                  {   bit 8: stereo(1) / mono(0) }
                  u_c:      Byte; { ultra click removal }
                  d_p:      Byte; { 252 when default channel pan positions are present }
                                  { in the end of the header (xxx3). If !=252 ST3 doesn't }
                                  { try to load channel pan settings. }
                  resrvd2:  array[0..7] of Byte;
                  special:  Word;
                  chan_set: array[1..32] of Byte;
                end;
type
  tS3M_ADLINS = Record
                  itype:   Byte; { 2:amel 3:abd 4:asnare 5:atom 6:acym 7:ahihat }
                  dosname: array[1..12] of Char;
                  id0:     array[0..2]  of Char;
                  fmdata:  array[0..11] of Byte; { D00..D0B contains the adlib instrument specs packed like this: }
                                                 { modulator:                                              carrier: }
                                                 { D00=[freq.muliplier]+[?scale env.]*16+[?sustain]*32+    =D01 }
                                                 {         [?pitch vib]*64+[?vol.vib]*128 }
                                                 { D02=[63-volume]+[levelscale&1]*128+[l.s.&2]*64          =D03 }
                                                 { D04=[attack]*16+[decay]                                 =D05 }
                                                 { D06=[15-sustain]*16+[release]                           =D07 }
                                                 { D08=[wave select]                                       =D09 }
                                                 { D0A=[modulation feedback]*2+[?additive synthesis] }
                                                 { D0B=unused }
                  vol:     Byte; { Default volume 0..64 }
                  dsk:     Byte;
                  resrvd1: array[0..1] of Byte;
                  c2spd:   Word; { 'Herz' for middle C. ST3 only uses lower 16 bits. }
                                 { Actually this is a modifier since there is no }
                                 { clear frequency for adlib instruments. It scales }
                                 { the note freq sent to adlib. }
                  hi_c2sp: Word;
                  resrvd2: array[0..11] of Byte;
                  smpname: array[1..28] of Char; { ASCIIZ }
                  id:      array[1..4]  of Char; { "SCRI" or "SCRS" }
                end;
const
  id_mod = 'SCRM';
  id_ins_adl = 'SCRI';
  id_ins_smp = 'SCRS';

var
  f: File;
  header: tS3M_HEADER;
  order_list: array[0..254] of Byte;
  paraptr_ins: array[1..99] of Word;
  default_vol: array[1..99] of Byte;
  paraptr_pat: array[0..99] of Word;
  temp,temp2: Longint;
  insdata: tS3M_ADLINS;
  pat,row,chan: Byte;
  note,ins,vol,cmd,info: Byte;
  patlen,index: Word;

procedure import_s3m_event(pattern,line,channel,note,ins,vol,cmd,info: Byte);

var
  chunk: tCHUNK;

function scale_slide(slide: Byte): Byte;
begin
  If (slide > 16) then scale_slide := Round(16+slide/8)
  else scale_slide := Round(slide*(2-slide/16));
end;

begin
  FillChar(chunk,SizeOf(chunk),0);
  chunk.instr_def := ins;

  Case note of
    254: chunk.note := BYTE_NULL;
    255: chunk.note := 0;
    else If (note AND $0f in [0..11]) then
           chunk.note := 12*((note SHR 4) AND 7)+(note AND $0f)+1
  end;

  If (vol <> BYTE_NULL) then
    begin
      chunk.effect_def2 := ef_SetInsVolume;
      chunk.effect2 := max(vol,63);
    end
  else
    If NOT (note in [254,255]) and
       (ins <> 0) and
       (max(default_vol[ins],63) <> 63) then
      begin
        chunk.effect_def2 := ef_SetInsVolume;
        chunk.effect2 := max(default_vol[ins],63);
      end;

  Case CHR(cmd+ORD('A')-1) of
  { NONE }
    '@': chunk.effect := info;

  { SET SPEED }
    'A': If (info <> 0) then
           begin
             chunk.effect_def := ef_SetSpeed;
             chunk.effect := info;
           end;

  { JUMP TO ORDER }
    'B': If (info <= 254) then
           begin
             chunk.effect_def := ef_PositionJump;
             chunk.effect := info;
           end;

  { BREAK PATTERN }
    'C': If (info < 64) then
           begin
             chunk.effect_def := ef_PatternBreak;
             chunk.effect := Str2num(Num2str(info,16),10);
           end;

  { VOLUME SLIDE }
    'D': { VOLUME SLIDE DOWN }
         Case info DIV 16 of
         { NORMAL }
           0: begin
                chunk.effect_def := ef_VolSlide;
                chunk.effect := info MOD 16;
              end;

         { FINE }
          15: begin
                chunk.effect_def := ef_VolSlideFine;
                chunk.effect := info MOD 16;
              end;
         else
           { VOLUME SLIDE UP }
           Case info MOD 16 of
           { NORMAL }
             0: begin
                  chunk.effect_def := ef_VolSlide;
                  chunk.effect := info AND $0f0;
                end;

           { FINE }
            15: begin
                  chunk.effect_def := ef_VolSlideFine;
                  chunk.effect := info AND $0f0;
                end;
           end;
         end;

  { SLIDE DOWN }
    'E': Case info DIV 16 of
         { NORMAL }
           0..13: begin
                    chunk.effect_def := ef_FSlideDown;
                    chunk.effect := scale_slide(info);
                  end;

         { EXTRA FINE }
           14: begin
                 chunk.effect_def := ef_Extended2;
                 If (info <> 0) then
                   chunk.effect := ef_ex2_FreqSlideDnXF*16+min((info AND $0f) DIV 4,1)
                 else chunk.effect := ef_ex2_FreqSlideDnXF*16;
               end;

         { FINE }
           15: begin
                 chunk.effect_def := ef_FSlideDownFine;
                 chunk.effect := info AND $0f;
               end;
         end;

  { SLIDE UP }
    'F': Case info DIV 16 of
         { NORMAL }
           0..13: begin
                    chunk.effect_def := ef_FSlideUp;
                    chunk.effect := scale_slide(info);
                  end;

         { EXTRA FINE }
           14: begin
                 chunk.effect_def := ef_Extended2;
                 If (info <> 0) then
                   chunk.effect := ef_ex2_FreqSlideUpXF*16+min((info AND $0f) DIV 4,1)
                 else chunk.effect := ef_ex2_FreqSlideUpXF*16;
               end;

         { FINE }
           15: begin
                 chunk.effect_def := ef_FSlideUpFine;
                 chunk.effect := info AND $0f;
               end;
         end;

  { TONE PORTAMENTO }
    'G': begin
           chunk.effect_def := ef_TonePortamento;
           chunk.effect := scale_slide(info);
         end;

  { VIBRATO }
    'H': begin
           chunk.effect_def := ef_Vibrato;
           chunk.effect := info;
         end;

  { FINE VIBRATO }
    'U': begin
           chunk.effect_def := ef_ExtraFineVibrato;
           chunk.effect := info;
         end;

  { TREMOR }
    'I': begin
           chunk.effect_def := ef_Tremor;
           chunk.effect := info;
         end;

  { ARPEGGIO }
    'J': begin
           chunk.effect_def := temp_ef_Arpeggio;
           chunk.effect := info;
         end;

  { VIBRATO + VOLUME SLIDE }
    'K': begin
           chunk.effect_def := ef_VibratoVolSlide;
           chunk.effect := info;
         end;

  { TONE PORTAMENTO + VOLUME SLIDE }
    'L': begin
           chunk.effect_def := ef_TPortamVolSlide;
           chunk.effect := info;
         end;

  { RETRIG NOTE + VOLUME SLIDE }
    'Q': begin
           chunk.effect_def := ef_MultiRetrigNote;
           chunk.effect := (info MOD 16)*16+info DIV 16;
         end;

  { TREMOLO }
    'R': begin
           chunk.effect_def := ef_Tremolo;
           chunk.effect := info;
         end;

  { SPECIAL COMMAND }
    'S': Case info DIV 16 of
         { PATTERN LOOP }
           $0b: begin
                  chunk.effect_def := ef_Extended;
                  chunk.effect := ef_ex_PatternLoop*16+info MOD 16;
                end;

         { NOTE CUT }
           $0c: begin
                  chunk.effect_def := ef_Extended2;
                  chunk.effect := ef_ex2_NoteCut*16+info MOD 16;
                end;

         { NOTE DELAY }
           $0d: begin
                  chunk.effect_def := ef_Extended2;
                  chunk.effect := ef_ex2_NoteDelay*16+info MOD 16;
                end;

         { PATTERN DELAY }
           $0e: begin
                  chunk.effect_def := ef_Extended2;
                  chunk.effect := ef_ex2_PatDelayRow*16+info MOD 16;
                end;
         end;

  { TEMPO }
    'T': If (info >= 32) then
           begin
             chunk.effect_def := ef_SetTempo;
             chunk.effect := Round(info/2.5);
           end;

  { SET GLOBAL VOLUME }
    'V': begin
           chunk.effect_def := ef_SetGlobalVolume;
           chunk.effect := max(info,63);
         end;
  end;

  If (chunk.effect_def = 0) and (chunk.effect <> 0) then
    chunk.effect := 0;
  put_chunk(pattern,line,channel,chunk);
end;

// experimental method to fix up note fine-tuning
function find_scale_factor(freq: Longint; var fine_tune: Shortint): Shortint;

const
  _factor: array[-3..3+1] of Real = (1/8,1/4,1/2,1,2,4,8,16);

const
  _freq: array[1..12+1] of Word =
    { C-2         C#2         D-2 }
    ( 33453 DIV 4,35441 DIV 4,37679 DIV 4,
    { D#2         E-2         F-2 }
      39772 DIV 4,42441 DIV 4,44744 DIV 4,
    { F#2         G-2         G#2 }
      47727 DIV 4,50416 DIV 4,53426 DIV 4,
    { A-2         A#2         B-2 }
      56370 DIV 4,59658 DIV 4,63354 DIV 4,
    { C-3 }
      33453 DIV 2);

const
  _fm_freq: array[1..12+1] of Word =
    ($156, $16b, $181, $198, $1b0, $1ca,
     $1e5, $202, $220, $241, $263, $287,
     $2ae);

var
  factor: Real;
  temp,scaler: Shortint;

begin
  scaler := -3;
  fine_tune := 0;

  For scaler := -3 to 3+1 do
    For temp := 1 to 12 do
      begin
        factor := _factor[scaler];
        If (freq >= Round(_freq[temp]*factor)) and
           (freq <= Round(_freq[SUCC(temp)]*factor)) then
          If (freq-Round(_freq[temp]*factor) < Round(_freq[SUCC(temp)]*factor)-freq) then
            begin
              fine_tune := Round((_fm_freq[SUCC(temp)]-_fm_freq[temp])/
                                 (_freq[SUCC(temp)]-_freq[temp])*
                                 (freq-Round(_freq[temp]*factor)));
              find_scale_factor := scaler*12+PRED(temp);
              EXIT; //find_scale_factor
            end
          else
            begin
              fine_tune := Round((_fm_freq[SUCC(temp)]-_fm_freq[temp])/
                                 (_freq[SUCC(temp)]-_freq[temp])*
                                 (freq-Round(_freq[SUCC(temp)]*factor)));
              If (temp <> 12) then find_scale_factor := scaler*12+temp
              else find_scale_factor := SUCC(scaler)*12;
              EXIT; //find_scale_factor
            end;
      end;

  find_scale_factor := -127;
  fine_tune := 0;

  //EXIT //find_scale_factor
end;

(*  // another method -- it's hard to say whether more or less accurate :)
function find_scale_factor(freq: Longint; var fine_tune: Shortint): Shortint;

const
  _factor: array[-3..3+1] of Real = (1/8,1/4,1/2,1,2,4,8,16);
  _finetune_factor: array[-3..3+1] of Real = (8,4,2,1,1/2,1/4,1/8,1/16);

const
  _freq: array[1..12+1] of Word =
    { C-2         C#2         D-2 }
    ( 33453 DIV 4,35441 DIV 4,37679 DIV 4,
    { D#2         E-2         F-2 }
      39772 DIV 4,42441 DIV 4,44744 DIV 4,
    { F#2         G-2         G#2 }
      47727 DIV 4,50416 DIV 4,53426 DIV 4,
    { A-2         A#2         B-2 }
      56370 DIV 4,59658 DIV 4,63354 DIV 4,
    { C-3 }
      33453 DIV 2);

var
  factor: Real;
  temp,scaler: Shortint;

begin
  scaler := -3;
  fine_tune := 0;

  For scaler := -3 to 3+1 do
    For temp := 1 to 12 do
      begin
        factor := _factor[scaler];
        If (freq >= Round(_freq[temp]*factor)) and
           (freq <= Round(_freq[SUCC(temp)]*factor)) then
          If (freq-Round(_freq[temp]*factor) < Round(_freq[SUCC(temp)]*factor)-freq) then
            begin
              fine_tune := Round((freq-Round(_freq[temp]*factor))/
                                  Round(16/_finetune_factor[scaler]));
              find_scale_factor := scaler*12+PRED(temp);
              EXIT; //find_scale_factor
            end
          else
            begin
              If (temp = 12) then Inc(scaler);
              fine_tune := Round((freq-Round(_freq[SUCC(temp)]*factor))/
                                  Round(16/_finetune_factor[scaler]));
              If (temp = 12) then temp := 0;
              find_scale_factor := scaler*12+temp;
              EXIT; //find_scale_factor
            end;
      end;

  find_scale_factor := -127;
  fine_tune := 0;

  //EXIT //find_scale_factor
end;
*)

begin
  _dbg_enter ({$I %FILE%}, 's3m_file_loader');

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
             '~O~KAY$',' S3M LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //s3m_file_loader
    end;

  BlockReadF(f,header,SizeOf(header),temp);
  If NOT ((temp = SizeOf(header)) and (header.id = id_mod)) then
    begin
      CloseF(f);
{$IFNDEF ADT2PLAY}
      Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' S3M LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //s3m_file_loader
    end;

{$IFDEF ADT2PLAY}
  load_flag := $7f;
{$ENDIF}
  If (header.byte1a <> $1a) or (header.ftype <> $10) then
    begin
      CloseF(f);
{$IFNDEF ADT2PLAY}
      Dialog('UNKNOWN FiLE FORMAT TYPE$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' S3M LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //s3m_file_loader
    end;

{$IFNDEF ADT2PLAY}
  If (play_status <> isStopped) then
    begin
      fade_out_playback(FALSE);
      stop_playing;
    end;
{$ENDIF}

  BlockReadF(f,order_list,header.ordnum,temp);
  If (IOresult <> 0) or (temp <> header.ordnum) then
    begin
      CloseF(f);
{$IFNDEF ADT2PLAY}
      Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' S3M LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //s3m_file_loader
    end;

  BlockReadF(f,paraptr_ins,header.insnum*2,temp);
  If (IOresult <> 0) or (temp <> header.insnum*2) then
    begin
      CloseF(f);
{$IFNDEF ADT2PLAY}
      Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' S3M LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //s3m_file_loader
    end;

  BlockReadF(f,paraptr_pat,header.patnum*2,temp);
  If (IOresult <> 0) or (temp <> header.patnum*2) then
    begin
      CloseF(f);
{$IFNDEF ADT2PLAY}
      Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
             'LOADiNG STOPPED$',
             '~O~KAY$',' S3M LOADER ',1);
{$ENDIF}
      _dbg_leave; EXIT; //s3m_file_loader
    end;

  init_songdata;
  load_flag := 0;

  If (header.i_s <> 0) then speed := header.i_s
  else speed := 1;

  If (Round(header.i_t/2.5) < 255) then tempo := Round(header.i_t/2.5)
  else tempo := 255;

  songdata.tempo := tempo;
  songdata.speed := speed;
  songdata.songname := truncate_string(asciiz_string(header.songname));
  songdata.common_flag := songdata.common_flag OR $80;
  import_old_flags;

  For temp := 32 downto 1 do
    If (header.chan_set[temp] <> 255) then BREAK;

{$IFNDEF ADT2PLAY}
  For temp2 := 1 to max(temp,18) do
    If (header.chan_set[temp2] OR $80 = header.chan_set[temp2]) then
      channel_flag[temp2] := FALSE;
{$ENDIF}

  songdata.patt_len := 64;
  If adjust_tracks then songdata.nm_tracks := max(temp,18)
  else If (songdata.nm_tracks < 18) then songdata.nm_tracks := 18;

  For temp := 1 to max(header.ordnum,128) do
    Case order_list[temp-1] of
      254: songdata.pattern_order[temp-1] := $80+temp;
      255: songdata.pattern_order[temp-1] := $80;
      else songdata.pattern_order[temp-1] := order_list[temp-1];
    end;

  FillChar(ins_c4factor,SizeOf(ins_c4factor),0);
  For temp := 1 to header.insnum do
    begin
      SeekF(f,paraptr_ins[temp]*16);
      If (IOresult <> 0) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' S3M LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //s3m_file_loader
        end;

      BlockReadF(f,insdata,SizeOf(insdata),temp2);
      If (IOresult <> 0) or (temp2 <> SizeOf(insdata)) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' S3M LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //s3m_file_loader
        end;

      If (truncate_string(insdata.smpname) <> '') then
        songdata.instr_names[temp] :=
          Copy(songdata.instr_names[temp],1,9)+
          Copy(truncate_string(asciiz_string(insdata.smpname)),1,32)
      else
        songdata.instr_names[temp] :=
          Copy(songdata.instr_names[temp],1,9)+
          truncate_string(insdata.dosname);

      If (insdata.itype in [2..7]) then
        begin
          If (insdata.id <> id_ins_adl) and (insdata.id <> id_ins_smp) then
            begin
              CloseF(f);
{$IFNDEF ADT2PLAY}
              Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                     'LOADiNG STOPPED$',
                     '~O~KAY$',' S3M LOADER ',1);
{$ENDIF}
              _dbg_leave; EXIT; //s3m_file_loader
            end;

          import_standard_instrument_alt (songdata.instr_data[temp], insdata.fmdata);
        end;

      default_vol[temp] := insdata.vol;
      If (insdata.c2spd <> 0) and
         (insdata.c2spd <> 8363) then
        ins_c4factor[temp] := find_scale_factor(insdata.c2spd,songdata.instr_data[temp].fine_tune);
    end;

  For pat := 0 to PRED(header.patnum) do
    begin
      SeekF(f,paraptr_pat[pat]*16);
      If (IOresult <> 0) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' S3M LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //s3m_file_loader
        end;

      BlockReadF(f,patlen,SizeOf(patlen),temp2);
      If (temp2 <> SizeOf(patlen)) then
        begin
          CloseF(f);
{$IFNDEF ADT2PLAY}
          Dialog('ERROR READiNG DATA - FiLE CORRUPTED$'+
                 'LOADiNG STOPPED$',
                 '~O~KAY$',' S3M LOADER ',1);
{$ENDIF}
          _dbg_leave; EXIT; //s3m_file_loader
        end;

      If (patlen = 0) then CONTINUE;
      FillChar(buf1,SizeOf(buf1),0);
      BlockReadF(f,buf1,patlen-2,temp2);

      index := 0;
      row := 0;

      Repeat
        If (buf1[index] <> 0) then
          begin
            note := BYTE_NULL;
            ins  := 0;
            vol  := BYTE_NULL;
            cmd  := 0;
            info := 0;
            temp := buf1[index];
            Inc(index);

            chan := SUCC(temp AND 31);
            If (temp OR $20 = temp) then
              begin
                note := buf1[index];
                Inc(index);
                ins := buf1[index];
                Inc(index);
              end;

            If (temp OR $40 = temp) then
              begin
                vol := buf1[index];
                Inc(index);
              end;

            If (temp OR $80 = temp) then
              begin
                cmd := buf1[index];
                Inc(index);
                info := buf1[index];
                Inc(index);
              end;

            If (chan > songdata.nm_tracks) then songdata.nm_tracks := max(chan,18);
            If (chan in [1..songdata.nm_tracks]) then
              import_s3m_event(pat,row,chan,note,ins,vol,cmd,info);
          end
        else
          begin
            Inc(row);
            Inc(index);
          end;
      until (row = 64);
    end;

  fix_s3m_commands(header.patnum);
  CloseF(f);
  songdata_title := NameOnly(songdata_source);
{$IFDEF ADT2PLAY}
  load_flag := 12;
{$ELSE}
  load_flag := 1;
{$ENDIF}

  _dbg_leave; //EXIT //s3m_file_loader
end;
