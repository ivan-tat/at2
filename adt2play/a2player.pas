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
procedure init_old_songdata; cdecl; external;
procedure init_songdata; cdecl; external;
procedure init_timer_proc; cdecl; external;
procedure done_timer_proc; cdecl; external;
procedure get_chunk(pattern,line,channel: Byte; var chunk: tCHUNK); cdecl; external;
procedure put_chunk(pattern,line,channel: Byte; var chunk: tCHUNK); cdecl; external;
procedure count_order(var entries: Byte); cdecl; external;
function  calc_following_order(order: Byte): Integer; cdecl; external;
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
function concw(lo,hi: Byte): Word; cdecl; external;
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
procedure update_fine_effects(chan: Byte); cdecl; external;
{$I adt2unit/pas/play_line.pas}
procedure portamento_up(chan: Byte; slide: Word; limit: Word); cdecl; external;
procedure portamento_down(chan: Byte; slide: Word; limit: Word); cdecl; external;
procedure macro_vibrato__porta_up(chan: Byte; depth: Byte); cdecl; external;
procedure macro_vibrato__porta_down(chan: Byte; depth: Byte); cdecl; external;
procedure tone_portamento(chan: Byte); cdecl; external;
procedure tone_portamento2(chan: Byte); cdecl; external;
//slide_carrier_volume_up
//slide_modulator_volume_up
procedure slide_volume_up(chan,slide: Byte); cdecl; external;
//slide_carrier_volume_down
//slide_modulator_volume_down
procedure slide_volume_down(chan,slide: Byte); cdecl; external;
procedure volume_slide(chan,up_speed,down_speed: Byte); cdecl; external;
procedure global_volume_slide(up_speed,down_speed: Byte); cdecl; external;
procedure arpeggio(chan: Byte); cdecl; external;
procedure arpeggio2(chan: Byte); cdecl; external;
procedure vibrato(chan: Byte); cdecl; external;
procedure vibrato2(chan: Byte); cdecl; external;
procedure tremolo(chan: Byte); cdecl; external;
procedure tremolo2(chan: Byte); cdecl; external;
procedure update_effects; cdecl; external;
//update_fine_effects
procedure update_extra_fine_effects; cdecl; external;
//calc_following_order
function calc_order_jump: Integer; cdecl; external;
procedure update_song_position; cdecl; external;
procedure poll_proc; cdecl; external;
//macro_poll_proc
//timer_poll_proc

const
  ___IRQ_DATA_END___: Dword = 0;

procedure ___IRQ_CODE_END___; begin end;

procedure TimerSetup(Hz: Longint); cdecl; external;
procedure TimerDone; cdecl; external;
//init_timer_proc
//done_timer_proc
function calc_pattern_pos(pattern: Byte): Byte; cdecl; external;

procedure init_buffers; cdecl; external;
procedure init_player; cdecl; external;
//stop_playing
//init_old_songdata
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
