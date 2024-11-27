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
  temp_instrument: tADTRACK2_INS; cvar; external;
  temp_instrument2: tADTRACK2_INS; cvar; external;
  temp_instrument_macro: tREGISTER_TABLE; cvar; external;
  temp_instrument_macro2: tREGISTER_TABLE; cvar; external;
  temp_instrument_dis_fmreg_col: tDIS_FMREG_COL; cvar; external;
  temp_instrument_dis_fmreg_col2: tDIS_FMREG_COL; cvar; external;
  temp_ins_type: Byte; cvar; external;
  pattord_page: Byte; cvar; external;
  pattord_hpos: Byte; cvar; external;
  pattord_vpos: Byte; cvar; external;
  instrum_page: Byte; cvar; external;
  pattern_patt: Byte; cvar; external;
  pattern_page: Byte; cvar; external;
  pattern_hpos: Byte; cvar; external;
  limit_exceeded: Boolean; cvar; external;
  load_flag: Byte; cvar; external;
  load_flag_alt: Byte; cvar; external;
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
                           line_dependent: Boolean);
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
procedure realtime_gfx_poll_proc; cdecl;
procedure decay_bars_refresh; cdecl;
procedure status_refresh; cdecl;
procedure trace_update_proc;

procedure get_chunk(pattern,line,channel: Byte; var chunk: tCHUNK); cdecl; external;
procedure put_chunk(pattern,line,channel: Byte; var chunk: tCHUNK); cdecl; external;

function  get_chanpos(var data; channels,scancode: Byte): Byte;
function  get_chanpos2(var data; channels,scancode: Byte): Byte;
function  count_channel(hpos: Byte): Byte;
function  count_pos(hpos: Byte): Byte;
function  calc_max_speedup(tempo: Byte): Word;
function  calc_bpm_speed(tempo,speed,rows_per_beat: Byte): Real;
function  calc_realtime_bpm_speed(tempo,speed,rows_per_beat: Byte): Real;
function  calc_order_jump: Integer; cdecl; external;
function  calc_following_order(order: Byte): Integer; cdecl; external;
function  is_4op_chan(chan: Byte): Boolean; cdecl; external;

procedure count_order(var entries: Byte); cdecl; external;
procedure count_patterns(var patterns: Byte);
procedure count_instruments(var instruments: Byte);
procedure init_old_songdata; cdecl; external;
procedure init_songdata; cdecl; external;
procedure update_instr_data(ins: Byte);
procedure load_instrument(var data; chan: Byte);
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

function  is_in_block(x0,y0,x1,y1: Byte): Boolean;
procedure fade_out_playback(fade_screen: Boolean); cdecl;

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

function  get_bank_position(bank_name: String; bank_size: Longint): Longint;
procedure add_bank_position(bank_name: String; bank_size: Longint; bank_position: Longint);

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

procedure calibrate_player(order,line: Byte; status_filter: Boolean;
                           line_dependent: Boolean);

var
  temp_channel_flag: array[1..20] of Boolean;
  old_debugging,
  old_repeat_pattern: Boolean;
  jump_count,loop_count,
  temp,previous_order,previous_line: Byte;
  status_backup: Record
                   replay_forbidden: Boolean;
                   play_status: tPLAY_STATUS;
                 end;

procedure update_status;

var
  temp: Byte;

begin
  _dbg_enter ({$I %FILE%}, 'calibrate_player.update_status');

  temp := songdata.pattern_order[current_order];
  If NOT (temp <= $7f) then temp := 0;
  show_str(17,03,byte2hex(current_order),pattern_bckg+status_dynamic_txt);
  show_str(20,03,byte2hex(temp),pattern_bckg+status_dynamic_txt);
  show_str(17,04,'--',pattern_bckg+status_dynamic_txt);
{$IFNDEF GO32V2}
  _draw_screen_without_delay := TRUE;
{$ENDIF}
  draw_screen;

  _dbg_leave; //EXIT //calibrate_player.update_status
end;

var
  _pattern_page,_pattord_page,
  _pattord_hpos,_pattord_vpos: Byte;

begin
  _dbg_enter ({$I %FILE%}, 'calibrate_player');

  If (calc_following_order(0) = -1) then
    begin
      _dbg_leave; EXIT; //calibrate_player
    end;
  calibrating := TRUE;
  status_backup.replay_forbidden := replay_forbidden;
  status_backup.play_status := play_status;
  If status_filter then no_status_refresh := TRUE;

  nul_volume_bars;
  Move(channel_flag,temp_channel_flag,SizeOf(temp_channel_flag));
  FillChar(channel_flag,SizeOf(channel_flag),BYTE(FALSE));

  old_debugging := debugging;
  old_repeat_pattern := repeat_pattern;
  debugging := FALSE;
  repeat_pattern := FALSE;

  If (play_status = isStopped) or
     (order < current_order) or
     (order = calc_following_order(0)) then
    begin
      If NOT no_sync_playing then stop_playing
      else begin
             stop_playing;
             no_sync_playing := TRUE;
           end;

      init_player;
      speed := songdata.speed;
      macro_speedup := songdata.macro_speedup;
      update_timer(songdata.tempo);
      current_order := calc_following_order(0);
      current_pattern := songdata.pattern_order[current_order];
      current_line := 0;
      pattern_break := FALSE;
      pattern_delay := FALSE;
      last_order := 0;
      next_line := 0;
      song_timer := 0;
      timer_temp := 0;
      song_timer_tenths := 0;
      time_playing := 0;
      ticklooper := 0;
      macro_ticklooper := 0;
      ticks := 0;
      tick0 := 0;
    end;

  If NOT no_sync_playing then
    begin
      show_str(13,07,' --:--.- ',status_background+status_border);
{$IFNDEF GO32V2}
      _draw_screen_without_delay := TRUE;
{$ENDIF}
      draw_screen;
    end;

  previous_order := current_order;
  previous_line := current_line;
  jump_count := 0;
  loop_count := 0;
  replay_forbidden := TRUE;
  seek_pattern_break := FALSE;

  If NOT no_sync_playing then
    While (current_line <> line) or
          (current_order <> order) do
      begin
        If scankey(SC_ESCAPE) then BREAK;
        If NOT ((previous_order = current_order) and
                (previous_line >= current_line) and NOT (pattern_break and
                (next_line AND $0f0 = pattern_loop_flag))) then loop_count := 0
        else begin
               Inc(loop_count);
               If (loop_count > 15) then BREAK;
             end;

        If (current_order = order) and (current_line >= line) and
           NOT line_dependent then BREAK;

        previous_order := current_order;
        previous_line := current_line;
        fast_forward := TRUE;

        poll_proc;
        If (macro_ticklooper = 0) then
          macro_poll_proc;

        fast_forward := FALSE;
        Inc(ticklooper);
        If (ticklooper >= IRQ_freq DIV tempo) then
          ticklooper := 0;

        Inc(macro_ticklooper);
        If (macro_ticklooper >= IRQ_freq DIV (tempo*macro_speedup)) then
          macro_ticklooper := 0;

        If (previous_order <> current_order) then
          begin
            update_status;
            Inc(jump_count);
            If (jump_count > $7f) then BREAK;
          end;

        If seek_pattern_break then
          If (current_order > order) or
             ((current_order = order) and
              ((current_line <> 0) or (line = 0))) then BREAK
          else seek_pattern_break := FALSE;
        keyboard_reset_buffer;
      end
  else
    begin
      start_playing;
      current_order := order;
      current_pattern := songdata.pattern_order[order];
      current_line := line;
    end;

  fade_out_volume := 63;
  Move(temp_channel_flag,channel_flag,SizeOf(channel_flag));

  For temp := 1 to 20 do reset_chan_data(temp);
  If (status_backup.play_status <> isStopped) then
    begin
      replay_forbidden := status_backup.replay_forbidden;
      play_status := status_backup.play_status;
    end
  else begin
         replay_forbidden := FALSE;
         play_status := isPlaying;
       end;

  debugging := old_debugging;
  repeat_pattern := old_repeat_pattern;
  synchronize_song_timer;
  calibrating := FALSE;
  If status_filter then no_status_refresh := FALSE;

  _pattern_page := line;
  _pattord_page := 0;
  _pattord_hpos := 1;
  _pattord_vpos := 1;

  While (current_order <> _pattord_vpos+4*(_pattord_hpos+_pattord_page-1)-1) do
    If (_pattord_vpos < 4) then Inc(_pattord_vpos)
    else If (_pattord_hpos < MAX_ORDER_COLS) then begin Inc(_pattord_hpos); _pattord_vpos := 1; end
         else If (_pattord_page < 23-(MAX_ORDER_COLS-9)) then begin Inc(_pattord_page); _pattord_vpos := 1; end;

  If tracing then
    begin
      PATTERN_ORDER_page_refresh(_pattord_page);
      PATTERN_page_refresh(_pattern_page);
    end;

  keyboard_reset_buffer;

  _dbg_leave; //EXIT //calibrate_player
end;

//init_buffers
//init_player
//reset_player
//start_playing
//stop_playing
//get_chunk
//put_chunk

function get_chanpos(var data; channels,scancode: Byte): Byte;

var
  result: Byte;

begin
  asm
        xor     ebx,ebx
@@1:    mov     edi,[data]
        add     edi,ebx
        xor     ecx,ecx
        mov     cl,channels
        mov     al,scancode
        sub     ecx,ebx
        repnz   scasb
        jnz     @@2
        xor     eax,eax
        mov     al,channels
        sub     eax,ecx
        jmp     @@3
@@2:    xor     eax,eax
        jmp     @@5
@@3:    pusha
        push    eax
        call    is_4op_chan
        or      al,al
        jz      @@4
        popa
        inc     ebx
        jmp     @@1
@@4:    popa
@@5:    mov     result,al
  end;
  get_chanpos := result;
end;

function get_chanpos2(var data; channels,scancode: Byte): Byte;

var
  result: Byte;

begin
  asm
        mov     edi,[data]
        xor     ecx,ecx
        mov     cl,channels
        mov     al,scancode
        repnz   scasb
        jnz     @@1
        xor     eax,eax
        mov     al,channels
        sub     eax,ecx
        jmp     @@2
@@1:    xor     eax,eax
@@2:    mov     result,al
  end;
  get_chanpos2 := result;
end;

function count_channel(hpos: Byte): Byte;

var
  result: Byte;

begin
  asm
        mov     al,_pattedit_lastpos
        xor     ah,ah
        mov     bl,MAX_TRACKS
        div     bl
        mov     bl,al
        mov     al,hpos
        xor     ah,ah
        div     bl
        or      ah,ah
        jz      @@1
        add     al,chan_pos
        jmp     @@2
@@1:    add     al,chan_pos
        dec     al
@@2:    mov     result,al
  end;
  count_channel := result;
end;

function count_pos(hpos: Byte): Byte;

var
  result: Byte;

begin
  asm
        mov     al,_pattedit_lastpos
        xor     ah,ah
        mov     bl,MAX_TRACKS
        div     bl
        mov     bl,al
        mov     al,hpos
        xor     ah,ah
        div     bl
        mov     al,ah
        dec     al
        or      ah,ah
        jnz     @@1
        dec     bl
        mov     al,bl
@@1:    mov     result,al
  end;
  count_pos := result;
end;

//count_order

procedure count_patterns(var patterns: Byte);

var
  temp1: Byte;

begin
  _dbg_enter ({$I %FILE%}, 'count_patterns');

  patterns := 0;
  For temp1 := 0 to PRED(max_patterns) do
    begin
      realtime_gfx_poll_proc;
      If NOT is_data_empty(pattdata^[temp1 DIV 8][temp1 MOD 8],PATTERN_SIZE) then
        patterns := temp1+1;
    end;

  _dbg_leave; //EXIT //count_patterns
end;

procedure count_instruments(var instruments: Byte);
begin
  _dbg_enter ({$I %FILE%}, 'count_instruments');

  instruments := 255;
  While (instruments > 0) and
        Empty(songdata.instr_data[instruments],INSTRUMENT_SIZE) do
    Dec(instruments);

  _dbg_leave; //EXIT //count_instruments
end;

function calc_max_speedup(tempo: Byte): Word;

var
  temp: Longint;
  result: Word;

begin
  _dbg_enter ({$I %FILE%}, 'calc_max_speedup');

  result := MAX_IRQ_FREQ DIV tempo;
  Repeat
    If (tempo = 18) and timer_fix then temp := TRUNC((tempo+0.2)*20)
    else temp := 250;
    While (temp MOD (tempo*result) <> 0) do Inc(temp);
    If (temp <= MAX_IRQ_FREQ) then Inc(result);
  until NOT (temp <= MAX_IRQ_FREQ);
  calc_max_speedup := PRED(result);

  _dbg_leave; //EXIT //calc_max_speedup
end;

function calc_bpm_speed(tempo,speed,rows_per_beat: Byte): Real;
begin
{$IFDEF GO32V2}
  calc_bpm_speed := tempo*60/speed/rows_per_beat*(1+(IRQ_freq_shift/IRQ_freq)-0.02);
{$ELSE}
  calc_bpm_speed := tempo*60/speed/rows_per_beat*(1+(IRQ_freq_shift/IRQ_freq)-sdl_timer_slowdown/100);
{$ENDIF}
end;

function calc_realtime_bpm_speed(tempo,speed,rows_per_beat: Byte): Real;
begin
{$IFDEF GO32V2}
  calc_realtime_bpm_speed := tempo*60/speed/rows_per_beat*(1+((IRQ_freq_shift+playback_speed_shift)/IRQ_freq)-0.02);
{$ELSE}
  calc_realtime_bpm_speed := tempo*60/speed/rows_per_beat*(1+((IRQ_freq_shift+playback_speed_shift)/IRQ_freq)-sdl_timer_slowdown/100);
{$ENDIF}
end;

//init_old_songdata
//init_songdata

procedure update_instr_data(ins: Byte);

var
  temp: Byte;

begin
  For temp := 1 to 20 do
    If (voice_table[temp] = ins) then
      begin
        reset_chan[temp] := TRUE;
        set_ins_data(ins,temp);
        change_frequency(temp,nFreq(PRED(event_table[temp].note AND $7f))+
                              SHORTINT(ins_parameter(ins,12)));
      end;
end;

procedure load_instrument(var data; chan: Byte);
begin
  fmpar_table[chan].connect := pBYTE(@data)[10] AND 1;
  fmpar_table[chan].feedb   := pBYTE(@data)[10] SHR 1 AND 7;
  fmpar_table[chan].multipM := pBYTE(@data)[0]  AND $0f;
  fmpar_table[chan].kslM    := pBYTE(@data)[2]  SHR 6;
  fmpar_table[chan].tremM   := pBYTE(@data)[0]  SHR 7;
  fmpar_table[chan].vibrM   := pBYTE(@data)[0]  SHR 6 AND 1;
  fmpar_table[chan].ksrM    := pBYTE(@data)[0]  SHR 4 AND 1;
  fmpar_table[chan].sustM   := pBYTE(@data)[0]  SHR 5 AND 1;
  fmpar_table[chan].multipC := pBYTE(@data)[1]  AND $0f;
  fmpar_table[chan].kslC    := pBYTE(@data)[3]  SHR 6;
  fmpar_table[chan].tremC   := pBYTE(@data)[1]  SHR 7;
  fmpar_table[chan].vibrC   := pBYTE(@data)[1]  SHR 6 AND 1;
  fmpar_table[chan].ksrC    := pBYTE(@data)[1]  SHR 4 AND 1;
  fmpar_table[chan].sustC   := pBYTE(@data)[1]  SHR 5 AND 1;

  fmpar_table[chan].adsrw_car.attck := pBYTE(@data)[5] SHR 4;
  fmpar_table[chan].adsrw_mod.attck := pBYTE(@data)[4] SHR 4;
  fmpar_table[chan].adsrw_car.dec   := pBYTE(@data)[5] AND $0f;
  fmpar_table[chan].adsrw_mod.dec   := pBYTE(@data)[4] AND $0f;
  fmpar_table[chan].adsrw_car.sustn := pBYTE(@data)[7] SHR 4;
  fmpar_table[chan].adsrw_mod.sustn := pBYTE(@data)[6] SHR 4;
  fmpar_table[chan].adsrw_car.rel   := pBYTE(@data)[7] AND $0f;
  fmpar_table[chan].adsrw_mod.rel   := pBYTE(@data)[6] AND $0f;
  fmpar_table[chan].adsrw_car.wform := pBYTE(@data)[9] AND $07;
  fmpar_table[chan].adsrw_mod.wform := pBYTE(@data)[8] AND $07;

  panning_table[chan] := pBYTE(@data)[11] AND 3;
  volume_table[chan] := concw(pBYTE(@data)[2] AND $3f,
                              pBYTE(@data)[3] AND $3f);

  update_modulator_adsrw(chan);
  update_carrier_adsrw(chan);
  update_fmpar(chan);
end;

function is_in_block(x0,y0,x1,y1: Byte): Boolean;
begin
  block_x1 := x1;
  block_x0 := block_xstart;
  If (block_x0 > block_x1) then
    begin
      block_x1 := block_x0;
      block_x0 := x1;
    end;

  block_y1 := y1;
  block_y0 := block_ystart;
  If (block_y0 > block_y1) then
    begin
      block_y1 := block_y0;
      block_y0 := y1;
    end;

  is_in_block := (x0 >= block_x0) and (x0 <= block_x1) and
                 (y0 >= block_y0) and (y0 <= block_y1);
end;

procedure fade_out_playback(fade_screen: Boolean); cdecl;
public name PUBLIC_PREFIX + 'fade_out_playback';

var
{$IFDEF GO32V2}
  idx,idx2: Byte;
  fade_buf: tFADE_BUF;
{$ELSE}
  temp: Byte;
  temp2,temp3: Byte;
  factor: Byte;
{$ENDIF}

begin
  _dbg_enter ({$I %FILE%}, 'fade_out_playback');

{$IFDEF GO32V2}

  If fade_screen then
    begin
      draw_screen;
      fade_buf.action := first;
      fade_speed := 32;
      GetPalette(fade_buf.pal0,0,255);

      For idx := fade_speed downto 0 do
        begin
          For idx2 := 0 to 255 do
            begin
              fade_buf.pal1[idx2].r := fade_buf.pal0[idx2].r * idx DIV fade_speed;
              fade_buf.pal1[idx2].g := fade_buf.pal0[idx2].g * idx DIV fade_speed;
              fade_buf.pal1[idx2].b := fade_buf.pal0[idx2].b * idx DIV fade_speed;
            end;

          SetPalette(fade_buf.pal1,0,255);
          If (play_status <> isStopped) then
            begin
              fade_out_volume := idx*2;
              set_global_volume;
              CRT.Delay(fade_out_volume DIV 32);
            end
          else CRT.Delay(1);

          realtime_gfx_poll_proc;
          draw_screen;
          keyboard_reset_buffer;
        end;
      fade_buf.action := fadeIn;
    end
  else If (play_status <> isStopped) then
         For idx := 63 downto 0 do
           begin
             fade_out_volume := idx;
             set_global_volume;
             CRT.Delay(fade_out_volume DIV 32);
             realtime_gfx_poll_proc;
             keyboard_reset_buffer;
           end;
{$ELSE}

  If fade_screen then factor := 255
  else factor := 63;

  If (global_volume > 0) then temp2 := factor DIV global_volume
  else temp2 := 0;
  temp3 := 0;
  fade_out_volume := 63;

  If (play_status <> isStopped) then
    For temp := 1 to factor do
      begin
        Inc(temp3);
        If (temp3 > temp2) then
          begin
            temp3 := 0;
            Dec(fade_out_volume);
            set_global_volume;
            If fade_screen or (temp MOD 5 = 0) then
              begin
                _draw_screen_without_delay := TRUE;
                draw_screen;
                keyboard_reset_buffer;
              end;
          end;
        If fade_screen then
          begin
            vid_FadeOut;
            SDL_Delay(1);
          end;
      end
  else
    For fade_out_volume := 1 to 255 do
      If fade_screen then
        begin
          vid_FadeOut;
          SDL_Delay(1);
        end;

{$ENDIF}

  _dbg_leave; //EXIT //fade_out_playback
end;

procedure realtime_gfx_poll_proc; cdecl;
public name PUBLIC_PREFIX + 'realtime_gfx_poll_proc';
begin
  If _realtime_gfx_no_update then
    EXIT; //realtime_gfx_poll_proc

{$IFDEF GO32V2}

  If NOT reset_gfx_ticks and
     (gfx_ticks > ((IRQ_freq+IRQ_freq_shift+playback_speed_shift) DIV 100)*SUCC(fps_down_factor)) then
    begin
      Inc(blink_ticks);
      If (blink_ticks = 40) then
        begin
          blink_flag := NOT blink_flag;
          blink_ticks := 0;
        end;

      Inc(_NRECM_blink_ticks);
      If (_NRECM_blink_ticks = 40) then
        begin
          _NRECM_blink_flag := NOT _NRECM_blink_flag;
          _NRECM_blink_ticks := 0;
        end;

      Inc(_IRQFREQ_blink_ticks);
      If (_IRQFREQ_blink_ticks = 20) then
        begin
          _IRQFREQ_blink_flag := NOT _IRQFREQ_blink_flag;
          _IRQFREQ_blink_ticks := 0;
        end;

{$ENDIF}

      If blink_flag then
        begin
          If debugging and (play_status = isStopped) then status_layout[isStopped][9] := #9
          else status_layout[isStopped][9] := ' ';
          If NOT debugging then status_layout[isPlaying][9] := #16
          else status_layout[isPlaying][9] := #9;
          status_layout[isPaused][8] := #8;
          If (@macro_preview_indic_proc <> NIL) then
            macro_preview_indic_proc(1);
        end
      else
        begin
          status_layout[isPlaying][9] := ' ';
          status_layout[isPaused] [8] := ' ';
          status_layout[isStopped][9] := ' ';
          If (@macro_preview_indic_proc <> NIL) then
            macro_preview_indic_proc(2);
        end;

      If (Addr(_show_bpm_realtime_proc) <> NIL) then
        _show_bpm_realtime_proc;

      decay_bars_refresh;
{$IFNDEF GO32V2}
      If opl3_channel_recording_mode then update_recorded_channels;
{$ENDIF}
      If do_synchronize then synchronize_screen;
{$IFNDEF GO32V2}
      If (_name_scrl_pending_frames > 0) then Dec(_name_scrl_pending_frames);
      Inc(_cursor_blink_pending_frames);
{$ENDIF}
      status_refresh;
      STATUS_LINE_refresh;
{$IFDEF GO32V2}
      reset_gfx_ticks := TRUE;
{$ENDIF}
      trace_update_proc;
      If (@mn_environment.ext_proc_rt <> NIL) then
        mn_environment.ext_proc_rt;
{$IFDEF GO32V2}
    end;
{$ENDIF}

  //EXIT //realtime_gfx_poll_proc
end;

function get_bank_position(bank_name: String; bank_size: Longint): Longint;

var
  idx: Longint;
  result: Longint;

begin
  _dbg_enter ({$I %FILE%}, 'get_bank_position');

  result := 0;
  bank_name := CutStr(Upper_filename(bank_name));
  For idx := 1 to bank_position_list_size do
    If (bank_position_list[idx].bank_name = bank_name) and
       ((bank_position_list[idx].bank_size = bank_size) or
        (bank_size = -1)) then
      begin
        result := bank_position_list[idx].bank_position;
        BREAK;
      end;
  get_bank_position := result;

  _dbg_leave; //EXIT //get_bank_position
end;

procedure add_bank_position(bank_name: String; bank_size: Longint; bank_position: Longint);

var
  idx,idx2: Longint;
  found_flag: Boolean;

begin
  _dbg_enter ({$I %FILE%}, 'add_bank_position');

  found_flag := FALSE;
  bank_name := CutStr(Upper_filename(bank_name));
  For idx := 1 to bank_position_list_size do
    If (bank_position_list[idx].bank_name = bank_name) and
       ((bank_position_list[idx].bank_size = bank_size) or
        (bank_size = -1)) then
      begin
        found_flag := TRUE;
        idx2 := idx;
        BREAK;
      end;

  If found_flag then
    begin
      bank_position_list[idx2].bank_position := bank_position;
      _dbg_leave; EXIT; //add_bank_position
    end;

  If (bank_position_list_size < MAX_NUM_BANK_POSITIONS) then
    Inc(bank_position_list_size)
  else
    begin
      bank_position_list_size := MAX_NUM_BANK_POSITIONS;
      For idx := 1 to PRED(bank_position_list_size) do
        bank_position_list[idx] := bank_position_list[idx+1];
    end;

  bank_position_list[bank_position_list_size].bank_name := bank_name;
  bank_position_list[bank_position_list_size].bank_size := bank_size;
  bank_position_list[bank_position_list_size].bank_position := bank_position;

  _dbg_leave; //EXIT //add_bank_position
end;

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
