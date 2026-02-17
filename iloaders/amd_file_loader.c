// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#define AMD_VERSION_RAW    16
#define AMD_VERSION_PACKED 17

#pragma pack(push, 1)
typedef struct
{
  char name[23];    // ASCIIZ
  uint8_t data[11];
} tAMD_INS;

typedef struct
{
  char title[24];       // ASCIIZ
  char composer[24];    // ASCIIZ
  tAMD_INS instruments[26];
  uint8_t length;       // song length (see `order')
  uint8_t patterns;     // number of patterns -1
  uint8_t order[0x80];  // pattern table
  char id[9];
  uint8_t version;
} tAMD_HEADER;
#pragma pack(pop)

static const char GCC_ATTRIBUTE((nonstring)) id_amd[9] = { "<o\xEFQU\xEERoR" };
static const char GCC_ATTRIBUTE((nonstring)) id_xms[9] = { "MaDoKaN96" };

static void import_amd_instrument (tADTRACK2_INS *ins, const void *data)
{
  ins->fm_data.AM_VIB_EG_modulator = ((uint8_t *)data)[0];
  ins->fm_data.KSL_VOLUM_modulator = ((uint8_t *)data)[1];
  ins->fm_data.ATTCK_DEC_modulator = ((uint8_t *)data)[2];
  ins->fm_data.SUSTN_REL_modulator = ((uint8_t *)data)[3];
  ins->fm_data.WAVEFORM_modulator  = ((uint8_t *)data)[4] & 3;
  ins->fm_data.AM_VIB_EG_carrier   = ((uint8_t *)data)[5];
  ins->fm_data.KSL_VOLUM_carrier   = ((uint8_t *)data)[6];
  ins->fm_data.ATTCK_DEC_carrier   = ((uint8_t *)data)[7];
  ins->fm_data.SUSTN_REL_carrier   = ((uint8_t *)data)[8];
  ins->fm_data.WAVEFORM_carrier    = ((uint8_t *)data)[9] & 3;
  ins->fm_data.FEEDBACK_FM         = ((uint8_t *)data)[10] & 0x0F;

  ins->panning = 0;
  ins->fine_tune = 0;
}

static void import_amd_event (tCHUNK *chunk, uint8_t a, uint8_t b, uint8_t param)
{
  memset (chunk, 0, sizeof (*chunk));

  chunk->instr_def = (b >> 4) + ((a & 1) << 4); // 1..31, 0=none

  if (((a >> 4) >= 1) && ((a >> 4) <= 12))
    chunk->note = 12 * ((a >> 1) & 7) + (a >> 4); // 1..96, 0=none

  param &= 0x7F;
  switch (b & 0x0F)
  {
    case 0: // ARPEGGIO
      chunk->effect_def = ef_Arpeggio;
      chunk->effect = dec2hex (param);
      break;

    case 1: // SLIDE FREQUENCY UP
      chunk->effect_def = ef_FSlideUp;
      chunk->effect = param;
      break;

    case 2: // SLIDE FREQUENCY DOWN
      chunk->effect_def = ef_FSlideDown;
      chunk->effect = param;
      break;

    case 3: // SET CARRIER/MODULATOR INTENSITY
      if ((param >= 10) && (param <= 99))
      {
        chunk->effect_def = ef_SetCarrierVol;
        chunk->effect = (param / 10) * 7;
      }
      else if ((param % 10) != 0)
      {
        chunk->effect_def = ef_SetModulatorVol;
        chunk->effect = (param % 10) * 7;
      }
      break;

    case 4: // SET THE VOLUME
      chunk->effect_def = ef_SetInsVolume;
      chunk->effect = param <= 63 ? param : 63;
      break;

    case 5: // JUMP INTO PATTERN
      chunk->effect_def = ef_PositionJump;
      chunk->effect = param <= 99 ? param : 99;
      break;

    case 6: // PATTERNBREAK
      chunk->effect_def = ef_PatternBreak;
      chunk->effect = param <= 63 ? param : 63;
      break;

    case 7: // SET SONGSPEED
      if (param < 99)
      {
        if ((param >= 1) && (param <= 31))
        {
          chunk->effect_def = ef_SetSpeed;
          chunk->effect = param;
        }
        else
        {
          chunk->effect_def = ef_SetTempo;
          chunk->effect = param == 0 ? 18 : param;
        }
      }
      break;

    case 8: // TONEPORTAMENTO
      chunk->effect_def = ef_TonePortamento;
      chunk->effect = param;
      break;

    case 9: // EXTENDED COMMAND
      switch (param / 10)
      {
        case 0: // DEFINE CELL-TREMOLO
          if ((param % 10) <= 1)
          {
            chunk->effect_def = ef_Extended;
            chunk->effect = dec2hex (param);
          }
          break;

        case 1: // DEFINE CELL-VIBRATO
          if ((param % 10) <= 1)
          {
            chunk->effect_def = ef_Extended;
            chunk->effect = 0x10 + dec2hex (param);
          }
          break;

        case 2: // INCREASE VOLUME FAST
          chunk->effect_def = ef_VolSlide;
          chunk->effect = (param % 10) * 16;
          break;

        case 3: // DECREASE VOLUME FAST
          chunk->effect_def = ef_VolSlide;
          chunk->effect = param % 10;
          break;

        case 4: // INCREASE VOLUME FINE
          chunk->effect_def = ef_Extended2;
          chunk->effect = ef_ex2_VolSlideUpXF * 16 + (param % 10);
          break;

        case 5: // DECREASE VOLUME FINE
          chunk->effect_def = ef_Extended2;
          chunk->effect = ef_ex2_VolSlideDnXF * 16 + (param % 10);
          break;

        default:
          break;
      }
      break;

    default:
      break;
  }

  // specific corrections for Amusic event
  if (chunk->note == 0) chunk->instr_def = 0;
}

static void next_amd_step (progress_callback_t *progress)
{
  if (progress != NULL)
  {
    progress->step++;
    progress->value = 1;
    progress->update (progress, 1, -1);
  }
}

// In: `progress' may be NULL.
// Returns `false' on success and `true' on error.
static bool import_amd_raw_patterns (int patterns, FILE *f,
                                     progress_callback_t *progress, char **error)
{
  bool result = true;
  void *data = NULL;
  size_t size = 64 * 9 * 3 * patterns;
  uint8_t (*event)[3];

  if ((data = malloc (size)) == NULL) goto _err_malloc;
  if (fread (data, size, 1, f) == 0) goto _err_fread;

  event = data;
  for (int pat = 0; pat < patterns; pat++)
  {
    for (int line = 0; line < 64; line++)
      for (int chan = 0; chan < 9; chan++)
      {
        tCHUNK chunk;

        import_amd_event (&chunk, (*event)[2], (*event)[1], (*event)[0]);
        put_chunk (pat, line, chan + 1, &chunk);
        event++;
      }
    next_amd_step (progress);
  }

  result = false;

_exit:
  if (data != NULL) free (data);

  return result;

_err_malloc:
  *error = "Memory allocation failed";
  goto _exit;

_err_fread:
  *error = "Failed to read input file";
  goto _exit;
}

// In: `progress' may be NULL.
// Returns `false' on success and `true' on error.
static bool import_amd_packed_patterns (int patterns, FILE *f,
                                        progress_callback_t *progress, char **error)
{
  bool result = true;
  uint16_t (*track_order)[1][9] = NULL;
  size_t size = sizeof (*track_order) * patterns;
  tCHUNK (*track)[64] = NULL;
  uint16_t tracks;
  uint_least8_t line = 0, count = 0;
  uint_least16_t pattern = 0xFFFF;
  uint_least8_t channel = 0xFF;

  if ((track_order = malloc (size)) == NULL) goto _err_malloc;
  if ((track = malloc (sizeof (*track))) == NULL) goto _err_malloc;

  if (fread (track_order, size, 1, f) == 0) goto _err_fread;
  if (fread (&tracks, 2, 1, f) == 0) goto _err_fread;
  tracks = uint16_LE (tracks);

  for (uint_least16_t i = 0; i < tracks;)
  {
    if (count == 0)
    {
      uint8_t event[3];

      if (line == 0)
      {
        uint16_t order;

        if (fread (&order, 2, 1, f) == 0) goto _err_fread;
        order = uint16_LE (order);
        pattern = order / 9;
        channel = order % 9;
      }

      if (fread (event, 1, 1, f) == 0) goto _err_fread;
      if ((event[0] & 0x80) == 0)
      {
        if (pattern < 64)
        {
          tCHUNK chunk;

          if (fread (&event[1], 2, 1, f) == 0) goto _err_fread;
          import_amd_event (&chunk, event[2], event[1], event[0]);
          put_chunk (pattern, line, channel + 1, &chunk);
        }
        else
        {
          if (fseek (f, 2, SEEK_CUR) != 0) goto _err_fread;
        }
      }
      else
        count = (event[0] & 0x7F) - 1;
    }
    else count--;

    if (++line == 64)
    {
      line = 0;
      count = 0;
      i++;
    }
  }

  for (int dst_pat = 0; dst_pat < patterns; dst_pat++)
  {
    for (int dst_chan = 0; dst_chan < 9; dst_chan++)
    {
      uint16_t order = uint16_LE ((*track_order)[dst_pat][dst_chan]);

      if (order < 64 * 9)
      {
        pattern = order / 9;
        channel = order % 9;

        for (line = 0; line < 64; line++) get_chunk (pattern, line, channel + 1, &(*track)[line]);
        for (line = 0; line < 64; line++) put_chunk (dst_pat, line, dst_chan + 1, &(*track)[line]);
      }
    }
    next_amd_step (progress);
  }

  result = false;

_exit:
  if (track_order != NULL) free (track_order);
  if (track != NULL) free (track);

  return result;

_err_malloc:
  *error = "Memory allocation failed";
  goto _exit;

_err_fread:
  *error = "Failed to read input file";
  goto _exit;
}

// In:
//   * `progress', `state' and `error' may be NULL.
//
// On success:
//   * Return value:
//     0: ID is AMD.
//     1: ID is XMS.
//   * `state' and `error' are untouched.
// On error:
//   * Return value:
//     -1: open/read error.
//     -2: unknown file format.
//     -3: unsupported file format version.
//     -4: format is supported.
//   * `state' (if set):
//     0: song is untouched.
//     1: song is cleared.
//     2: song is partly loaded.
//   * `error' (if set) is set to error description.
int8_t amd_file_loader (const String *_fname, progress_callback_t *progress, uint8_t *state, char **error)
{
  tFIXED_SONGDATA *song = &songdata;
  int8_t result = -1;
  uint8_t result_state = 0;
  char *result_error = NULL;
  FILE *f = NULL;
  tAMD_HEADER header;
  bool is_amd;
  char fname[255+1];

  DBG_ENTER ("amd_file_loader");

  if (progress != NULL)
  {
    snprintf (progress->msg, sizeof (progress->msg), "Reading file...");
    progress->value = 1;
    progress->update (progress, 0, -1);
  }

  StringToStr (fname, _fname, sizeof (fname) - 1);
  if ((f = fopen (fname, "rb")) == NULL) goto _err_fopen;

  if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;

  result = -2;

  is_amd = memcmp (header.id, id_amd, sizeof (header.id)) == 0;
  if (!is_amd && (memcmp (header.id, id_xms, sizeof (header.id)) != 0)) goto _err_format;

  result = -3;

  if ((header.version != AMD_VERSION_RAW) && (header.version != AMD_VERSION_PACKED)) goto _err_version;

  result = -4;

  if (progress != NULL) progress->num_steps = header.patterns + 1;

  init_songdata ();
  song->patt_len = 64;
  if (adjust_tracks || (song->nm_tracks < 9)) song->nm_tracks = 9;
  result_state = 1;

  tempo = 50;
  speed = 6;
  song->tempo = tempo;
  song->speed = speed;

  // import patterns order
  for (unsigned i = 0; i < header.length; i++)
    if ((i < 0x80) && (header.order[i] <= header.patterns))
    {
      song->pattern_order[i] = header.order[i];
      result_state = 2;
    }

  // import instrument names
  for (int i = 0; i < 26; i++)
  {
    String_t s, t;

    import_amd_instrument (&song->instr_data[i], header.instruments[i].data);
    StrToString ((String *)&s, header.instruments[i].name, sizeof (header.instruments[0].name));
    t = truncate_string ((String *)&s);
    s = Copy (song->instr_names[i], 1, 9);
    AppendString ((String *)&s, (String *)&t, sizeof (s) - 1);
    CopyString (song->instr_names[i], (String *)&s, sizeof (song->instr_names[0]));
  }
  result_state = 2;

  // import patterns
  if (header.version == AMD_VERSION_RAW)
  {
    if (import_amd_raw_patterns (header.patterns + 1, f, progress, &result_error)) goto _exit;
  }
  else
  {
    if (import_amd_packed_patterns (header.patterns + 1, f, progress, &result_error)) goto _exit;
  }

  song->common_flag |= 0x80;
  {
    String_t s, t;

    StrToString ((String *)&s, header.title, sizeof (header.title));
    t = asciiz_string ((String *)&s);
    s = CutStr ((String *)&t);
    CopyString (song->songname, (String *)&s, sizeof (song->songname) - 1);

    StrToString ((String *)&s, header.composer, sizeof (header.composer));
    t = asciiz_string ((String *)&s);
    s = CutStr ((String *)&t);
    CopyString (song->composer, (String *)&s, sizeof (song->composer) - 1);
  }
  import_old_flags ();
  {
    String_t s = NameOnly (_fname);
    CopyString (songdata_title, (String *)&s, sizeof (songdata_title) - 1);
  }
  result = is_amd ? 0 : 1;

_exit:
  if (f != NULL) fclose (f);
  if (result < 0)
  {
    if (state != NULL) *state = result_state;
    if (error != NULL) *error = result_error;
  }

  DBG_LEAVE (); //EXIT //amd_file_loader
  return result;

_err_fopen:
  result_error = "Failed to open input file";
  goto _exit;

_err_fread:
  result_error = "Failed to read input file";
  goto _exit;

_err_format:
  result_error = "Unknown file format";
  goto _exit;

_err_version:
  result_error = "Unsupported file format version";
  goto _exit;
}
