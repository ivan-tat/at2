// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// HSC AdLib Composer / HSC-Tracker song loader (*.hsc)
// Warning: no file signature available - use file extension instead.

#define HSC_INSTRUMENTS_MAX 128
#define HSC_CHANNELS_MAX 9
#define HSC_PATTERN_LEN 64
#define HSC_PATTERNS_MAX 50
#define HSC_ORDER_LEN 50

#pragma pack(push, 1)
typedef struct { uint8_t command, param; } hsc_event_t;
typedef hsc_event_t hsc_pattern_t[HSC_PATTERN_LEN][HSC_CHANNELS_MAX];
typedef hsc_pattern_t hsc_patterns_t[HSC_PATTERNS_MAX];
typedef struct
{
  uint8_t instruments[HSC_INSTRUMENTS_MAX][12];
  uint8_t order[HSC_ORDER_LEN];
  uint8_t unused;
} hsc_header_t;
#pragma pack(pop)

// patt: 0..HSC_PATTERNS_MAX-1
// line: 0..HSC_PATTERN_LEN-1
// chan: 0..HSC_CHANNELS_MAX-1
static void import_hsc_event (uint8_t patt, uint8_t line, uint8_t chan, hsc_event_t event)
{
  uint8_t command = event.command, param = event.param;
  tCHUNK chunk;

  memset (&chunk, 0, sizeof (chunk));

  if ((command >= 1) && (command <= 12*8+1))  // REGULAR NOTE
    chunk.note = fix_c_note_bug && (command < 12*8+1) ? command + 1 : command;
  else if (command == 0x7F) // PAUSE
    chunk.note = UINT8_NULL;
  else if (command == 0x80) // INSTRUMENT
  {
    chunk.effect_def = ef_Extended;
    chunk.effect = ef_ex_ExtendedCmd2 * 16 + ef_ex_cmd2_ResetVol;
    chunk.instr_def = param + 1;  // FIXME: check range
    chunk.note = UINT8_NULL;
  }

  if (command != 0x80)
    switch (param >> 4)
    {
      case 0: // PATTERN BREAK
        if ((param & 0x0F) == 1) chunk.effect_def = ef_PatternBreak;
        break;

      case 1: // MANUAL SLIDE UP
        chunk.effect_def = ef_Extended2;
        chunk.effect = ef_ex2_FineTuneUp * 16 + max ((param & 0x0F) + 1, 15);
        break;

      case 2: // MANUAL SLIDE DOWN
        chunk.effect_def = ef_Extended2;
        chunk.effect = ef_ex2_FineTuneDown * 16 + max ((param & 0x0F) + 1, 15);
        break;

      case 10:  // SET CARRIER VOLUME
        chunk.effect_def = ef_SetCarrierVol;
        chunk.effect = 63 - (param & 0x0F) * 4;
        chunk.instr_def = param + 1; // FIXME: 0xA1..0xB0 ???
        break;

      case 11:  // SET MODULATOR VOLUME
        chunk.effect_def = ef_SetModulatorVol;
        chunk.effect = 63 - (param & 0x0F) * 4;
        break;

      case 12:  // SET INSTRUMENT VOLUME
        chunk.effect_def = ef_SetInsVolume;
        chunk.effect = 63 - (param & 0x0F) * 4;
        break;

      case 15:  // SET SPEED
        chunk.effect_def = ef_SetSpeed;
        chunk.effect = (param & 0x0F) + 1;
        break;

      default: break;
    }

  put_chunk (patt, line, chan + 1, &chunk);
}

/*static*/ void import_hsc_patterns (const void *data, uint8_t patterns)
{
  tFIXED_SONGDATA *song = &songdata;
  uint8_t voice[HSC_CHANNELS_MAX];
  uint_least8_t order, patt;
  bool patts[255];

  if (!accurate_conv)
  {
    for (uint_least8_t chan = 0; chan < HSC_CHANNELS_MAX; chan++) voice[chan] = chan + 1;
  }
  else memset (voice, 0, sizeof (voice));
  memset (patts, false, sizeof (patts));

  for (patt = 0; patt < HSC_PATTERNS_MAX; patt++)
    for (uint_least8_t line = 0; line < HSC_PATTERN_LEN; line++)
      for (uint_least8_t chan = 0; chan < HSC_CHANNELS_MAX; chan++)
      {
        hsc_event_t *event = &(*(hsc_patterns_t *)data)[patt][line][chan];
        if ((event->command != 0) || (event->param != 0)) import_hsc_event (patt, line, chan, *event);
      }

  order = 0;

  do
  {
    patt = song->pattern_order[order];

    if (patt < HSC_PATTERNS_MAX)
    {
      const hsc_event_t *event = &(*(hsc_patterns_t *)data)[patt][0][0];
      uint8_t patt_break = UINT8_NULL;

      for (uint_least8_t line = 0; line < HSC_PATTERN_LEN; line++)
        for (uint_least8_t chan = 0; chan < HSC_CHANNELS_MAX; chan++)
        {
          uint8_t command = event->command, param = event->param;
          tCHUNK chunk;

          get_chunk (patt, line, chan + 1, &chunk);

          if ((command >= 1) && (command <= 12*8+1))  // REGULAR NOTE
          {
            if (accurate_conv)
            {
              if (voice[chan] == 0)
              {
                voice[chan] = chan + 1;
                chunk.instr_def = voice[chan];
              }
            }
            else
              chunk.instr_def = voice[chan];
          }
          else if (command == 0x80) // INSTRUMENT
          {
            if (line != patt_break)
            {
              voice[chan] = param + 1;
              if (!accurate_conv)
              {
                chunk.instr_def = voice[chan];
                chunk.note = UINT8_NULL;
              }
            }
          }

          if (command != 0x80)
            switch (param >> 4)
            {
              case 0: // PATTERN BREAK
                if ((param & 0x0F) == 1) patt_break = line + 1;
                break;

              case 10:  // SET CARRIER VOLUME
                if (chunk.instr_def == 0)
                {
                  if (!accurate_conv) chunk.instr_def = voice[chan];
                  else if (voice[chan] == 0) chunk.instr_def = chan + 1;
                }
                break;

              case 11:  // SET MODULATOR VOLUME
                if (chunk.instr_def == 0)
                {
                  if (!accurate_conv) chunk.instr_def = voice[chan];
                  else if (voice[chan] == 0) chunk.instr_def = chan + 1;
                }
                break;

              case 12:  // SET INSTRUMENT VOLUME
                if (chunk.instr_def == 0)
                {
                  if (!accurate_conv) chunk.instr_def = voice[chan];
                  else if (voice[chan] == 0) chunk.instr_def = chan + 1;
                }
                break;

              default: break;
            }

          if (!patts[patt]) put_chunk (patt, line, chan + 1, &chunk);
          event++;
        }
      patts[patt] = true;
    }
    order++;
  } while ((patt < patterns) && (order < sizeof (song->pattern_order) / sizeof (song->pattern_order[0])));
}

static void next_hsc_step (progress_callback_t *progress)
{
  if (progress != NULL)
  {
    progress->step++;
    progress->value = 1;
    progress->update (progress, 1, -1);
  }
}

static const uint8_t HSC_KSL[4] = { 0, 3, 2, 1 };

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
int8_t hsc_file_loader (const String *_fname, progress_callback_t *progress, uint8_t *state, char **error)
{
  tFIXED_SONGDATA *song = &songdata;
  int8_t result = -1;
  uint8_t result_state = 0;
  char *result_error = NULL;
  FILE *f = NULL;
  hsc_header_t *header = NULL;
  hsc_patterns_t *patterns = NULL;
  long fsize;
  char fname[255+1];

  DBG_ENTER ("hsc_file_loader");

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
  if (   (fsize < (long)sizeof (*header))
      || (fsize > (long)(sizeof (*header) + sizeof (*patterns))))
    goto _err_format;
  if (fseek (f, 0, SEEK_SET) != 0) goto _err_fread;

  if ((header = malloc (sizeof (*header))) == NULL) goto _err_malloc;

  if (fread (header, sizeof (*header), 1, f) == 0) goto _err_fread;

  result = -4;

  if (progress != NULL) progress->num_steps = 3;

  // initialize song
  init_songdata ();
  song->patt_len = HSC_PATTERN_LEN;
  if (adjust_tracks || (song->nm_tracks < HSC_CHANNELS_MAX)) song->nm_tracks = HSC_CHANNELS_MAX;
  tempo = 18;
  speed = 2;
  song->common_flag |= 2;
  song->tempo = tempo;
  song->speed = speed;
  {
    String_t s = NameOnly (_fname);
    CopyString (songdata_title, (String *)&s, sizeof (songdata_title) - 1);
  }
  import_old_flags ();
  result_state = 1;
  next_hsc_step (progress);

  // import instruments
  for (uint_least8_t i = 0; i < HSC_INSTRUMENTS_MAX; i++)
  {
    tADTRACK2_INS *ins = &song->instr_data[i];
    import_hsc_instrument_alt (ins, header->instruments[i]);
    // specific corrections for HSC-Tracker instrument
    ins->fm_data.KSL_VOLUM_modulator = (ins->fm_data.KSL_VOLUM_modulator & 0x3F)
                                     + (HSC_KSL[ins->fm_data.KSL_VOLUM_modulator >> 6] << 6);
    ins->fm_data.KSL_VOLUM_carrier = (ins->fm_data.KSL_VOLUM_carrier & 0x3F)
                                   + (HSC_KSL[ins->fm_data.KSL_VOLUM_carrier >> 6] << 6);
  }
  result_state = 2;

  // import patterns order
  for (uint_least8_t i = 0; i < HSC_ORDER_LEN; i++)
    song->pattern_order[i] = header->order[i] <= 0xB0 ? header->order[i] : 0x80;

  next_hsc_step (progress);

  // import patterns
  if (fsize > (long)sizeof (*header))
  {
    size_t data_size = fsize - sizeof (*header);
    uint8_t num_patterns = (data_size + sizeof (hsc_pattern_t) - 1) / sizeof (hsc_pattern_t);

    if (num_patterns != 0)
    {
      size_t patterns_size = num_patterns * sizeof (hsc_pattern_t);
      const hsc_event_t *event_end = &(*patterns)[num_patterns][0][0];

      if ((patterns = malloc (patterns_size)) == NULL) goto _err_malloc;
      if (fread (patterns, data_size, 1, f) == 0) goto _err_fread;
      if (data_size < patterns_size)  // clear tail
        memset ((uint8_t *)patterns + data_size, 0, patterns_size - data_size);

      for (hsc_event_t *event = &(*patterns)[0][0][0]; event < event_end; event++)
      {
        uint8_t command = event->command, param = event->param >> 4;

        if ((command > 12*8+1) && (command != 0x7F) && (command != 0x80))
          event->command = 0;
        if (((param >= 3) && (param <= 9)) || (param == 13) || (param == 14))
          event->param = 0;
      }

      import_hsc_patterns (patterns, num_patterns);
    }
  }
  next_hsc_step (progress);

  result = 0;

_exit:
  if (f != NULL) fclose (f);
  if (header != NULL) free (header);
  if (patterns != NULL) free (patterns);
  if (result < 0)
  {
    if (state != NULL) *state = result_state;
    if (error != NULL) *error = result_error;
  }

  DBG_LEAVE (); //EXIT //hsc_file_loader
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
}
