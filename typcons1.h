// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#define UINT8_NULL 0xFF
#define UINT16_NULL 0xFFFF
#define UINT32_NULL 0xFFFFFFFF
#define INT8_NULL (-1)
#define INT16_NULL (-1)
#define INT32_NULL (-1)
#define BYTE_NULL UINT8_NULL
#define WORD_NULL UINT16_NULL
#define DWORD_NULL UINT32_NULL

#define FFVER_A2M 14
#define FFVER_A2T 14
#define FFVER_A2I 10
#define FFVER_A2F 2
#define FFVER_A2P 11
#define FFVER_A2B 10
#define FFVER_A2W 3

#define GENERIC_IO_BUFFER_SIZE 1500*1024 // 1.5MB I/O Buffer

typedef uint8_t tGENERIC_IO_BUFFER[GENERIC_IO_BUFFER_SIZE]; // HINT: (FPC) start index 0

#pragma pack(push, 1)

typedef struct {
  uint8_t AM_VIB_EG_modulator;
  uint8_t AM_VIB_EG_carrier;
  uint8_t KSL_VOLUM_modulator;
  uint8_t KSL_VOLUM_carrier;
  uint8_t ATTCK_DEC_modulator;
  uint8_t ATTCK_DEC_carrier;
  uint8_t SUSTN_REL_modulator;
  uint8_t SUSTN_REL_carrier;
  uint8_t WAVEFORM_modulator;
  uint8_t WAVEFORM_carrier;
  uint8_t FEEDBACK_FM;
} tFM_INST_DATA;

typedef struct {
  tFM_INST_DATA fm_data;
  uint8_t panning;
  int8_t fine_tune;
  uint8_t perc_voice;
} tADTRACK2_INS;

typedef struct {
  tFM_INST_DATA fm_data;
  uint8_t panning;
  int8_t fine_tune;
} tOLD_ADTRACK2_INS;

typedef struct {
  uint8_t length;
  uint8_t speed;
  uint8_t loop_begin;
  uint8_t loop_length;
  uint8_t keyoff_pos;
  uint8_t data[255]; // HINT: (FPC) start index 1
} tARPEGGIO_TABLE;

typedef struct {
   uint8_t length;
   uint8_t speed;
   uint8_t delay;
   uint8_t loop_begin;
   uint8_t loop_length;
   uint8_t keyoff_pos;
   int8_t data[255]; // HINT: (FPC) start index 1
} tVIBRATO_TABLE;

typedef struct {
  tFM_INST_DATA fm_data;
  int8_t freq_slide;
  uint8_t panning;
  uint8_t duration;
} tREGISTER_TABLE_DEF;

typedef struct {
  uint8_t length;
  uint8_t loop_begin;
  uint8_t loop_length;
  uint8_t keyoff_pos;
  uint8_t arpeggio_table;
  uint8_t vibrato_table;
  tREGISTER_TABLE_DEF data[255]; // HINT: (FPC) start index 1
} tREGISTER_TABLE;

typedef struct {
  tARPEGGIO_TABLE arpeggio;
  tVIBRATO_TABLE vibrato;
} tMACRO_TABLE;

typedef tMACRO_TABLE tARP_VIB_MACRO_TABLE[255]; // HINT: (FPC) start index 1

typedef struct {
  struct {
    uint8_t attck, dec, sustn, rel;
    uint8_t wform;
  } adsrw_car,
    adsrw_mod;
  uint8_t connect;
  uint8_t feedb;
  uint8_t multipM, kslM, tremM, vibrM, ksrM, sustM;
  uint8_t multipC, kslC, tremC, vibrC, ksrC, sustC;
} tFM_PARAMETER_TABLE;

typedef bool tDIS_FMREG_COL[28]; // HINT: (FPC) start index 0

typedef struct {
  uint8_t num_4op;
  uint8_t idx_4op[128]; // HINT: (FPC) start index 1
} tINS_4OP_FLAGS;

typedef struct {
  String songname[42+1];
  String composer[42+1];
  String instr_names[250][32+1]; // HINT: (FPC) start index 1
  tOLD_ADTRACK2_INS instr_data[250]; // HINT: (FPC) start index 1
  uint8_t pattern_order[128]; // HINT: (FPC) start index 0
  uint8_t tempo;
  uint8_t speed;
  uint8_t common_flag;
} tOLD_FIXED_SONGDATA;

#define MB_SIGNATURE MK_UINT32 ('M', 'B', '_', '1') // Message Board v1.0
#define MB_HSIZE 50 // 50x20 chr
#define MB_VSIZE 20

typedef struct {
  uint32_t signature;
  String data[MB_VSIZE][MB_HSIZE+1]; // HINT: (FPC) start index 1
} tMESSAGE_BOARD_DATA;

typedef struct {
  String songname[42+1];
  String composer[42+1];
  String instr_names[255][42+1]; // HINT: (FPC) start index 1
  tADTRACK2_INS instr_data[255]; // HINT: (FPC) start index 1
  tREGISTER_TABLE instr_macros[255]; // HINT: (FPC) start index 1
  tARP_VIB_MACRO_TABLE macro_table;
  uint8_t pattern_order[0x80]; // HINT: (FPC) start index 0
  uint8_t tempo;
  uint8_t speed;
  uint8_t common_flag;
  uint16_t patt_len;
  uint8_t nm_tracks;
  uint16_t macro_speedup;
  uint8_t flag_4op;
  uint8_t lock_flags[20]; // HINT: (FPC) start index 1
  String pattern_names[0x80][42+1]; // HINT: (FPC) start index 0
  tDIS_FMREG_COL dis_fmreg_col[255]; // HINT: (FPC) start index 1
  tINS_4OP_FLAGS ins_4op_flags;
  uint8_t reserved_data[1024]; // HINT: (FPC) start index 0
  struct {
    uint8_t rows_per_beat;
    int16_t tempo_finetune;
  } bpm_data;
} tFIXED_SONGDATA;

typedef struct {
  uint8_t note;
  uint8_t instr_def;
  uint8_t effect_def;
  uint8_t effect;
} tOLD_CHUNK;

typedef struct {
  uint8_t note;
  uint8_t instr_def;
  uint8_t effect_def;
  uint8_t effect;
  uint8_t effect_def2;
  uint8_t effect2;
} tCHUNK;

typedef tOLD_CHUNK tOLD_VARIABLE_DATA1[0x10][0x40][9]; // HINT: (FPC) start index 0,0,1

typedef tOLD_CHUNK tOLD_VARIABLE_DATA2[8][18][0x40]; // HINT: (FPC) start index 0,1,0

typedef tCHUNK tVARIABLE_DATA[8][20][0x100]; // HINT: (FPC) start index 0,1,0

typedef tVARIABLE_DATA tPATTERN_DATA[16]; // HINT: (FPC) start index 0

typedef enum {
  objNone,
  objPatternDef,
  objPatternTable,
  objInstrument,
  objInstrumentBank,
  objNote,
  objInstrumentDef,
  objEffect,
  objEffect2,
  objLine,
  objTrack,
  objPattern,
  objMarkedBlock,
  objMacroTableLine,
  objMacroTableColumn,
  objMacroTable,
  objInstrumentWithMacros
} COPY_OBJECT_enum;
typedef uint8_t tCOPY_OBJECT; // holds COPY_OBJECT_enum

typedef enum {
  mttFM_reg_table,
  mttArpeggio_table,
  mttVibrato_table
} MACRO_TABLE_TYPE_enum;
typedef uint8_t tMACRO_TABLE_TYPE; // holds MACRO_TABLE_TYPE_enum

typedef struct {
  tCOPY_OBJECT object_type;
  uint8_t block_hsize;
  uint8_t block_vsize;
  uint8_t pattern_order[0x80]; // HINT: (FPC) start index 0
  tCHUNK pattern[20][0x100]; // HINT: (FPC) start index 1,0
  struct {
    String names[255][32+1]; // HINT: (FPC) start index 1
    tADTRACK2_INS data[255]; // HINT: (FPC) start index 1
    tREGISTER_TABLE macros[255]; // HINT: (FPC) start index 1
    tMACRO_TABLE arpvib[255]; // HINT: (FPC) start index 1
    tDIS_FMREG_COL dis_fmreg_col[255]; // HINT: (FPC) start index 1
  } instrument;
  tMACRO_TABLE_TYPE mcrtab_type;
  tREGISTER_TABLE fmreg_table;
  tMACRO_TABLE macro_table;
  uint8_t fmtab_def_typ;
  String _string[255+1];
} tCLIPBOARD;

typedef struct {
  int16_t dir1;
  int16_t dir2;
  double lvl1;
  double lvl2;
} tDECAY_BAR;

typedef struct {
  int16_t dir;
  double lvl;
} tVOLUM_BAR;

typedef enum {
  isPlaying,
  isPaused,
  isStopped
} PLAY_STATUS_enum;
typedef uint8_t tPLAY_STATUS; // holds PLAY_STATUS_enum

typedef struct {
#if GO32
  uint8_t r, g, b;
#else // !GO32
  uint8_t r, g, b, a;
#endif // !GO32
} tRGB;

typedef uint8_t tByteSet[32]; // HINT: (FPC) set of byte

#define INSTRUMENT_SIZE (sizeof (tADTRACK2_INS))
#define CHUNK_SIZE (sizeof (tCHUNK))
#define PATTERN_SIZE (20 * 256 * CHUNK_SIZE)

#pragma pack(pop)

extern String status_layout[3][10+1]; // HINT: (FPC) start index 0 (see tPLAY_STATUS)

extern const char PATHSEP;
extern const String WILDCARD_ASTERISK[];

#define inst_vpos_max 21
#define inst_hpos_max 7

extern uint8_t inst_hpos[inst_vpos_max][inst_hpos_max]; // HINT: (FPC) start index 1,1
extern const uint8_t inst_hpos_ext_2op[6][inst_hpos_max]; // HINT: (FPC) start index 1,1
extern const uint8_t inst_hpos_ext_perc[6][inst_hpos_max]; // HINT: (FPC) start index 1,1
extern const uint8_t inst_hpos_ext_4op[6][inst_hpos_max]; // HINT: (FPC) start index 1,1
extern const uint8_t inst_vpos[inst_vpos_max]; // HINT: (FPC) start index 1

extern String note_layout[12*8+2][3+1]; // HINT: (FPC) start index 0

// TODO: replace scancodes (GO32-only) in `board_scancodes' with keys (all)
extern const uint8_t board_scancodes[29]; // HINT: (FPC) start index 1

extern const char fx_digits[48]; // HINT: (FPC) start index 0

#define NM_FX_DIGITS (sizeof (fx_digits))

extern char b_note;
extern const String connection_str[2][2+1]; // HINT: (FPC) start index 0
extern const String macro_retrig_str[8][1+1]; // HINT: (FPC) start index 0, original string size was 255

extern const uint8_t pos1[22]; // HINT: (FPC) start index 1
extern const uint8_t pos2[13]; // HINT: (FPC) start index 1
extern const uint8_t pos3[44]; // HINT: (FPC) start index 1
extern const uint8_t pos4[110]; // HINT: (FPC) start index 1
extern const uint8_t pos5[35]; // HINT: (FPC) start index 1
extern const uint8_t pos5vw[35]; // HINT: (FPC) start index 1
extern const uint8_t pos6[11]; // HINT: (FPC) start index 1

extern CharSet_t _valid_characters; // HINT: (FPC) set of char
extern CharSet_t _valid_characters_fname; // HINT: (FPC) set of char

extern const uint8_t _panning[3]; // HINT: (FPC) start index 0

extern const uint8_t _instr_data_ofs[13]; // HINT: (FPC) start index 1
extern const uint8_t _instr[12]; // HINT: (FPC) start index 0

typedef uint16_t tTRACK_ADDR[20]; // HINT: (FPC) start index 1

extern const tTRACK_ADDR _chmm_n; // HINT: (FPC) start index 1
extern const tTRACK_ADDR _chmm_m; // HINT: (FPC) start index 1
extern const tTRACK_ADDR _chmm_c; // HINT: (FPC) start index 1

extern const tTRACK_ADDR _chpm_n; // HINT: (FPC) start index 1
extern const tTRACK_ADDR _chpm_m; // HINT: (FPC) start index 1
extern const tTRACK_ADDR _chpm_c; // HINT: (FPC) start index 1

extern const uint8_t _4op_tracks_hi[256/8]; // HINT: (FPC) set of byte
extern const uint8_t _4op_tracks_lo[256/8]; // HINT: (FPC) set of byte

extern const uint8_t _4op_main_chan[6]; // HINT: (FPC) start index 1
extern const uint8_t _perc_sim_chan[2]; // HINT: (FPC) start index 19

extern tTRACK_ADDR _chan_n; // HINT: (FPC) start index 1
extern tTRACK_ADDR _chan_m; // HINT: (FPC) start index 1
extern tTRACK_ADDR _chan_c; // HINT: (FPC) start index 1
extern tTRACK_ADDR _ch_tmp; // HINT: (FPC) start index 1

extern const uint8_t fade_delay_tab[64]; // HINT: (FPC) start index 0

extern bool tracing;
extern bool track_notes;
extern bool marking;
extern bool debugging;
extern bool quick_cmd;

#define ef_Arpeggio           0
#define ef_FSlideUp           1
#define ef_FSlideDown         2
#define ef_TonePortamento     3
#define ef_Vibrato            4
#define ef_TPortamVolSlide    5
#define ef_VibratoVolSlide    6
#define ef_FSlideUpFine       7
#define ef_FSlideDownFine     8
#define ef_SetModulatorVol    9
#define ef_VolSlide           10
#define ef_PositionJump       11
#define ef_SetInsVolume       12
#define ef_PatternBreak       13
#define ef_SetTempo           14
#define ef_SetSpeed           15
#define ef_TPortamVSlideFine  16
#define ef_VibratoVSlideFine  17
#define ef_SetCarrierVol      18
#define ef_SetWaveform        19
#define ef_VolSlideFine       20
#define ef_RetrigNote         21
#define ef_Tremolo            22
#define ef_Tremor             23
#define ef_ArpggVSlide        24
#define ef_ArpggVSlideFine    25
#define ef_MultiRetrigNote    26
#define ef_FSlideUpVSlide     27
#define ef_FSlideDownVSlide   28
#define ef_FSlUpFineVSlide    29
#define ef_FSlDownFineVSlide  30
#define ef_FSlUpVSlF          31
#define ef_FSlDownVSlF        32
#define ef_FSlUpFineVSlF      33
#define ef_FSlDownFineVSlF    34
#define ef_Extended           35
#define ef_Extended2          36
#define ef_SetGlobalVolume    37
#define ef_SwapArpeggio       38
#define ef_SwapVibrato        39
#define ef_ForceInsVolume     40
#define ef_Extended3          41
#define ef_ExtraFineArpeggio  42
#define ef_ExtraFineVibrato   43
#define ef_ExtraFineTremolo   44
#define ef_SetCustomSpeedTab  45
#define ef_GlobalFSlideUp     46
#define ef_GlobalFSlideDown   47
#define ef_ex_SetTremDepth    0
#define ef_ex_SetVibDepth     1
#define ef_ex_SetAttckRateM   2
#define ef_ex_SetDecayRateM   3
#define ef_ex_SetSustnLevelM  4
#define ef_ex_SetRelRateM     5
#define ef_ex_SetAttckRateC   6
#define ef_ex_SetDecayRateC   7
#define ef_ex_SetSustnLevelC  8
#define ef_ex_SetRelRateC     9
#define ef_ex_SetFeedback     10
#define ef_ex_SetPanningPos   11
#define ef_ex_PatternLoop     12
#define ef_ex_PatternLoopRec  13
#define ef_ex_ExtendedCmd     14
#define ef_ex_cmd_MKOffLoopDi 0
#define ef_ex_cmd_MKOffLoopEn 1
#define ef_ex_cmd_TPortaFKdis 2
#define ef_ex_cmd_TPortaFKenb 3
#define ef_ex_cmd_RestartEnv  4
#define ef_ex_cmd_4opVlockOff 5
#define ef_ex_cmd_4opVlockOn  6
#define ef_ex_cmd_ForceBpmSld 7
#define ef_ex_ExtendedCmd2    15
#define ef_ex_cmd2_RSS        0
#define ef_ex_cmd2_ResetVol   1
#define ef_ex_cmd2_LockVol    2
#define ef_ex_cmd2_UnlockVol  3
#define ef_ex_cmd2_LockVP     4
#define ef_ex_cmd2_UnlockVP   5
#define ef_ex_cmd2_VSlide_mod 6
#define ef_ex_cmd2_VSlide_car 7
#define ef_ex_cmd2_VSlide_def 8
#define ef_ex_cmd2_LockPan    9
#define ef_ex_cmd2_UnlockPan  10
#define ef_ex_cmd2_VibrOff    11
#define ef_ex_cmd2_TremOff    12
#define ef_ex_cmd2_FVib_FGFS  13
#define ef_ex_cmd2_FTrm_XFGFS 14
#define ef_ex_cmd2_NoRestart  15
#define ef_ex2_PatDelayFrame  0
#define ef_ex2_PatDelayRow    1
#define ef_ex2_NoteDelay      2
#define ef_ex2_NoteCut        3
#define ef_ex2_FineTuneUp     4
#define ef_ex2_FineTuneDown   5
#define ef_ex2_GlVolSlideUp   6
#define ef_ex2_GlVolSlideDn   7
#define ef_ex2_GlVolSlideUpF  8
#define ef_ex2_GlVolSlideDnF  9
#define ef_ex2_GlVolSldUpXF   10
#define ef_ex2_GlVolSldDnXF   11
#define ef_ex2_VolSlideUpXF   12
#define ef_ex2_VolSlideDnXF   13
#define ef_ex2_FreqSlideUpXF  14
#define ef_ex2_FreqSlideDnXF  15
#define ef_ex3_SetConnection  0
#define ef_ex3_SetMultipM     1
#define ef_ex3_SetKslM        2
#define ef_ex3_SetTremoloM    3
#define ef_ex3_SetVibratoM    4
#define ef_ex3_SetKsrM        5
#define ef_ex3_SetSustainM    6
#define ef_ex3_SetMultipC     7
#define ef_ex3_SetKslC        8
#define ef_ex3_SetTremoloC    9
#define ef_ex3_SetVibratoC    10
#define ef_ex3_SetKsrC        11
#define ef_ex3_SetSustainC    12

#define ef_fix1 0x80
#define ef_fix2 0x90

extern String   home_dir_path[255+1];
extern String   a2b_default_path[255+1];
extern String   a2f_default_path[255+1];
extern String   a2i_default_path[255+1];
extern String   a2m_default_path[255+1];
extern String   a2p_default_path[255+1];
extern String   a2t_default_path[255+1];
extern String   a2w_default_path[255+1];

#if GO32

extern uint8_t  screen_mode;
extern uint8_t  comp_text_mode;
extern bool     custom_svga_mode;
extern uint8_t  fps_down_factor;

extern uint8_t  typematic_rate;
extern uint8_t  typematic_delay;

extern bool     mouse_disabled;
extern uint16_t mouse_hspeed;
extern uint16_t mouse_vspeed;
extern uint16_t mouse_threshold;

#else // !GO32

extern uint8_t  sdl_screen_mode;
extern uint8_t  sdl_frame_rate;

extern int32_t  sdl_typematic_delay;
extern int32_t  sdl_typematic_rate;

extern uint8_t  sdl_opl3_emulator;
extern int32_t  sdl_sample_rate;
extern int32_t  sdl_sample_buffer;
extern uint8_t  sdl_timer_slowdown;
extern String   sdl_wav_directory[255+1];

#endif // !GO32

extern uint8_t  init_tempo;
extern uint8_t  init_speed;
extern uint16_t init_macro_speedup;
extern bool     midiboard;
extern uint8_t  default_octave;
extern uint16_t patt_len;
extern uint8_t  nm_tracks;
extern bool     mod_description;
extern bool     highlight_controls;
extern bool     use_H_for_B;
extern bool     linefeed;
extern bool     lf_in_mboard_mode;
extern bool     update_ins;
extern bool     cycle_pattern;
extern bool     keep_track_pos;
extern bool     remember_ins_pos;
extern uint8_t  command_typing;
extern uint8_t  backspace_dir;
extern uint8_t  mark_line;
extern bool     scroll_bars;
extern uint8_t  fforward_factor;
extern uint8_t  rewind_factor;
extern int32_t  ssaver_time;
extern bool     timer_fix;
extern float    decay_bar_rise;
extern float    decay_bar_fall;
extern uint8_t  force_ins;
extern uint8_t  pattern_layout;
extern bool     trace_by_default;
extern bool     nosync_by_default;
extern bool     keep_position;
extern bool     alt_ins_name;
extern bool     adjust_tracks;
extern bool     mark_lines;
extern bool     use_large_cursor;
extern bool     accurate_conv;
extern bool     fix_c_note_bug;

extern uint8_t  analyzer_bckg;
extern uint8_t  analyzer;
extern uint8_t  analyzer_ovrllvol;
extern uint8_t  analyzer_volumelvl;
extern uint8_t  analyzer_modulator;
extern uint8_t  analyzer_carrier;

extern uint8_t  pattern_bckg;
extern uint8_t  pattern_border;
extern uint8_t  pattern_pos_indic;
extern uint8_t  pattern_pan_indic;
extern uint8_t  pattern_gpan_indic;
extern uint8_t  pattern_lock_indic;
extern uint8_t  pattern_4op_indic;
extern uint8_t  pattern_perc_indic;
extern uint8_t  pattern_chan_indic;
extern uint8_t  pattern_row_bckg;
extern uint8_t  pattern_row_bckg_p;
extern uint8_t  pattern_row_bckg_m;
extern uint8_t  pattern_block_bckg;
extern uint8_t  pattern_line;
extern uint8_t  pattern_line_p;
extern uint8_t  pattern_line_m;
extern uint8_t  pattern_hi_line;
extern uint8_t  pattern_hi_line_m;
extern uint8_t  pattern_note;
extern uint8_t  pattern_hi_note;
extern uint8_t  pattern_note0;
extern uint8_t  pattern_hi_note0;
extern uint8_t  pattern_note_hid;
extern uint8_t  pattern_hi_note_h;
extern uint8_t  pattern_inst;
extern uint8_t  pattern_hi_inst;
extern uint8_t  pattern_inst0;
extern uint8_t  pattern_hi_inst0;
extern uint8_t  pattern_cmnd;
extern uint8_t  pattern_hi_cmnd;
extern uint8_t  pattern_cmnd0;
extern uint8_t  pattern_hi_cmnd0;
extern uint8_t  pattern_note_m;
extern uint8_t  pattern_note0_m;
extern uint8_t  pattern_note_hid_m;
extern uint8_t  pattern_inst_m;
extern uint8_t  pattern_inst0_m;
extern uint8_t  pattern_cmnd_m;
extern uint8_t  pattern_cmnd0_m;
extern uint8_t  pattern_note_b;
extern uint8_t  pattern_note0_b;
extern uint8_t  pattern_note_hid_b;
extern uint8_t  pattern_inst_b;
extern uint8_t  pattern_inst0_b;
extern uint8_t  pattern_cmnd_b;
extern uint8_t  pattern_cmnd0_b;
extern uint8_t  pattern_fix_note;
extern uint8_t  pattern_hi_fx_note;
extern uint8_t  pattern_fix_note_m;
extern uint8_t  pattern_fix_note_b;
extern uint8_t  pattern_cmnd_ctrl;
extern uint8_t  pattern_input_bckg;
extern uint8_t  pattern_input;
extern uint8_t  pattern_input_warn;

extern uint8_t  debug_info_bckg;
extern uint8_t  debug_info_bckg2;
extern uint8_t  debug_info_border;
extern uint8_t  debug_info_border2;
extern uint8_t  debug_info_title;
extern uint8_t  debug_info_topic;
extern uint8_t  debug_info_txt;
extern uint8_t  debug_info_hi_txt;
extern uint8_t  debug_info_txt_hid;
extern uint8_t  debug_info_mod;
extern uint8_t  debug_info_hi_mod;
extern uint8_t  debug_info_car;
extern uint8_t  debug_info_hi_car;
extern uint8_t  debug_info_4op;
extern uint8_t  debug_info_perc;
extern uint8_t  debug_info_bpm;

extern uint8_t  help_background;
extern uint8_t  help_title;
extern uint8_t  help_border;
extern uint8_t  help_topic;
extern uint8_t  help_text;
extern uint8_t  help_hi_text;
extern uint8_t  help_keys;
extern uint8_t  help_indicators;

extern uint8_t  dialog_background;
extern uint8_t  dialog_title;
extern uint8_t  dialog_border;
extern uint8_t  dialog_text;
extern uint8_t  dialog_hi_text;
extern uint8_t  dialog_hid;
extern uint8_t  dialog_item;
extern uint8_t  dialog_short;
extern uint8_t  dialog_item_dis;
extern uint8_t  dialog_sel_itm_bck;
extern uint8_t  dialog_sel_itm;
extern uint8_t  dialog_sel_short;
extern uint8_t  dialog_context;
extern uint8_t  dialog_context_dis;
extern uint8_t  dialog_contxt_dis2;
extern uint8_t  dialog_input_bckg;
extern uint8_t  dialog_input;
extern uint8_t  dialog_def_bckg;
extern uint8_t  dialog_def;
extern uint8_t  dialog_prog_bar1;
extern uint8_t  dialog_prog_bar2;
extern uint8_t  dialog_topic;
extern uint8_t  dialog_hi_topic;
extern uint8_t  dialog_mod_text;
extern uint8_t  dialog_car_text;
extern uint8_t  dialog_misc_indic;

extern uint8_t  macro_background;
extern uint8_t  macro_title;
extern uint8_t  macro_border;
extern uint8_t  macro_topic;
extern uint8_t  macro_topic2;
extern uint8_t  macro_hi_topic;
extern uint8_t  macro_text;
extern uint8_t  macro_hi_text;
extern uint8_t  macro_text_dis;
extern uint8_t  macro_text_loop;
extern uint8_t  macro_text_keyoff;
extern uint8_t  macro_current_bckg;
extern uint8_t  macro_current;
extern uint8_t  macro_current_dis;
extern uint8_t  macro_current_loop;
extern uint8_t  macro_current_koff;
extern uint8_t  macro_input_bckg;
extern uint8_t  macro_input;
extern uint8_t  macro_def_bckg;
extern uint8_t  macro_def;
extern uint8_t  macro_scrbar_bckg;
extern uint8_t  macro_scrbar_text;
extern uint8_t  macro_scrbar_mark;
extern uint8_t  macro_hint;
extern uint8_t  macro_item;
extern uint8_t  macro_short;
extern uint8_t  macro_item_dis;
extern uint8_t  macro_sel_itm_bck;
extern uint8_t  macro_sel_itm;
extern uint8_t  macro_sel_short;
extern uint8_t  macro_context;
extern uint8_t  macro_context_dis;

extern uint8_t  scrollbar_bckg;
extern uint8_t  scrollbar_text;
extern uint8_t  scrollbar_mark;
extern uint8_t  scrollbar_2nd_mark;

extern uint8_t  main_background;
extern uint8_t  main_title;
extern uint8_t  main_border;
extern uint8_t  main_stat_line;
extern uint8_t  main_hi_stat_line;
extern uint8_t  main_dis_stat_line;
extern uint8_t  main_behavior;
extern uint8_t  main_behavior_dis;

extern uint8_t  status_background;
extern uint8_t  status_border;
extern uint8_t  status_static_txt;
extern uint8_t  status_dynamic_txt;
extern uint8_t  status_play_state;
extern uint8_t  status_text_dis;

extern uint8_t  order_background;
extern uint8_t  order_hi_bckg;
extern uint8_t  order_border;
extern uint8_t  order_entry;
extern uint8_t  order_hi_entry;
extern uint8_t  order_pattn;
extern uint8_t  order_pattn_jump;
extern uint8_t  order_hi_pattn;
extern uint8_t  order_played_b;
extern uint8_t  order_played;
extern uint8_t  order_input_bckg;
extern uint8_t  order_input;
extern uint8_t  order_input_warn;

extern uint8_t  instrument_bckg;
extern uint8_t  instrument_title;
extern uint8_t  instrument_border;
extern uint8_t  instrument_text;
extern uint8_t  instrument_hi_text;
extern uint8_t  instrument_glob;
extern uint8_t  instrument_hi_glob;
extern uint8_t  instrument_hid;
extern uint8_t  instrument_mod;
extern uint8_t  instrument_car;
extern uint8_t  instrument_hi_mod;
extern uint8_t  instrument_hi_car;
extern uint8_t  instrument_context;
extern uint8_t  instrument_con_dis;
extern uint8_t  instrument_adsr;
extern uint8_t  instrument_ai_off;
extern uint8_t  instrument_ai_on;
extern uint8_t  instrument_ai_trig;

extern tRGB rgb_color[16]; // HINT: (FPC) start index 0

#if !GO32
extern const tRGB vga_rgb_color[16]; // HINT: (FPC) start index 0
#endif // !GO32
