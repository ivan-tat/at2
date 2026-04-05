// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Reality AdLib Tracker song loader (*.rad)

#define RAD_INSTRUMENT_SIZE 11
#define RAD_CHANNELS_MAX 9
#define RAD_PATTERN_LEN 64
#define RAD_PATTERNS_MAX 32

#define RAD_ID_SIZE 16
static const char GCC_ATTRIBUTE((nonstring)) rad_id[RAD_ID_SIZE] = { "RAD by REALiTY!!" };

#pragma pack(push, 1)
typedef struct
{
  char id[RAD_ID_SIZE];
  uint8_t version;
  uint8_t flags;  // bit 7: 1=description follows
                  // bit 6: 1="slow-timer" tune
                  // bit 5: ?
                  // bits 4..0: initial speed (1..31)
} rad_header_t;
#pragma pack(pop)

static void truncate_rad_string (const uint8_t **end_ptr, size_t *len)
{
  size_t l = *len;
  if (l != 0) // try to truncate string
  {
    const uint8_t *p = *end_ptr - 1;
    while ((l != 0) && ((*p == ' ') || (*p == (uint8_t) '\xFF')))
    {
      p--;
      l--;
    }
    *end_ptr = p + 1;
    *len = l;
  }
}

static bool import_rad_description (void **output, const void *input, size_t input_size, char **error)
{
  bool result = true;
  char *result_error = NULL;
  const uint8_t *in = input;
  uint8_t *out = NULL;
  size_t out_size = 0, lines_count = 0;
  size_t len = 0; // current line length

  // pass 1/2: determine text data size (without NULL-terminators) and total lines count
  for (size_t pos = 0; (pos < input_size) && (in[pos] != '\0'); pos++)
  {
    uint8_t c = in[pos];

    if (c == 1) // end of line
    {
      out_size += len;
      len = 0;
      lines_count++;
    }
    else if (c <= 31) len += c; // add spaces (2..31) to current line
    else len++; // add single character to current line
  }
  // add remaining characters as line
  if (len != 0)
  {
    out_size += len;
    lines_count++;
  }

  if ((out_size != 0) && (lines_count != 0))
  {
    fast_str_t *lines;
    uint8_t *data;
    uint8_t *line_start;
    size_t line;

    // sizeof (song_desc_s) + NULL-terminators for all lines
    out_size += sizeof (size_t) + sizeof (fast_str_t) * lines_count + lines_count;
    if ((out = malloc (out_size)) == NULL) goto _err_malloc;

    *((size_t *)out) = lines_count; // =song_desc_s.count
    lines = (fast_str_t *)(&out[sizeof (size_t)]);  // =&out[offsetof (song_desc_s, lines)]
    data = (uint8_t *)&lines[lines_count];  // =&out[sizeof (song_desc_s)]
    line_start = data;

    // pass 2/2: store lines
    line = 0;
    len = 0;
    for (size_t pos = 0; (pos < input_size) && (in[pos] != '\0'); pos++)
    {
      uint8_t c = in[pos];

      if (c == 1) // end of line
      {
        truncate_rad_string ((const uint8_t **)&data, &len);
        *data = '\0';
        data++;
        lines[line].len = len;
        lines[line].data = line_start;
        line_start = data;
        line++;
        len = 0;
      }
      else if (c <= 31) // add spaces (2..31) to current line
      {
        memset (data, ' ', c);
        data += c;
        len += c;
      }
      else  // add single character to current line
      {
        *data = c;
        data++;
        len++;
      }
    }
    // add remaining characters as line
    if (len != 0)
    {
      truncate_rad_string ((const uint8_t **)&data, &len);
      *data = '\0';
      data++;
      lines[line].len = len;
      lines[line].data = line_start;
      line_start = data;
    }
    // shrink memory block if needed
    if (line_start < out + out_size)
    {
      void *p = realloc (out, line_start - out);
      if (p == NULL) goto _err_malloc;
      out = p;
    }
  }

  result = false;

_exit:
  if (result)
  {
    if (out != NULL) free (out);
    if (error != NULL) *error = result_error;
  }
  else *output = out;

  return result;

_err_malloc:
  result_error = "Memory allocation failed";
  goto _exit;
}

static void import_rad_event (uint8_t pattern, uint8_t line, uint8_t channel,
                              uint8_t byte1, uint8_t byte2, uint8_t param)
{
  tCHUNK chunk;

  memset (&chunk, 0, sizeof (chunk));
  if (((byte2 >> 4) + ((byte1 & 0x80) >> 3)) != 0)
    chunk.instr_def = (byte2 >> 4) + ((byte1 & 0x80) >> 3);

  if (((byte1 & 0x0F) >= 1) && ((byte1 & 0x0F) <= 12))
    chunk.note = 12 * ((byte1 >> 4) & 7) + (byte1 & 0x0F) + 1;
  else if ((byte1 & 0x0F) == 15)
    chunk.note = UINT8_NULL;

  switch (byte2 & 0x0F)
  {
    case 1: // PORTAMENTO (FREQUENCY SLIDE) UP
      chunk.effect_def = ef_FSlideUp;
      chunk.effect = param;
      break;

    case 2: // PORTAMENTO (FREQUENCY SLIDE) DOWN
      chunk.effect_def = ef_FSlideDown;
      chunk.effect = param;
      break;

    case 3: // PORTAMENTO TO NOTE
      chunk.effect_def = ef_TonePortamento;
      chunk.effect = param;
      break;

    case 5: // PORTAMENTO TO NOTE WITH VOLUME SLIDE
      if ((param > 0) && (param < 50))
      {
        chunk.effect_def = ef_TPortamVolSlide;
        chunk.effect = max (param, 15);

        if (param > 15)
        {
          chunk.effect_def2 = ef_TPortamVolSlide;
          chunk.effect2 = max (param - 15, 15);
        }
      }
      else if ((param > 50) && (param < 100))
      {
        chunk.effect_def = ef_TPortamVolSlide;
        chunk.effect = max (param - 50, 15) * 16;

        if (param - 50 > 15)
        {
          chunk.effect_def2 = ef_TPortamVolSlide;
          chunk.effect2 = max (param - 50 - 15, 15);
        }
      }
      break;

    case 10:  // VOLUME SLIDE
      if ((param > 0) && (param < 50))
      {
        chunk.effect_def = ef_VolSlide;
        chunk.effect = max (param, 15);

        if (param > 15)
        {
          chunk.effect_def2 = ef_VolSlide;
          chunk.effect2 = max (param - 15, 15);
        }
      }
      else if ((param > 50) && (param < 100))
      {
        chunk.effect_def = ef_VolSlide;
        chunk.effect = max (param - 50, 15) * 16;

        if (param - 50 > 15)
        {
          chunk.effect_def2 = ef_VolSlide;
          chunk.effect2 = max (param - 50 - 15, 15);
        }
      }
      break;

    case 12:  // SET VOLUME
      chunk.effect_def = ef_SetInsVolume;
      chunk.effect = param <= 63 ? param : 63;
      break;

    case 13:  // JUMP TO NEXT PATTERN IN ORDER LIST
      chunk.effect_def = ef_PatternBreak;
      chunk.effect = param <= 63 ? param : 63;
      break;

    case 15:  // SET SPEED
      chunk.effect_def = ef_SetSpeed;
      chunk.effect = param;
      break;

    default: break;
  }

  // specific corrections for RAd-Tracker event
  if (  (chunk.effect_def == ef_TonePortamento)
     || (chunk.effect_def == ef_TPortamVolSlide))
  {
    if (chunk.note == UINT8_NULL) chunk.note = 0;
    chunk.instr_def = 0;
  }
  if (chunk.note == 0) chunk.instr_def = 0;

  put_chunk (pattern, line, channel + 1, &chunk);
}

// In:
//   * `desc', `progress', `state' and `error' may be NULL.
//
// On success:
//   * Return value is 0.
//   * `desc' (if set) is set to allocated song description structure.
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
int8_t rad_file_loader (const String *_fname, void **desc, progress_callback_t *progress, uint8_t *state, char **error)
{
  tFIXED_SONGDATA *song = &songdata;
  int8_t result = -1;
  void *result_desc = NULL;
  uint8_t result_state = 0;
  char *result_error = NULL;
  FILE *f = NULL;
  rad_header_t *header = NULL;
  long fsize;
  mem_stream_t stream;
  uint16_t pattoffs[RAD_PATTERNS_MAX];
  char fname[255+1];

  DBG_ENTER ("rad_file_loader");

  if (progress != NULL)
  {
    snprintf (progress->msg, sizeof (progress->msg), "Reading file...");
    progress->num_steps = 5;
    progress->value = 1;
    progress->update (progress, 0, -1);
  }

  StringToStr (fname, _fname, sizeof (fname) - 1);
  if ((f = fopen (fname, "rb")) == NULL) goto _err_fopen;

  result = -2;

  // check file size
  if (fseek (f, 0, SEEK_END) != 0) goto _err_fread;
  if ((fsize = ftell (f)) < 0) goto _err_fread;
  if (fsize < (long)sizeof (header)) goto _err_format;
  if (fseek (f, 0, SEEK_SET) != 0) goto _err_fread;

  // read header
  if ((header = malloc (sizeof (*header))) == NULL) goto _err_malloc;
  if (fread (header, sizeof (*header), 1, f) == 0) goto _err_fread;
  if (memcmp (header->id, rad_id, sizeof (header->id)) != 0) goto _err_format;
  result = -3;
  if (header->version != 16) goto _err_version;

  result = -4;

  init_songdata ();
  song->patt_len = RAD_PATTERN_LEN;
  if (adjust_tracks || (song->nm_tracks < RAD_CHANNELS_MAX)) song->nm_tracks = RAD_CHANNELS_MAX;
  tempo = (header->flags & 0x40) != 0 ? 18 : 50;
  speed = (header->flags & 0x1F) != 0 ? header->flags & 0x1F : 2;
  song->tempo = tempo;
  song->speed = speed;
  {
    String_t s = NameOnly (_fname);
    CopyString (songdata_title, (String *)&s, sizeof (songdata_title) - 1);
  }
  result_state = 1;
  if (progress != NULL) next_progress_step (progress);

  // read data
  if (fsize > (long)sizeof (*header))
  {
    void *p = realloc (header, fsize);
    if (p == NULL) goto _err_malloc;
    header = p;
    if (fread (&header[1], fsize - sizeof (*header), 1, f) == 0) goto _err_fread;
  }

  set_mem_stream (&stream, header, fsize);
  seek_stream (&stream, sizeof (*header), SEEK_SET);

  // read description
  if ((header->flags & 0x80) != 0)
  {
    size_t desc_size = 0;
    uint8_t x;

    do
    {
      if (read_bytes (&x, sizeof (x), &stream)) goto _err_eod;
      desc_size++;
    } while (x != 0);
    seek_stream (&stream, -desc_size, SEEK_CUR);
    if (import_rad_description (&result_desc, stream.curptr, desc_size, &result_error)) goto _exit;
    seek_stream (&stream, +desc_size, SEEK_CUR);
  }
  if (progress != NULL) next_progress_step (progress);

  // read instruments
  if (stream.curptr < stream.endptr)
  {
    uint8_t idx;

    do
    {
      if (read_bytes (&idx, sizeof (idx), &stream)) goto _err_eod;

      if ((idx != 0) && ((uint8_t *)stream.curptr + RAD_INSTRUMENT_SIZE <= (uint8_t *)stream.endptr))
      {
        import_hsc_instrument_alt (&song->instr_data[idx - 1], stream.curptr);
        song->instr_data[idx - 1].fine_tune = 0;
        seek_stream (&stream, +RAD_INSTRUMENT_SIZE, SEEK_CUR);
        result_state = 2;
      }
      else break;
    } while ((idx != 0) && (stream.curptr < stream.endptr));
  }
  if (progress != NULL) next_progress_step (progress);

  // read patterns order
  if (stream.curptr < stream.endptr)
  {
    uint8_t order_len;
    if (read_bytes (&order_len, sizeof (order_len), &stream)) goto _err_eod;
    if (order_len != 0)
    {
      uint8_t len = order_len <= sizeof (song->pattern_order) / sizeof (song->pattern_order[0])
                  ? order_len : sizeof (song->pattern_order) / sizeof (song->pattern_order[0]);
      if (read_bytes (song->pattern_order, len, &stream)) goto _err_eod;
      if (order_len > len)
        if (seek_stream (&stream, order_len - len, SEEK_CUR)) goto _err_eod;
    }
  }
  if (progress != NULL) next_progress_step (progress);

  // read patterns
  if (stream.curptr < stream.endptr)
  {
    // read offsets
    if (read_bytes (pattoffs, sizeof (pattoffs), &stream)) goto _err_eod;
    for (uint_least8_t patt = 0; patt < RAD_PATTERNS_MAX; patt++)
      pattoffs [patt] = uint16_LE (pattoffs[patt]);

    // read events
    for (uint_least8_t patt = 0; patt < RAD_PATTERNS_MAX; patt++)
    {
      size_t poff = pattoffs[patt];

      if ((poff != 0) && ((long)poff < fsize))
      {
        uint8_t line;

        seek_stream (&stream, poff, SEEK_SET);
        do
        {
          uint8_t channel;

          if (read_bytes (&line, sizeof (line), &stream)) goto _err_eod;
          do
          {
            uint8_t byte1, byte2, param;

            if (read_bytes (&channel, sizeof (channel), &stream)) goto _err_eod;
            if (read_bytes (&byte1, sizeof (byte1), &stream)) goto _err_eod;
            if (read_bytes (&byte2, sizeof (byte2), &stream)) goto _err_eod;
            if ((byte2 & 0x0F) != 0)
            {
              if (read_bytes (&param, sizeof (param), &stream)) goto _err_eod;
            }
            else param = 0;

            if ((channel & 0x0F) < RAD_CHANNELS_MAX)
              import_rad_event (patt, line & 0x3F, channel & 0x0F, byte1, byte2, param);
          } while (((channel & 0x80) == 0) && (stream.curptr < stream.endptr));
        } while (((line & 0x80) == 0) && (stream.curptr < stream.endptr));
      }
    }
  }
  if (progress != NULL) next_progress_step (progress);

  result = 0;

_exit:
  if (f != NULL) fclose (f);
  if (header != NULL) free (header);
  if (result < 0)
  {
    if (result_desc != NULL) free (result_desc);
    if (state != NULL) *state = result_state;
    if (error != NULL) *error = result_error;
  }
  else
    if (desc != NULL) *desc = result_desc;

  DBG_LEAVE (); //EXIT //rad_file_loader
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

_err_version:
  result_error = "Unsupported file format version";
  goto _exit;

_err_eod:
  result_error = "Unexpected end of input data";
  goto _exit;
}
