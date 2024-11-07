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

const
  NULL = $0ffffffff;
  BYTE_NULL = BYTE(NULL);
  WORD_NULL = WORD(NULL);
  DWORD_NULL = LONGINT(NULL);

const
  FFVER_A2M = 14;
  FFVER_A2T = 14;
  FFVER_A2I = 10;
  FFVER_A2F = 2;
  FFVER_A2P = 11;
  FFVER_A2B = 10;
  FFVER_A2W = 3;

{$IFNDEF ADT2PLAY}
const
  GENERIC_IO_BUFFER_SIZE = 1500*1024; // 1.5MB I/O Buffer

type
  pGENERIC_IO_BUFFER = ^tGENERIC_IO_BUFFER;
  tGENERIC_IO_BUFFER = array[0..PRED(GENERIC_IO_BUFFER_SIZE)] of Byte;
{$ENDIF} // NOT DEFINED(ADT2PLAY)

type
  tFM_INST_DATA = Record
                    AM_VIB_EG_modulator,
                    AM_VIB_EG_carrier,
                    KSL_VOLUM_modulator,
                    KSL_VOLUM_carrier,
                    ATTCK_DEC_modulator,
                    ATTCK_DEC_carrier,
                    SUSTN_REL_modulator,
                    SUSTN_REL_carrier,
                    WAVEFORM_modulator,
                    WAVEFORM_carrier,
                    FEEDBACK_FM: Byte;
                  end;
type
  tADTRACK2_INS = Record
                    fm_data: tFM_INST_DATA;
                    panning: Byte;
                    fine_tune: Shortint;
                    perc_voice: Byte;
                  end;
type
  tOLD_ADTRACK2_INS = Record
                        fm_data: tFM_INST_DATA;
                        panning: Byte;
                        fine_tune: Shortint;
                      end;
type
  tARPEGGIO_TABLE = Record
                      length,
                      speed,
                      loop_begin,
                      loop_length,
                      keyoff_pos: Byte;
                      data: array[1..255] of Byte;
                    end;
type
  tVIBRATO_TABLE = Record
                     length,
                     speed,
                     delay,
                     loop_begin,
                     loop_length,
                     keyoff_pos: Byte;
                     data: array[1..255] of Shortint;
                   end;
type
  tREGISTER_TABLE_DEF = Record
                          fm_data: tFM_INST_DATA;
                          freq_slide: Smallint;
                          panning: Byte;
                          duration: Byte;
                        end;
type
  tREGISTER_TABLE = Record
                      length,
                      loop_begin,
                      loop_length,
                      keyoff_pos,
                      arpeggio_table,
                      vibrato_table: Byte;
                      data: array[1..255] of tREGISTER_TABLE_DEF;
                    end;
type
  tMACRO_TABLE = Record
                   arpeggio: tARPEGGIO_TABLE;
                   vibrato:  tVIBRATO_TABLE;
                 end;
type
  tARP_VIB_MACRO_TABLE = array[1..255] of tMACRO_TABLE;

type
  tFM_PARAMETER_TABLE = Record
                          adsrw_car,
                          adsrw_mod: Record
                                       attck,dec,sustn,rel,
                                       wform: Byte;
                                     end;
                          connect,
                          feedb,
                          multipM,kslM,tremM,vibrM,ksrM,sustM,
                          multipC,kslC,tremC,vibrC,ksrC,sustC: Byte;
                        end;
type
  tDIS_FMREG_COL = array[0..27] of Boolean;

type
  tINS_4OP_FLAGS = Record
                     num_4op: Byte;
                     idx_4op: array[1..128] of Byte;
                   end;
type
  pOLD_FIXED_SONGDATA = ^tOLD_FIXED_SONGDATA;
  tOLD_FIXED_SONGDATA = Record
                          songname:      String[42];
                          composer:      String[42];
                          instr_names:   array[1..250] of String[32];
                          instr_data:    array[1..250] of tOLD_ADTRACK2_INS;
                          pattern_order: array[0..$7f] of Byte;
                          tempo:         Byte;
                          speed:         Byte;
                          common_flag:   Byte;
                        end;
const
  MB_SIGNATURE = 'MB_1'; // Message Board v1.0
  MB_HSIZE = 50;         // 50x20 chr
  MB_VSIZE = 20;

type
  pMESSAGE_BOARD_DATA = ^tMESSAGE_BOARD_DATA;
  tMESSAGE_BOARD_DATA = Record
                          signature: array[0..3] of Char;
                          data: array[1..MB_VSIZE] of String[MB_HSIZE];
                        end;
type
  pFIXED_SONGDATA = ^tFIXED_SONGDATA;
  tFIXED_SONGDATA = Record
                      songname:      String[42];
                      composer:      String[42];
                      instr_names:   array[1..255] of String[42];
                      instr_data:    array[1..255] of tADTRACK2_INS;
                      instr_macros:  array[1..255] of tREGISTER_TABLE;
                      macro_table:   tARP_VIB_MACRO_TABLE;
                      pattern_order: array[0..$7f] of Byte;
                      tempo:         Byte;
                      speed:         Byte;
                      common_flag:   Byte;
                      patt_len:      Word;
                      nm_tracks:     Byte;
                      macro_speedup: Word;
                      flag_4op:      Byte;
                      lock_flags:    array[1..20]  of Byte;
                      pattern_names: array[0..$7f] of String[42];
                      dis_fmreg_col: array[1..255] of tDIS_FMREG_COL;
                      ins_4op_flags: tINS_4OP_FLAGS;
                      reserved_data: array[0..PRED(1024)] of Byte;
                      bpm_data:      Record
                                       rows_per_beat: Byte;
                                       tempo_finetune: Integer;
                                     end;
                    end;
type
  tOLD_CHUNK = Record
                 note:       Byte;
                 instr_def:  Byte;
                 effect_def: Byte;
                 effect:     Byte;
               end;
type
  tCHUNK = Record
             note:        Byte;
             instr_def:   Byte;
             effect_def:  Byte;
             effect:      Byte;
             effect_def2: Byte;
             effect2:     Byte;
           end;
type
  tOLD_VARIABLE_DATA1 = array[0..$0f] of array[0..$3f] of
                        array[1..9]   of tOLD_CHUNK;
type
  tOLD_VARIABLE_DATA2 = array[0..7]   of array[1..18] of
                        array[0..$3f] of tOLD_CHUNK;
type
  tVARIABLE_DATA = array[0..7]    of array[1..20] of
                   array[0..$0ff] of tCHUNK;
type
  tPATTERN_DATA = array[0..15] of tVARIABLE_DATA;

{$IFNDEF ADT2PLAY}

type
  tCOPY_OBJECT = (objNone,
                  objPatternDef,objPatternTable,
                  objInstrument,objInstrumentBank,
                  objNote,objInstrumentDef,objEffect,objEffect2,
                  objLine,objTrack,objPattern,objMarkedBlock,
                  objMacroTableLine,objMacroTableColumn,objMacroTable,
                  objInstrumentWithMacros);
type
  tMACRO_TABLE_TYPE = (mttFM_reg_table,mttArpeggio_table,mttVibrato_table);

type
  tCLIPBOARD = Record
                 object_type:   tCOPY_OBJECT;
                 block_hsize:   Byte;
                 block_vsize:   Byte;
                 pattern_order: array[0..$7f] of Byte;
                 pattern:       array[1..20] of array[0..$0ff] of tCHUNK;
                 instrument:    Record
                                  names:  array[1..255] of String[32];
                                  data:   array[1..255] of tADTRACK2_INS;
                                  macros: array[1..255] of tREGISTER_TABLE;
                                  arpvib: array[1..255] of tMACRO_TABLE;
                                  dis_fmreg_col: array[1..255] of tDIS_FMREG_COL;
                                end;
                 mcrtab_type:   tMACRO_TABLE_TYPE;
                 fmreg_table:   tREGISTER_TABLE;
                 macro_table:   tMACRO_TABLE;
                 fmtab_def_typ: Byte;
                 _string:       String;
               end;

{$ENDIF} // NOT DEFINED(ADT2PLAY)

{$IFDEF ADT2PLAY}

type
  tDECAY_BAR = Record
                  dir: Integer;
                  lvl,max_lvl: Real;
                end;

{$ELSE} // NOT DEFINED(ADT2PLAY)

type
  tDECAY_BAR = Record
                  dir1: Integer;
                  dir2: Integer;
                  lvl1: Real;
                  lvl2: Real;
                end;

type
  tVOLUM_BAR = Record
                  dir: Integer;
                  lvl: Real;
                end;

{$ENDIF} // NOT DEFINED(ADT2PLAY)

type
  tPLAY_STATUS = (isPlaying,isPaused,isStopped);

{$IFNDEF ADT2PLAY}
var
  status_layout: array[tPLAY_STATUS] of String[10]; cvar; external;
{$ENDIF} // NOT DEFINED(ADT2PLAY)

type
  tByteSet = Set of Byte;

const
  INSTRUMENT_SIZE = SizeOf(tADTRACK2_INS);
  CHUNK_SIZE = SizeOf(tCHUNK);
  PATTERN_SIZE = 20*256*CHUNK_SIZE;

const
  ef_Arpeggio           = 0;
  ef_FSlideUp           = 1;
  ef_FSlideDown         = 2;
  ef_TonePortamento     = 3;
  ef_Vibrato            = 4;
  ef_TPortamVolSlide    = 5;
  ef_VibratoVolSlide    = 6;
  ef_FSlideUpFine       = 7;
  ef_FSlideDownFine     = 8;
  ef_SetModulatorVol    = 9;
  ef_VolSlide           = 10;
  ef_PositionJump       = 11;
  ef_SetInsVolume       = 12;
  ef_PatternBreak       = 13;
  ef_SetTempo           = 14;
  ef_SetSpeed           = 15;
  ef_TPortamVSlideFine  = 16;
  ef_VibratoVSlideFine  = 17;
  ef_SetCarrierVol      = 18;
  ef_SetWaveform        = 19;
  ef_VolSlideFine       = 20;
  ef_RetrigNote         = 21;
  ef_Tremolo            = 22;
  ef_Tremor             = 23;
  ef_ArpggVSlide        = 24;
  ef_ArpggVSlideFine    = 25;
  ef_MultiRetrigNote    = 26;
  ef_FSlideUpVSlide     = 27;
  ef_FSlideDownVSlide   = 28;
  ef_FSlUpFineVSlide    = 29;
  ef_FSlDownFineVSlide  = 30;
  ef_FSlUpVSlF          = 31;
  ef_FSlDownVSlF        = 32;
  ef_FSlUpFineVSlF      = 33;
  ef_FSlDownFineVSlF    = 34;
  ef_Extended           = 35;
  ef_Extended2          = 36;
  ef_SetGlobalVolume    = 37;
  ef_SwapArpeggio       = 38;
  ef_SwapVibrato        = 39;
  ef_ForceInsVolume     = 40;
  ef_Extended3          = 41;
  ef_ExtraFineArpeggio  = 42;
  ef_ExtraFineVibrato   = 43;
  ef_ExtraFineTremolo   = 44;
  ef_SetCustomSpeedTab  = 45;
  ef_GlobalFSlideUp     = 46;
  ef_GlobalFSlideDown   = 47;
  ef_ex_SetTremDepth    = 0;
  ef_ex_SetVibDepth     = 1;
  ef_ex_SetAttckRateM   = 2;
  ef_ex_SetDecayRateM   = 3;
  ef_ex_SetSustnLevelM  = 4;
  ef_ex_SetRelRateM     = 5;
  ef_ex_SetAttckRateC   = 6;
  ef_ex_SetDecayRateC   = 7;
  ef_ex_SetSustnLevelC  = 8;
  ef_ex_SetRelRateC     = 9;
  ef_ex_SetFeedback     = 10;
  ef_ex_SetPanningPos   = 11;
  ef_ex_PatternLoop     = 12;
  ef_ex_PatternLoopRec  = 13;
  ef_ex_ExtendedCmd     = 14;
  ef_ex_cmd_MKOffLoopDi = 0;
  ef_ex_cmd_MKOffLoopEn = 1;
  ef_ex_cmd_TPortaFKdis = 2;
  ef_ex_cmd_TPortaFKenb = 3;
  ef_ex_cmd_RestartEnv  = 4;
  ef_ex_cmd_4opVlockOff = 5;
  ef_ex_cmd_4opVlockOn  = 6;
  ef_ex_cmd_ForceBpmSld = 7;
  ef_ex_ExtendedCmd2    = 15;
  ef_ex_cmd2_RSS        = 0;
  ef_ex_cmd2_ResetVol   = 1;
  ef_ex_cmd2_LockVol    = 2;
  ef_ex_cmd2_UnlockVol  = 3;
  ef_ex_cmd2_LockVP     = 4;
  ef_ex_cmd2_UnlockVP   = 5;
  ef_ex_cmd2_VSlide_mod = 6;
  ef_ex_cmd2_VSlide_car = 7;
  ef_ex_cmd2_VSlide_def = 8;
  ef_ex_cmd2_LockPan    = 9;
  ef_ex_cmd2_UnlockPan  = 10;
  ef_ex_cmd2_VibrOff    = 11;
  ef_ex_cmd2_TremOff    = 12;
  ef_ex_cmd2_FVib_FGFS  = 13;
  ef_ex_cmd2_FTrm_XFGFS = 14;
  ef_ex_cmd2_NoRestart  = 15;
  ef_ex2_PatDelayFrame  = 0;
  ef_ex2_PatDelayRow    = 1;
  ef_ex2_NoteDelay      = 2;
  ef_ex2_NoteCut        = 3;
  ef_ex2_FineTuneUp     = 4;
  ef_ex2_FineTuneDown   = 5;
  ef_ex2_GlVolSlideUp   = 6;
  ef_ex2_GlVolSlideDn   = 7;
  ef_ex2_GlVolSlideUpF  = 8;
  ef_ex2_GlVolSlideDnF  = 9;
  ef_ex2_GlVolSldUpXF   = 10;
  ef_ex2_GlVolSldDnXF   = 11;
  ef_ex2_VolSlideUpXF   = 12;
  ef_ex2_VolSlideDnXF   = 13;
  ef_ex2_FreqSlideUpXF  = 14;
  ef_ex2_FreqSlideDnXF  = 15;
  ef_ex3_SetConnection  = 0;
  ef_ex3_SetMultipM     = 1;
  ef_ex3_SetKslM        = 2;
  ef_ex3_SetTremoloM    = 3;
  ef_ex3_SetVibratoM    = 4;
  ef_ex3_SetKsrM        = 5;
  ef_ex3_SetSustainM    = 6;
  ef_ex3_SetMultipC     = 7;
  ef_ex3_SetKslC        = 8;
  ef_ex3_SetTremoloC    = 9;
  ef_ex3_SetVibratoC    = 10;
  ef_ex3_SetKsrC        = 11;
  ef_ex3_SetSustainC    = 12;

const
  ef_fix1 = $80;
  ef_fix2 = $90;

{$IFNDEF ADT2PLAY}

const
  inst_vpos_max = 21;
  inst_hpos_max = 7;

var
  inst_hpos: array[1..inst_vpos_max,1..inst_hpos_max] of Byte; cvar; external;
  inst_hpos_ext_2op: array[1..6,1..inst_hpos_max] of Byte; cvar; external; //const
  inst_hpos_ext_perc: array[1..6,1..inst_hpos_max] of Byte; cvar; external; //const
  inst_hpos_ext_4op: array[1..6,1..inst_hpos_max] of Byte; cvar; external; //const
  inst_vpos: array[1..inst_vpos_max] of Byte; cvar; external; //const

  note_layout: array[0..12*8+1] of String[3]; cvar; external;

  board_scancodes: array[1..29] of Byte; cvar; external; // const

  fx_digits: array[0..47] of Char; cvar; external; //const

const
  NM_FX_DIGITS = SizeOf(fx_digits);

var
  b_note: Char; cvar; external;
  connection_str: array[0..1] of String[2]; cvar; external; //const
  macro_retrig_str: array[0..7] of String[1]; cvar; external; //const

  pos1: array[1..22] of Byte; cvar; external; //const
  pos2: array[1..13] of Byte; cvar; external; //const
  pos3: array[1..44] of Byte; cvar; external; //const
  pos4: array[1..110] of Byte; cvar; external; //const
  pos5: array[1..35] of Byte; cvar; external; //const
  pos5vw: array[1..35] of Byte; cvar; external; //const
  pos6: array[1..11] of Byte; cvar; external; //const

  _valid_characters: Set of Char; cvar; external;
  _valid_characters_fname: Set of Char; cvar; external;

{$ENDIF} // NOT DEFINED(ADT2PLAY)

var
  _panning: array[0..2] of Byte; cvar; external; //const

  _instr_data_ofs: array[1..13] of Byte; cvar; external; //const
  _instr:  array[0..11] of Byte; cvar; external; //const

type
  tTRACK_ADDR = array[1..20] of Word;

var
  _chmm_n: tTRACK_ADDR; cvar; external; //const
  _chmm_m: tTRACK_ADDR; cvar; external; //const
  _chmm_c: tTRACK_ADDR; cvar; external; //const

  _chpm_n: tTRACK_ADDR; cvar; external; //const
  _chpm_m: tTRACK_ADDR; cvar; external; //const
  _chpm_c: tTRACK_ADDR; cvar; external; //const

  _4op_tracks_hi: Set of Byte; cvar; external; //const
  _4op_tracks_lo: Set of Byte; cvar; external; //const

  _4op_main_chan: array[1..6] of Byte; cvar; external; //const
{$IFNDEF ADT2PLAY}
  _perc_sim_chan: array[19..20] of Byte; cvar; external; //const
{$ENDIF} // NOT DEFINED(ADT2PLAY)

  _chan_n: tTRACK_ADDR; cvar; external;
  _chan_m: tTRACK_ADDR; cvar; external;
  _chan_c: tTRACK_ADDR; cvar; external;
{$IFNDEF ADT2PLAY}
  _ch_tmp: tTRACK_ADDR; cvar; external;
{$ENDIF} // NOT DEFINED(ADT2PLAY)

{$IFNDEF ADT2PLAY}

var
  fade_delay_tab: array[0..63] of Byte; cvar; external; //const

  tracing:     Boolean; cvar; external;
  track_notes: Boolean; cvar; external;
  marking:     Boolean; cvar; external;
  debugging:   Boolean; cvar; external;
  quick_cmd:   Boolean; cvar; external;

  home_dir_path:      String; cvar; external;
  a2m_default_path:   String; cvar; external;
  a2t_default_path:   String; cvar; external;
  a2i_default_path:   String; cvar; external;
  a2f_default_path:   String; cvar; external;
  a2p_default_path:   String; cvar; external;
  a2b_default_path:   String; cvar; external;
  a2w_default_path:   String; cvar; external;

{$IFDEF GO32V2}

  screen_mode:        Byte; cvar; external;
  comp_text_mode:     Byte; cvar; external;
  custom_svga_mode:   Boolean; cvar; external;
  fps_down_factor:    Byte; cvar; external;

  typematic_delay:    Byte; cvar; external;
  typematic_rate:     Byte; cvar; external;

  mouse_disabled:     Boolean; cvar; external;
  mouse_hspeed:       Word; cvar; external;
  mouse_vspeed:       Word; cvar; external;
  mouse_threshold:    Word; cvar; external;

{$ELSE} // NOT DEFINED(GO32V2)

  sdl_screen_mode:     Byte; cvar; external;
  sdl_frame_rate:      Byte; cvar; external;

  sdl_typematic_delay: Longint; cvar; external;
  sdl_typematic_rate:  Longint; cvar; external;

  sdl_opl3_emulator:   Byte; cvar; external;
  sdl_sample_rate:     Longint; cvar; external;
  sdl_sample_buffer:   Longint; cvar; external;
  sdl_timer_slowdown:  Byte; cvar; external;
  sdl_wav_directory:   String; cvar; external;

{$ENDIF} // NOT DEFINED(GO32V2)

  init_tempo:         Byte; cvar; external;
  init_speed:         Byte; cvar; external;
  init_macro_speedup: Word; cvar; external;
  midiboard:          Boolean; cvar; external;
  default_octave:     Byte; cvar; external;
  patt_len:           Word; cvar; external;
  nm_tracks:          Byte; cvar; external;
  mod_description:    Boolean; cvar; external;
  highlight_controls: Boolean; cvar; external;
  use_H_for_B:        Boolean; cvar; external;
  linefeed:           Boolean; cvar; external;
  lf_in_mboard_mode:  Boolean; cvar; external;
  update_ins:         Boolean; cvar; external;
  cycle_pattern:      Boolean; cvar; external;
  keep_track_pos:     Boolean; cvar; external;
  remember_ins_pos:   Boolean; cvar; external;
  command_typing:     Byte; cvar; external;
  backspace_dir:      Byte; cvar; external;
  mark_line:          Byte; cvar; external;
  scroll_bars:        Boolean; cvar; external;
  fforward_factor:    Byte; cvar; external;
  rewind_factor:      Byte; cvar; external;
  ssaver_time:        Longint; cvar; external;
  timer_fix:          Boolean; cvar; external;
  decay_bar_rise:     Single; cvar; external;
  decay_bar_fall:     Single; cvar; external;
  force_ins:          Byte; cvar; external;
  pattern_layout:     Byte; cvar; external;
  trace_by_default:   Boolean; cvar; external;
  nosync_by_default:  Boolean; cvar; external;
  keep_position:      Boolean; cvar; external;
  alt_ins_name:       Boolean; cvar; external;
  adjust_tracks:      Boolean; cvar; external;
  mark_lines:         Boolean; cvar; external;
  use_large_cursor:   Boolean; cvar; external;
  accurate_conv:      Boolean; cvar; external;
  fix_c_note_bug:     Boolean; cvar; external;

  analyzer_bckg:      Byte; cvar; external;
  analyzer:           Byte; cvar; external;
  analyzer_ovrllvol:  Byte; cvar; external;
  analyzer_volumelvl: Byte; cvar; external;
  analyzer_modulator: Byte; cvar; external;
  analyzer_carrier:   Byte; cvar; external;

  pattern_bckg:       Byte; cvar; external;
  pattern_border:     Byte; cvar; external;
  pattern_pos_indic:  Byte; cvar; external;
  pattern_pan_indic:  Byte; cvar; external;
  pattern_gpan_indic: Byte; cvar; external;
  pattern_lock_indic: Byte; cvar; external;
  pattern_4op_indic:  Byte; cvar; external;
  pattern_perc_indic: Byte; cvar; external;
  pattern_chan_indic: Byte; cvar; external;
  pattern_row_bckg:   Byte; cvar; external;
  pattern_row_bckg_p: Byte; cvar; external;
  pattern_row_bckg_m: Byte; cvar; external;
  pattern_block_bckg: Byte; cvar; external;
  pattern_line:       Byte; cvar; external;
  pattern_line_p:     Byte; cvar; external;
  pattern_line_m:     Byte; cvar; external;
  pattern_hi_line:    Byte; cvar; external;
  pattern_hi_line_m:  Byte; cvar; external;
  pattern_note:       Byte; cvar; external;
  pattern_hi_note:    Byte; cvar; external;
  pattern_note0:      Byte; cvar; external;
  pattern_hi_note0:   Byte; cvar; external;
  pattern_note_hid:   Byte; cvar; external;
  pattern_hi_note_h:  Byte; cvar; external;
  pattern_inst:       Byte; cvar; external;
  pattern_hi_inst:    Byte; cvar; external;
  pattern_inst0:      Byte; cvar; external;
  pattern_hi_inst0:   Byte; cvar; external;
  pattern_cmnd:       Byte; cvar; external;
  pattern_hi_cmnd:    Byte; cvar; external;
  pattern_cmnd0:      Byte; cvar; external;
  pattern_hi_cmnd0:   Byte; cvar; external;
  pattern_note_m:     Byte; cvar; external;
  pattern_note0_m:    Byte; cvar; external;
  pattern_note_hid_m: Byte; cvar; external;
  pattern_inst_m:     Byte; cvar; external;
  pattern_inst0_m:    Byte; cvar; external;
  pattern_cmnd_m:     Byte; cvar; external;
  pattern_cmnd0_m:    Byte; cvar; external;
  pattern_note_b:     Byte; cvar; external;
  pattern_note0_b:    Byte; cvar; external;
  pattern_note_hid_b: Byte; cvar; external;
  pattern_inst_b:     Byte; cvar; external;
  pattern_inst0_b:    Byte; cvar; external;
  pattern_cmnd_b:     Byte; cvar; external;
  pattern_cmnd0_b:    Byte; cvar; external;
  pattern_fix_note:   Byte; cvar; external;
  pattern_hi_fx_note: Byte; cvar; external;
  pattern_fix_note_m: Byte; cvar; external;
  pattern_fix_note_b: Byte; cvar; external;
  pattern_cmnd_ctrl:  Byte; cvar; external;
  pattern_input_bckg: Byte; cvar; external;
  pattern_input:      Byte; cvar; external;
  pattern_input_warn: Byte; cvar; external;

  debug_info_bckg:    Byte; cvar; external;
  debug_info_bckg2:   Byte; cvar; external;
  debug_info_border:  Byte; cvar; external;
  debug_info_border2: Byte; cvar; external;
  debug_info_title:   Byte; cvar; external;
  debug_info_topic:   Byte; cvar; external;
  debug_info_txt:     Byte; cvar; external;
  debug_info_hi_txt:  Byte; cvar; external;
  debug_info_txt_hid: Byte; cvar; external;
  debug_info_mod:     Byte; cvar; external;
  debug_info_hi_mod:  Byte; cvar; external;
  debug_info_car:     Byte; cvar; external;
  debug_info_hi_car:  Byte; cvar; external;
  debug_info_4op:     Byte; cvar; external;
  debug_info_perc:    Byte; cvar; external;
  debug_info_bpm:     Byte; cvar; external;

  help_background:    Byte; cvar; external;
  help_title:         Byte; cvar; external;
  help_border:        Byte; cvar; external;
  help_topic:         Byte; cvar; external;
  help_text:          Byte; cvar; external;
  help_hi_text:       Byte; cvar; external;
  help_keys:          Byte; cvar; external;
  help_indicators:    Byte; cvar; external;

  dialog_background:  Byte; cvar; external;
  dialog_title:       Byte; cvar; external;
  dialog_border:      Byte; cvar; external;
  dialog_text:        Byte; cvar; external;
  dialog_hi_text:     Byte; cvar; external;
  dialog_hid:         Byte; cvar; external;
  dialog_item:        Byte; cvar; external;
  dialog_short:       Byte; cvar; external;
  dialog_item_dis:    Byte; cvar; external;
  dialog_sel_itm_bck: Byte; cvar; external;
  dialog_sel_itm:     Byte; cvar; external;
  dialog_sel_short:   Byte; cvar; external;
  dialog_context:     Byte; cvar; external;
  dialog_context_dis: Byte; cvar; external;
  dialog_contxt_dis2: Byte; cvar; external;
  dialog_input_bckg:  Byte; cvar; external;
  dialog_input:       Byte; cvar; external;
  dialog_def_bckg:    Byte; cvar; external;
  dialog_def:         Byte; cvar; external;
  dialog_prog_bar1:   Byte; cvar; external;
  dialog_prog_bar2:   Byte; cvar; external;
  dialog_topic:       Byte; cvar; external;
  dialog_hi_topic:    Byte; cvar; external;
  dialog_mod_text:    Byte; cvar; external;
  dialog_car_text:    Byte; cvar; external;
  dialog_misc_indic:  Byte; cvar; external;

  macro_background:   Byte; cvar; external;
  macro_title:        Byte; cvar; external;
  macro_border:       Byte; cvar; external;
  macro_topic:        Byte; cvar; external;
  macro_topic2:       Byte; cvar; external;
  macro_hi_topic:     Byte; cvar; external;
  macro_text:         Byte; cvar; external;
  macro_hi_text:      Byte; cvar; external;
  macro_text_dis:     Byte; cvar; external;
  macro_text_loop:    Byte; cvar; external;
  macro_text_keyoff:  Byte; cvar; external;
  macro_current_bckg: Byte; cvar; external;
  macro_current:      Byte; cvar; external;
  macro_current_dis:  Byte; cvar; external;
  macro_current_loop: Byte; cvar; external;
  macro_current_koff: Byte; cvar; external;
  macro_input_bckg:   Byte; cvar; external;
  macro_input:        Byte; cvar; external;
  macro_def_bckg:     Byte; cvar; external;
  macro_def:          Byte; cvar; external;
  macro_scrbar_bckg:  Byte; cvar; external;
  macro_scrbar_text:  Byte; cvar; external;
  macro_scrbar_mark:  Byte; cvar; external;
  macro_hint:         Byte; cvar; external;
  macro_item:         Byte; cvar; external;
  macro_short:        Byte; cvar; external;
  macro_item_dis:     Byte; cvar; external;
  macro_sel_itm_bck:  Byte; cvar; external;
  macro_sel_itm:      Byte; cvar; external;
  macro_sel_short:    Byte; cvar; external;
  macro_context:      Byte; cvar; external;
  macro_context_dis:  Byte; cvar; external;

  scrollbar_bckg:     Byte; cvar; external;
  scrollbar_text:     Byte; cvar; external;
  scrollbar_mark:     Byte; cvar; external;
  scrollbar_2nd_mark: Byte; cvar; external;

  main_background:    Byte; cvar; external;
  main_title:         Byte; cvar; external;
  main_border:        Byte; cvar; external;
  main_stat_line:     Byte; cvar; external;
  main_hi_stat_line:  Byte; cvar; external;
  main_dis_stat_line: Byte; cvar; external;
  main_behavior:      Byte; cvar; external;
  main_behavior_dis:  Byte; cvar; external;

  status_background:  Byte; cvar; external;
  status_border:      Byte; cvar; external;
  status_static_txt:  Byte; cvar; external;
  status_dynamic_txt: Byte; cvar; external;
  status_play_state:  Byte; cvar; external;
  status_text_dis:    Byte; cvar; external;

  order_background:   Byte; cvar; external;
  order_hi_bckg:      Byte; cvar; external;
  order_border:       Byte; cvar; external;
  order_entry:        Byte; cvar; external;
  order_hi_entry:     Byte; cvar; external;
  order_pattn:        Byte; cvar; external;
  order_pattn_jump:   Byte; cvar; external;
  order_hi_pattn:     Byte; cvar; external;
  order_played_b:     Byte; cvar; external;
  order_played:       Byte; cvar; external;
  order_input_bckg:   Byte; cvar; external;
  order_input:        Byte; cvar; external;
  order_input_warn:   Byte; cvar; external;

  instrument_bckg:    Byte; cvar; external;
  instrument_title:   Byte; cvar; external;
  instrument_border:  Byte; cvar; external;
  instrument_text:    Byte; cvar; external;
  instrument_hi_text: Byte; cvar; external;
  instrument_glob:    Byte; cvar; external;
  instrument_hi_glob: Byte; cvar; external;
  instrument_hid:     Byte; cvar; external;
  instrument_mod:     Byte; cvar; external;
  instrument_car:     Byte; cvar; external;
  instrument_hi_mod:  Byte; cvar; external;
  instrument_hi_car:  Byte; cvar; external;
  instrument_context: Byte; cvar; external;
  instrument_con_dis: Byte; cvar; external;
  instrument_adsr:    Byte; cvar; external;
  instrument_ai_off:  Byte; cvar; external;
  instrument_ai_on:   Byte; cvar; external;
  instrument_ai_trig: Byte; cvar; external;

{$ENDIF} // NOT DEFINED(ADT2PLAY)

type
{$IFDEF ADT2PLAY}
  tCHAR8x8  = array[char] of array[0..7] of Byte;
  tCHAR8x16 = array[char] of array[0..15] of Byte;
{$ENDIF} // DEFINED(ADT2PLAY)

{$IFDEF GO32V2}
  tRGB = Record r,g,b: Byte end;
{$ELSE} // NOT DEFINED(GO32V2)
  tRGB = Record r,g,b,a: Byte end;
{$ENDIF} // NOT DEFINED(GO32V2)

{$IFDEF ADT2PLAY}

type
  tRGB_PALETTE = array[0..255] of tRGB;

{$ELSE} // NOT DEFINED(ADT2PLAY)

var
  rgb_color: array[0..15] of tRGB; cvar; external;
{$IFNDEF GO32V2}
  vga_rgb_color: array[0..15] of tRGB; cvar; external; //const
{$ENDIF} // NOT DEFINED(GO32V2)

{$ENDIF} // NOT DEFINED(ADT2PLAY)
