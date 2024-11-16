//  This file is part of Adlib Tracker II (AT2).
//
//  AT2 is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  AT2 is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with AT2.  If not, see <http://www.gnu.org/licenses/>.

unit A2player;
{$S-,Q-,R-,V-,B-,X+}
{$PACKRECORDS 1}
{$MODESWITCH CVAR}
{$L a2player.o}
interface

const
  ___UNIT_DATA_START___: Dword = 0;

const
  MAX_IRQ_FREQ = 1000;
  MIN_IRQ_FREQ = 50;

var
  timer_poll_proc_ptr: Pointer; cvar; external;
  timer_initialized: Boolean; cvar; external;

{$I typcons1.inc}

var
  error_code: Integer; cvar; external;
  current_order: Byte; cvar; external;
  current_pattern: Byte; cvar; external;
  current_line: Byte; cvar; external;
  tempo: Byte; cvar; external;
  speed: Byte; cvar; external;
  macro_speedup: Word; cvar; external;
  max_patterns: Byte; cvar; external;
  fast_forward: Boolean; cvar; external;
  overall_volume: Byte; cvar; external;
  global_volume: Byte; cvar; external;
  fade_out_volume: Byte; cvar; external;

  song_timer: Word; cvar; external;
  song_timer_tenths: Word; cvar; external;
  ticklooper: Longint; cvar; external;
  macro_ticklooper: Longint; cvar; external;
  timer_ticklooper: Longint; cvar; external;
  timer_200hz_counter: Longint; cvar; external;
  timer_50hz_counter: Longint; cvar; external;
  timer_20hz_counter: Longint; cvar; external;
  timer_200hz_flag: Boolean; cvar; external;
  timer_50hz_flag: Boolean; cvar; external;
  timer_20hz_flag: Boolean; cvar; external;

  timer_temp: Word; cvar; external;
  ticks: Longint; cvar; external;
  tick0: Longint; cvar; external;
  tickD: Longint; cvar; external;
  tickXF: Longint; cvar; external;
  limit_exceeded: Boolean; cvar; external;

  time_playing: Double; cvar; external;
  pattdata: ^tPATTERN_DATA; cvar; external;
  songdata: tFIXED_SONGDATA; cvar; external;
  old_songdata: tOLD_FIXED_SONGDATA; cvar; external;
  old_hash_buffer: tOLD_VARIABLE_DATA1; cvar; external;
  hash_buffer: tOLD_VARIABLE_DATA2; cvar; external;

  IRQ_freq: Word; cvar; external;
  IRQ_freq_shift: Integer; cvar; external;
  playback_speed_shift: Longint; cvar; external;
  timer_fix: Boolean; cvar; external;
  pattern_break: Boolean; cvar; external;
  pattern_break_loop: Boolean; cvar; external;
  pattern_break_docmd: Boolean; cvar; external;
  pattern_break_oldord: Byte; cvar; external;
  pattern_delay: Boolean; cvar; external;
  next_line: Byte; cvar; external;
  play_status: tPLAY_STATUS; cvar; external;
  replay_forbidden: Boolean; cvar; external;
  force_macro_keyon: Boolean; cvar; external;

const
  DECAY_BARS = 25;

var
  decay_bar: array[1..DECAY_BARS] of tDECAY_BAR; cvar; external;

procedure start_playing; cdecl; external;
procedure set_overall_volume(level: Byte); cdecl; external;
procedure stop_playing; cdecl; external;
procedure init_old_songdata;
procedure init_songdata; cdecl; external;
procedure init_timer_proc;
procedure done_timer_proc;
procedure get_chunk(pattern,line,channel: Byte; var chunk: tCHUNK); cdecl; external;
procedure put_chunk(pattern,line,channel: Byte; var chunk: tCHUNK); cdecl; external;
procedure count_order(var entries: Byte); cdecl; external;
procedure timer_poll_proc; cdecl;
function  calc_following_order(order: Byte): Integer;
function  is_4op_chan(chan: Byte): Boolean; cdecl; external;
function  min(value: Longint; minimum: Longint): Longint; cdecl; external;
function  max(value: Longint; maximum: Longint): Longint; cdecl; external;

// adt2opl3 {
var
  opl3port: Word; cvar; external;

type
  tOPLOUT_proc = procedure(reg,data: Word); cdecl;

procedure opl2out(reg,data: Word); cdecl; external;
procedure opl3exp(data: Word); cdecl; external;

var
  opl3out: tOPLOUT_proc; cvar; external;
// } adt2opl3

const
  ___UNIT_DATA_END___: Dword = 0;

implementation

uses
  pascal,
  debug,
  DOS,
  GO32,
  ISS_TIM,
  A2fileIO;

{$I go32/PIT/pas/PIT_consts.inc}

const
  ___IRQ_DATA_START___: Dword = 0;

procedure ___IRQ_CODE_START___; begin end;

const
  keyoff_flag        = $080;
  fixed_note_flag    = $090;
  pattern_loop_flag  = $0e0;
  pattern_break_flag = $0f0;

const
  MACRO_NOTE_RETRIG_FLAG = $80;
  MACRO_ENVELOPE_RESTART_FLAG = $40;
  MACRO_ZERO_FREQ_FLAG = $20;

var
  def_vibtrem_speed_factor: Byte; cvar; external; //const
  def_vibtrem_table_size: Byte; cvar; external; //const
  def_vibtrem_table: array[0..255] of Byte; cvar; external; //const

  vibtrem_speed_factor: Byte; cvar; external;
  vibtrem_table_size: Byte; cvar; external;
  vibtrem_table: array[0..255] of Byte; cvar; external;

type
  tVIBRATO_TREMOLO_TABLE =
    array[1..20] of Record
                      pos: Byte;
                      dir: Byte;
                      speed: Byte;
                      depth: Byte;
                      fine: Boolean;
                    end;

var
  vibr_table:  tVIBRATO_TREMOLO_TABLE; cvar; external;
  vibr_table2: tVIBRATO_TREMOLO_TABLE; cvar; external;
  trem_table:  tVIBRATO_TREMOLO_TABLE; cvar; external;
  trem_table2: tVIBRATO_TREMOLO_TABLE; cvar; external;

  fmpar_table:   array[1..20] of tFM_PARAMETER_TABLE; cvar; external;
  volume_lock:   array[1..20] of Boolean; cvar; external;
  vol4op_lock:   array[1..20] of Boolean; cvar; external;
  volume_table:  array[1..20] of Word; cvar; external;
  vscale_table:  array[1..20] of Word; cvar; external;
  peak_lock:     array[1..20] of Boolean; cvar; external;
  pan_lock:      array[1..20] of Boolean; cvar; external;
  event_table:   array[1..20] of tCHUNK; cvar; external;
  voice_table:   array[1..20] of Byte; cvar; external;
  modulator_vol: array[1..20] of Byte; cvar; external;
  carrier_vol:   array[1..20] of Byte; cvar; external;
  freq_table:    array[1..20] of Word; cvar; external;
  zero_fq_table: array[1..20] of Word; cvar; external;
  effect_table:  array[1..20] of Word; cvar; external;
  effect_table2: array[1..20] of Word; cvar; external;
  fslide_table:  array[1..20] of Byte; cvar; external;
  fslide_table2: array[1..20] of Byte; cvar; external;
  glfsld_table:  array[1..20] of Word; cvar; external;
  glfsld_table2: array[1..20] of Word; cvar; external;
  porta_table:   array[1..20] of Record freq: Word; speed: Byte; end; cvar; external;
  porta_table2:  array[1..20] of Record freq: Word; speed: Byte; end; cvar; external;
  portaFK_table: array[1..20] of Boolean; cvar; external;
  arpgg_table:   array[1..20] of Record state,note,add1,add2: Byte; end; cvar; external;
  arpgg_table2:  array[1..20] of Record state,note,add1,add2: Byte; end; cvar; external;
  retrig_table:  array[1..20] of Byte; cvar; external;
  retrig_table2: array[1..20] of Byte; cvar; external;
  tremor_table:  array[1..20] of Record pos: Integer; volume: Word; end; cvar; external;
  tremor_table2: array[1..20] of Record pos: Integer; volume: Word; end; cvar; external;
  panning_table: array[1..20] of Byte; cvar; external;
  last_effect:   array[1..20] of Word; cvar; external;
  last_effect2:  array[1..20] of Word; cvar; external;
  volslide_type: array[1..20] of Byte; cvar; external;
  event_new:     array[1..20] of Boolean; cvar; external;
  notedel_table: array[1..20] of Byte; cvar; external;
  notecut_table: array[1..20] of Byte; cvar; external;
  ftune_table:   array[1..20] of Shortint; cvar; external;
  keyoff_loop:   array[1..20] of Boolean; cvar; external;
  macro_table:   array[1..20] of Record
                                   fmreg_pos,arpg_pos,vib_pos: Word;
                                   fmreg_count,fmreg_duration,arpg_count,
                                   vib_count,vib_delay: Byte;
                                   vib_paused: Boolean;
                                   fmreg_table,arpg_table,vib_table: Byte;
                                   arpg_note: Byte;
                                   vib_freq: Word;
                                 end; cvar; external;

  loopbck_table: array[1..20] of Byte; cvar; external;
  loop_table:    array[1..20,0..255] of Byte; cvar; external;
  misc_register: Byte; cvar; external;

  current_tremolo_depth: Byte; cvar; external;
  current_vibrato_depth: Byte; cvar; external;

  speed_update: Boolean; cvar; external;
  lockvol: Boolean; cvar; external;
  panlock: Boolean; cvar; external;
  lockVP: Boolean; cvar; external;
  tremolo_depth: Byte; cvar; external;
  vibrato_depth: Byte; cvar; external;
  volume_scaling: Boolean; cvar; external;
  percussion_mode: Boolean; cvar; external;
  last_order: Byte; cvar; external;
  reset_chan: array[1..20] of Boolean; cvar; external;

const
  FreqStart = $156;
  FreqEnd   = $2ae;
  FreqRange = FreqEnd-FreqStart;

function nFreq(note: Byte): Word; cdecl; external;
function calc_freq_shift_up(freq,shift: Word): Word; cdecl; external;
function calc_freq_shift_down(freq,shift: Word): Word; cdecl; external;
function calc_vibtrem_shift(chan: Byte; var table_data): Word; cdecl; external;
procedure change_freq(chan: Byte; freq: Word); cdecl; external;
function ins_parameter(ins,param: Byte): Byte; cdecl; external;
function is_chan_adsr_data_empty(chan: Byte): Boolean; cdecl; external;
function is_ins_adsr_data_empty(ins: Byte): Boolean; cdecl; external;
function is_data_empty(var buf; size: Longint): Boolean; cdecl; external;
//min
//max
{$I adt2unit/pas/concw.pas}
procedure synchronize_song_timer; cdecl; external;
procedure change_frequency(chan: Byte; freq: Word); cdecl; external;
function _macro_speedup: Word; cdecl; external;
procedure TimerSetup(Hz: Longint); cdecl; forward;
procedure update_timer(Hz: Word); cdecl; external;
procedure update_playback_speed(speed_shift: Longint); cdecl; external;
procedure key_on(chan: Byte); cdecl; external;
procedure key_off(chan: Byte); cdecl; external;
procedure release_sustaining_sound(chan: Byte); cdecl; external;
function scale_volume(volume,scale_factor: Byte): Byte; cdecl; external;
function _4op_data_flag(chan: Byte): Dword; cdecl; external;
function _4op_vol_valid_chan(chan: Byte): Boolean; cdecl; external;
procedure set_ins_volume(modulator,carrier,chan: Byte); cdecl; external;
procedure set_ins_volume_4op(volume,chan: Byte); cdecl; external;
procedure reset_ins_volume(chan: Byte); cdecl; external;
procedure set_global_volume; cdecl; external;
//set_overall_volume
procedure init_macro_table(chan,note,ins: Byte; freq: Word); cdecl; external;
procedure set_ins_data(ins,chan: Byte); cdecl; external;
procedure update_modulator_adsrw(chan: Byte); cdecl; external;
procedure update_carrier_adsrw(chan: Byte); cdecl; external;
procedure update_fmpar(chan: Byte); cdecl; external;
//is_4op_chan
procedure output_note(note,ins,chan: Byte;
                      restart_macro,restart_adsr: Boolean); cdecl; external;
procedure generate_custom_vibrato(value: Byte); cdecl; external;

procedure update_fine_effects(chan: Byte); forward;

procedure play_line;

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

  If (current_line = 0) and
     (current_order = calc_following_order(0)) then
    time_playing := 0;

  If NOT (pattern_break and (next_line AND $0f0 = pattern_loop_flag)) and
         (current_order <> last_order) then
    begin
      FillChar(loopbck_table,SizeOf(loopbck_table),BYTE_NULL);
      FillChar(loop_table,SizeOf(loop_table),BYTE_NULL);
      last_order := current_order;
    end;

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
      else begin
             event_table[chan].effect_def := event[chan].effect_def;
             event_table[chan].effect := event[chan].effect;
           end;

      If (event[chan].effect_def2+event[chan].effect2 = 0) then
        If (glfsld_table2[chan] = 0) then effect_table2[chan] := 0
      else begin
             event_table[chan].effect_def2 := event[chan].effect_def2;
             event_table[chan].effect2 := event[chan].effect2;
           end;

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
               output_note(event[chan].note,voice_table[chan],chan,TRUE,TRUE)
             else output_note(event[chan].note,voice_table[chan],chan,TRUE,FALSE)
          else If (event[chan].note <> 0) and tporta_flag and
                  (event_table[chan].note = event_table[chan].note OR keyoff_flag) then
                 output_note(event_table[chan].note AND NOT keyoff_flag,voice_table[chan],chan,FALSE,TRUE)
               else If (event[chan].note <> 0) then
                      If portaFK_table[chan] and tporta_flag then
                        output_note(event[chan].note,event[chan].instr_def,chan,FALSE,TRUE)
                      else event_table[chan].note := event[chan].note;

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

procedure portamento_up(chan: Byte; slide: Word; limit: Word);

var
  freq: Word;

begin
  If (freq_table[chan] AND $1fff = 0) then
    EXIT; //portamento_up
  freq := calc_freq_shift_up(freq_table[chan] AND $1fff,slide);
  If (freq <= limit) then change_frequency(chan,freq)
  else change_frequency(chan,limit);

  //EXIT //portamento_up
end;

procedure portamento_down(chan: Byte; slide: Word; limit: Word);

var
  freq: Word;

begin
  If (freq_table[chan] AND $1fff = 0) then
    EXIT; //portamento_down
  freq := calc_freq_shift_down(freq_table[chan] AND $1fff,slide);
  If (freq >= limit) then change_frequency(chan,freq)
  else change_frequency(chan,limit);

  //EXIT //portamento_down
end;

procedure macro_vibrato__porta_up(chan: Byte; depth: Byte);

var
  freq: Word;

begin
  freq := calc_freq_shift_up(macro_table[chan].vib_freq AND $1fff,depth);
  If (freq <= nFreq(12*8+1)) then change_freq(chan,freq)
  else change_freq(chan,nFreq(12*8+1));
end;

procedure macro_vibrato__porta_down(chan: Byte; depth: Byte);

var
  freq: Word;

begin
  freq := calc_freq_shift_down(macro_table[chan].vib_freq AND $1fff,depth);
  If (freq >= nFreq(0)) then change_freq(chan,freq)
  else change_freq(chan,nFreq(0));
end;

procedure tone_portamento(chan: Byte);
begin
  If (freq_table[chan] AND $1fff > porta_table[chan].freq) then
    portamento_down(chan,porta_table[chan].speed,porta_table[chan].freq)
  else If (freq_table[chan] AND $1fff < porta_table[chan].freq) then
         portamento_up(chan,porta_table[chan].speed,porta_table[chan].freq);
end;

procedure tone_portamento2(chan: Byte);
begin
  If (freq_table[chan] AND $1fff > porta_table2[chan].freq) then
    portamento_down(chan,porta_table2[chan].speed,porta_table2[chan].freq)
  else If (freq_table[chan] AND $1fff < porta_table2[chan].freq) then
         portamento_up(chan,porta_table2[chan].speed,porta_table2[chan].freq);
end;

procedure slide_carrier_volume_up(chan: Byte; slide,limit: Byte);

var
  vol: Word;
  vLo,vHi: Byte;

begin
  vLo := LO(volume_table[chan]);
  vHi := HI(volume_table[chan]);
  If (vHi-slide >= limit) then vol := concw(vLo,vHi-slide)
  else vol := concw(vLo,limit);
  set_ins_volume(BYTE_NULL,HI(vol),chan);
  volume_table[chan] := vol;
end;

procedure slide_modulator_volume_up(chan: Byte; slide,limit: Byte);

var
  vol: Word;
  vLo,vHi: Byte;

begin
  vLo := LO(volume_table[chan]);
  vHi := HI(volume_table[chan]);
  If (vLo-slide >= limit) then vol := concw(vLo-slide,vHi)
  else vol := concw(limit,vHi);
  set_ins_volume(LO(vol),BYTE_NULL,chan);
  volume_table[chan] := vol;
end;

procedure slide_volume_up(chan,slide: Byte);

var
  limit1,limit2: Byte;
  limit1_4op,limit2_4op: Word;
  _4op_flag: Dword;
  _4op_conn: Byte;
  _4op_ch1,_4op_ch2: Byte;
  _4op_ins1,_4op_ins2: Byte;

begin
  _4op_flag := _4op_data_flag(chan);
  _4op_conn := (_4op_flag SHR 1) AND 3;
  _4op_ch1 := (_4op_flag SHR 3) AND 15;
  _4op_ch2 := (_4op_flag SHR 7) AND 15;
  _4op_ins1 := BYTE(_4op_flag SHR 11);
  _4op_ins2 := BYTE(_4op_flag SHR 19);

  If _4op_vol_valid_chan(chan) then
    begin
      If NOT peak_lock[_4op_ch1] then limit1_4op := 0
      else limit1_4op := ins_parameter(_4op_ins1,3) AND $3f SHL 16 +
                         ins_parameter(_4op_ins1,2) AND $3f;
      If NOT peak_lock[_4op_ch2] then limit2_4op := 0
      else limit2_4op := ins_parameter(_4op_ins2,3) AND $3f SHL 16 +
                         ins_parameter(_4op_ins2,2) AND $3f;
    end
  else begin
         If NOT peak_lock[chan] then limit1 := 0
         else limit1 := ins_parameter(event_table[chan].instr_def,3) AND $3f;
         If NOT peak_lock[chan] then limit2 := 0
         else limit2 := ins_parameter(event_table[chan].instr_def,2) AND $3f;
       end;

  Case volslide_type[chan] of
    0: begin
         If NOT _4op_vol_valid_chan(chan) then
           begin
             slide_carrier_volume_up(chan,slide,limit1);
             If (ins_parameter(voice_table[chan],10) AND 1 = 1) or
                (percussion_mode and (chan in [17..20])) then
               slide_modulator_volume_up(chan,slide,limit2);
           end
         else
           Case _4op_conn of
             // FM/FM
             0: slide_carrier_volume_up(_4op_ch1,slide,HI(limit1_4op));
             // FM/AM
             1: begin
                  slide_carrier_volume_up(_4op_ch1,slide,HI(limit1_4op));
                  slide_modulator_volume_up(_4op_ch2,slide,LO(limit2_4op));
                end;
             // AM/FM
             2: begin
                  slide_carrier_volume_up(_4op_ch1,slide,HI(limit1_4op));
                  slide_carrier_volume_up(_4op_ch2,slide,HI(limit2_4op));
                end;
             // AM/AM
             3: begin
                  slide_carrier_volume_up(_4op_ch1,slide,HI(limit1_4op));
                  slide_modulator_volume_up(_4op_ch1,slide,LO(limit1_4op));
                  slide_modulator_volume_up(_4op_ch2,slide,LO(limit2_4op));
                end;
           end;
       end;
    1: slide_carrier_volume_up(chan,slide,limit1);
    2: slide_modulator_volume_up(chan,slide,limit2);
    3: begin
         slide_carrier_volume_up(chan,slide,limit1);
         slide_modulator_volume_up(chan,slide,limit2);
       end;
  end;
end;

procedure slide_carrier_volume_down(chan: Byte; slide: Byte);

var
  vol: Word;
  vLo,vHi: Byte;

begin
  vLo := LO(volume_table[chan]);
  vHi := HI(volume_table[chan]);
  If (vHi+slide <= 63) then vol := concw(vLo,vHi+slide)
  else vol := concw(vLo,63);
  set_ins_volume(BYTE_NULL,HI(vol),chan);
  volume_table[chan] := vol;
end;

procedure slide_modulator_volume_down(chan: Byte; slide: Byte);

var
  vol: Word;
  vLo,vHi: Byte;

begin
  vLo := LO(volume_table[chan]);
  vHi := HI(volume_table[chan]);
  If (vLo+slide <= 63) then vol := concw(vLo+slide,vHi)
  else vol := concw(63,vHi);
  set_ins_volume(LO(vol),BYTE_NULL,chan);
  volume_table[chan] := vol;
end;

procedure slide_volume_down(chan,slide: Byte);

var
  _4op_flag: Dword;
  _4op_conn: Byte;
  _4op_ch1,_4op_ch2: Byte;

begin
  _4op_flag := _4op_data_flag(chan);
  _4op_conn := (_4op_flag SHR 1) AND 3;
  _4op_ch1 := (_4op_flag SHR 3) AND 15;
  _4op_ch2 := (_4op_flag SHR 7) AND 15;

  Case volslide_type[chan] of
    0: begin
         If NOT _4op_vol_valid_chan(chan) then
           begin
             slide_carrier_volume_down(chan,slide);
             If (ins_parameter(voice_table[chan],10) AND 1 = 1) or
                (percussion_mode and (chan in [17..20])) then
               slide_modulator_volume_down(chan,slide);
           end
         else
           Case _4op_conn of
             // FM/FM
             0: slide_carrier_volume_down(_4op_ch1,slide);
             // FM/AM
             1: begin
                  slide_carrier_volume_down(_4op_ch1,slide);
                  slide_modulator_volume_down(_4op_ch2,slide);
                end;
             // AM/FM
             2: begin
                  slide_carrier_volume_down(_4op_ch1,slide);
                  slide_carrier_volume_down(_4op_ch2,slide);
                end;
             // AM/AM
             3: begin
                  slide_carrier_volume_down(_4op_ch1,slide);
                  slide_modulator_volume_down(_4op_ch1,slide);
                  slide_modulator_volume_down(_4op_ch2,slide);
                end;
           end;
       end;
    1: slide_carrier_volume_down(chan,slide);
    2: slide_modulator_volume_down(chan,slide);
    3: begin
         slide_carrier_volume_down(chan,slide);
         slide_modulator_volume_down(chan,slide);
       end;
  end;
end;

procedure volume_slide(chan,up_speed,down_speed: Byte);
begin
  If (up_speed <> 0) then slide_volume_up(chan,up_speed)
  else If (down_speed <> 0) then slide_volume_down(chan,down_speed);
end;

procedure global_volume_slide(up_speed,down_speed: Byte);
begin
  If (up_speed <> BYTE_NULL) then
    global_volume := max(global_volume+up_speed,63);
  If (down_speed <> BYTE_NULL) then
    If (global_volume >= down_speed) then Dec(global_volume,down_speed)
    else global_volume := 0;
  set_global_volume;
end;

procedure arpeggio(chan: Byte);

const
  arpgg_state: array[0..2] of Byte = (1,2,0);

var
  freq: Word;

begin
  Case arpgg_table[chan].state of
    0: freq := nFreq(arpgg_table[chan].note-1);
    1: freq := nFreq(arpgg_table[chan].note-1 +arpgg_table[chan].add1);
    2: freq := nFreq(arpgg_table[chan].note-1 +arpgg_table[chan].add2);
  end;

  arpgg_table[chan].state := arpgg_state[arpgg_table[chan].state];
  change_frequency(chan,freq+
    SHORTINT(ins_parameter(event_table[chan].instr_def,12)));
end;

procedure arpeggio2(chan: Byte);

const
  arpgg_state: array[0..2] of Byte = (1,2,0);

var
  freq: Word;

begin
  Case arpgg_table2[chan].state of
    0: freq := nFreq(arpgg_table2[chan].note-1);
    1: freq := nFreq(arpgg_table2[chan].note-1 +arpgg_table2[chan].add1);
    2: freq := nFreq(arpgg_table2[chan].note-1 +arpgg_table2[chan].add2);
  end;

  arpgg_table2[chan].state := arpgg_state[arpgg_table2[chan].state];
  change_frequency(chan,freq+
    SHORTINT(ins_parameter(event_table[chan].instr_def,12)));
end;

procedure vibrato(chan: Byte);

var
  freq,old_freq: Word;

begin
  Inc(vibr_table[chan].pos,vibr_table[chan].speed*vibtrem_speed_factor);
  freq := calc_vibtrem_shift(chan,vibr_table);
  old_freq := freq_table[chan];
  If (vibr_table[chan].dir = 0) then portamento_down(chan,freq,nFreq(0))
  else portamento_up(chan,freq,nFreq(12*8+1));
  freq_table[chan] := old_freq;
end;

procedure vibrato2(chan: Byte);

var
  freq,old_freq: Word;

begin
  Inc(vibr_table2[chan].pos,vibr_table2[chan].speed*vibtrem_speed_factor);
  freq := calc_vibtrem_shift(chan,vibr_table2);
  old_freq := freq_table[chan];
  If (vibr_table2[chan].dir = 0) then portamento_down(chan,freq,nFreq(0))
  else portamento_up(chan,freq,nFreq(12*8+1));
  freq_table[chan] := old_freq;
end;

procedure tremolo(chan: Byte);

var
  vol,old_vol: Word;

begin
  Inc(trem_table[chan].pos,trem_table[chan].speed*vibtrem_speed_factor);
  vol := calc_vibtrem_shift(chan,trem_table);
  old_vol := volume_table[chan];
  If (trem_table[chan].dir = 0) then slide_volume_down(chan,vol)
  else slide_volume_up(chan,vol);
  volume_table[chan] := old_vol;
end;

procedure tremolo2(chan: Byte);

var
  vol,old_vol: Word;

begin
  Inc(trem_table2[chan].pos,trem_table2[chan].speed*vibtrem_speed_factor);
  vol := calc_vibtrem_shift(chan,trem_table2);
  old_vol := volume_table[chan];
  If (trem_table2[chan].pos = 0) then slide_volume_down(chan,vol)
  else slide_volume_up(chan,vol);
  volume_table[chan] := old_vol;
end;

procedure update_effects;

var
  chan,eLo,eHi,
  eLo2,eHi2: Byte;

function chanvol(chan: Byte): Byte;
begin
  If (ins_parameter(voice_table[chan],10) AND 1 = 0) then chanvol := 63-HI(volume_table[chan])
  else chanvol := 63-Round((LO(volume_table[chan])+HI(volume_table[chan]))/2);
end;

begin
  For chan := 1 to songdata.nm_tracks do
    begin
      eLo  := LO(effect_table[chan]);
      eHi  := HI(effect_table[chan]);
      eLo2 := LO(effect_table2[chan]);
      eHi2 := HI(effect_table2[chan]);

      Case eLo of
        ef_Arpeggio+ef_fix1:
          arpeggio(chan);

        ef_ArpggVSlide:
          begin
            volume_slide(chan,eHi DIV 16,eHi MOD 16);
            arpeggio(chan);
          end;

        ef_ArpggVSlideFine:
          arpeggio(chan);

        ef_FSlideUp:
          portamento_up(chan,eHi,nFreq(12*8+1));

        ef_FSlideDown:
          portamento_down(chan,eHi,nFreq(0));

        ef_FSlideUpVSlide:
          begin
            portamento_up(chan,fslide_table[chan],nFreq(12*8+1));
            volume_slide(chan,eHi DIV 16,eHi MOD 16);
          end;

        ef_FSlUpVSlF:
          portamento_up(chan,fslide_table[chan],nFreq(12*8+1));

        ef_FSlideDownVSlide:
          begin
            portamento_down(chan,fslide_table[chan],nFreq(0));
            volume_slide(chan,eHi DIV 16,eHi MOD 16);
          end;

        ef_FSlDownVSlF:
          portamento_down(chan,fslide_table[chan],nFreq(0));

        ef_FSlUpFineVSlide:
          volume_slide(chan,eHi DIV 16,eHi MOD 16);

        ef_FSlDownFineVSlide:
          volume_slide(chan,eHi DIV 16,eHi MOD 16);

        ef_TonePortamento:
          tone_portamento(chan);

        ef_TPortamVolSlide:
          begin
            volume_slide(chan,eHi DIV 16,eHi MOD 16);
            tone_portamento(chan);
          end;

        ef_TPortamVSlideFine:
          tone_portamento(chan);

        ef_Vibrato:
          If NOT vibr_table[chan].fine then
            vibrato(chan);

        ef_Tremolo:
          If NOT trem_table[chan].fine then
            tremolo(chan);

        ef_VibratoVolSlide:
          begin
            volume_slide(chan,eHi DIV 16,eHi MOD 16);
            If NOT vibr_table[chan].fine then
              vibrato(chan);
          end;

        ef_VibratoVSlideFine:
          If NOT vibr_table[chan].fine then
            vibrato(chan);

        ef_VolSlide:
          volume_slide(chan,eHi DIV 16,eHi MOD 16);

        ef_RetrigNote:
          If (retrig_table[chan] >= eHi) then
            begin
              retrig_table[chan] := 0;
              output_note(event_table[chan].note,
                          event_table[chan].instr_def,chan,TRUE,TRUE);
            end
          else Inc(retrig_table[chan]);

        ef_MultiRetrigNote:
          If (retrig_table[chan] >= eHi DIV 16) then
            begin
              Case eHi MOD 16 of
                0,8: ;

                1: slide_volume_down(chan,1);
                2: slide_volume_down(chan,2);
                3: slide_volume_down(chan,4);
                4: slide_volume_down(chan,8);
                5: slide_volume_down(chan,16);

                9: slide_volume_up(chan,1);
               10: slide_volume_up(chan,2);
               11: slide_volume_up(chan,4);
               12: slide_volume_up(chan,8);
               13: slide_volume_up(chan,16);


                6: slide_volume_down(chan,chanvol(chan)-
                                          Round(chanvol(chan)*2/3));
                7: slide_volume_down(chan,chanvol(chan)-
                                          Round(chanvol(chan)*1/2));

               14: slide_volume_up(chan,max(Round(chanvol(chan)*3/2)-
                                            chanvol(chan),63));
               15: slide_volume_up(chan,max(Round(chanvol(chan)*2)-
                                            chanvol(chan),63));
              end;

             retrig_table[chan] := 0;
             output_note(event_table[chan].note,
                         event_table[chan].instr_def,chan,TRUE,TRUE);
            end
          else Inc(retrig_table[chan]);

        ef_Tremor:
          If (tremor_table[chan].pos >= 0) then
            begin
              If (SUCC(tremor_table[chan].pos) <= eHi DIV 16) then
                Inc(tremor_table[chan].pos)
              else begin
                     slide_volume_down(chan,63);
                     tremor_table[chan].pos := -1;
                   end;
            end
          else If (PRED(tremor_table[chan].pos) >= -(eHi MOD 16)) then
                 Dec(tremor_table[chan].pos)
               else begin
                      set_ins_volume(LO(tremor_table[chan].volume),
                                     HI(tremor_table[chan].volume),chan);
                      tremor_table[chan].pos := 1;
                    end;

        ef_extended2+ef_fix2+ef_ex2_NoteDelay:
          If (notedel_table[chan] = 0) then
            begin
              notedel_table[chan] := BYTE_NULL;
              output_note(event_table[chan].note,
                          event_table[chan].instr_def,chan,TRUE,TRUE);
            end
          else Dec(notedel_table[chan]);

        ef_extended2+ef_fix2+ef_ex2_NoteCut:
          If (notecut_table[chan] = 0) then
            begin
              notecut_table[chan] := BYTE_NULL;
              key_off(chan);
            end
          else Dec(notecut_table[chan]);

        ef_extended2+ef_fix2+ef_ex2_GlVolSlideUp:
          global_volume_slide(eHi,BYTE_NULL);

        ef_extended2+ef_fix2+ef_ex2_GlVolSlideDn:
          global_volume_slide(BYTE_NULL,eHi);
      end;

      Case eLo2 of
        ef_Arpeggio+ef_fix1:
          arpeggio2(chan);

        ef_ArpggVSlide:
          begin
            volume_slide(chan,eHi2 DIV 16,eHi2 MOD 16);
            arpeggio2(chan);
          end;

        ef_ArpggVSlideFine:
          arpeggio2(chan);

        ef_FSlideUp:
          portamento_up(chan,eHi2,nFreq(12*8+1));

        ef_FSlideDown:
          portamento_down(chan,eHi2,nFreq(0));

        ef_FSlideUpVSlide:
          begin
            portamento_up(chan,fslide_table2[chan],nFreq(12*8+1));
            volume_slide(chan,eHi2 DIV 16,eHi2 MOD 16);
          end;

        ef_FSlUpVSlF:
          portamento_up(chan,fslide_table2[chan],nFreq(12*8+1));

        ef_FSlideDownVSlide:
          begin
            portamento_down(chan,fslide_table2[chan],nFreq(0));
            volume_slide(chan,eHi2 DIV 16,eHi2 MOD 16);
          end;

        ef_FSlDownVSlF:
          portamento_down(chan,fslide_table2[chan],nFreq(0));

        ef_FSlUpFineVSlide:
          volume_slide(chan,eHi2 DIV 16,eHi2 MOD 16);

        ef_FSlDownFineVSlide:
          volume_slide(chan,eHi2 DIV 16,eHi2 MOD 16);

        ef_TonePortamento:
          tone_portamento2(chan);

        ef_TPortamVolSlide:
          begin
            volume_slide(chan,eHi2 DIV 16,eHi2 MOD 16);
            tone_portamento2(chan);
          end;

        ef_TPortamVSlideFine:
          tone_portamento2(chan);

        ef_Vibrato:
          If NOT vibr_table2[chan].fine then
            vibrato2(chan);

        ef_Tremolo:
          If NOT trem_table2[chan].fine then
            tremolo2(chan);

        ef_VibratoVolSlide:
          begin
            volume_slide(chan,eHi2 DIV 16,eHi2 MOD 16);
            If NOT vibr_table2[chan].fine then
              vibrato2(chan);
          end;

        ef_VibratoVSlideFine:
          If NOT vibr_table2[chan].fine then
            vibrato2(chan);

        ef_VolSlide:
          volume_slide(chan,eHi2 DIV 16,eHi2 MOD 16);

        ef_RetrigNote:
          If (retrig_table2[chan] >= eHi2) then
            begin
              retrig_table2[chan] := 0;
              output_note(event_table[chan].note,
                          event_table[chan].instr_def,chan,TRUE,TRUE);
            end
          else Inc(retrig_table2[chan]);

        ef_MultiRetrigNote:
          If (retrig_table2[chan] >= eHi2 DIV 16) then
            begin
              Case eHi2 MOD 16 of
                0,8: ;

                1: slide_volume_down(chan,1);
                2: slide_volume_down(chan,2);
                3: slide_volume_down(chan,4);
                4: slide_volume_down(chan,8);
                5: slide_volume_down(chan,16);

                9: slide_volume_up(chan,1);
               10: slide_volume_up(chan,2);
               11: slide_volume_up(chan,4);
               12: slide_volume_up(chan,8);
               13: slide_volume_up(chan,16);


                6: slide_volume_down(chan,chanvol(chan)-
                                          Round(chanvol(chan)*2/3));
                7: slide_volume_down(chan,chanvol(chan)-
                                          Round(chanvol(chan)*1/2));

               14: slide_volume_up(chan,max(Round(chanvol(chan)*3/2)-
                                            chanvol(chan),63));
               15: slide_volume_up(chan,max(Round(chanvol(chan)*2)-
                                            chanvol(chan),63));
              end;

              retrig_table2[chan] := 0;
              output_note(event_table[chan].note,
                          event_table[chan].instr_def,chan,TRUE,TRUE);
            end
          else Inc(retrig_table2[chan]);

        ef_Tremor:
          If (tremor_table2[chan].pos >= 0) then
            begin
              If (SUCC(tremor_table2[chan].pos) <= eHi2 DIV 16) then
                Inc(tremor_table2[chan].pos)
              else begin
                     slide_volume_down(chan,63);
                     tremor_table2[chan].pos := -1;
                   end;
            end
          else If (PRED(tremor_table2[chan].pos) >= -(eHi2 MOD 16)) then
                 Dec(tremor_table2[chan].pos)
               else begin
                      set_ins_volume(LO(tremor_table2[chan].volume),
                                     HI(tremor_table2[chan].volume),chan);
                      tremor_table2[chan].pos := 1;
                    end;

        ef_extended2+ef_fix2+ef_ex2_NoteDelay:
          If (notedel_table[chan] = 0) then
            begin
              notedel_table[chan] := BYTE_NULL;
              output_note(event_table[chan].note,
                          event_table[chan].instr_def,chan,TRUE,TRUE);
            end
          else Dec(notedel_table[chan]);

        ef_extended2+ef_fix2+ef_ex2_NoteCut:
          If (notecut_table[chan] = 0) then
            begin
              notecut_table[chan] := BYTE_NULL;
              key_off(chan);
            end
          else Dec(notecut_table[chan]);

        ef_extended2+ef_fix2+ef_ex2_GlVolSlideUp:
          global_volume_slide(eHi2,BYTE_NULL);

        ef_extended2+ef_fix2+ef_ex2_GlVolSlideDn:
          global_volume_slide(BYTE_NULL,eHi2);
      end;
    end;
end;

procedure update_fine_effects(chan: Byte);

var
  eLo,eHi,
  eLo2,eHi2: Byte;

begin
  eLo  := LO(effect_table[chan]);
  eHi  := HI(effect_table[chan]);
  eLo2 := LO(effect_table2[chan]);
  eHi2 := HI(effect_table2[chan]);

  Case eLo of
    ef_ArpggVSlideFine:
      volume_slide(chan,eHi DIV 16,eHi MOD 16);

    ef_FSlideUpFine:
      portamento_up(chan,eHi,nFreq(12*8+1));

    ef_FSlideDownFine:
      portamento_down(chan,eHi,nFreq(0));

    ef_FSlUpVSlF:
      volume_slide(chan,eHi DIV 16,eHi MOD 16);

    ef_FSlDownVSlF:
      volume_slide(chan,eHi DIV 16,eHi MOD 16);

    ef_FSlUpFineVSlide:
      portamento_up(chan,fslide_table[chan],nFreq(12*8+1));

    ef_FSlUpFineVSlF:
      begin
        portamento_up(chan,fslide_table[chan],nFreq(12*8+1));
        volume_slide(chan,eHi DIV 16,eHi MOD 16);
      end;

    ef_FSlDownFineVSlide:
      portamento_down(chan,fslide_table[chan],nFreq(0));

    ef_FSlDownFineVSlF:
      begin
        portamento_down(chan,fslide_table[chan],nFreq(0));
        volume_slide(chan,eHi DIV 16,eHi MOD 16);
      end;

    ef_TPortamVSlideFine:
      volume_slide(chan,eHi DIV 16,eHi MOD 16);

    ef_Vibrato:
      If vibr_table[chan].fine then
        vibrato(chan);

    ef_Tremolo:
      If trem_table[chan].fine then
        tremolo(chan);

    ef_VibratoVolSlide:
      If vibr_table[chan].fine then
        vibrato(chan);

    ef_VibratoVSlideFine:
      begin
        volume_slide(chan,eHi DIV 16,eHi MOD 16);
        If vibr_table[chan].fine then
          vibrato(chan);
      end;

    ef_VolSlideFine:
      volume_slide(chan,eHi DIV 16,eHi MOD 16);

    ef_extended2+ef_fix2+ef_ex2_GlVolSlideUpF:
      global_volume_slide(eHi,BYTE_NULL);

    ef_extended2+ef_fix2+ef_ex2_GlVolSlideDnF:
      global_volume_slide(BYTE_NULL,eHi);
  end;

  Case eLo2 of
    ef_ArpggVSlideFine:
      volume_slide(chan,eHi2 DIV 16,eHi2 MOD 16);

    ef_FSlideUpFine:
      portamento_up(chan,eHi2,nFreq(12*8+1));

    ef_FSlideDownFine:
      portamento_down(chan,eHi2,nFreq(0));

    ef_FSlUpVSlF:
      volume_slide(chan,eHi2 DIV 16,eHi2 MOD 16);

    ef_FSlDownVSlF:
      volume_slide(chan,eHi2 DIV 16,eHi2 MOD 16);

    ef_FSlUpFineVSlide:
      portamento_up(chan,fslide_table2[chan],nFreq(12*8+1));

    ef_FSlUpFineVSlF:
      begin
        portamento_up(chan,fslide_table2[chan],nFreq(12*8+1));
        volume_slide(chan,eHi2 DIV 16,eHi2 MOD 16);
      end;

    ef_FSlDownFineVSlide:
      portamento_down(chan,fslide_table2[chan],nFreq(0));

    ef_FSlDownFineVSlF:
      begin
        portamento_down(chan,fslide_table2[chan],nFreq(0));
        volume_slide(chan,eHi2 DIV 16,eHi2 MOD 16);
      end;

    ef_TPortamVSlideFine:
      volume_slide(chan,eHi2 DIV 16,eHi2 MOD 16);

    ef_Vibrato:
      If vibr_table2[chan].fine then
        vibrato2(chan);

    ef_Tremolo:
      If trem_table2[chan].fine then
        tremolo2(chan);

    ef_VibratoVolSlide:
      If vibr_table2[chan].fine then
        vibrato2(chan);

    ef_VibratoVSlideFine:
      begin
        volume_slide(chan,eHi2 DIV 16,eHi2 MOD 16);
        If vibr_table2[chan].fine then
          vibrato2(chan);
      end;

    ef_VolSlideFine:
      volume_slide(chan,eHi2 DIV 16,eHi2 MOD 16);

    ef_extended2+ef_fix2+ef_ex2_GlVolSlideUpF:
      global_volume_slide(eHi2,BYTE_NULL);

    ef_extended2+ef_fix2+ef_ex2_GlVolSlideDnF:
      global_volume_slide(BYTE_NULL,eHi2);
  end;
end;

procedure update_extra_fine_effects;

var
  chan,eLo,eHi,
  eLo2,eHi2: Byte;

begin
  For chan := 1 to songdata.nm_tracks do
    begin
      eLo  := LO(effect_table[chan]);
      eHi  := HI(effect_table[chan]);
      eLo2 := LO(effect_table2[chan]);
      eHi2 := HI(effect_table2[chan]);

      Case eLo of
        ef_extended2+ef_fix2+ef_ex2_GlVolSldUpXF:
          global_volume_slide(eHi,BYTE_NULL);

        ef_extended2+ef_fix2+ef_ex2_GlVolSldDnXF:
          global_volume_slide(BYTE_NULL,eHi);

        ef_extended2+ef_fix2+ef_ex2_VolSlideUpXF:
          volume_slide(chan,eHi,0);

        ef_extended2+ef_fix2+ef_ex2_VolSlideDnXF:
          volume_slide(chan,0,eHi);

        ef_extended2+ef_fix2+ef_ex2_FreqSlideUpXF:
          portamento_up(chan,eHi,nFreq(12*8+1));

        ef_extended2+ef_fix2+ef_ex2_FreqSlideDnXF:
          portamento_down(chan,eHi,nFreq(0));

        ef_ExtraFineArpeggio:
          arpeggio(chan);

        ef_ExtraFineVibrato:
          If NOT vibr_table[chan].fine then
            vibrato(chan);

        ef_ExtraFineTremolo:
          If NOT trem_table[chan].fine then
            tremolo(chan);
      end;

      Case eLo2 of
        ef_extended2+ef_fix2+ef_ex2_GlVolSldUpXF:
          global_volume_slide(eHi2,BYTE_NULL);

        ef_extended2+ef_fix2+ef_ex2_GlVolSldDnXF:
          global_volume_slide(BYTE_NULL,eHi2);

        ef_extended2+ef_fix2+ef_ex2_VolSlideUpXF:
          volume_slide(chan,eHi2,0);

        ef_extended2+ef_fix2+ef_ex2_VolSlideDnXF:
          volume_slide(chan,0,eHi2);

        ef_extended2+ef_fix2+ef_ex2_FreqSlideUpXF:
          portamento_up(chan,eHi2,nFreq(12*8+1));

        ef_extended2+ef_fix2+ef_ex2_FreqSlideDnXF:
          portamento_down(chan,eHi2,nFreq(0));

        ef_ExtraFineArpeggio:
          arpeggio2(chan);

        ef_ExtraFineVibrato:
          If NOT vibr_table2[chan].fine then
            vibrato2(chan);

        ef_ExtraFineTremolo:
          If NOT trem_table2[chan].fine then
            tremolo2(chan);
      end;
    end;
end;

function calc_following_order(order: Byte): Integer;

var
  result: Integer;
  index,jump_count: Byte;

begin
  _dbg_enter ({$I %FILE%}, 'calc_following_order');

  result := -1;
  index := order;
  jump_count := 0;

  Repeat
    If (songdata.pattern_order[index] < $80) then result := index
    else begin
           index := songdata.pattern_order[index]-$80;
           Inc(jump_count);
         end;
  until (jump_count > $7f) or
        (result <> -1);

  calc_following_order := result;

  _dbg_leave; //EXIT //calc_following_order
end;

function calc_order_jump: Integer; cdecl; external;

procedure update_song_position;

var
  temp: Byte;

begin
  _dbg_enter ({$I %FILE%}, 'update_song_position');

  If (current_line < PRED(songdata.patt_len)) and NOT pattern_break then Inc(current_line)
  else begin
         If NOT (pattern_break and (next_line AND $0f0 = pattern_loop_flag)) and
                (current_order < $7f) then
           begin
             FillChar(loopbck_table,SizeOf(loopbck_table),BYTE_NULL);
             FillChar(loop_table,SizeOf(loop_table),BYTE_NULL);
             Inc(current_order);
           end;

         If pattern_break and (next_line AND $0f0 = pattern_loop_flag) then
           begin
             temp := next_line-pattern_loop_flag;
             next_line := loopbck_table[temp];
             If (loop_table[temp][current_line] <> 0) then
               Dec(loop_table[temp][current_line]);
           end
         else If pattern_break and (next_line AND $0f0 = pattern_break_flag) then
                begin
                  If (event_table[next_line-pattern_break_flag].effect_def2 = ef_PositionJump) then
                    current_order := event_table[next_line-pattern_break_flag].effect2
                  else current_order := event_table[next_line-pattern_break_flag].effect;
                  pattern_break := FALSE;
                end
              else If (current_order > $7f) then
                     current_order := 0;

         If (songdata.pattern_order[current_order] > $7f) then
           If (calc_order_jump = -1) then
             begin
               _dbg_leave; EXIT; //update_song_position
             end;

         current_pattern := songdata.pattern_order[current_order];
         If NOT pattern_break then current_line := 0
         else begin
                pattern_break := FALSE;
                current_line := next_line;
              end;
       end;

  For temp := 1 to songdata.nm_tracks do
    begin
      glfsld_table[temp] := 0;
      glfsld_table2[temp] := 0;
    end;

  If (current_line = 0) and
     (current_order = calc_following_order(0)) and speed_update then
    begin
      tempo := songdata.tempo;
      speed := songdata.speed;
      update_timer(tempo);
    end;

  _dbg_leave; //EXIT //update_song_position
end;

procedure poll_proc;

var
  temp: Byte;

begin
  _dbg_enter ({$I %FILE%}, '_poll_proc');

  If (NOT pattern_delay and (ticks-tick0+1 >= speed)) or
     fast_forward then
    begin
      If (songdata.pattern_order[current_order] > $7f) then
        If (calc_order_jump = -1) then
          begin
            _dbg_leave; EXIT; //_poll_proc
          end;

      current_pattern := songdata.pattern_order[current_order];
      play_line;
      If NOT fast_forward then update_effects
      else For temp := 1 to speed do
             begin
               update_effects;
               If (temp MOD 4 = temp) then
                 update_extra_fine_effects;
               Inc(ticks);
             end;

      pattern_break_docmd := pattern_break;
      pattern_break_oldord := current_order;
      If fast_forward or NOT pattern_delay then
        update_song_position;

      If (pattern_break_docmd = TRUE) then
        pattern_break_loop := current_order = pattern_break_oldord;

      tick0 := ticks;
      If fast_forward then
        If NOT pattern_delay then synchronize_song_timer;

      If fast_forward and pattern_delay then
        begin
          tickD := 0;
          pattern_delay := FALSE;
        end;
    end
  else
    begin
      update_effects;
      Inc(ticks);
      If pattern_delay and (tickD > 1) then Dec(tickD)
      else begin
             If pattern_delay then
               begin
                 tick0 := ticks;
                 update_song_position;
               end;
             pattern_delay := FALSE;
           end;
    end;

  Inc(tickXF);
  If (tickXF MOD 4 = 0) then
    begin
      update_extra_fine_effects;
      Dec(tickXF,4);
    end;

  _dbg_leave; //EXIT //_poll_proc
end;

procedure macro_poll_proc;

const
  IDLE = $0fff;
  FINISHED = $0ffff;

var
  chan: Byte;
  finished_flag: Word;

begin
  _dbg_enter ({$I %FILE%}, 'macro_poll_proc');

  For chan := 1 to songdata.nm_tracks do
    begin
      If NOT keyoff_loop[chan] then finished_flag := FINISHED
      else finished_flag := IDLE;

      With macro_table[chan] do
        begin
          With songdata.instr_macros[fmreg_table] do
            If (fmreg_table <> 0) and (speed <> 0) then
              If (fmreg_duration > 1) then Dec(fmreg_duration)
              else begin
                     fmreg_count := 1;
                     If (fmreg_pos <= length) then
                       If (loop_begin <> 0) and (loop_length <> 0) then
                         If (fmreg_pos = loop_begin+PRED(loop_length)) then
                           fmreg_pos := loop_begin
                         else If (fmreg_pos < length) then Inc(fmreg_pos)
                              else fmreg_pos := finished_flag
                       else If (fmreg_pos < length) then Inc(fmreg_pos)
                            else fmreg_pos := finished_flag
                     else fmreg_pos := finished_flag;

                     If (freq_table[chan] OR $2000 = freq_table[chan]) and
                        (keyoff_pos <> 0) and
                        (fmreg_pos >= keyoff_pos) then
                       fmreg_pos := IDLE
                     else If (freq_table[chan] OR $2000 <> freq_table[chan]) and
                             (fmreg_pos <> 0) and (keyoff_pos <> 0) and
                             ((fmreg_pos < keyoff_pos) or (fmreg_pos = IDLE)) then
                            fmreg_pos := keyoff_pos;

                     If (fmreg_pos <> 0) and
                        (fmreg_pos <> IDLE) and (fmreg_pos <> finished_flag) then
                       begin
                         fmreg_duration := data[fmreg_pos].duration;
                         If (fmreg_duration <> 0) then
                           With data[fmreg_pos] do
                             begin
                               // force KEY-ON with missing ADSR instrument data
                               force_macro_keyon := FALSE;
                               If (fmreg_pos = 1) then
                                 If is_ins_adsr_data_empty(voice_table[chan]) and
                                    NOT (songdata.dis_fmreg_col[fmreg_table][0] and
                                         songdata.dis_fmreg_col[fmreg_table][1] and
                                         songdata.dis_fmreg_col[fmreg_table][2] and
                                         songdata.dis_fmreg_col[fmreg_table][3] and
                                         songdata.dis_fmreg_col[fmreg_table][12] and
                                         songdata.dis_fmreg_col[fmreg_table][13] and
                                         songdata.dis_fmreg_col[fmreg_table][14] and
                                         songdata.dis_fmreg_col[fmreg_table][15]) then
                                   force_macro_keyon := TRUE;

                               If NOT songdata.dis_fmreg_col[fmreg_table][0] then
                                 fmpar_table[chan].adsrw_mod.attck := fm_data.ATTCK_DEC_modulator SHR 4;

                               If NOT songdata.dis_fmreg_col[fmreg_table][1] then
                                 fmpar_table[chan].adsrw_mod.dec := fm_data.ATTCK_DEC_modulator AND $0f;

                               If NOT songdata.dis_fmreg_col[fmreg_table][2] then
                                 fmpar_table[chan].adsrw_mod.sustn := fm_data.SUSTN_REL_modulator SHR 4;

                               If NOT songdata.dis_fmreg_col[fmreg_table][3] then
                                 fmpar_table[chan].adsrw_mod.rel := fm_data.SUSTN_REL_modulator AND $0f;

                               If NOT songdata.dis_fmreg_col[fmreg_table][4] then
                                 fmpar_table[chan].adsrw_mod.wform := fm_data.WAVEFORM_modulator AND $07;

                               If NOT songdata.dis_fmreg_col[fmreg_table][6] then
                                 fmpar_table[chan].kslM := fm_data.KSL_VOLUM_modulator SHR 6;

                               If NOT songdata.dis_fmreg_col[fmreg_table][7] then
                                 fmpar_table[chan].multipM := fm_data.AM_VIB_EG_modulator AND $0f;

                               If NOT songdata.dis_fmreg_col[fmreg_table][8] then
                                 fmpar_table[chan].tremM := fm_data.AM_VIB_EG_modulator SHR 7;

                               If NOT songdata.dis_fmreg_col[fmreg_table][9] then
                                 fmpar_table[chan].vibrM := fm_data.AM_VIB_EG_modulator SHR 6 AND 1;

                               If NOT songdata.dis_fmreg_col[fmreg_table][10] then
                                 fmpar_table[chan].ksrM := fm_data.AM_VIB_EG_modulator SHR 4 AND 1;

                               If NOT songdata.dis_fmreg_col[fmreg_table][11] then
                                 fmpar_table[chan].sustM := fm_data.AM_VIB_EG_modulator SHR 5 AND 1;

                               If NOT songdata.dis_fmreg_col[fmreg_table][12] then
                                 fmpar_table[chan].adsrw_car.attck := fm_data.ATTCK_DEC_carrier SHR 4;

                               If NOT songdata.dis_fmreg_col[fmreg_table][13] then
                                 fmpar_table[chan].adsrw_car.dec := fm_data.ATTCK_DEC_carrier AND $0f;

                               If NOT songdata.dis_fmreg_col[fmreg_table][14] then
                                 fmpar_table[chan].adsrw_car.sustn := fm_data.SUSTN_REL_carrier SHR 4;

                               If NOT songdata.dis_fmreg_col[fmreg_table][15] then
                                 fmpar_table[chan].adsrw_car.rel := fm_data.SUSTN_REL_carrier AND $0f;

                               If NOT songdata.dis_fmreg_col[fmreg_table][16] then
                                 fmpar_table[chan].adsrw_car.wform := fm_data.WAVEFORM_carrier AND $07;

                               If NOT songdata.dis_fmreg_col[fmreg_table][18] then
                                 fmpar_table[chan].kslC := fm_data.KSL_VOLUM_carrier SHR 6;

                               If NOT songdata.dis_fmreg_col[fmreg_table][19] then
                                 fmpar_table[chan].multipC := fm_data.AM_VIB_EG_carrier AND $0f;

                               If NOT songdata.dis_fmreg_col[fmreg_table][20] then
                                 fmpar_table[chan].tremC := fm_data.AM_VIB_EG_carrier SHR 7;

                               If NOT songdata.dis_fmreg_col[fmreg_table][21] then
                                 fmpar_table[chan].vibrC := fm_data.AM_VIB_EG_carrier SHR 6 AND 1;

                               If NOT songdata.dis_fmreg_col[fmreg_table][22] then
                                 fmpar_table[chan].ksrC := fm_data.AM_VIB_EG_carrier SHR 4 AND 1;

                               If NOT songdata.dis_fmreg_col[fmreg_table][23] then
                                 fmpar_table[chan].sustC := fm_data.AM_VIB_EG_carrier SHR 5 AND 1;

                               If NOT songdata.dis_fmreg_col[fmreg_table][24] then
                                 fmpar_table[chan].connect := fm_data.FEEDBACK_FM AND 1;

                               If NOT songdata.dis_fmreg_col[fmreg_table][25] then
                                 fmpar_table[chan].feedb := fm_data.FEEDBACK_FM SHR 1 AND 7;

                               If NOT songdata.dis_fmreg_col[fmreg_table][27] then
                                 If NOT pan_lock[chan] then
                                   panning_table[chan] := panning;

                               If NOT songdata.dis_fmreg_col[fmreg_table][5] then
                                 set_ins_volume(63-fm_data.KSL_VOLUM_modulator AND $3f,
                                                BYTE_NULL,chan);

                               If NOT songdata.dis_fmreg_col[fmreg_table][17] then
                                 set_ins_volume(BYTE_NULL,
                                                63-fm_data.KSL_VOLUM_carrier AND $3f,chan);

                               update_modulator_adsrw(chan);
                               update_carrier_adsrw(chan);
                               update_fmpar(chan);

                               If force_macro_keyon or
                                  NOT (fm_data.FEEDBACK_FM OR MACRO_NOTE_RETRIG_FLAG <> fm_data.FEEDBACK_FM) then
                                 begin
                                   If NOT (is_4op_chan(chan) and (chan in _4op_tracks_hi)) then
                                     begin
                                       output_note(event_table[chan].note,
                                                   event_table[chan].instr_def,chan,FALSE,TRUE);
                                       If (is_4op_chan(chan) and (chan in _4op_tracks_lo)) then
                                         init_macro_table(PRED(chan),0,voice_table[PRED(chan)],0);
                                     end;
                                 end
                               else
                                 If NOT (fm_data.FEEDBACK_FM OR MACRO_ENVELOPE_RESTART_FLAG <> fm_data.FEEDBACK_FM) then
                                   begin
                                     key_on(chan);
                                     change_freq(chan,freq_table[chan]);
                                   end
                                 else
                                   If NOT (fm_data.FEEDBACK_FM OR MACRO_ZERO_FREQ_FLAG <> fm_data.FEEDBACK_FM) then
                                     If (freq_table[chan] <> 0) then
                                       begin
                                         zero_fq_table[chan] := freq_table[chan];
                                         freq_table[chan] := freq_table[chan] AND NOT $1fff;
                                         change_freq(chan,freq_table[chan]);
                                       end
                                     else
                                   else
                                     If (zero_fq_table[chan] <> 0) then
                                       begin
                                         freq_table[chan] := zero_fq_table[chan];
                                         zero_fq_table[chan] := 0;
                                         change_freq(chan,freq_table[chan]);
                                       end;

                               If NOT songdata.dis_fmreg_col[fmreg_table][26] then
                                 If (freq_slide > 0) then
                                   portamento_up(chan,freq_slide,nFreq(12*8+1))
                                 else If (freq_slide < 0) then
                                        portamento_down(chan,Abs(freq_slide),nFreq(0));
                             end;
                       end;
                   end;

          With songdata.macro_table[arpg_table].arpeggio do
            If (arpg_table <> 0) and (speed <> 0) then
              If (arpg_count = speed) then
                begin
                  arpg_count := 1;
                  If (arpg_pos <= length) then
                    If (loop_begin <> 0) and (loop_length <> 0) then
                      If (arpg_pos = loop_begin+PRED(loop_length)) then
                        arpg_pos := loop_begin
                      else If (arpg_pos < length) then Inc(arpg_pos)
                           else arpg_pos := finished_flag
                    else If (arpg_pos < length) then Inc(arpg_pos)
                         else arpg_pos := finished_flag
                  else arpg_pos := finished_flag;

                  If (freq_table[chan] OR $2000 = freq_table[chan]) and
                     (keyoff_pos <> 0) and
                     (arpg_pos >= keyoff_pos) then
                    arpg_pos := IDLE
                  else If (freq_table[chan] OR $2000 <> freq_table[chan]) and
                          (keyoff_pos <> 0) and (keyoff_pos <> 0) and
                          ((arpg_pos < keyoff_pos) or (arpg_pos = IDLE)) then
                         arpg_pos := keyoff_pos;

                  If (arpg_pos <> 0) and
                     (arpg_pos <> IDLE) and (arpg_pos <> finished_flag) then
                    Case data[arpg_pos] of
                      0: change_frequency(chan,
                           nFreq(arpg_note-1)+
                           SHORTINT(ins_parameter(event_table[chan].instr_def,12)));

                      1..96:
                        change_frequency(chan,
                          nFreq(max(arpg_note+data[arpg_pos],97)-1)+
                          SHORTINT(ins_parameter(event_table[chan].instr_def,12)));

                      $80..$80+12*8+1:
                        change_frequency(chan,nFreq(data[arpg_pos]-$80-1)+
                          SHORTINT(ins_parameter(event_table[chan].instr_def,12)));
                    end;
                end
              else Inc(arpg_count);

          With songdata.macro_table[vib_table].vibrato do
            If NOT vib_paused and
               (vib_table <> 0) and (speed <> 0) then
              If (vib_count = speed) then
                If (vib_delay <> 0) then Dec(vib_delay)
                else begin
                       vib_count := 1;
                       If (vib_pos <= length) then
                         If (loop_begin <> 0) and (loop_length <> 0) then
                           If (vib_pos = loop_begin+PRED(loop_length)) then
                             vib_pos := loop_begin
                           else If (vib_pos < length) then Inc(vib_pos)
                                else vib_pos := finished_flag
                         else If (vib_pos < length) then Inc(vib_pos)
                              else vib_pos := finished_flag
                       else vib_pos := finished_flag;

                       If (freq_table[chan] OR $2000 = freq_table[chan]) and
                          (keyoff_pos <> 0) and
                          (vib_pos >= keyoff_pos) then
                         vib_pos := IDLE
                       else If (freq_table[chan] OR $2000 <> freq_table[chan]) and
                               (vib_pos <> 0) and (keyoff_pos <> 0) and
                               ((vib_pos < keyoff_pos) or (vib_pos = IDLE)) then
                              vib_pos := keyoff_pos;

                       If (vib_pos <> 0) and
                          (vib_pos <> IDLE) and (vib_pos <> finished_flag) then
                         If (data[vib_pos] > 0) then
                           macro_vibrato__porta_up(chan,data[vib_pos])
                         else If (data[vib_pos] < 0) then
                                macro_vibrato__porta_down(chan,Abs(data[vib_pos]))
                              else change_freq(chan,vib_freq);
                     end
              else Inc(vib_count);
        end;
    end;

  _dbg_leave; //EXIT //macro_poll_proc
end;

procedure timer_poll_proc; cdecl;
public name PUBLIC_PREFIX + 'timer_poll_proc';
begin
  _dbg_enter ({$I %FILE%}, 'timer_poll_proc');

  If (timer_200hz_counter < (IRQ_freq+IRQ_freq_shift+playback_speed_shift) DIV 200) then
    Inc(timer_200hz_counter)
  else begin
         timer_200hz_counter := 0;
         timer_200hz_flag := TRUE;
       end;

  If (timer_50hz_counter < (IRQ_freq+IRQ_freq_shift+playback_speed_shift) DIV 50) then
    Inc(timer_50hz_counter)
  else begin
         timer_50hz_counter := 0;
         timer_50hz_flag := TRUE;
       end;

  If (timer_20hz_counter < (IRQ_freq+IRQ_freq_shift+playback_speed_shift) DIV 20) then
    Inc(timer_20hz_counter)
  else begin
         timer_20hz_counter := 0;
         timer_20hz_flag := TRUE;
       end;

  If NOT replay_forbidden then
    begin
      If (current_order = 0) and (current_line = 0) and
         (tick0 = ticks) then
        begin
          song_timer := 0;
          timer_temp := 0;
          song_timer_tenths := 0;
        end;

      If (play_status = isPlaying) then
        begin
          song_timer_tenths := TRUNC(100/(IRQ_freq+IRQ_freq_shift+playback_speed_shift)*timer_temp);
          If (song_timer_tenths >= 100) then song_timer_tenths := 0;
          If (timer_temp < IRQ_freq+IRQ_freq_shift+playback_speed_shift) then Inc(timer_temp)
          else begin
                 Inc(song_timer);
                 timer_temp := 1;
               end;
        end;

      If (song_timer > 3600-1) then
        begin
          song_timer := 0;
          timer_temp := 0;
          song_timer_tenths := 0;
        end;

      If (ticklooper = 0) or fast_forward then
        poll_proc;

      If (macro_ticklooper = 0) then
        macro_poll_proc;

      Inc(ticklooper);
      If (ticklooper >= IRQ_freq DIV tempo) then
        ticklooper := 0;

      Inc(macro_ticklooper);
      If (macro_ticklooper >= IRQ_freq DIV (tempo*_macro_speedup)) then
        macro_ticklooper := 0;
    end;

  _dbg_leave; //EXIT //timer_poll_proc
end;

const
  ___IRQ_DATA_END___: Dword = 0;

procedure ___IRQ_CODE_END___; begin end;

procedure TimerSetup(Hz: Longint); cdecl; external;
procedure TimerDone; cdecl; external;

{$I adt2unit/pas/init_timer_proc.pas}
{$I adt2unit/pas/done_timer_proc.pas}
{$I adt2unit/pas/calc_pattern_pos.pas}

procedure init_buffers; cdecl; external;
procedure init_player; cdecl; external;

//stop_playing

procedure init_old_songdata;
begin
  _dbg_enter ({$I %FILE%}, 'init_old_songdata');

  FillChar(old_songdata,SizeOf(old_songdata),0);
  FillChar(old_songdata.pattern_order,SizeOf(old_songdata.pattern_order),$080);
  FillChar(old_songdata.instr_data,SizeOf(old_songdata.instr_data),0);

  _dbg_leave; //EXIT //init_old_songdata
end;

//init_songdata
//start_playing
//get_chunk
//put_chunk
//count_order

procedure init_a2player; cdecl; external;

var
  old_exit_proc: procedure;

procedure new_exit_proc;
begin
  Unlock_Data(___UNIT_DATA_START___,DWORD(Addr(___UNIT_DATA_END___))-DWORD(Addr(___UNIT_DATA_START___)));
  Unlock_Data(___IRQ_DATA_START___,DWORD(Addr(___IRQ_DATA_END___))-DWORD(Addr(___IRQ_DATA_START___)));
  Unlock_Code(@___IRQ_CODE_START___,DWORD(@___IRQ_CODE_END___)-DWORD(@___IRQ_CODE_START___));
  ExitProc := @old_exit_proc;
end;

begin
  init_a2player;
  Lock_Data(___UNIT_DATA_START___,DWORD(Addr(___UNIT_DATA_END___))-DWORD(Addr(___UNIT_DATA_START___)));
  Lock_Data(___IRQ_DATA_START___,DWORD(Addr(___IRQ_DATA_END___))-DWORD(Addr(___IRQ_DATA_START___)));
  Lock_Code(@___IRQ_CODE_START___,DWORD(@___IRQ_CODE_END___)-DWORD(@___IRQ_CODE_START___));
  @old_exit_proc := ExitProc;
  ExitProc := @new_exit_proc;
end.
