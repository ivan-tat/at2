// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Digital-FM Tracker song loader (*.dfm)

#define DFM_INSTRUMENTS_MAX 32
#define DFM_CHANNELS_MAX 9
#define DFM_PATTERN_LEN 64
#define DFM_PATTERNS_MAX 128
#define DFM_ORDER_LEN 128

#define DFM_ID_SIZE 4
static const char GCC_ATTRIBUTE((nonstring)) dfm_id[DFM_ID_SIZE] = { "DFM\x1A" };

#pragma pack(push, 1)
typedef struct
{
  char id[DFM_ID_SIZE];
  uint16_t version;     // tracker version (not used)
  String composer[32+1];
  uint8_t tempo;
  String instruments_names[DFM_INSTRUMENTS_MAX][11+1];
  tFM_INST_DATA instruments_data[DFM_INSTRUMENTS_MAX];
  uint8_t order[DFM_ORDER_LEN];
  uint8_t num_patterns; // number of patterns (not used)
} dfm_header_t;
#pragma pack(pop)

// patt: 0..DFM_PATTERNS_MAX-1
// line: 0..DFM_PATTERN_LEN-1
// chan: 0..DFM_CHANNELS_MAX-1
static void import_dfm_event (uint8_t patt, uint8_t line, uint8_t chan, uint8_t note, uint8_t fx)
{
  tCHUNK chunk;
  int semitone;

  memset (&chunk, 0, sizeof (chunk));

  semitone = note & 0x0F;
  if ((semitone >= 1) && (semitone <= 12))
    chunk.note = semitone + (note >> 4) * 12;
  else if (semitone == 15)
    chunk.note = UINT8_NULL;

  switch (fx >> 5)
  {
    case 1: // INSTRUMENT CHANGE
      chunk.instr_def = (fx & 0x1F) + 1;
      break;

    case 2: // SET INSTRUMENT VOLUME
      chunk.effect_def = ef_SetInsVolume;
      chunk.effect = (fx & 0x1F) * 2;
      break;

    case 3: // TEMPO CHANGE
      chunk.effect_def = ef_SetSpeed;
      chunk.effect = (fx & 0x1F) + 1;
      break;

    case 4: // SLIDE UP
      chunk.effect_def = ef_FSlideUpFine;
      chunk.effect = fx & 0x1F;
      break;

    case 5: // SLIDE DOWN
      chunk.effect_def = ef_FSlideDownFine;
      chunk.effect = fx & 0x1F;
      break;

    case 7: // END OF PATTERN
      chunk.effect_def = ef_PatternBreak;
      break;

    default: break;
  }

  put_chunk (patt, line, chan + 1, &chunk);
}

static void process_dfm_patterns (tFIXED_SONGDATA *song, uint8_t song_len)
{
  uint8_t instr_cache[DFM_CHANNELS_MAX];
  bool patts[DFM_PATTERNS_MAX];

  memset (instr_cache, 0, sizeof (instr_cache));
  memset (patts, false, sizeof (patts));

  for (uint_least8_t order = 0; order < song_len; order++)
  {
    uint8_t patt = song->pattern_order[order];

    if (!patts[patt])
    {
      for (uint_least8_t line = 0; line < DFM_PATTERN_LEN; line++)
        for (uint_least8_t chan = 0; chan < DFM_CHANNELS_MAX; chan++)
        {
          tCHUNK chunk;

          get_chunk (patt, line, chan + 1, &chunk);

          if (chunk.instr_def != 0)
          {
            chunk.effect_def = ef_Extended;
            chunk.effect = ef_ex_ExtendedCmd2 * 16 + ef_ex_cmd2_ResetVol;
            instr_cache[chan] = chunk.instr_def;
            if (!accurate_conv && ((chunk.note == 0) || (chunk.note > 12*8+1)))
              chunk.instr_def = 0;
          }
          else if (!accurate_conv && (chunk.note >= 1) && (chunk.note <= 12*8+1))
            chunk.instr_def = instr_cache[chan];

          put_chunk (patt, line, chan + 1, &chunk);
        }
      patts[patt] = true;
    }
  }
}

static void next_dfm_step (progress_callback_t *progress)
{
  if (progress != NULL)
  {
    progress->step++;
    progress->value = 1;
    progress->update (progress, 1, -1);
  }
}

// In:
//   * `progress', `state' and `error' may be NULL.
//
// On success:
//   * Return value is 0.
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
int8_t dfm_file_loader (const String *_fname, progress_callback_t *progress, uint8_t *state, char **error)
{
  tFIXED_SONGDATA *song = &songdata;
  int8_t result = -1;
  uint8_t result_state = 0;
  char *result_error = NULL;
  FILE *f = NULL;
  void *buffer = NULL;
  long fsize;
  dfm_header_t header;
  int_least16_t song_len;
  char fname[255+1];

  DBG_ENTER ("dfm_file_loader");

  if (progress != NULL)
  {
    snprintf (progress->msg, sizeof (progress->msg), "Reading file...");
    progress->value = 1;
    progress->update (progress, 0, -1);
  }

  StringToStr (fname, _fname, sizeof (fname) - 1);
  if ((f = fopen (fname, "rb")) == NULL) goto _err_fopen;

  // check file size
  if (fseek (f, 0, SEEK_END) != 0) goto _err_fread;
  if ((fsize = ftell (f)) < 0) goto _err_fread;
  if (   (fsize < (long)sizeof (header))
      || (fsize > (long)sizeof (header) + (1 + 2 * DFM_CHANNELS_MAX * DFM_PATTERN_LEN) * DFM_PATTERNS_MAX))
    goto _err_format;
  if (fseek (f, 0, SEEK_SET) != 0) goto _err_fread;

  if (fread (&header, sizeof (header), 1, f) == 0)  goto _err_fread;

  result = -2;

  if (memcmp (header.id, dfm_id, sizeof (header.id)) != 0) goto _err_format;

  result = -4;

  if (progress != NULL) progress->num_steps = 4;

  // initialize song
  init_songdata ();
  song->patt_len = DFM_PATTERN_LEN;
  if (adjust_tracks || (song->nm_tracks < DFM_CHANNELS_MAX)) song->nm_tracks = DFM_CHANNELS_MAX;
  tempo = 135;
  speed = header.tempo + 1;
  song->tempo = tempo;
  song->speed = speed;
  song->common_flag |= 1 | 2 | 8 | 0x10;
  import_old_flags ();
  {
    String_t s;
    if (Length (header.composer) >= sizeof (header.composer))
      SetLength (header.composer, sizeof (header.composer) - 1);
    s = CutStr (header.composer);
    CopyString (song->songname, (String *)&s, sizeof (song->songname) - 1);
    s = NameOnly (_fname);
    CopyString (songdata_title, (String *)&s, sizeof (songdata_title) - 1);
  }
  result_state = 1;
  next_dfm_step (progress);

  // import instruments
  for (uint_least8_t i = 0; i < DFM_INSTRUMENTS_MAX; i++)
  {
    String_t s, t;

    if (Length (header.instruments_names[i]) >= sizeof (header.instruments_names[0]))
      SetLength (header.instruments_names[i], sizeof (header.instruments_names[0]) - 1);

    s = Copy (song->instr_names[i], 1, 9);
    t = CutStr (header.instruments_names[i]);
    AppendString ((String *)&s, (String *)&t, sizeof (s) - 1);
    CopyString (song->instr_names[i], (String *)&s, sizeof (song->instr_names[0]) - 1);

    while (  (Length (song->instr_names[i]) != 0)
          && (GetStr (song->instr_names[i])[Length (song->instr_names[i]) - 1] < 32))
      SetLength (song->instr_names[i], Length (song->instr_names[i]) - 1);

    import_standard_instrument_alt (&song->instr_data[i], &header.instruments_data[i]);
  }
  result_state = 2;
  next_dfm_step (progress);

  // import patterns order
  song_len = -1;
  for (uint_least8_t order = 0; order < DFM_ORDER_LEN; order++)
  {
    uint8_t patt = header.order[order];

    if (patt < DFM_PATTERNS_MAX)
      song->pattern_order[order] = patt;
    else if (patt == 0x80)  // end mark
    {
      if (song_len < 0) song_len = order;
      break;
    }
    else
      song->pattern_order[order] = 0x80 + order;
  }
  next_dfm_step (progress);

  // import patterns
  if (fsize > (long)sizeof (header))
  {
    size_t size = fsize - sizeof (header);
    mem_stream_t stream;
    int_least16_t last_patt;

    if ((buffer = malloc (size)) == NULL) goto _err_malloc;

    if ((fread (buffer, size, 1, f)) == 0) goto _err_fread;

    set_mem_stream (&stream, buffer, size);
    last_patt = -1;
    do
    {
      uint8_t patt;

      if (read_bytes (&patt, sizeof (patt), &stream)) goto _err_eod;

      if ((last_patt >= patt) || (patt >= DFM_PATTERNS_MAX)) goto _err_format;
      last_patt = patt;

      for (uint_least8_t line = 0; line < DFM_PATTERN_LEN; line++)
        for (uint_least8_t chan = 0; chan < DFM_CHANNELS_MAX; chan++)
        {
          uint8_t note, fx;

          if (read_bytes (&note, sizeof (note), &stream)) goto _err_eod;

          if ((note & 0x80) != 0)
          {
            note &= ~0x80;
            if (read_bytes (&fx, sizeof (fx), &stream)) goto _err_eod;
          }
          else
            fx = 0;

          import_dfm_event (patt, line, chan, note, fx);
        }
    } while (stream.curptr < stream.endptr);

    if (song_len > 0) process_dfm_patterns (song, song_len);
  }
  next_dfm_step (progress);

  result = 0;

_exit:
  if (f != NULL) fclose (f);
  if (buffer != NULL) free (buffer);
  if (result < 0)
  {
    if (state != NULL) *state = result_state;
    if (error != NULL) *error = result_error;
  }

  DBG_LEAVE (); //EXIT //dfm_file_loader
  return result;

_err_malloc:
  result_error = "Memory allocation failed";
  goto _exit;

_err_fopen:
  result_error = "Failed to open input file";
  goto _exit;

_err_fread:
  result_error = "Failed to read input file";
  goto _exit;

_err_format:
  result_error = "Unknown file format";
  goto _exit;

_err_eod:
  result_error = "Unexpected end of input data";
  goto _exit;
}
