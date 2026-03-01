// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// MPU-401 Trakker song loader (*.mtk)
// Patterns structure is similar to that from HSC loader.

#define MTK_INSTRUMENTS_MAX HSC_INSTRUMENTS_MAX
#define MTK_CHANNELS_MAX    HSC_CHANNELS_MAX
#define MTK_PATTERN_LEN     HSC_PATTERN_LEN
#define MTK_PATTERNS_MAX    HSC_PATTERNS_MAX
#define MTK_ORDER_LEN 128

typedef hsc_event_t mtk_event_t;
typedef hsc_pattern_t mtk_pattern_t;
typedef hsc_patterns_t mtk_patterns_t;

#define MTK_ID_SIZE 18
static const char GCC_ATTRIBUTE((nonstring)) mtk_id[MTK_ID_SIZE] = { "mpu401tr\x92kk\xEEr@data" };

#pragma pack(push, 1)
typedef struct
{
  char id[MTK_ID_SIZE];
  uint16_t crc16;
  uint16_t blen;  // unpacked data size
} mtk_header_t;

typedef struct
{
  String title[33+1];
  String composer[33+1];
  String instruments_names[MTK_INSTRUMENTS_MAX][33+1];
  uint8_t instruments_data[MTK_INSTRUMENTS_MAX][12];
  uint8_t order[MTK_ORDER_LEN];
  uint8_t unused;
} mtk_song_header_t;
#pragma pack(pop)

static void next_mtk_step (progress_callback_t *progress)
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
int8_t mtk_file_loader (const String *_fname, progress_callback_t *progress, uint8_t *state, char **error)
{
  tFIXED_SONGDATA *song = &songdata;
  int8_t result = -1;
  uint8_t result_state = 0;
  char *result_error = NULL;
  FILE *f = NULL;
  void *packed_buffer = NULL;
  uint8_t *raw_buffer = NULL;
  mtk_song_header_t *song_header;
  long fsize;
  size_t packed_size, unpacked_size, raw_size;
  uint16_t crc;
  mtk_header_t header;
  char fname[255+1];

  DBG_ENTER ("mtk_file_loader");

  if (progress != NULL)
  {
    snprintf (progress->msg, sizeof (progress->msg), "Reading file...");
    progress->num_steps = 6;
    progress->value = 1;
    progress->update (progress, 0, -1);
  }

  StringToStr (fname, _fname, sizeof (fname) - 1);
  if ((f = fopen (fname, "rb")) == NULL) goto _err_fopen;

  result = -2;

  // check file size
  if (fseek (f, 0, SEEK_END) != 0) goto _err_fread;
  if ((fsize = ftell (f)) < 0) goto _err_fread;
  if (   (fsize < (long)sizeof (header))
      || (fsize > (long)(sizeof (header) + sizeof (mtk_song_header_t) + sizeof (mtk_patterns_t))))
    goto _err_format;
  if (fseek (f, 0, SEEK_SET) != 0) goto _err_fread;

  result = -1;

  if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;

  result = -2;

  if (memcmp (header.id, mtk_id, sizeof (header.id)) != 0) goto _err_format;
  header.blen = uint16_LE (header.blen);
  if (header.blen < sizeof (mtk_song_header_t)) goto _err_format;

  result = -4;
  next_mtk_step (progress);

  // read packed data
  packed_size = fsize - sizeof (header);
  if ((packed_buffer = malloc (packed_size)) == NULL) goto _err_malloc;
  if (fread (packed_buffer, packed_size, 1, f) == 0) goto _err_fread;
  next_mtk_step (progress);

  // checksum
  crc = Update16 (packed_buffer, packed_size, /*CRC16_INITVAL*/0);
  if (crc != header.crc16) goto _err_checksum;
  next_mtk_step (progress);

  // unpack
  raw_size = sizeof (mtk_song_header_t) + sizeof (mtk_patterns_t);
  if ((raw_buffer = malloc (raw_size)) == NULL) goto _err_malloc;
  unpacked_size = RDC_decompress (packed_buffer, raw_buffer, packed_size);
  if (unpacked_size != header.blen) goto _err_unpack;
  free (packed_buffer);
  packed_buffer = NULL;
  if (unpacked_size < raw_size)
    memset (raw_buffer + unpacked_size, raw_size - unpacked_size, 0);
  next_mtk_step (progress);

  // initialize song
  song_header = (mtk_song_header_t *)raw_buffer;
  init_songdata ();
  song->patt_len = MTK_PATTERN_LEN;
  if (adjust_tracks || (song->nm_tracks < MTK_CHANNELS_MAX)) song->nm_tracks = MTK_CHANNELS_MAX;
  tempo = 18;
  speed = 2;
  song->common_flag |= 2;
  song->tempo = tempo;
  song->speed = speed;
  import_old_flags ();
  {
    String_t s;
    if (Length (song_header->title) >= sizeof (song_header->title))
      SetLength (song_header->title, sizeof (song_header->title) - 1);
    s = CutStr (song_header->title);
    CopyString (song->songname, (String *)&s, sizeof (song->songname) - 1);
    if (Length (song_header->composer) >= sizeof (song_header->composer))
      SetLength (song_header->composer, sizeof (song_header->composer) - 1);
    s = CutStr (song_header->composer);
    CopyString (song->composer, (String *)&s, sizeof (song->composer) - 1);
    s = NameOnly (_fname);
    CopyString (songdata_title, (String *)&s, sizeof (songdata_title) - 1);
  }
  result_state = 1;

  // import instruments
  for (uint_least8_t i = 0; i < MTK_INSTRUMENTS_MAX; i++)
  {
    tADTRACK2_INS *ins = &song->instr_data[i];
    String_t s, t;
    import_hsc_instrument_alt (ins, song_header->instruments_data[i]);
    // specific corrections for MPU-401 Trakker instrument
    if (ins->fm_data.KSL_VOLUM_modulator > 128)
      ins->fm_data.KSL_VOLUM_modulator /= 3;
    if (ins->fm_data.KSL_VOLUM_carrier > 128)
      ins->fm_data.KSL_VOLUM_carrier /= 3;
    if (Length (song_header->instruments_names[i]) > sizeof (song_header->instruments_names[0]) - 1)
      SetLength (song_header->instruments_names[i], sizeof (song_header->instruments_names[0]) - 1);
    if (Length (song_header->instruments_names[i]) > 0)
    {
      s = truncate_string (song_header->instruments_names[i]);
      if (s.len >= 10)
        t = Copy ((String *)&s, 10, s.len - 9);
      else
        t.len = 0;
    }
    else t.len = 0;
    s = Copy (song->instr_names[i], 1, 9);
    AppendString ((String *)&s, (String *)&t, sizeof (t) - 1);
    CopyString (song->instr_names[i], (String *)&s, sizeof (song->instr_names[0]) - 1);
  }
  result_state = 2;

  // import patterns order
  for (uint_least8_t i = 0; i < MTK_ORDER_LEN; i++)
    song->pattern_order[i] = song_header->order[i] != 0xFF ? song_header->order[i] : 0x80;

  next_mtk_step (progress);

  // import patterns
  if (header.blen > sizeof (mtk_song_header_t))
  {
    size_t patterns_size = header.blen - sizeof (mtk_song_header_t);
    uint_least8_t num_patterns = (patterns_size + sizeof (mtk_pattern_t) - 1) / sizeof (mtk_pattern_t);
    if (num_patterns > MTK_PATTERNS_MAX) num_patterns = MTK_PATTERNS_MAX;
    import_hsc_patterns (raw_buffer + sizeof (mtk_song_header_t), num_patterns, false);
  }
  next_mtk_step (progress);

  result = 0;

_exit:
  if (f != NULL) fclose (f);
  if (packed_buffer != NULL) free (packed_buffer);
  if (raw_buffer != NULL) free (raw_buffer);
  if (result < 0)
  {
    if (state != NULL) *state = result_state;
    if (error != NULL) *error = result_error;
  }

  DBG_LEAVE (); //EXIT //mtk_file_loader
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

_err_checksum:
  result_error = "Checksum mismatch - file corrupted";
  goto _exit;

_err_unpack:
  result_error = "Failed to unpack input data";
  goto _exit;
}
