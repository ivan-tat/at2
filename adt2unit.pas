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
  decay_bar:     array[1..20] of tDECAY_BAR; cvar; external;
  volum_bar:     array[1..20] of tVOLUM_BAR; cvar; external;
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
procedure init_macro_table(chan,note,ins: Byte; freq: Word); cdecl;
procedure set_ins_volume(modulator,carrier,chan: Byte); cdecl; external;
procedure update_modulator_adsrw(chan: Byte); cdecl; external;
procedure update_carrier_adsrw(chan: Byte); cdecl; external;
procedure update_fmpar(chan: Byte);
procedure reset_chan_data(chan: Byte);
procedure poll_proc; cdecl;
procedure macro_poll_proc; cdecl;
procedure init_buffers;
procedure init_player;
procedure reset_player;
procedure start_playing;
procedure stop_playing;
procedure update_song_position;
procedure change_frequency(chan: Byte; freq: Word); cdecl; external;
procedure set_global_volume;
procedure set_ins_data(ins,chan: Byte); cdecl; external;
procedure init_timer_proc; cdecl; external;
procedure done_timer_proc; cdecl; external;
procedure realtime_gfx_poll_proc; cdecl;
procedure decay_bars_refresh; cdecl;
procedure status_refresh; cdecl;
procedure trace_update_proc;

function  hscroll_bar(x,y: Byte; size: Byte; len1,len2,pos: Word;
                      atr1,atr2: Byte): Byte;
function  vscroll_bar(x,y: Byte; size: Byte; len1,len2,pos: Word;
                      atr1,atr2: Byte): Byte;

procedure centered_frame(var xstart,ystart: Byte; hsize,vsize: Byte;
                         name: String; atr1,atr2: Byte; border: String);

procedure get_chunk(pattern,line,channel: Byte; var chunk: tCHUNK);
procedure put_chunk(pattern,line,channel: Byte; chunk: tCHUNK);

function  get_chanpos(var data; channels,scancode: Byte): Byte;
function  get_chanpos2(var data; channels,scancode: Byte): Byte;
function  count_channel(hpos: Byte): Byte;
function  count_pos(hpos: Byte): Byte;
function  calc_max_speedup(tempo: Byte): Word;
function  calc_bpm_speed(tempo,speed,rows_per_beat: Byte): Real;
function  calc_realtime_bpm_speed(tempo,speed,rows_per_beat: Byte): Real;
function  calc_order_jump: Integer;
function  calc_following_order(order: Byte): Integer; cdecl;
function  is_4op_chan(chan: Byte): Boolean; cdecl; external;

procedure count_order(var entries: Byte);
procedure count_patterns(var patterns: Byte);
procedure count_instruments(var instruments: Byte);
procedure init_old_songdata;
procedure init_songdata;
procedure update_instr_data(ins: Byte);
procedure load_instrument(var data; chan: Byte);
procedure output_note(note,ins,chan: Byte;
                      restart_macro,restart_adsr: Boolean);

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
procedure fade_out_playback(fade_screen: Boolean);

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

procedure timer_poll_proc; cdecl; // TODO: remove when done (private)

implementation

uses
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
  pascal,
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

procedure update_fmpar(chan: Byte);
begin
  opl3out(_instr[00]+_chan_m[chan],fmpar_table[chan].multipM+
                                   fmpar_table[chan].ksrM  SHL 4+
                                   fmpar_table[chan].sustM SHL 5+
                                   fmpar_table[chan].vibrM SHL 6+
                                   fmpar_table[chan].tremM SHL 7);
  opl3out(_instr[01]+_chan_c[chan],fmpar_table[chan].multipC+
                                   fmpar_table[chan].ksrC  SHL 4+
                                   fmpar_table[chan].sustC SHL 5+
                                   fmpar_table[chan].vibrC SHL 6+
                                   fmpar_table[chan].tremC SHL 7);

  opl3out(_instr[10]+_chan_n[chan],(fmpar_table[chan].connect+
                                    fmpar_table[chan].feedb SHL 1) OR
                                   _panning[panning_table[chan]]);

  vscale_table[chan] := concw(fmpar_table[chan].kslM SHL 6,
                              fmpar_table[chan].kslC SHL 6);
  set_ins_volume(LO(volume_table[chan]),
                 HI(volume_table[chan]),chan);
end;

procedure reset_chan_data(chan: Byte);
begin
  If (percussion_mode and (chan in [17..20])) then
    If channel_flag[chan] then
      reset_ins_volume(chan)
    else set_ins_volume(0,0,chan)
  else
    begin
      opl3out(_instr[02]+_chan_m[chan],63);
      opl3out(_instr[03]+_chan_c[chan],63);
      key_on(chan);

      opl3out(_instr[04]+_chan_m[chan],BYTE_NULL);
      opl3out(_instr[05]+_chan_c[chan],BYTE_NULL);
      opl3out(_instr[06]+_chan_m[chan],BYTE_NULL);
      opl3out(_instr[07]+_chan_c[chan],BYTE_NULL);
      key_off(chan);
      update_fmpar(chan);
    end;

  reset_adsrw[chan] := TRUE;
  If (play_status <> isStopped) then
    If (event_table[chan].note AND $7f in [1..12*8+1]) then
      init_macro_table(chan,event_table[chan].note AND $7f,voice_table[chan],freq_table[chan])
    else init_macro_table(chan,0,voice_table[chan],freq_table[chan]);
end;

procedure init_macro_table(chan,note,ins: Byte; freq: Word); cdecl;
public name PUBLIC_PREFIX + 'init_macro_table';
begin
  macro_table[chan].fmreg_count := 1;
  macro_table[chan].fmreg_pos := 0;
  macro_table[chan].fmreg_duration := 0;
  macro_table[chan].fmreg_table := ins;
  macro_table[chan].arpg_count := 1;
  macro_table[chan].arpg_pos := 0;
  macro_table[chan].arpg_table := songdata.instr_macros[ins].arpeggio_table;
  macro_table[chan].arpg_note := note;
  macro_table[chan].vib_count := 1;
  macro_table[chan].vib_paused := FALSE;
  macro_table[chan].vib_pos := 0;
  macro_table[chan].vib_table := songdata.instr_macros[ins].vibrato_table;
  macro_table[chan].vib_freq := freq;
  macro_table[chan].vib_delay := songdata.macro_table[macro_table[chan].vib_table].vibrato.delay;
  zero_fq_table[chan] := 0;
end;

procedure output_note(note,ins,chan: Byte;
                      restart_macro,restart_adsr: Boolean);
var
  freq: Word;

begin
  If (note = 0) and (ftune_table[chan] = 0) then EXIT;
  If NOT (note in [1..12*8+1]) then freq := freq_table[chan]
  else begin
         freq := nFreq(note-1)+SHORTINT(ins_parameter(ins,12));
         If restart_adsr then key_on(chan);

         freq_table[chan] := concw(LO(freq_table[chan]),
                                   HI(freq_table[chan]) OR $20);

         If channel_flag[chan] then
           If is_4op_chan(chan) then
             begin
               If NOT (percussion_mode and (chan in [17..20])) then
                 If (ins_parameter(voice_table[chan],10) AND 1 = 1) then
                   If (volum_bar[chan].lvl < (carrier_vol[chan]+modulator_vol[chan]) DIV 2) then
                     volum_bar[chan].dir := 1
                   else
                 else If (volum_bar[chan].lvl < carrier_vol[chan]) then
                        volum_bar[chan].dir := 1
                      else
               else If (volum_bar[chan].lvl < modulator_vol[chan]) then
                      volum_bar[chan].dir := 1;

               If NOT (percussion_mode and (PRED(chan) in [17..20])) then
                 If (ins_parameter(voice_table[PRED(chan)],10) AND 1 = 1) then
                   If (volum_bar[PRED(chan)].lvl < (carrier_vol[PRED(chan)]+modulator_vol[PRED(chan)]) DIV 2) then
                     volum_bar[PRED(chan)].dir := 1
                   else
                 else If (volum_bar[PRED(chan)].lvl < carrier_vol[PRED(chan)]) then
                        volum_bar[PRED(chan)].dir := 1
                      else
               else If (volum_bar[PRED(chan)].lvl < modulator_vol[PRED(chan)]) then
                      volum_bar[PRED(chan)].dir := 1;

               If (decay_bar[chan].lvl1 < carrier_vol[chan]) then
                 decay_bar[chan].dir1 := 1;

               If (decay_bar[chan].lvl2 < modulator_vol[chan]) then
                 decay_bar[chan].dir2 := 1;

               If (decay_bar[PRED(chan)].lvl1 < carrier_vol[PRED(chan)]) then
                 decay_bar[PRED(chan)].dir1 := 1;

               If (decay_bar[PRED(chan)].lvl2 < modulator_vol[PRED(chan)]) then
                 decay_bar[PRED(chan)].dir2 := 1;

               If (play_status <> isPlaying) then
                 begin
                   volum_bar[chan].dir := -1;
                   decay_bar[chan].dir1 := -1;
                   decay_bar[chan].dir2 := -1;
                   decay_bar[PRED(chan)].dir1 := -1;
                   decay_bar[PRED(chan)].dir2 := -1;
                 end;
             end
           else begin
                  If NOT (percussion_mode and (chan in [17..20])) then
                    If (ins_parameter(voice_table[chan],10) AND 1 = 1) then
                      If (volum_bar[chan].lvl < (carrier_vol[chan]+modulator_vol[chan]) DIV 2) then
                        volum_bar[chan].dir := 1
                      else
                    else If (volum_bar[chan].lvl < carrier_vol[chan]) then
                           volum_bar[chan].dir := 1
                         else
                  else If (volum_bar[chan].lvl < modulator_vol[chan]) then
                         volum_bar[chan].dir := 1;

                  If (decay_bar[chan].lvl1 < carrier_vol[chan]) then
                    decay_bar[chan].dir1 := 1;

                  If (decay_bar[chan].lvl2 < modulator_vol[chan]) then
                    decay_bar[chan].dir2 := 1;

                  If (play_status <> isPlaying) then
                    begin
                      volum_bar[chan].dir := -1;
                      decay_bar[chan].dir1 := -1;
                      decay_bar[chan].dir2 := -1;
                    end;
                end;
       end;

  If (ftune_table[chan] = -127) then ftune_table[chan] := 0;
  freq := freq+ftune_table[chan];
  change_frequency(chan,freq);

  If (note <> 0) then
    begin
      event_table[chan].note := note;
      If is_4op_chan(chan) then
        event_table[PRED(chan)].note := note;
      If restart_macro then
        With event_table[chan] do
           If NOT (((effect_def = ef_Extended) and
                   (effect DIV 16 = ef_ex_ExtendedCmd2) and
                   (effect MOD 16 = ef_ex_cmd2_NoRestart)) or
                  ((effect_def2 = ef_Extended) and
                   (effect2 DIV 16 = ef_ex_ExtendedCmd2) and
                   (effect2 MOD 16 = ef_ex_cmd2_NoRestart))) then
             init_macro_table(chan,note,ins,freq)
           else macro_table[chan].arpg_note := note;
    end;
end;

procedure generate_custom_vibrato(value: Byte);

const
  vibtab_size: array[0..15] of Byte = (
    16,16,16,16,32,32,32,32,64,64,64,64,128,128,128,128);

var
  mul_r: Real;
  mul_b: Byte;
  idx,idx2: Byte;

function min0(value: Longint): Longint;
begin
  If (value >= 0) then min0 := value
  else min0 := 0;
end;

begin
  Case value of
    // set default speed table
    0: begin
         vibtrem_table_size := def_vibtrem_table_size;
         Move(def_vibtrem_table,vibtrem_table,SizeOf(vibtrem_table));
       end;

    // set custom speed table (fixed size = 32)
    1..239:
       begin
         vibtrem_table_size := def_vibtrem_table_size;
         mul_r := value/16;
         For idx2 := 0 to 7 do
          begin
            vibtrem_table[idx2*32] := 0;
            For idx := 1 to 16 do
              vibtrem_table[idx2*32+idx] := ROUND(idx*mul_r);
            For idx := 17 to 31 do
              vibtrem_table[idx2*32+idx] := ROUND((32-idx)*mul_r);
          end;
       end;

    // set custom speed table (speed factor = 1-4)
    240..255:
       begin
         vibtrem_speed_factor := SUCC((value-240) MOD 4);
         vibtrem_table_size := 2*vibtab_size[value-240];
         mul_b := 256 DIV (vibtab_size[value-240]);
         For idx2 := 0 to PRED(128 DIV vibtab_size[value-240]) do
           begin
             vibtrem_table[2*vibtab_size[value-240]*idx2] := 0;
             For idx := 1 to vibtab_size[value-240] do
               vibtrem_table[2*vibtab_size[value-240]*idx2+idx] :=
                 min0(idx*mul_b-1);
             For idx := vibtab_size[value-240]+1 to
                        2*vibtab_size[value-240]-1 do
               vibtrem_table[2*vibtab_size[value-240]*idx2+idx] :=
                 min0((2*vibtab_size[value-240]-idx)*mul_b-1);
           end;
       end;
  end;
end;

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
  If NOT (pattern_break and (next_line AND $0f0 = pattern_loop_flag)) and
         (current_order <> last_order) then
    begin
      FillChar(loopbck_table,SizeOf(loopbck_table),BYTE_NULL);
      FillChar(loop_table,SizeOf(loop_table),BYTE_NULL);
      last_order := current_order;
    end;

  For chan := 1 to songdata.nm_tracks do
    If channel_flag[chan] and reset_adsrw[chan] then
      begin
        update_modulator_adsrw(chan);
        update_carrier_adsrw(chan);
        reset_adsrw[chan] := FALSE;
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
            seek_pattern_break := TRUE;
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
            seek_pattern_break := TRUE;
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
        else
      else begin
             event_table[chan].effect_def := event[chan].effect_def;
             event_table[chan].effect := event[chan].effect;
           end;

      If (event[chan].effect_def2+event[chan].effect2 = 0) then
        If (glfsld_table2[chan] = 0) then effect_table2[chan] := 0
        else
      else begin
             event_table[chan].effect_def2 := event[chan].effect_def2;
             event_table[chan].effect2 := event[chan].effect2;
           end;

      // treat Tone Portamento based effects
      //   vs. note Key-Off's
      //   vs. step-playing with Spacebar in Pattern Editor window
      If (event[chan].note = event[chan].note OR keyoff_flag) then key_off(chan)
      else If NOT (LO(effect_table[chan]) in [ef_TonePortamento,
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
               If NOT ignore_note_once[chan] then
                 output_note(event[chan].note,voice_table[chan],chan,TRUE,TRUE)
               else
             else output_note(event[chan].note,voice_table[chan],chan,TRUE,FALSE)
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
end;

procedure portamento_up(chan: Byte; slide: Word; limit: Word);

var
  freq: Word;

begin
  If (freq_table[chan] AND $1fff = 0) then EXIT;
  freq := calc_freq_shift_up(freq_table[chan] AND $1fff,slide);
  If (freq <= limit) then change_frequency(chan,freq)
  else change_frequency(chan,limit);
end;

procedure portamento_down(chan: Byte; slide: Word; limit: Word);

var
  freq: Word;

begin
  If (freq_table[chan] AND $1fff = 0) then EXIT;
  freq := calc_freq_shift_down(freq_table[chan] AND $1fff,slide);
  If (freq >= limit) then change_frequency(chan,freq)
  else change_frequency(chan,limit);
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

function calc_following_order(order: Byte): Integer; cdecl;
public name PUBLIC_PREFIX + 'calc_following_order';

var
  result: Integer;
  index,jump_count: Byte;

begin
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
end;

function calc_order_jump: Integer;

var
  temp: Byte;
  result: Integer;

begin
  result := 0;
  temp := 0;

  Repeat
    If (songdata.pattern_order[current_order] > $7f) then
      current_order := songdata.pattern_order[current_order]-$80;
    Inc(temp);
  until (temp > $7f) or (songdata.pattern_order[current_order] < $80);

  If (temp > $7f) then begin stop_playing; result := -1; end;
  calc_order_jump := result;
end;

procedure update_song_position;

var
  temp: Byte;

begin
  For temp := 9 downto 2 do
    play_pos_buf[temp] := play_pos_buf[temp-1];
  play_pos_buf[1] := (current_pattern SHL 8)+current_line;

  If NOT _rewind then
    begin
      If (current_line < PRED(songdata.patt_len)) and NOT pattern_break then Inc(current_line)
      else begin
             If NOT (pattern_break and (next_line AND $0f0 = pattern_loop_flag)) and
                repeat_pattern then
               begin
                 FillChar(loopbck_table,SizeOf(loopbck_table),BYTE_NULL);
                 FillChar(loop_table,SizeOf(loop_table),BYTE_NULL);
                 current_line := 0;
                 pattern_break := FALSE;
               end
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

                    If NOT play_single_patt then
                      If (songdata.pattern_order[current_order] > $7f) then
                        If (calc_order_jump = -1) then EXIT;

                    If NOT play_single_patt then
                      current_pattern := songdata.pattern_order[current_order];

                    If NOT pattern_break then current_line := 0
                    else begin
                           pattern_break := FALSE;
                           current_line := next_line;
                         end;
                  end;
           end;
    end
  else
    If (current_line > 0) then Dec(current_line);

  For temp := 1 to songdata.nm_tracks do
    begin
      ignore_note_once[temp] := FALSE;
      glfsld_table[temp] := 0;
      glfsld_table2[temp] := 0;
    end;

  If NOT play_single_patt then
    If (current_line = 0) and
       (current_order = calc_following_order(0)) and speed_update then
      begin
        tempo := songdata.tempo;
        speed := songdata.speed;
        update_timer(tempo);
      end;
end;

procedure poll_proc; cdecl;
public name PUBLIC_PREFIX + 'poll_proc';

var
  temp: Byte;
  factor: Real;
{$IFDEF GO32V2}
  _debug_str_bak_: String;
{$ENDIF}

begin
{$IFDEF GO32V2}
  _debug_str_bak_ := _debug_str_;
  _debug_str_ := 'ADT2UNIT.PAS:_poll_proc';
{$ENDIF}

  If (NOT pattern_delay and (ticks-tick0+1 >= speed)) or
     fast_forward or _rewind or single_play then
    begin
      If debugging and
         NOT single_play and NOT pattern_break and
         (NOT space_pressed or no_step_debugging) then EXIT;

      If NOT single_play and
         NOT play_single_patt then
        begin
          If (songdata.pattern_order[current_order] > $7f) then
            If (calc_order_jump = -1) then EXIT;
          current_pattern := songdata.pattern_order[current_order];
        end;

      play_line;
      If NOT single_play and NOT (fast_forward or _rewind) then update_effects
      else For temp := 1 to speed do
             begin
               update_effects;
               If (temp MOD 4 = temp) then
                 update_extra_fine_effects;
               Inc(ticks);
             end;

      tick0 := ticks;
      If NOT single_play and (fast_forward or _rewind or
                              NOT pattern_delay) then
        update_song_position;

      If fast_forward or _rewind then
        If NOT pattern_delay then synchronize_song_timer;

      If (fast_forward or _rewind) and pattern_delay then
        begin
          tickD := 0;
          pattern_delay := FALSE;
        end;

      If fast_forward or _rewind then
        begin
{$IFDEF GO32V2}
          keyboard_reset_buffer;
{$ENDIF}
          If fast_forward then factor := 1/(5-fforward_factor+1)
          else factor := 1/(5-rewind_factor+1);
          If (Abs(time_playing-time_playing0) > factor*(1+(1/255*tempo))/speed) then
            begin
              fast_forward := FALSE;
              _rewind := FALSE;
              time_playing0 := time_playing;
              synchronize_song_timer;
            end;
        end;
    end
  else
    begin
      update_effects;
      Inc(ticks);

      If NOT (debugging and NOT single_play and (NOT space_pressed or no_step_debugging)) then
        If pattern_delay and (tickD > 1) then Dec(tickD)
        else begin
               If pattern_delay and NOT single_play then
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
{$IFDEF GO32V2}
  _debug_str_ := _debug_str_bak_;
{$ENDIF}
end;

procedure macro_poll_proc; cdecl;
public name PUBLIC_PREFIX + 'macro_poll_proc';

const
  IDLE = $0fff;
  FINISHED = $0ffff;

var
  chan: Byte;
  finished_flag: Word;

{$IFDEF GO32V2}
var
  _debug_str_bak_: String;
{$ENDIF}

begin
{$IFDEF GO32V2}
  _debug_str_bak_ := _debug_str_;
  _debug_str_ := 'ADT2UNIT.PAS:macro_poll_proc';
{$ENDIF}
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
{$IFDEF GO32V2}
  _debug_str_ := _debug_str_bak_;
{$ENDIF}
end;

procedure set_global_volume;

var
  chan: Byte;

begin
  For chan := 1 to songdata.nm_tracks do
    If _4op_vol_valid_chan(chan) then
      set_ins_volume_4op(BYTE_NULL,chan)
    else If NOT ((carrier_vol[chan] = 0) and
                 (modulator_vol[chan] = 0)) then
           If (ins_parameter(voice_table[chan],10) AND 1 = 0) then
             set_ins_volume(BYTE_NULL,HI(volume_table[chan]),chan)
           else set_ins_volume(LO(volume_table[chan]),HI(volume_table[chan]),chan);
end;

{$IFDEF GO32V2}

var
  regs: tRealRegs;

procedure update_mouse_position;
begin
  If mouse_active then
    begin
      FillChar(regs,SizeOf(regs),0);
      regs.ax := $04;
      regs.cx := scr_scroll_x;
      regs.dx := scr_scroll_y;
      RealIntr($33,regs);
    end;
end;

{$ENDIF}

procedure synchronize_screen;
begin
{$IFDEF GO32V2}
  _last_debug_str_ := _debug_str_;
  _debug_str_ := 'ADT2UNIT.PAS:synchronize_screen';

  If mouse_active then
    begin
      FillChar(regs,SizeOf(regs),0);
      regs.ax := $03;
      RealIntr($33,regs);
      scr_scroll_x := regs.cx;
      scr_scroll_y := regs.dx;
    end;

  If (scr_scroll_x <> old_scr_scroll_x) or
     (scr_scroll_y <> old_scr_scroll_y) then
    begin
      old_scr_scroll_x := scr_scroll_x;
      old_scr_scroll_y := scr_scroll_y;

      If (scr_scroll_x > scr_font_width*MaxCol-scr_font_width*hard_maxcol) then
        begin
          scr_scroll_x := scr_font_width*MaxCol-scr_font_width*hard_maxcol;
          update_mouse_position;
        end;

      If (scr_scroll_y > scr_font_height*MaxLn-scr_font_height*hard_maxln) then
        begin
          scr_scroll_y := scr_font_height*MaxLn-scr_font_height*hard_maxln;
          update_mouse_position;
        end;

      WaitRetrace;
      If NOT is_VESA_emulated_mode then
        SetTextDisp(scr_scroll_x,scr_scroll_y)
      else virtual_screen__first_row := scr_scroll_y*800;
    end;
{$ELSE}
  If NOT is_default_screen_mode then EXIT;
  If (screen_scroll_offset > 16*MaxLn-16*hard_maxln) then
    screen_scroll_offset := 16*MaxLn-16*hard_maxln;
  virtual_screen__first_row := screen_scroll_offset*SCREEN_RES_X;
{$ENDIF}
end;

//_macro_speedup

procedure timer_poll_proc; cdecl;
public name PUBLIC_PREFIX + 'timer_poll_proc';

{$IFDEF GO32V2}
var
  _debug_str_bak_: String;
{$ENDIF}

begin

{$IFDEF GO32V2}

  _debug_str_bak_ := _debug_str_;
  _debug_str_ := 'ADT2UNIT.PAS:timer_poll_proc';
  If (timer_determinator < IRQ_freq+IRQ_freq_shift+playback_speed_shift) then
    Inc(timer_determinator)
  else begin
         timer_determinator := 1;
         Inc(seconds_counter);
       end;

  If (timer_det2 < (IRQ_freq+IRQ_freq_shift+playback_speed_shift) DIV 100) then
    Inc(timer_det2)
  else begin
         timer_det2 := 1;
         Inc(hundereds_counter);
         Inc(_cursor_blink_pending_frames);
         Inc(_screen_refresh_pending_frames);
       end;

{$ENDIF}

{$IFDEF GO32V2}

  If NOT reset_gfx_ticks then Inc(gfx_ticks)
  else begin
         gfx_ticks := 0;
         reset_gfx_ticks := FALSE;
       end;

  scroll_ticks := scroll_ticks+1/(IRQ_freq+IRQ_freq_shift+playback_speed_shift)*100;
  If (scroll_ticks > 0.2) then
    begin
      If ctrl_tab_pressed then
        begin
          If scankey(SC_UP) then
            If (scr_scroll_y > 0) then
              begin
                Dec(scr_scroll_y);
                update_mouse_position;
              end;

          If scankey(SC_DOWN) then
            If (scr_scroll_y < scr_font_height*MaxLn-scr_font_height*hard_maxln) then
              begin
                Inc(scr_scroll_y);
                update_mouse_position;
              end;

          If scankey(SC_PAGEUP) then
            begin
              scr_scroll_y := 0;
              update_mouse_position;
            end;

          If scankey(SC_PAGEDOWN) then
            begin
              scr_scroll_y := scr_font_height*MaxLn-scr_font_height*hard_maxln;
              update_mouse_position;
            end;

          If scankey(SC_HOME) then
            begin
              scr_scroll_x := 0;
              update_mouse_position;
            end;

          If scankey(SC_END) then
            begin
              scr_scroll_x := scr_font_width*MaxCol-scr_font_width*hard_maxcol;
              update_mouse_position;
            end;

          If scankey(SC_LEFT) then
            If (scr_scroll_x > 0) then
              begin
                Dec(scr_scroll_x);
                update_mouse_position;
              end;

          If scankey(SC_RIGHT) then
            If (scr_scroll_x < scr_font_width*MaxCol-scr_font_width*hard_maxcol) then
              begin
                Inc(scr_scroll_x);
                update_mouse_position;
              end;
        end;
      scroll_ticks := 0;
    end;

{$ELSE}

  Inc(blink_ticks);
  If ((fast_forward or _rewind or (space_pressed and debugging) or
       (@macro_preview_indic_proc <> NIL)) and
      (blink_ticks > 50)) or (blink_ticks > 50) then
    begin
      blink_flag := NOT blink_flag;
      blink_ticks := 0;
    end;

  Inc(_WAVREC_blink_ticks);
  If ((fast_forward or _rewind or (space_pressed and debugging) or
       (@macro_preview_indic_proc <> NIL)) and
      (_WAVREC_blink_ticks > 50)) or (_WAVREC_blink_ticks > 50) then
    begin
      _WAVREC_blink_flag := NOT _WAVREC_blink_flag;
      _WAVREC_blink_ticks := 0;
    end;

  Inc(_NRECM_blink_ticks);
  If ((fast_forward or _rewind or (space_pressed and debugging) or
       (@macro_preview_indic_proc <> NIL)) and
      (_NRECM_blink_ticks > 50)) or (_NRECM_blink_ticks > 50) then
    begin
      _NRECM_blink_flag := NOT _NRECM_blink_flag;
      _NRECM_blink_ticks := 0;
    end;

  Inc(_IRQFREQ_blink_ticks);
  If ((fast_forward or _rewind or (space_pressed and debugging) or
       (@macro_preview_indic_proc <> NIL)) and
      (_IRQFREQ_blink_ticks > 25)) or (_IRQFREQ_blink_ticks > 25) then
    begin
      _IRQFREQ_blink_flag := NOT _IRQFREQ_blink_flag;
      _IRQFREQ_blink_ticks := 0;
    end;

  If ctrl_tab_pressed then
    begin
      If scankey(SC_UP) then
        If (screen_scroll_offset > 0) then
          Dec(screen_scroll_offset,2);

      If scankey(SC_DOWN) then
        If (screen_scroll_offset < 16*MaxLn-16*hard_maxln) then
          Inc(screen_scroll_offset,2);

      If scankey(SC_PAGEUP) then
        screen_scroll_offset := 0;

      If scankey(SC_PAGEDOWN) then
        screen_scroll_offset := 16*MaxLn-16*hard_maxln;
    end;

{$ENDIF}

  If (play_status = isPlaying) and
     NOT (debugging and (NOT space_pressed or no_step_debugging)) then
    begin
{$IFDEF GO32V2}
      song_timer_tenths := Trunc(100/(IRQ_freq+IRQ_freq_shift+playback_speed_shift)*timer_temp);
{$ELSE}
      song_timer_tenths := timer_temp;
{$ENDIF}
      If (song_timer_tenths >= 100) then song_timer_tenths := 0;
{$IFDEF GO32V2}
      If (timer_temp < IRQ_freq+IRQ_freq_shift+playback_speed_shift) then Inc(timer_temp)
{$ELSE}
      If (timer_temp < 100) then Inc(timer_temp)
{$ENDIF}
      else begin
             Inc(song_timer);
             timer_temp := 1;
           end;
    end
  else If debugging and (NOT space_pressed or no_step_debugging) then
         If NOT pattern_delay then synchronize_song_timer;

  If (song_timer > 3600-1) then
    begin
      song_timer := 0;
      timer_temp := 0;
      song_timer_tenths := 0;
    end;

{$IFDEF GO32V2}

  If (ticklooper > 0) then
    If (fast_forward or _rewind) and NOT replay_forbidden then
      poll_proc
    else
  else If NOT replay_forbidden then
         poll_proc;

  If (macro_ticklooper = 0) then
    macro_poll_proc;

  Inc(ticklooper);
  If (ticklooper >= IRQ_freq DIV tempo) then
    ticklooper := 0;

  Inc(macro_ticklooper);
  If (macro_ticklooper >= IRQ_freq DIV (tempo*_macro_speedup)) then
    macro_ticklooper := 0;

  _debug_str_ := _debug_str_bak_;
{$ENDIF}
end;

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
{$IFDEF GO32V2}
  _last_debug_str_ := _debug_str_;
  _debug_str_ := 'ADT2UNIT.PAS:calibrate_player:update_status';
{$ENDIF}
  temp := songdata.pattern_order[current_order];
  If NOT (temp <= $7f) then temp := 0;
  show_str(17,03,byte2hex(current_order),pattern_bckg+status_dynamic_txt);
  show_str(20,03,byte2hex(temp),pattern_bckg+status_dynamic_txt);
  show_str(17,04,'--',pattern_bckg+status_dynamic_txt);
{$IFNDEF GO32V2}
  _draw_screen_without_delay := TRUE;
{$ENDIF}
  draw_screen;
end;

var
  _pattern_page,_pattord_page,
  _pattord_hpos,_pattord_vpos: Byte;

begin
{$IFDEF GO32V2}
  _last_debug_str_ := _debug_str_;
  _debug_str_ := 'ADT2UNIT.PAS:calibrate_player';
{$ENDIF}
  If (calc_following_order(0) = -1) then EXIT;
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
end;

procedure init_buffers;

var
  temp: Byte;

begin
{$IFDEF GO32V2}
  _last_debug_str_ := _debug_str_;
  _debug_str_ := 'ADT2UNIT.PAS:init_buffers';
{$ENDIF}
  FillChar(fmpar_table,SizeOf(fmpar_table),0);
  FillChar(pan_lock,SizeOf(pan_lock),BYTE(panlock));
  FillChar(volume_table,SizeOf(volume_table),63);
  FillChar(vscale_table,SizeOf(vscale_table),0);
  FillChar(modulator_vol,SizeOf(modulator_vol),0);
  FillChar(carrier_vol,SizeOf(carrier_vol),0);
  FillChar(decay_bar,SizeOf(decay_bar),0);
  FillChar(volum_bar,SizeOf(volum_bar),0);
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
  FillChar(freqtable2,SizeOf(freqtable2),0);
  FillChar(notedel_table,SizeOf(notedel_table),BYTE_NULL);
  FillChar(notecut_table,SizeOf(notecut_table),BYTE_NULL);
  FillChar(ftune_table,SizeOf(ftune_table),0);
  FillChar(loopbck_table,SizeOf(loopbck_table),BYTE_NULL);
  FillChar(loop_table,SizeOf(loop_table),BYTE_NULL);
  FillChar(reset_chan,SizeOf(reset_chan),BYTE(FALSE));
  FillChar(reset_adsrw,SizeOf(reset_adsrw),BYTE(FALSE));
  FillChar(keyoff_loop,SizeOf(keyoff_loop),BYTE(FALSE));
  FillChar(macro_table,SizeOf(macro_table),0);
  FillChar(ignore_note_once,SizeOf(ignore_note_once),FALSE);

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
end;

procedure init_player;

var
  temp: Byte;

begin
{$IFDEF GO32V2}
  _last_debug_str_ := _debug_str_;
  _debug_str_ := 'ADT2UNIT.PAS:init_player';
{$ELSE}
  opl3_init;
{$ENDIF}

  FillChar(ai_table,SizeOf(ai_table),0);
  opl2out($01,0);

  For temp := 1 to 18 do opl2out($0b0+_chan_n[temp],0);
  For temp := $080 to $08d do opl2out(temp,BYTE_NULL);
  For temp := $090 to $095 do opl2out(temp,BYTE_NULL);

  misc_register := tremolo_depth SHL 7+
                   vibrato_depth SHL 6+
                   BYTE(percussion_mode) SHL 5;

  opl2out($01,$20);
  opl2out($08,$40);
  opl3exp($0105);
  opl3exp($04+songdata.flag_4op SHL 8);

  key_off(17);
  key_off(18);
  opl2out(_instr[11],misc_register);
  init_buffers;

  current_tremolo_depth := tremolo_depth;
  current_vibrato_depth := vibrato_depth;
  global_volume := 63;
  macro_ticklooper := 0;
  vibtrem_speed_factor := def_vibtrem_speed_factor;
  vibtrem_table_size := def_vibtrem_table_size;
  Move(def_vibtrem_table,vibtrem_table,SizeOf(vibtrem_table));

  For temp := 1 to 20 do
    begin
      arpgg_table[temp].state := 1;
      arpgg_table2[temp].state := 1;
      voice_table[temp] := temp;
    end;
end;

procedure reset_player;

var
  temp: Byte;

begin
{$IFDEF GO32V2}
  _last_debug_str_ := _debug_str_;
  _debug_str_ := 'ADT2UNIT.PAS:reset_player';
{$ENDIF}
  opl2out($01,0);

  For temp := 1 to 18 do opl2out($0b0+_chan_n[temp],0);
  For temp := $080 to $08d do opl2out(temp,BYTE_NULL);
  For temp := $090 to $095 do opl2out(temp,BYTE_NULL);

  misc_register := tremolo_depth SHL 7+
                   vibrato_depth SHL 6+
                   BYTE(percussion_mode) SHL 5;

  opl2out($01,$20);
  opl2out($08,$40);
  opl3exp($0105);
  opl3exp($04+songdata.flag_4op SHL 8);

  key_off(17);
  key_off(18);
  opl2out(_instr[11],misc_register);
  For temp := 1 to 20 do reset_chan_data(temp);
end;

procedure start_playing;
begin
{$IFDEF GO32V2}
  _last_debug_str_ := _debug_str_;
  _debug_str_ := 'ADT2UNIT.PAS:start_playing';
{$ENDIF}
  init_player;
  If (start_pattern = BYTE_NULL) then current_order := 0
  else If (start_order = BYTE_NULL) then
         begin
           If (calc_pattern_pos(start_pattern) <> BYTE_NULL) then
             current_order := calc_pattern_pos(start_pattern)
           else If NOT play_single_patt then
                  begin
                    start_pattern := BYTE_NULL;
                    current_order := 0;
                    EXIT;
                  end;
         end
       else begin
              current_order := start_order;
              current_pattern := start_pattern;
            end;

  If NOT play_single_patt then
    If (songdata.pattern_order[current_order] > $7f) then
      If (calc_order_jump = -1) then EXIT;

  If NOT play_single_patt then
    current_pattern := songdata.pattern_order[current_order]
  else current_pattern := start_pattern;

  If (start_line = BYTE_NULL) then current_line := 0
  else current_line := start_line;
  pattern_break := FALSE;
  pattern_delay := FALSE;
  tickXF := 0;
  last_order := 0;
  next_line := 0;
  song_timer := 0;
  timer_temp := 0;
  song_timer_tenths := 0;
  time_playing := 0;
  ticklooper := 0;
  macro_ticklooper := 0;
  debugging := FALSE;
  ticks := 0;
  tick0 := 0;
  fade_out_volume := 63;
  playback_speed_shift := 0;
  replay_forbidden := FALSE;
  play_status := isPlaying;
  speed := songdata.speed;
  macro_speedup := songdata.macro_speedup;
  update_timer(songdata.tempo);
  no_status_refresh := FALSE;
  really_no_status_refresh := FALSE;
  FillChar(play_pos_buf,SizeOf(play_pos_buf),0);
end;

procedure stop_playing;

var
  temp: Byte;

begin
{$IFDEF GO32V2}
  _last_debug_str_ := _debug_str_;
  _debug_str_ := 'ADT2UNIT.PAS:stop_playing';
{$ELSE}
  flush_WAV_data;
  If NOT opl3_flushmode then
    renew_wav_files_flag := TRUE;
{$ENDIF}
  replay_forbidden := TRUE;
  play_status := isStopped;
  fade_out_volume := 63;
  repeat_pattern := FALSE;
  global_volume := 63;
  no_sync_playing := FALSE;
  play_single_patt := FALSE;
  current_tremolo_depth := tremolo_depth;
  current_vibrato_depth := vibrato_depth;
  pattern_break := FALSE;
  current_order := 0;
  current_pattern := 0;
  current_line := 0;
  start_order := BYTE_NULL;
  start_pattern := BYTE_NULL;
  start_line := BYTE_NULL;
  song_timer := 0;
  timer_temp := 0;
  song_timer_tenths := 0;
  time_playing := 0;
  playback_speed_shift := 0;

  skip_macro_flag := TRUE;
  For temp := 1 to 20 do reset_chan_data(temp);
  opl2out(_instr[11],0);
  init_buffers;
  skip_macro_flag := FALSE;

  speed := songdata.speed;
  update_timer(songdata.tempo);
end;

function _partial(max,val: Byte; base: Byte): Byte;

var
  temp1,temp2: Real;
  temp3: Byte;

begin
  temp1 := max/base;
  temp2 := (max/base)/2;
  temp3 := 0;
  While (temp2 < val) do
    begin
      temp2 := temp2+temp1;
      Inc(temp3);
    end;
  _partial := temp3;
end;

function hscroll_bar(x,y: Byte; size: Byte; len1,len2,pos: Word;
                     atr1,atr2: Byte): Byte;
var
  temp: Byte;

begin
  If (size > work_MaxCol-x) then size := work_MaxCol-x;
  If (size < 5) then size := 5;

  If (size-2-1 < 10) then temp := _partial(len1,len2,size-2-1)
  else temp := _partial(len1,len2,size-2-1-2);

  If (pos = temp) and NOT force_scrollbars then
    begin
      hscroll_bar := temp;
      EXIT;
    end;

  If (size < len1*4) and (len1 > 4) then
    begin
      pos := temp;
      show_str(x,y,#17+ExpStrL('',size-2,#176)+#16,atr1);
      If (size-2-1 < 10) then show_str(x+1+temp,y,#178,atr2)
      else show_str(x+1+temp,y,#178#178#178,atr2);
    end
  else show_Str(x,y,#17+ExpStrL('',size-2,#177)+#16,atr1);
  hscroll_bar := pos;
end;

function vscroll_bar(x,y: Byte; size: Byte; len1,len2,pos: Word;
                     atr1,atr2: Byte): Byte;
var
  temp: Byte;

begin
  If (size > work_MaxLn-y) then size := work_MaxLn-y;
  If (size < 5) then size := 5;

  If (size-2-1 < 10) then temp := _partial(len1,len2,size-2-1)
  else temp := _partial(len1,len2,size-2-1-2);

  If (pos = temp) and NOT force_scrollbars then
    begin
      vscroll_bar := temp;
      EXIT;
    end;

  If (size < len1*4) and (len1 > 5) then
    begin
      pos := temp;
      show_vstr(x,y,#30+ExpStrL('',size-2,#176)+#31,atr1);
      If (size-2-1 < 10) then show_str(x,y+1+temp,#178,atr2)
      else show_vstr(x,y+1+temp,#178#178#178,atr2);
    end
  else show_vstr(x,y,#30+ExpStrL('',size-2,#177)+#31,atr1);
  vscroll_bar := pos;
end;

procedure centered_frame(var xstart,ystart: Byte; hsize,vsize: Byte;
                         name: String; atr1,atr2: Byte; border: String);
begin
  xstart := (work_MaxCol-hsize) DIV 2;
  ystart := (work_MaxLn -vsize) DIV 2+(work_MaxLn-vsize) MOD 2;

  Frame(centered_frame_vdest,xstart,ystart,xstart+hsize,ystart+vsize,
        atr1,name,atr2,border);
end;

procedure get_chunk(pattern,line,channel: Byte; var chunk: tCHUNK);
begin
  asm
        mov     esi,[pattdata]
        mov     edi,[chunk]
        mov     al,pattern
        inc     al
        cmp     al,max_patterns
        jbe     @@1
        mov     ecx,CHUNK_SIZE
        xor     al,al
        rep     stosb
        jmp     @@2
@@1:    xor     eax,eax
        mov     al,line
        mov     ebx,CHUNK_SIZE
        mul     ebx
        mov     ecx,eax
        xor     eax,eax
        mov     al,channel
        dec     eax
        mov     ebx,256*CHUNK_SIZE
        mul     ebx
        add     ecx,eax
        xor     eax,eax
        mov     al,pattern
        mov     ebx,8
        div     ebx
        push    eax
        mov     eax,edx
        mov     ebx,20*256*CHUNK_SIZE
        mul     ebx
        add     ecx,eax
        pop     eax
        mov     ebx,8*20*256*CHUNK_SIZE
        mul     ebx
        add     ecx,eax
        add     esi,ecx
        mov     ecx,CHUNK_SIZE
        rep     movsb
@@2:
  end;
end;

procedure put_chunk(pattern,line,channel: Byte; chunk: tCHUNK);
begin
  asm
        lea     esi,[chunk]
        mov     edi,[pattdata]
        mov     al,pattern
        inc     al
        cmp     al,max_patterns
        jbe     @@1
        mov     limit_exceeded,TRUE
        jmp     @@2
@@1:    xor     eax,eax
        mov     al,line
        mov     ebx,CHUNK_SIZE
        mul     ebx
        mov     ecx,eax
        xor     eax,eax
        mov     al,channel
        dec     eax
        mov     ebx,256*CHUNK_SIZE
        mul     ebx
        add     ecx,eax
        xor     eax,eax
        mov     al,pattern
        mov     ebx,8
        div     ebx
        push    eax
        mov     eax,edx
        mov     ebx,20*256*CHUNK_SIZE
        mul     ebx
        add     ecx,eax
        pop     eax
        mov     ebx,8*20*256*CHUNK_SIZE
        mul     ebx
        add     ecx,eax
        add     edi,ecx
        mov     ecx,CHUNK_SIZE
        rep     movsb
        mov     module_archived,FALSE
@@2:
  end;
end;

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

procedure count_order(var entries: Byte);

var
  index,
  index2: Byte;

begin
{$IFDEF GO32V2}
  _last_debug_str_ := _debug_str_;
  _debug_str_ := 'ADT2UNIT.PAS:count_order';
{$ENDIF}
  index := 0;
  index2 := 0;

  Repeat
    If (songdata.pattern_order[index] <> $80) then
      begin
        If (songdata.pattern_order[index] > $80) then
          If (songdata.pattern_order[index]-$80 <> index2) then
            begin
              index := songdata.pattern_order[index]-$80;
              index2 := index;
            end
          else BREAK;
      end
    else BREAK;
    If (index < $80) then Inc(index);
  until (index > $7f);

  entries := index;
end;

procedure count_patterns(var patterns: Byte);

var
  temp1: Byte;

begin
{$IFDEF GO32V2}
  _last_debug_str_ := _debug_str_;
  _debug_str_ := 'ADT2UNIT.PAS:count_patterns';
{$ENDIF}
  patterns := 0;
  For temp1 := 0 to PRED(max_patterns) do
    begin
      realtime_gfx_poll_proc;
      If NOT is_data_empty(pattdata^[temp1 DIV 8][temp1 MOD 8],PATTERN_SIZE) then
        patterns := temp1+1;
    end;
end;

procedure count_instruments(var instruments: Byte);
begin
{$IFDEF GO32V2}
  _last_debug_str_ := _debug_str_;
  _debug_str_ := 'ADT2UNIT.PAS:count_instruments';
{$ENDIF}
  instruments := 255;
  While (instruments > 0) and
        Empty(songdata.instr_data[instruments],INSTRUMENT_SIZE) do
    Dec(instruments);
end;

function calc_max_speedup(tempo: Byte): Word;

var
  temp: Longint;
  result: Word;

begin
{$IFDEF GO32V2}
  _last_debug_str_ := _debug_str_;
  _debug_str_ := 'ADT2UNIT.PAS:calc_max_speedup';
{$ENDIF}
  result := MAX_IRQ_FREQ DIV tempo;
  Repeat
    If (tempo = 18) and timer_fix then temp := TRUNC((tempo+0.2)*20)
    else temp := 250;
    While (temp MOD (tempo*result) <> 0) do Inc(temp);
    If (temp <= MAX_IRQ_FREQ) then Inc(result);
  until NOT (temp <= MAX_IRQ_FREQ);
  calc_max_speedup := PRED(result);
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

procedure init_old_songdata;

var
  temp: Byte;

begin
{$IFDEF GO32V2}
  _last_debug_str_ := _debug_str_;
  _debug_str_ := 'ADT2UNIT.PAS:init_old_songdata';
{$ENDIF}
  FillChar(old_songdata,SizeOf(old_songdata),0);
  FillChar(old_songdata.pattern_order,SizeOf(old_songdata.pattern_order),$080);
  FillChar(old_songdata.instr_data,SizeOf(old_songdata.instr_data),0);

  For temp := 1 to 250 do
    old_songdata.instr_names[temp] :=
      ' iNS_'+byte2hex(temp)+#247' ';
end;

procedure init_songdata;

var
  temp: Byte;

begin
{$IFDEF GO32V2}
  _last_debug_str_ := _debug_str_;
  _debug_str_ := 'ADT2UNIT.PAS:init_songdata';
{$ENDIF}
  If (play_status <> isStopped) then
    begin
      fade_out_playback(FALSE);
      stop_playing;
    end
  else init_buffers;

  FillChar(songdata,SizeOf(songdata),0);
  FillChar(songdata.pattern_order,SizeOf(songdata.pattern_order),$080);
  For temp := 1 to max_patterns DIV 8 do
    FillChar(pattdata^[PRED(temp)],8*PATTERN_SIZE,0);

  songdata.tempo := tempo;
  songdata.speed := speed;
  songdata.macro_speedup := init_macro_speedup;
  speed_update := FALSE;
  IRQ_freq_shift := 0;
  playback_speed_shift := 0;
  songdata.patt_len := patt_len;
  songdata.nm_tracks := nm_tracks;
  songdata.bpm_data.rows_per_beat := mark_line;
  songdata.bpm_data.tempo_finetune := IRQ_freq_shift;
  lockvol := FALSE;
  panlock := FALSE;
  lockVP  := FALSE;
  tremolo_depth := 0;
  vibrato_depth := 0;
  volume_scaling := FALSE;

  old_chan_pos := 1;
  old_hpos := 1;
  old_page := 0;
  old_block_chan_pos := 1;
  old_block_patt_hpos := 1;
  old_block_patt_page := 0;
  marking := FALSE;

  If (nm_tracks <= 18) then
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

  For temp := 1 to 255 do
    songdata.instr_names[temp] := ' iNS_'+byte2hex(temp)+#247' ';

  For temp := 0 to $7f do
    songdata.pattern_names[temp] := ' PAT_'+byte2hex(temp)+'  '#247' ';

  songdata_crc_ord := Update32(songdata.pattern_order,
                               SizeOf(songdata.pattern_order),0);
  module_archived := TRUE;
end;

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

procedure fade_out_playback(fade_screen: Boolean);

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

{$IFDEF GO32V2}

  _last_debug_str_ := _debug_str_;
  _debug_str_ := 'ADT2UNIT.PAS:fade_out_playback';

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

end;

procedure realtime_gfx_poll_proc; cdecl;
public name PUBLIC_PREFIX + 'realtime_gfx_poll_proc';
begin
  If _realtime_gfx_no_update then EXIT;

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
end;

function get_bank_position(bank_name: String; bank_size: Longint): Longint;

var
  idx: Longint;
  result: Longint;

begin
{$IFDEF GO32V2}
  _last_debug_str_ := _debug_str_;
  _debug_str_ := 'ADT2UNIT.PAS:get_bank_position';
{$ENDIF}
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
end;

procedure add_bank_position(bank_name: String; bank_size: Longint; bank_position: Longint);

var
  idx,idx2: Longint;
  found_flag: Boolean;

begin
{$IFDEF GO32V2}
  _last_debug_str_ := _debug_str_;
  _debug_str_ := 'ADT2UNIT.PAS:add_bank_position';
{$ENDIF}
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
      EXIT;
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
