// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#define CFF_ID1_SIZE 16
static const char GCC_ATTRIBUTE((nonstring)) cff_id1[CFF_ID1_SIZE] = { "<CUD-FM-File>\x1A\xDE\xE0" };

#define CFF_ID2_SIZE 31
static const char GCC_ATTRIBUTE((nonstring)) cff_id2[CFF_ID2_SIZE] = { "CUD-FM-File - SEND A POSTCARD -" };

#define CFF_INSTRUMENTS_MAX 47
#define CFF_PATTERNS_MAX 37
#define CFF_ORDER_LEN 65

#pragma pack(push, 1)
typedef struct
{
  char id[CFF_ID1_SIZE];
  uint8_t version;      // format version (unused)
  uint16_t size;        // file size -32 (unused)
  uint8_t packed_flag;  // 1=compressed data, otherwise=raw
} cff_header_t;

#define CFF_RAW_DATA_START 0x20
#define CFF_PACKED_DATA_START 0x30

typedef struct
{
  uint8_t data[12];
  char name[20];
} cff_instrument_t;

typedef struct
{
  cff_instrument_t instruments[CFF_INSTRUMENTS_MAX];
  uint8_t num_patterns;
  char id[CFF_ID2_SIZE];
  char composer[20];
  char title[20];
  uint8_t order[CFF_ORDER_LEN];
} cff_header2_t;

typedef struct { uint8_t note, fx, param; } cff_event_t;
typedef cff_event_t cff_patterns_t[CFF_PATTERNS_MAX][64][9];
#pragma pack(pop)

static bool is_empty_cff_event (const cff_event_t *event)
{
  return (event->note == 0) && (event->fx == 0) && (event->param == 0);
}

// patt: 0..CFF_PATTERNS_MAX-1
// line: 0..63
// chan: 0..8
static void import_cff_event (uint8_t patt, uint8_t line, uint8_t chan,
                              const cff_event_t *event)
{
  tCHUNK chunk;

  memset (&chunk, 0, sizeof (chunk));

  switch (event->fx)
  {
    case 'A': // SET SPEED
    if (event->param > 0)
    {
      chunk.effect_def = ef_SetSpeed;
      chunk.effect = event->param;
    }
    break;

    case 'B': // SET CARRIER WAVEFORM
      if (event->param < 4)
      {
        chunk.effect_def = ef_SetWaveform;
        chunk.effect = event->param * 16;
      }
      break;

    case 'C': // SET MODULATOR VOLUME
      chunk.effect_def = ef_SetModulatorVol;
      chunk.effect = event->param < 64 ? 63 - event->param : 0;
      break;

    case 'D': // VOLUME SLIDE UP/DOWN
      chunk.effect_def = ef_VolSlide;
      chunk.effect = event->param;
      break;

    case 'E': // SLIDE DOWN
      if (event->param != 0)
      {
        chunk.effect_def = ef_FSlideDown;
        chunk.effect = event->param;
      }
      break;

    case 'F': // SLIDE UP
      if (event->param != 0)
      {
        chunk.effect_def = ef_FSlideUp;
        chunk.effect = event->param;
      }
      break;

    case 'G': // SET CARRIER VOLUME
      chunk.effect_def = ef_SetCarrierVol;
      chunk.effect = event->param < 64 ? 63 - event->param : 0;
      break;

    case 'H': // SET TEMPO
      if (event->param > 0)
      {
        uint8_t temp = event->param <= 21 ? 125 : event->param;
        uint32_t period = 1412926UL / ((uint32_t)temp >> 1);

        chunk.effect_def = ef_SetTempo;
        for (chunk.effect = 1; (PIT_FREQ_MAX / chunk.effect > period) && (chunk.effect < 255); chunk.effect++);
      }
      break;

    case 'I': // SET INSTRUMENT
      if (event->param < CFF_INSTRUMENTS_MAX)
      {
        chunk.effect_def = ef_Extended;
        chunk.effect = ef_ex_ExtendedCmd2 * 16 + ef_ex_cmd2_ResetVol;
        chunk.instr_def = event->param + 1;
      }
      break;

    case 'J': // ARPEGGIO
      chunk.effect_def = ef_Arpeggio;
      chunk.effect = event->param;
      break;

    case 'K': // JUMP TO ORDER
      if (event->param < 128)
      {
        chunk.effect_def = ef_PositionJump;
        chunk.effect = event->param;
      }
      break;

    case 'L': // JUMP TO NEXT PATTERN IN ORDER
      chunk.effect_def = ef_PatternBreak;
      break;

    case 'M': // SET TREMOLO HIGHER / SET VIBRATO DEEPER
      chunk.effect_def = ef_Extended;
      switch (event->param)
      {
        case 0x01: chunk.effect = 10; break;
        case 0x10: chunk.effect = 1; break;
        case 0x11: chunk.effect = 11; break;
        default: break;
      }
      break;

    default: break;
  }

  if (event->note >= 1 && event->note <= 12*8+1) // REGULAR NOTE
  {
    if (!fix_c_note_bug) chunk.note = event->note;
    else
    {
      chunk.note = event->note + 1;
      if (chunk.note > 12*8+1) chunk.note = 12*8+1;
    }
  }
  else if (event->note == 0x6D) chunk.note = UINT8_NULL; // PAUSE

  put_chunk (patt, line, chan + 1, &chunk);
}

static void import_cff_patterns (const cff_patterns_t *data, size_t size, uint8_t patterns)
{
  tFIXED_SONGDATA *song = &songdata;
  uint8_t voice[9];
  uint8_t arpgg[9];
  size_t offset;
  const cff_event_t *event;
  uint8_t order, patt;
  uint8_t patts_count;
  uint8_t patts[255];

  if (!accurate_conv)
    for (int chan = 0; chan < 9; chan++) voice[chan] = chan + 1;
  else
    memset (voice, 0, sizeof (voice));
  memset (arpgg, 0, sizeof (arpgg));

  offset = 0;
  event = &(*data)[0][0][0];
  for (int pat = 0; pat < CFF_PATTERNS_MAX; pat++)
    for (int line = 0; line < 64; line++)
      for (int chan = 0; chan < 9; chan++)
      {
        if (offset + sizeof (*event) <= size)
        {
          if (!is_empty_cff_event (event))
            import_cff_event (pat, line, chan, event);
        }
        else if (offset < size)
        {
          cff_event_t broken_event;
          memset (&broken_event, 0, sizeof (broken_event));
          memcpy (&broken_event, event, size - offset);
          if (!is_empty_cff_event (&broken_event))
            import_cff_event (pat, line, chan, &broken_event);
        }
        offset += sizeof (*event);
        event++;
      }

  order = 0;
  patt = UINT8_NULL;
  patts_count = 0;
  do
  {
    if (song->pattern_order[order] < CFF_PATTERNS_MAX)
    {
      uint8_t patt_break = UINT8_NULL;

      patt = song->pattern_order[order];
      event = &(*data)[patt][0][0];
      for (int line = 0; line < 64; line++)
        for (int chan = 0; chan < 9; chan++)
        {
          tCHUNK chunk;

          get_chunk (patt, line, chan + 1, &chunk);

          switch (event->fx)
          {
            case 'C': // SET MODULATOR VOLUME
              if (chunk.instr_def == 0)
              {
                if (!accurate_conv)
                  chunk.instr_def = voice[chan];
                else if (voice[chan] == 0)
                  chunk.instr_def = chan + 1;
              }
              break;

            case 'G': // SET CARRIER VOLUME
              if (chunk.instr_def == 0)
              {
                if (!accurate_conv)
                  chunk.instr_def = voice[chan];
                else if (voice[chan] == 0)
                  chunk.instr_def = chan + 1;
              }
              break;

            case 'I': // SET INSTRUMENT
              if (event->param < CFF_INSTRUMENTS_MAX)
                if (line != patt_break)
                {
                  voice[chan] = event->param + 1;
                  if (accurate_conv) chunk.instr_def = voice[chan];
                }
              break;

            case 'J': // ARPEGGIO
              chunk.effect_def = ef_Arpeggio;
              if (event->param != 0)
              {
                chunk.effect = event->param;
                arpgg[chan] = event->param;
              }
              else chunk.effect = arpgg[chan];
              break;

            case 'K': // JUMP TO ORDER
              if (event->param < 128) patt_break = line + 1;
              break;

            case 'L': // JUMP TO NEXT PATTERN IN ORDER
              patt_break = line + 1;
              break;

            default: break;
          }

          if ((event->note >= 1) && (event->note <= 12*8+1)) // REGULAR NOTE
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

          if ((patts_count == 0) || (memchr (patts, patt, patts_count) == NULL))
            put_chunk (patt, line, chan + 1, &chunk);

          event++;
        }

      if (patts_count < sizeof (patts))
      {
        patts[patts_count] = patt;
        patts_count++;
      }
    }

    order++;
  } while ((patt < patterns) && (order < CFF_ORDER_LEN));
}

/*** CFF decoder ***/

// CFF decoder state

// flags
#define CFFD_FL_INFIN (1 << 0) // infinite input (reads zeroes outside input buffer)

#pragma pack(push, 1)
typedef struct { uint8_t len, str[255]; } cffd_string_t;

typedef struct
{
  unsigned flags;
  const uint8_t *input;
  uint8_t *output;
  size_t input_size;
  size_t input_idx;
  size_t output_limit;
  size_t output_idx;  // actual output size
  cffd_string_t the_string;
  uint8_t code_length;
  int32_t bits_buffer;
  uint16_t bits_left;
  uint16_t heap_length;
  uint16_t dictionary_length;
  uint8_t heap[0x10000];
  uint16_t dictionary[0x8000];  // offset in `heap'
  char *error;
} cffds_t;
#pragma pack(pop)

typedef bool cffd_status_t;
#define CFFD_SUCCESS false
#define CFFD_ERROR true

static cffd_status_t cffd_get_code (cffds_t *ds, int32_t *code)
{
  cffd_status_t status = CFFD_ERROR;
  int32_t result_code;

  while (ds->bits_left < ds->code_length)
  {
    if (ds->input_idx < ds->input_size)
      ds->bits_buffer |= ds->input[ds->input_idx++] << ds->bits_left;
    else if ((ds->flags & CFFD_FL_INFIN) == 0) goto _err_eod;
    ds->bits_left += 8;
  }

  result_code = ds->bits_buffer & ((1 << ds->code_length) - 1);
  ds->bits_buffer >>= ds->code_length;
  ds->bits_left -= ds->code_length;
  status = CFFD_SUCCESS;

_exit:
  if (status == CFFD_SUCCESS) *code = result_code;
  return status;

_err_eod:
  ds->error = "Unexpected end of packed data";
  goto _exit;
}

static void cffd_translate_code (cffds_t *ds, int32_t code, cffd_string_t *str)
{
  const cffd_string_t *translated_string;
#pragma pack(push, 1)
  struct { uint8_t len, str[1]; } translated_char;
#pragma pack(pop)

  if (code >= 0x104)
    translated_string = (const cffd_string_t *)&ds->heap[ds->dictionary[code - 0x104]];
  else
  {
    translated_char.len = 1;
    translated_char.str[0] = (code - 4) & 0xFF;
    translated_string = (const cffd_string_t *)&translated_char;
  }

  memcpy (str, translated_string, translated_string->len + 1);
}

static cffd_status_t cffd_startup (cffds_t *ds)
{
  cffd_status_t status = CFFD_ERROR;
  int32_t old_code;

  if (cffd_get_code (ds, &old_code) != CFFD_SUCCESS) goto _exit;
  cffd_translate_code (ds, old_code, &ds->the_string);

  for (int i = 0; i < ds->the_string.len; i++)
  {
    if (ds->output_idx >= ds->output_limit) goto _err_overrun;
    ds->output[ds->output_idx++] = ds->the_string.str[i];
  }

  status = CFFD_SUCCESS;

_exit:
  return status;

_err_overrun:
  ds->error = "Output buffer overrun";
  goto _exit;
}

static void cffd_cleanup (cffds_t *ds)
{
  ds->code_length = 9;
  ds->bits_buffer = 0;
  ds->bits_left = 0;
  ds->heap_length = 0;
  ds->dictionary_length = 0;
}

static cffd_status_t cffd_expand_dictionary (cffds_t *ds, const cffd_string_t *str)
{
  cffd_status_t status = CFFD_ERROR;

  if (str->len < 0xF0)
  {
    size_t size = str->len + 1;
    if ((size_t)ds->heap_length + size >= sizeof (ds->heap) / sizeof (ds->heap[0])) goto _err_heap;
    if ((size_t)ds->dictionary_length + 1 >= sizeof (ds->dictionary) / sizeof (ds->dictionary[0])) goto _err_dict;
    memcpy (&ds->heap[ds->heap_length], str, size);
    ds->dictionary[ds->dictionary_length++] = ds->heap_length;
    ds->heap_length += size;
  }
  status = CFFD_SUCCESS;

_exit:
  return status;

_err_heap:
  ds->error = "Decoder's heap overrun";
  goto _exit;

_err_dict:
  ds->error = "Decoder's dictionary overrun";
  goto _exit;
}

// In:
//   * `error' may be NULL.
//
// On success:
//   * Return value is 0.
//   * `out_size' is set.
//   * `error' is untouched.
// On error:
//   * Return value is -1.
//   * `out_size' is set.
//   * `error' (if set) is set to error description.
static int unpack_cff_block (const void *input, size_t input_size, bool infinite,
                             void *output, size_t output_limit,
                             size_t *out_size, char **error)
{
  int result = -1;
  size_t result_out_size = 0;
  char *result_error = NULL;
  cffds_t *ds = NULL;

  DBG_ENTER ("unpack_cff_block");

  if ((ds = malloc (sizeof (*ds))) == NULL) goto _err_malloc;

  ds->flags = infinite ? CFFD_FL_INFIN : 0;
  ds->input = input;
  ds->output = output;
  ds->input_size = input_size;
  ds->input_idx = 0;
  ds->output_limit = output_limit;
  ds->output_idx = 0;
  cffd_cleanup (ds);
  if (cffd_startup (ds) != CFFD_SUCCESS) goto _err_decoder;

  for (;;)
  {
    int32_t new_code;

    if (cffd_get_code (ds, &new_code) != CFFD_SUCCESS) goto _err_decoder;
    if (new_code == 0)  // 0x00: end of data
      break;
    else if (new_code == 1) // 0x01: end of block
    {
      cffd_cleanup (ds);
      if (cffd_startup (ds) != CFFD_SUCCESS) goto _err_decoder;
      continue;
    }
    else if (new_code == 2) // 0x02: expand code length
    {
      ds->code_length++;
      continue;
    }
    else if (new_code == 3) // 0x03: RLE
    {
      uint8_t old_code_length = ds->code_length;
      uint8_t repeat_length;
      int32_t repeat_counter;
      int32_t code;

      ds->code_length = 2;
      if (cffd_get_code (ds, &code) != CFFD_SUCCESS) goto _err_decoder;
      repeat_length = code + 1;
      if (cffd_get_code (ds, &code) != CFFD_SUCCESS) goto _err_decoder;
      ds->code_length = 4 << code;
      if (cffd_get_code (ds, &repeat_counter) != CFFD_SUCCESS) goto _err_decoder;

      for (uint_least16_t i = 0; i < repeat_counter * repeat_length; i++)
      {
        if (ds->output_idx >= ds->output_limit) goto _err_overrun;
        ds->output[ds->output_idx] = ds->output_idx >= repeat_length ? ds->output[ds->output_idx - repeat_length] : 0;
        ds->output_idx++;
      }

      ds->code_length = old_code_length;
      if (cffd_startup (ds) != CFFD_SUCCESS) goto _err_decoder;
      continue;
    }
    else if (new_code >= 0x104 + ds->dictionary_length)
      ds->the_string.str[ds->the_string.len++] = ds->the_string.str[0];
    else
    {
      cffd_string_t s;

      cffd_translate_code (ds, new_code, &s);
      ds->the_string.str[ds->the_string.len++] = s.str[0];
    }

    if (cffd_expand_dictionary (ds, &ds->the_string) != CFFD_SUCCESS) goto _err_decoder;
    cffd_translate_code (ds, new_code, &ds->the_string);

    for (int i = 0; i < ds->the_string.len; i++)
    {
      if (ds->output_idx >= ds->output_limit) goto _err_overrun;
      ds->output[ds->output_idx++] = ds->the_string.str[i];
    }
  }

  result = 0;
  result_out_size = ds->output_idx;

_exit:
  if (ds != NULL) free (ds);
  *out_size = result_out_size;
  if (result < 0)
    if (error != NULL) *error = result_error;

  DBG_LEAVE (); //EXIT //unpack_cff_block
  return result;

_err_malloc:
  result_error = "Memory allocation failed";
  goto _exit;

_err_overrun:
  result_error = "Output buffer overrun";
  goto _exit;

_err_decoder:
  result_error = ds->error;
  goto _exit;
}

static void cff_new_song (tFIXED_SONGDATA *song, cff_header2_t *header2, const String *fname)
{
  // clear song
  init_songdata ();
  song->patt_len = 64;
  if (adjust_tracks || (song->nm_tracks < 9)) song->nm_tracks = 9;
  tempo = 51;
  speed = 6;
  song->tempo = tempo;
  song->speed = speed;

  // import base properties
  song->common_flag |= 0x02;
  {
    String_t s, t;

    t = CutStr (StrToString ((String *)&s, header2->title, sizeof (header2->title) /*-1*/));
    CopyString (song->songname, (String *)&t, sizeof (song->songname) - 1);
    t = CutStr (StrToString ((String *)&s, header2->composer, sizeof (header2->composer) /*-1*/));
    CopyString (song->composer, (String *)&t, sizeof (song->composer) - 1);
    s = NameOnly (fname);
    CopyString (songdata_title, (String *)&s, sizeof (songdata_title) - 1);
  }
  import_old_flags ();

  // import pattern order
  for (int i = 0; i < CFF_ORDER_LEN; i++)
    if (header2->order[i] <= header2->num_patterns)
      song->pattern_order[i] = header2->order[i];

  // import instruments
  for (int i = 0; i < CFF_INSTRUMENTS_MAX; i++)
  {
    String_t s, t;

    import_hsc_instrument_alt (&song->instr_data[i], header2->instruments[i].data);
    song->instr_data[i].fine_tune = 0;

    s.len = sizeof (header2->instruments[0].name);
    memcpy (s.str, header2->instruments[i].name, sizeof (header2->instruments[0].name));
    t = truncate_string ((String *)&s);
    s = Copy (song->instr_names[i], 1, 9);
    AppendString ((String *)&s, (String *)&t, sizeof (s) - 1);
    CopyString (song->instr_names[i], (String *)&s, sizeof (song->instr_names[0]) - 1);
  }
}

static void next_cff_step (progress_callback_t *progress)
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
int8_t cff_file_loader (const String *_fname, progress_callback_t *progress, uint8_t *state, char **error)
{
  tFIXED_SONGDATA *song = &songdata;
  int8_t result = -1;
  uint8_t result_state = 0;
  char *result_error = NULL;
  FILE *f = NULL;
  uint8_t *raw_buffer = NULL;
  uint8_t *packed_buffer = NULL;
  long fsize;
  cff_header_t header;
  bool header2_allocated = false;
  cff_header2_t *header2 = NULL;
  char fname[255+1];

  DBG_ENTER ("cff_file_loader");

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
  if (fsize < CFF_RAW_DATA_START) goto _err_format;
  if (fseek (f, 0, SEEK_SET) != 0) goto _err_fread;

  if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;

  result = -2;

  if (memcmp (header.id, cff_id1, sizeof (header.id)) != 0) goto _err_format;

  result = -1;  // one more ID to check

  if (progress != NULL) progress->num_steps = 4;

  if (header.packed_flag != 1)  // raw
  {
    size_t patterns_size;

    // read header and check
    if (fsize < (long)(CFF_RAW_DATA_START + sizeof (*header2))) goto _err_format;
    if ((header2 = malloc (sizeof (*header2))) == NULL) goto _err_malloc;
    header2_allocated = true;
    if (fseek (f, CFF_RAW_DATA_START, SEEK_SET) != 0) goto _err_fread;
    if (fread (header2, sizeof (*header2), 1, f) == 0) goto _err_fread;
    if (memcmp (header2->id, cff_id2, sizeof (header2->id)) != 0) goto _err_format;
    next_cff_step (progress);

    result = -4;

    // read patterns
    patterns_size = fsize - CFF_RAW_DATA_START - sizeof (*header2);
    if (patterns_size != 0)
    {
      if ((raw_buffer = malloc (patterns_size)) == NULL) goto _err_malloc;
      if (fread (raw_buffer, patterns_size, 1, f) == 0) goto _err_fread;
    }
    fclose (f);
    f = NULL;
    next_cff_step (progress);

    // initialize song
    cff_new_song (song, header2, _fname);
    result_state = 2;
    next_cff_step (progress);

    // import patterns
    if (patterns_size != 0)
      import_cff_patterns ((const cff_patterns_t *)raw_buffer, patterns_size,
                           header2->num_patterns);
    next_cff_step (progress);
  }
  else  // packed
  {
    size_t packed_size, raw_size;

    // read packed data
    if (fsize < CFF_PACKED_DATA_START) goto _err_format;
    packed_size = fsize - CFF_PACKED_DATA_START;
    if (packed_size == 0) goto _err_fread;
    if (packed_size > sizeof (*header2) + sizeof (cff_patterns_t)) goto _err_format;
    if ((packed_buffer = malloc (packed_size)) == NULL) goto _err_malloc;
    if (fseek (f, CFF_PACKED_DATA_START, SEEK_SET) != 0) goto _err_fread;
    if (fread (packed_buffer, packed_size, 1, f) == 0) goto _err_fread;
    fclose (f);
    f = NULL;
    next_cff_step (progress);

    result = -2;

    // unpack
    if ((raw_buffer = malloc (sizeof (*header2) + sizeof (cff_patterns_t))) == NULL) goto _err_malloc;
    if (unpack_cff_block (packed_buffer, packed_size, true,
                          raw_buffer, sizeof (*header2) + sizeof (cff_patterns_t), &raw_size,
                          &result_error)) goto _exit;
    free (packed_buffer);
    packed_buffer = NULL;
    next_cff_step (progress);

    // check format
    if (raw_size < sizeof (*header2)) goto _err_eod;
    header2 = (cff_header2_t *)raw_buffer;
    if (memcmp (header2->id, cff_id2, sizeof (header2->id)) != 0) goto _err_format;

    result = -4;

    // initialize song
    cff_new_song (song, header2, _fname);
    result_state = 2;
    next_cff_step (progress);

    // import patterns
    if (raw_size > sizeof (*header2))
      import_cff_patterns ((const cff_patterns_t *)&raw_buffer[sizeof (*header2)],
                           raw_size - sizeof (*header2), header2->num_patterns);
    next_cff_step (progress);
  }

  result = 0;

_exit:
  if (f != NULL) fclose (f);
  if ((header2 != NULL) && header2_allocated) free (header2);
  if (raw_buffer != NULL) free (raw_buffer);
  if (packed_buffer != NULL) free (packed_buffer);
  if (result < 0)
  {
    if (state != NULL) *state = result_state;
    if (error != NULL) *error = result_error;
  }

  DBG_LEAVE (); //EXIT //cff_file_loader
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
  result_error = "Unexpected end of unpacked data";
  goto _exit;
}
