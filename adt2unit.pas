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

unit AdT2unit;
{$S-,Q-,R-,V-,B-,X+}
{$PACKRECORDS 1}
{$MODESWITCH CVAR}
{$L adt2unit.o}
interface

{$IFDEF GO32V2}
const
  ___UNIT_DATA_START___: Dword = 0;
{$ENDIF} // DEFINED(GO32V2)

const
  MIN_IRQ_FREQ = 50;
  MAX_IRQ_FREQ = 1000;

{$i typcons1.inc}
{$i typcons2.inc}

var
  IRQ_freq:          Word; cvar; external;
  IRQ_freq_shift:    Integer; cvar; external;
  tempo:             Byte; cvar; external;
  speed:             Byte; cvar; external;
  macro_speedup:     Word; cvar; external;
  timer_initialized: Boolean; cvar; external;
  repeat_pattern:    Boolean; cvar; external;
  fast_forward:      Boolean; cvar; external;
  _rewind:           Boolean; cvar; external;
  pattern_break:     Boolean; cvar; external;
  pattern_delay:     Boolean; cvar; external;
  next_line:         Byte; cvar; external;
  start_order:       Byte; cvar; external;
  start_pattern:     Byte; cvar; external;
  start_line:        Byte; cvar; external;
  replay_forbidden:  Boolean; cvar; external;
  single_play:       Boolean; cvar; external;
  calibrating:       Boolean; cvar; external;
  no_status_refresh: Boolean; cvar; external;
  do_synchronize:    Boolean; cvar; external;
  space_pressed:     Boolean; cvar; external;
  module_archived:   Boolean; cvar; external;
  force_scrollbars:  Boolean; cvar; external;
  no_sync_playing:   Boolean; cvar; external;
  no_step_debugging: Boolean; cvar; external;
  play_single_patt:  Boolean; cvar; external;
  no_trace_pattord:  Boolean; cvar; external;
  skip_macro_flag:   Boolean; cvar; external;
  max_patterns:      Byte; cvar; external;
  jump_mark_mode:    Boolean; cvar; external;
  force_macro_keyon: Boolean; cvar; external;
  ins_trailing_flag: Boolean; cvar; external;

  def_vibtrem_speed_factor: Byte; cvar; external;
  def_vibtrem_table_size: Byte; cvar; external;
  def_vibtrem_table: array[0..255] of Byte; cvar; external;

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

  macro_preview_indic_proc: procedure(state: Byte); cdecl; cvar; external;

  seconds_counter: Longint; cvar; external;
  hundereds_counter: Longint; cvar; external;
  really_no_status_refresh: Boolean; cvar; external;

const
  keyoff_flag        = $080;
  fixed_note_flag    = $090;
  pattern_loop_flag  = $0e0;
  pattern_break_flag = $0f0;

var
  fmpar_table:   array[1..20] of tFM_PARAMETER_TABLE; cvar; external;
  volume_lock:   array[1..20] of Boolean; cvar; external;
  vol4op_lock:   array[1..20] of Boolean; cvar; external;
  volume_table:  array[1..20] of Word; cvar; external;
  vscale_table:  array[1..20] of Word; cvar; external;
  peak_lock:     array[1..20] of Boolean; cvar; external;
  pan_lock:      array[1..20] of Boolean; cvar; external;
  modulator_vol: array[1..20] of Byte; cvar; external;
  carrier_vol:   array[1..20] of Byte; cvar; external;
{$IFNDEF ADT2PLAY}
  decay_bar:     array[1..20] of tDECAY_BAR; cvar; external;
  volum_bar:     array[1..20] of tVOLUM_BAR; cvar; external;
{$ENDIF} // NOT DEFINED(ADT2PLAY)
  channel_flag:  array[1..20] of Boolean; cvar; external;
  event_table:   array[1..20] of tCHUNK; cvar; external;
  voice_table:   array[1..20] of Byte; cvar; external;
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
  freqtable2:    array[1..20] of Word; cvar; external;
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
  ai_table:      array[1..255] of Byte; cvar; external;

  overall_volume: Byte; cvar; external;
  global_volume: Byte; cvar; external;
  fade_out_volume: Byte; cvar; external;
  playback_speed_shift: Longint; cvar; external;
  play_status: tPLAY_STATUS; cvar; external;
  chan_pos: Byte; cvar; external;
  chpos: Byte; cvar; external;
  transpos: Byte; cvar; external;
  track_chan_start: Byte; cvar; external;
  nm_track_chan: Byte; cvar; external;
  play_pos_buf: array[1..9] of Word; cvar; external;
  rec_correction: Byte; cvar; external;

const
  MACRO_NOTE_RETRIG_FLAG = $80;
  MACRO_ENVELOPE_RESTART_FLAG = $40;
  MACRO_ZERO_FREQ_FLAG = $20;

var
  current_order: Byte; cvar; external;
  current_pattern: Byte; cvar; external;
  current_line: Byte; cvar; external;
  current_tremolo_depth: Byte; cvar; external;
  current_vibrato_depth: Byte; cvar; external;
  current_inst: Byte; cvar; external;
  current_octave: Byte; cvar; external;

  adt2_title: array[0..36] of String[18]; cvar; external;

  songdata_source: String; cvar; external;
  instdata_source: String; cvar; external;
  songdata_title:  String; cvar; external;

  songdata_crc: Longint; cvar; external;
  songdata_crc_ord: Longint; cvar; external;
  temp_instrument: temp_instrument_t; cvar; external;
  pattord_page: Byte; cvar; external;
  pattord_hpos: Byte; cvar; external;
  pattord_vpos: Byte; cvar; external;
  instrum_page: Byte; cvar; external;
  pattern_patt: Byte; cvar; external;
  pattern_page: Byte; cvar; external;
  pattern_hpos: Byte; cvar; external;
  limit_exceeded: Boolean; cvar; external;
  load_flag: Byte; cvar; external;
  reset_chan: array[1..20] of Boolean; cvar; external;
  reset_adsrw: array[1..20] of Boolean; cvar; external;
  ignore_note_once: array[1..20] of Boolean; cvar; external;
  track_notes_ins: Boolean; cvar; external;
  seek_pattern_break: Boolean; cvar; external;

  speed_update: Boolean; cvar; external;
  lockvol: Boolean; cvar; external;
  panlock: Boolean; cvar; external;
  lockVP: Boolean; cvar; external;
  tremolo_depth: Byte; cvar; external;
  vibrato_depth: Byte; cvar; external;
  volume_scaling: Boolean; cvar; external;
  percussion_mode: Boolean; cvar; external;
  last_order: Byte; cvar; external;

  buf1: tGENERIC_IO_BUFFER; cvar; external;
  buf2: tGENERIC_IO_BUFFER; cvar; external;
  buf3: array[WORD] of Byte; cvar; external;
  buf4: array[WORD] of Byte; cvar; external;

  pattdata: ^tPATTERN_DATA; cvar; external;

  old_hash_buffer: tOLD_VARIABLE_DATA1; cvar; external;
  hash_buffer: tOLD_VARIABLE_DATA2; cvar; external;
  old_songdata: tOLD_FIXED_SONGDATA; cvar; external;
  dos_memavail: Word; cvar; external;

  songdata:      tFIXED_SONGDATA; cvar; external;
  songdata_bak:  tFIXED_SONGDATA; cvar; external;
  temp_songdata: tFIXED_SONGDATA; cvar; external;
  clipboard:     tCLIPBOARD; cvar; external;
{
  ptr_songdata:      Pointer; cvar; external;
  ptr_songdata_bak:  Pointer; cvar; external;
  ptr_temp_songdata: Pointer; cvar; external;
  ptr_clipboard:     Pointer; cvar; external;
}
  song_timer: Longint; cvar; external;
  timer_temp: Longint; cvar; external;
  song_timer_tenths: Longint; cvar; external;
  ticks: Longint; cvar; external;
  tick0: Longint; cvar; external;
  tickD: Longint; cvar; external;
  tickXF: Longint; cvar; external;
  time_playing: Double; cvar; external;
  time_playing0: Double; cvar; external;

{$IFDEF GO32V2}
  timer_determinator: Longint; cvar; external;
  timer_det2: Longint; cvar; external;
  scr_scroll_x: Word; cvar; external;
  old_scr_scroll_x: Word; cvar; external;
  scr_scroll_y: Word; cvar; external;
  old_scr_scroll_y: Word; cvar; external;
{$ELSE} // NOT DEFINED(GO32V2)
  screen_scroll_offset: Word; cvar; external;
{$ENDIF} // NOT DEFINED(GO32V2)

  common_flag_backup: Byte; cvar; external;
  volume_scaling_backup: Boolean; cvar; external;
  event_table_backup: array[1..20] of tCHUNK; cvar; external;
  freq_table_backup: array[1..20] of Word; cvar; external;
  freqtable2_backup: array[1..20] of Word; cvar; external;
  keyoff_loop_backup: array[1..20] of Boolean; cvar; external;
  channel_flag_backup: array[1..20] of Boolean; cvar; external;
  fmpar_table_backup: array[1..20] of tFM_PARAMETER_TABLE; cvar; external;
  volume_table_backup: array[1..20] of Word; cvar; external;
  pan_lock_backup: array[1..20] of Boolean; cvar; external;
  volume_lock_backup: array[1..20] of Boolean; cvar; external;
  peak_lock_backup: array[1..20] of Boolean; cvar; external;
  panning_table_backup: array[1..20] of Byte; cvar; external;
  voice_table_backup: array[1..20] of Byte; cvar; external;
  flag_4op_backup: Byte; cvar; external;
  status_backup: Record
                   replay_forbidden: Boolean;
                   play_status: tPLAY_STATUS;
                 end; cvar; external;

function  nFreq(note: Byte): Word; cdecl; external;
procedure change_freq(chan: Byte; freq: Word); cdecl; external;
function  calc_pattern_pos(pattern: Byte): Byte; cdecl; external;
function  concw(lo,hi: Byte): Word; cdecl; external;
function  ins_parameter(ins,param: Byte): Byte; cdecl; external;
function  is_chan_adsr_data_empty(chan: Byte): Boolean; cdecl; external;
function  is_ins_adsr_data_empty(ins: Byte): Boolean; cdecl; external;
function  scale_volume(volume,scale_factor: Byte): Byte; cdecl; external;
function  _macro_speedup: Word; cdecl; external;
procedure calibrate_player(order,line: Byte; status_filter: Boolean;
                           line_dependent: Boolean); cdecl; external;
procedure update_timer(Hz: Word); cdecl; external;
procedure key_on(chan: Byte); cdecl; external;
procedure key_off(chan: Byte); cdecl; external;
procedure release_sustaining_sound(chan: Byte); cdecl; external;
procedure init_macro_table(chan,note,ins: Byte; freq: Word); cdecl; external;
procedure set_ins_volume(modulator,carrier,chan: Byte); cdecl; external;
procedure update_modulator_adsrw(chan: Byte); cdecl; external;
procedure update_carrier_adsrw(chan: Byte); cdecl; external;
procedure update_fmpar(chan: Byte); cdecl; external;
procedure reset_chan_data(chan: Byte); cdecl; external;
procedure poll_proc; cdecl; external;
procedure macro_poll_proc; cdecl; external;
procedure init_buffers; cdecl; external;
procedure init_player; cdecl; external;
procedure reset_player; cdecl; external;
procedure start_playing; cdecl; external;
procedure stop_playing; cdecl; external;
procedure update_song_position; cdecl; external;
procedure change_frequency(chan: Byte; freq: Word); cdecl; external;
procedure set_global_volume; cdecl; external;
procedure set_ins_data(ins,chan: Byte); cdecl; external;
procedure init_timer_proc; cdecl; external;
procedure done_timer_proc; cdecl; external;
procedure realtime_gfx_poll_proc; cdecl; external;
procedure decay_bars_refresh; cdecl; external;
procedure status_refresh; cdecl; external;
procedure trace_update_proc; cdecl; external;

procedure get_chunk(pattern,line,channel: Byte; var chunk: tCHUNK); cdecl; external;
procedure put_chunk(pattern,line,channel: Byte; var chunk: tCHUNK); cdecl; external;

function  get_chanpos(var data; channels,scancode: Byte): Byte; cdecl; external;
function  get_chanpos2(var data; channels,scancode: Byte): Byte; cdecl; external;
function  count_channel(hpos: Byte): Byte; cdecl; external;
function  count_pos(hpos: Byte): Byte; cdecl; external;
function  calc_max_speedup(tempo: Byte): Word; cdecl; external;
function  calc_bpm_speed(tempo,speed,rows_per_beat: Byte): Double; cdecl; external;
function  calc_realtime_bpm_speed(tempo,speed,rows_per_beat: Byte): Double; cdecl; external;
function  calc_order_jump: Integer; cdecl; external;
function  calc_following_order(order: Byte): Integer; cdecl; external;
function  is_4op_chan(chan: Byte): Boolean; cdecl; external;

procedure count_order(var entries: Byte); cdecl; external;
procedure count_patterns(var patterns: Byte); cdecl; external;
procedure count_instruments(var instruments: Byte); cdecl; external;
procedure init_old_songdata; cdecl; external;
procedure init_songdata; cdecl; external;
procedure update_instr_data(ins: Byte); cdecl; external;
procedure load_instrument(var data; chan: Byte); cdecl; external;
procedure output_note(note,ins,chan: Byte;
                      restart_macro,restart_adsr: Boolean); cdecl; external;

function  min(value: Longint; minimum: Longint): Longint; cdecl; external;
function  max(value: Longint; maximum: Longint): Longint; cdecl; external;
procedure TimerSetup(Hz: Longint); cdecl; external;

var
  block_xstart: Byte; cvar; external;
  block_ystart: Byte; cvar; external;

  block_x0: Byte; cvar; external;
  block_y0: Byte; cvar; external;
  block_x1: Byte; cvar; external;
  block_y1: Byte; cvar; external;

function  is_in_block(x0,y0,x1,y1: Byte): Boolean; cdecl; external;
procedure fade_out_playback(fade_screen: Boolean); cdecl; external;

var
  ticklooper: Longint; cvar; external;
  macro_ticklooper: Longint; cvar; external;

const
{$IFDEF GO32V2}
  MAX_NUM_BANK_POSITIONS = 1000;
{$ELSE} // NOT DEFINED(GO32V2)
  MAX_NUM_BANK_POSITIONS = 500;
{$ENDIF} // NOT DEFINED(GO32V2)

var
  bank_position_list_size: Longint; cvar; external;
  bank_position_list:
    array[1..MAX_NUM_BANK_POSITIONS] of Record
{$IFDEF GO32V2}
                                          bank_name: String[50];
{$ELSE} // NOT DEFINED(GO32V2)
                                          bank_name: String;
{$ENDIF} // NOT DEFINED(GO32V2)
                                          bank_size: Longint;
                                          bank_position: Longint;
                                        end; cvar; external;

function  get_bank_position(bank_name: String; bank_size: Longint): Longint; cdecl; external;
procedure add_bank_position(bank_name: String; bank_size: Longint; bank_position: Longint); cdecl; external;

{$IFDEF GO32V2}
const
  ___UNIT_DATA_END___: Dword = 0;
{$ENDIF} // DEFINED(GO32V2)

procedure init_adt2unit; cdecl; external;

implementation

uses
  debug,
  pascal,
{$IFDEF GO32V2}
  CRT,
  DOS,
  GO32,
  PIT,
  ISS_Tim,
{$ELSE} // NOT DEFINED(GO32V2)
  DOS,
  SDL_Types,
  SDL_Timer,
{$ENDIF} // NOT DEFINED(GO32V2)
  AdT2opl3,
  AdT2sys,
  AdT2extn,
  AdT2ext2,
  AdT2keyb,
  font,
  TxtScrIO,
  StringIO,
  DialogIO,
  ParserIO;

{$IFDEF GO32V2}

const
  ___IRQ_DATA_START___: Dword = 0;

procedure ___IRQ_CODE_START___; begin end;

{$ENDIF} // DEFINED(GO32V2)

const
  FreqStart = $156;
  FreqEnd   = $2ae;
  FreqRange = FreqEnd-FreqStart;

//nFreq
function calc_freq_shift_up(freq,shift: Word): Word; cdecl; external;
function calc_freq_shift_down(freq,shift: Word): Word; cdecl; external;
function calc_vibtrem_shift(chan: Byte; var table_data): Word; cdecl; external;
function is_data_empty(var buf; size: Longint): Boolean; cdecl; external;
//min
//max
//concw
procedure synchronize_song_timer; cdecl; external;
//change_frequency
//update_timer
procedure update_playback_speed(speed_shift: Longint); cdecl; external;
//key_on
//key_off
//release_sustaining_sound
//scale_volume
function _4op_data_flag(chan: Byte): Dword; cdecl; external;
function _4op_vol_valid_chan(chan: Byte): Boolean; cdecl; external;
//set_ins_volume
procedure set_ins_volume_4op(volume,chan: Byte); cdecl; external;
procedure reset_ins_volume(chan: Byte); cdecl; external;
//set_ins_data
//update_modulator_adsrw
//update_carrier_adsrw
//update_fmpar
//reset_chan_data
//init_macro_table
//output_note
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
//calc_order_jump
//update_song_position
//poll_proc
//macro_poll_proc
//set_global_volume
{$IFDEF GO32V2}
//update_mouse_position
{$ENDIF}
procedure synchronize_screen; cdecl; external;
//_macro_speedup
//timer_poll_proc

{$IFDEF GO32V2}

const
  ___IRQ_DATA_END___: Dword = 0;

procedure ___IRQ_CODE_END___; begin end;

//TimerSetup
procedure TimerDone; cdecl; external;
procedure TimerInstallHandler(handler: Pointer); cdecl; external;
procedure TimerRemoveHandler; cdecl; external;

{$ELSE} // NOT DEFIED(GO32V2)

var
  timer_handler: Procedure; cdecl; cvar; external;
  TimerID: SDL_TimerID; cvar; external;
  _interval: longint; cvar; external;

//TimerCallback
//TimerSetup
procedure TimerDone; cdecl; external;
procedure TimerInstallHandler(handler: Pointer); cdecl; external;
procedure TimerRemoveHandler; cdecl; external;

{$ENDIF} // NOT DEFIED(GO32V2)

//init_timer_proc
//done_timer_proc

{$i realtime.inc}

//calc_pattern_pos
//update_status
//calibrate_player
//init_buffers
//init_player
//reset_player
//start_playing
//stop_playing
//get_chunk
//put_chunk
//get_chanpos
//get_chanpos2
//count_channel
//count_pos
//count_order
//count_patterns
//count_instruments
//calc_max_speedup
//calc_bpm_speed
//calc_realtime_bpm_speed
//init_old_songdata
//init_songdata
//update_instr_data
//load_instrument
//is_in_block
//fade_out_playback
//realtime_gfx_poll_proc
//get_bank_position
//add_bank_position

{$IFDEF GO32V2}

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
  init_adt2unit;
  Lock_Data(___UNIT_DATA_START___,DWORD(Addr(___UNIT_DATA_END___))-DWORD(Addr(___UNIT_DATA_START___)));
  Lock_Data(___IRQ_DATA_START___,DWORD(Addr(___IRQ_DATA_END___))-DWORD(Addr(___IRQ_DATA_START___)));
  Lock_Code(@___IRQ_CODE_START___,DWORD(@___IRQ_CODE_END___)-DWORD(@___IRQ_CODE_START___));
  @old_exit_proc := ExitProc;
  ExitProc := @new_exit_proc;

{$ENDIF}

end.
