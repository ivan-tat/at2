// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Surprise! AdLib Tracker song loader (*.sat, *.sa2)
// All values are little-endian.

#define SAT_INSTRUMENTS_MAX 31
#define SAT_CHANNELS_MAX 9
#define SAT_PATTERN_LEN 64
#define SAT_PATTERNS_MAX 64
#define SAT_ORDER_V1_LEN 255
#define SAT_ORDER_V5_LEN 128

#define SAT_ID MK_UINT32('S','A','d','T')

#pragma pack(push, 1)
typedef struct
{
  uint32_t id;
  uint8_t version;
} sat_header_t;

// version 1

#define SAT_INS_NAME_LEN 22

typedef uint8_t sat_ins_data_t[11];
typedef uint8_t sat_ins_names_t[496];

typedef struct
{
  sat_ins_data_t ins_data[SAT_INSTRUMENTS_MAX];
  sat_ins_names_t ins_names;
  uint8_t order[SAT_ORDER_V1_LEN];
  uint16_t num_patterns;
  uint8_t order_len;
  uint8_t restart_pos;
  uint16_t calls_per_second;
  // * here comes pattern data for `num_patterns'
} sat_header_v1_t;

typedef struct
{
  uint8_t
    note,
    ins,
    effect,
    def1,
    def2;
} sat_event_v1_t;

typedef sat_event_v1_t sat_pattern_v1_t[SAT_PATTERN_LEN][SAT_CHANNELS_MAX];

// version 5, 6

typedef struct
{
  sat_ins_data_t data;
  uint8_t arpeggio_start, arpeggio_speed, current_pos, speed_count; // arpeggio (unused)
} sat_ins_v5_t;

typedef struct { uint8_t list[256], commands[256]; } sat_arp_data_t;

typedef struct
{
  sat_ins_v5_t ins_data[SAT_INSTRUMENTS_MAX];
  sat_ins_names_t ins_names;
  uint8_t order[SAT_ORDER_V5_LEN];
  uint16_t num_patterns;
  uint8_t order_len;
  uint8_t restart_pos;
  uint16_t calls_per_second;
  sat_arp_data_t arpeggio_data; // unused
  // * here comes pattern data for `num_patterns'
} sat_header_v5_t;

// version 8, 9

typedef struct
{
  sat_ins_v5_t ins_data[SAT_INSTRUMENTS_MAX];
  sat_ins_names_t ins_names;
  uint8_t order[SAT_ORDER_V5_LEN];
  uint16_t num_patterns;
  uint8_t order_len;
  uint8_t restart_pos;
  uint16_t bpm;
  sat_arp_data_t arpeggio_data; // unused
  uint8_t track_order[SAT_PATTERNS_MAX][SAT_CHANNELS_MAX];
  // * for version 9 here comes a 2-byte `channel_set' bit-filed.
  // * here comes track data for `num_patterns'.
} sat_header_v8_t;

typedef uint8_t sat_event_v8_t[3];

typedef sat_event_v8_t sat_track_v8_t[SAT_PATTERN_LEN];
#pragma pack(pop)

// idx: 0..SAT_INSTRUMENTS_MAX-1
static String_t import_sat_instrument_name (sat_ins_names_t *ins_names, uint8_t idx)
{
  mem_stream_t stream;
  uint8_t cur_ins = 0;
  String_t r = { .len = 0 };

  set_mem_stream (&stream, ins_names, sizeof (*ins_names));

  while (stream.curptr < stream.endptr)
  {
    uint8_t c;

    if (read_bytes (&c, sizeof (c), &stream)) break;
    if (c == 16) cur_ins++;
    if (cur_ins == idx + 1)
    {
      if (stream.curptr < stream.endptr)
      {
        do
        {
          if (read_bytes (&c, sizeof (c), &stream)) break;
          if (c >= '\x20') r.str[r.len++] = c;
        } while ((stream.curptr < stream.endptr) && (c >= '\x20') && (r.len < SAT_INS_NAME_LEN));
      }
      break;
    }
  }

  return r;
}

static void import_sat_order (tFIXED_SONGDATA *song, const uint8_t *order, uint8_t len, uint8_t restart)
{
  uint8_t i;

  if (len > AT_ORDER_LEN) len = AT_ORDER_LEN;

  for (i = 0; i < len; i++)
    if (order[i] < SAT_PATTERNS_MAX)
      song->pattern_order[i] = order[i];

  if (i + 1 < len)
    song->pattern_order[i + 1] = AT_ORDER_JUMP + (restart < len ? restart : 0);
}

static uint8_t _sar (int16_t op1, int16_t op2) { return op1 >> op2; }

static void import_sat_effect (uint8_t effect, uint8_t def1, uint8_t def2,
                               uint8_t *out1, uint8_t *out2)
{
  switch (effect)
  {
    case 0: // NORMAL PLAY | ARPEGGIO
      *out1 = ef_Arpeggio;
      *out2 = (def1 << 4) + def2;
      break;

    case 1: // SLIDE UP
      *out1 = ef_FSlideUp;
      *out2 = (def1 << 4) + def2;
      break;

    case 2: // SLIDE DOWN
      *out1 = ef_FSlideDown;
      *out2 = (def1 << 4) + def2;
      break;

    case 3: // TONE PORTAMENTO
      *out1 = ef_TonePortamento;
      *out2 = (def1 << 4) + def2;
      break;

    case 4: // VIBRATO
      *out1 = ef_Vibrato;
      *out2 = (def1 << 4) + def2;
      break;

    case 5: // TONE PORTAMENTO + VOLUME SLIDE
      if (def1 + def2 != 0)
      {
        if ((def1 >= 1) && (def1 <= 15))
        {
          *out1 = ef_TPortamVolSlide;
          *out2 = min (_sar (def1, 2), 1) << 4;
        }
        else
        {
          *out1 = ef_TPortamVolSlide;
          *out2 = min (_sar (def2, 2), 1);
        }
      }
      else
      {
        *out1 = ef_TPortamVolSlide;
        *out2 = (def1 << 4) + def2;
      }
      break;

    case 6: // VIBRATO + VOLUME SLIDE
      if (def1 + def2 != 0)
      {
        if ((def1 >= 1) && (def1 <= 15))
        {
          *out1 = ef_VibratoVolSlide;
          *out2 = min (_sar (def1, 2), 1) << 4;
        }
        else
        {
          *out1 = ef_VibratoVolSlide;
          *out2 = min (_sar (def2, 2), 1);
        }
      }
      else
      {
        *out1 = ef_VibratoVolSlide;
        *out2 = (def1 << 4) + def2;
      }
      break;

    case 8: // RELEASE SUSTAINING SOUND
      *out1 = ef_Extended;
      *out2 = (ef_ex_ExtendedCmd2 << 4) + ef_ex_cmd2_RSS;
      break;

    case 10:  // VOLUME SLIDE
      if (def1+def2 != 0)
      {
        if ((def1 >= 1) && (def1 <= 15))
        {
          *out1 = ef_VolSlide;
          *out2 = min (_sar (def1, 2), 1) << 4;
        }
        else
        {
          *out1 = ef_VolSlide;
          *out2 = min (_sar (def2, 2), 1);
        }
      }
      else
      {
        *out1 = ef_VolSlide;
        *out2 = (def1 << 4) + def2;
      }
      break;

    case 11:  // POSITION JUMP
      if (def1*16+def2 < 128)
      {
        *out1 = ef_PositionJump;
        *out2 = (def1 << 4) + def2;
      }
      break;

    case 12:  // SET VOLUME
      *out1 = ef_SetInsVolume;
      *out2 = max ((def1 << 4) + def2, 63);
      break;

    case 13:  // PATTERN BREAK
      if ((def1 << 4) + def2 < 64)
      {
        *out1 = ef_PatternBreak;
        *out2 = (def1 << 4) + def2;
      }
      break;

    case 15:  // SET SPEED
      if ((def1 << 4) + def2 < 0x20)
      {
        *out1 = ef_SetSpeed;
        *out2 = (def1 << 4) + def2;
      }
      else if ((def1 < 16) && (def2 < 16))
      {
        *out1 = ef_SetTempo;
        *out2 = ((def1 << 4) + def2) / 2.5f;
      }
      else
      {
        *out1 = 0;
        *out2 = 0;
      }
      break;

    default: break;
  }
}

static void import_sat_event_v1 (uint8_t patt, uint8_t line, uint8_t chan,
                                 const sat_event_v1_t *ev, uint8_t note_inc)
{
  tCHUNK chunk;

  memset (&chunk, 0, sizeof (chunk));

  if ((ev->note >= 1) && (ev->note <= 12*8+1))
    chunk.note = ev->note + note_inc;

  if ((ev->ins >= 1) && (ev->ins <= 31))
    chunk.instr_def = ev->ins;

  import_sat_effect (ev->effect, ev->def1, ev->def2, &chunk.effect_def, &chunk.effect);

  if (   (chunk.effect_def == ef_Extended)
      && (chunk.effect     == (ef_ex_ExtendedCmd2 << 4) + ef_ex_cmd2_RSS)
      && (chunk.note       == AT_NOTE_EMPTY))
  {
    chunk.note       = AT_NOTE_OFF;
    chunk.effect_def = 0;
    chunk.effect     = 0;
  }

  put_chunk (patt, line, chan + 1, &chunk);
}

static void import_sat_event_v8 (uint8_t patt, uint8_t line, uint8_t chan,
                                 sat_event_v8_t *ev)
{
  tCHUNK chunk;
  uint8_t note, ins;

  memset (&chunk, 0, sizeof (chunk));

  note = (*ev)[0] >> 1;
  if ((note >= 1) && (note <= 12*8+1)) chunk.note = note;

  ins = (((*ev)[0] & 1) << 4) + ((*ev)[1] >> 4);
  if ((ins >= 1) && (ins <= SAT_INSTRUMENTS_MAX)) chunk.instr_def = ins;

  import_sat_effect ((*ev)[1] & 0x0F, (*ev)[2] >> 4, (*ev)[2] & 0x0F, &chunk.effect_def, &chunk.effect);

  if (   (chunk.effect_def == ef_Extended)
      && (chunk.effect     == (ef_ex_ExtendedCmd2 << 4) + ef_ex_cmd2_RSS)
      && (chunk.note       == AT_NOTE_EMPTY))
  {
    chunk.note       = AT_NOTE_OFF;
    chunk.effect_def = 0;
    chunk.effect     = 0;
  }

  put_chunk (patt, line, chan + 1, &chunk);
}

static void next_sat_step (progress_callback_t *progress)
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
int8_t sat_file_loader (const String *_fname, progress_callback_t *progress, uint8_t *state, char **error)
{
  tFIXED_SONGDATA *song = &songdata;
  int8_t result = -1;
  uint8_t result_state = 0;
  char *result_error = NULL;
  FILE *f = NULL;
  sat_header_t header;
  union
  {
    void *data;
    sat_header_v1_t *v1;
    sat_header_v5_t *v5;
    sat_header_v8_t *v8;
  } song_header = { .data = NULL };
  uint16_t channels_set;  // active channels (bit-field)
  uint8_t note_inc;
  uint16_t num_patterns, num_tracks;
  sat_pattern_v1_t *patterns = NULL;
  sat_track_v8_t *tracks = NULL;
  char fname[255+1];

  DBG_ENTER ("sat_file_loader");

  if (progress != NULL)
  {
    snprintf (progress->msg, sizeof (progress->msg), "Reading file...");
    progress->num_steps = 4;
    progress->value = 1;
    progress->update (progress, 0, -1);
  }

  StringToStr (fname, _fname, sizeof (fname) - 1);
  if ((f = fopen (fname, "rb")) == NULL) goto _err_fopen;

  if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;

  result = -2;

  if (header.id != SAT_ID) goto _err_format;

  result = -4;

  switch (header.version)
  {
    case 1:
      if ((song_header.v1 = malloc (sizeof (*song_header.v1))) == NULL) goto _err_malloc;
      if (fread (song_header.v1, sizeof (*song_header.v1), 1, f) == 0) goto _err_fread;
      song_header.v1->num_patterns = uint16_LE (song_header.v1->num_patterns);
      song_header.v1->calls_per_second = uint16_LE (song_header.v1->calls_per_second);
      if (song_header.v1->num_patterns > SAT_PATTERNS_MAX) goto _err_format;
      num_patterns = song_header.v1->num_patterns;
      num_tracks = 0; // not used
      note_inc = 12 * 2;
      break;

    case 5:
    case 6:
      if ((song_header.v5 = malloc (sizeof (*song_header.v5))) == NULL) goto _err_malloc;
      if (fread (song_header.v5, sizeof (*song_header.v5), 1, f) == 0) goto _err_fread;
      song_header.v5->num_patterns = uint16_LE (song_header.v5->num_patterns);
      song_header.v5->calls_per_second = uint16_LE (song_header.v5->calls_per_second);
      if (   (song_header.v5->num_patterns > SAT_PATTERNS_MAX)
          || (song_header.v5->order_len > SAT_ORDER_V5_LEN)) goto _err_format;
      num_patterns = song_header.v5->num_patterns;
      num_tracks = 0; // not used
      note_inc = header.version == 5 ? 12 : 0;
      break;

    case 8:
    case 9:
      if ((song_header.v8 = malloc (sizeof (*song_header.v8))) == NULL) goto _err_malloc;
      if (fread (song_header.v8, sizeof (*song_header.v8), 1, f) == 0) goto _err_fread;
      song_header.v8->num_patterns = uint16_LE (song_header.v8->num_patterns);
      song_header.v8->bpm = uint16_LE (song_header.v8->bpm);
      if (   (song_header.v8->num_patterns > SAT_PATTERNS_MAX)
          || (song_header.v8->order_len > SAT_ORDER_V5_LEN)) goto _err_format;
      if (header.version == 9)
      {
        if (fread (&channels_set, sizeof (channels_set), 1, f) == 0) goto _err_fread;
        channels_set = uint16_LE (channels_set);
      }
      num_patterns = song_header.v5->num_patterns;
      num_tracks = SAT_CHANNELS_MAX * num_patterns;
      note_inc = 0; // not used
      break;

    default:
      result = -3;
      goto _err_version;
  }

  // calculate actual number of patterns or tracks
  {
    long size, pos;
    size_t count;

    if ((pos = ftell (f)) < 0) goto _err_fread;
    if (fseek (f, 0, SEEK_END)) goto _err_fread;
    if ((size = ftell (f)) < 0) goto _err_fread;
    if ((pos != size) && (fseek (f, pos, SEEK_SET) != 0)) goto _err_fread;
    size -= pos;
    switch (header.version)
    {
      case 1:
      case 5:
      case 6:
        count = size / sizeof (sat_pattern_v1_t);
        if (num_patterns > count) num_patterns = count;
        break;

      case 8:
      case 9:
        count = size / sizeof (sat_track_v8_t);
        if (num_tracks > count) num_tracks = count;
        break;

      default: break;
    }
  }

  init_songdata ();
  if (adjust_tracks || (song->nm_tracks < SAT_CHANNELS_MAX)) song->nm_tracks = SAT_CHANNELS_MAX;
  song->patt_len = SAT_PATTERN_LEN;
  switch (header.version)
  {
    case 1:
      tempo = song_header.v1->calls_per_second <= 255 ? song_header.v1->calls_per_second : 255;
      break;

    case 5:
    case 6:
      tempo = song_header.v5->calls_per_second <= 255 ? song_header.v5->calls_per_second : 255;
      break;

    case 8:
    case 9:
      {
        uint_least16_t new_tempo = song_header.v8->bpm / 2.5f;
        tempo = new_tempo <= 255 ? new_tempo : 255;
      }
      break;

    default: break;
  }
  speed = 6;
  song->tempo = tempo;
  song->speed = speed;
  song->common_flag |= 0x08 | 0x10;
  import_old_flags ();
  for (uint8_t chan = 0; chan < AT_CHANNELS_MAX; chan++)
    song->lock_flags[chan] |= 0x04 | 0x08; // must be after a call to `import_old_flags()'
  {
    String_t s = NameOnly (_fname);
    CopyString (songdata_title, (String *)&s, sizeof (songdata_title) - 1);
  }
  result_state = 1;
  next_sat_step (progress);

  // import instruments
  for (uint8_t i = 0; i < SAT_INSTRUMENTS_MAX; i++)
  {
    String_t s;

    switch (header.version)
    {
      case 1:
        import_sat_instrument_alt (&song->instr_data[i], &song_header.v1->ins_data[i]);
        break;

      case 5:
      case 6:
        import_sat_instrument_alt (&song->instr_data[i], &song_header.v5->ins_data[i].data);
        break;

      case 8:
      case 9:
        import_sat_instrument_alt (&song->instr_data[i], &song_header.v8->ins_data[i].data);
        break;

      default: break;
    }
    if (Length (song->instr_names[i]) > 9) SetLength (song->instr_names[i], 9);
    switch (header.version)
    {
      case 1:
        s = import_sat_instrument_name (&song_header.v1->ins_names, i);
        break;

      case 5:
      case 6:
        s = import_sat_instrument_name (&song_header.v5->ins_names, i);
        break;

      case 8:
      case 9:
        s = import_sat_instrument_name (&song_header.v8->ins_names, i);
        break;

      default: break;
    }
    s = truncate_string ((String *)&s);
    AppendString (song->instr_names[i], (String *)&s, sizeof (song->instr_names[0]) - 1);
  }
  result_state = 2;
  next_sat_step (progress);

  // import patterns order
  switch (header.version)
  {
    case 1:
      import_sat_order (song, song_header.v1->order, song_header.v1->order_len, song_header.v1->restart_pos);
      free (song_header.data);
      song_header.data = NULL;
      break;

    case 5:
    case 6:
      import_sat_order (song, song_header.v5->order, song_header.v5->order_len, song_header.v5->restart_pos);
      free (song_header.data);
      song_header.data = NULL;
      break;

    case 8:
    case 9:
      import_sat_order (song, song_header.v8->order, song_header.v8->order_len, song_header.v8->restart_pos);
      break;

    default: break;
  }
  next_sat_step (progress);

  // import patterns
  if (num_patterns != 0)
    switch (header.version)
    {
      case 1:
      case 5:
      case 6:
        {
          size_t size = sizeof (sat_pattern_v1_t) * num_patterns;
          const sat_event_v1_t *event;

          if ((patterns = malloc (size)) == NULL) goto _err_malloc;
          if (fread (patterns, size, 1, f) == 0) goto _err_fread;

          event = &(*patterns)[0][0];
          for (uint8_t patt = 0; patt < num_patterns; patt++)
            for (uint8_t line = 0; line < SAT_PATTERN_LEN; line++)
              for (uint8_t chan = 0; chan < SAT_CHANNELS_MAX; chan++)
              {
                import_sat_event_v1 (patt, line, chan, event, note_inc);
                event++;
              }
        }
        break;

      case 8:
      case 9:
        {
          size_t size = sizeof (sat_track_v8_t) * num_tracks;
          uint8_t patt = 0;
          int32_t line = 0;
          int32_t chan = 0;

          if ((tracks = malloc (size)) == NULL) goto _err_malloc;
          if (fread (tracks, size, 1, f) == 0) goto _err_fread;

          do
          {
            while (   (patt < num_patterns)
                   && (song_header.v8->track_order[patt][chan] == 0))
            {
              if (++chan == SAT_CHANNELS_MAX)
              {
                chan = 0;
                patt++;
              }
            }

            if (patt < num_patterns)
            {
              uint8_t track = song_header.v8->track_order[patt][chan] - 1;

              if (track < num_tracks)
              {
                import_sat_event_v8 (patt, line, chan, &(tracks[track][line]));
                if (++line == SAT_PATTERN_LEN)
                {
                  line = 0;
                  if (++chan == SAT_CHANNELS_MAX)
                  {
                    chan = 0;
                    patt++;
                  }
                }
              }
            }
          } while (patt < num_patterns);
        }
        break;

      default: break;
    }
  next_sat_step (progress);

  result = 0;

_exit:
  if (f != NULL) fclose (f);
  if (song_header.data != NULL) free (song_header.data);
  if (patterns != NULL) free (patterns);
  if (tracks != NULL) free (tracks);
  if (result < 0)
  {
    if (state != NULL) *state = result_state;
    if (error != NULL) *error = result_error;
  }

  DBG_LEAVE (); //EXIT //sat_file_loader
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
}
