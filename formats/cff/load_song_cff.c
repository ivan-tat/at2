// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Returns `false' on success, `true' on error.
static bool check_header2_cff (header2_cff_t *src)
{
  return (memcmp (src->id, id_cff_v2, sizeof (src->id)) != 0)
      || (src->num_patterns > PATTERNS_MAX_CFF)
      || (src->byteFF != 0xFF);
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
int8_t load_song_cff (const String *_fname, progress_callback_t *progress, uint8_t *state, char **error)
{
  tFIXED_SONGDATA *song = &songdata;
  int8_t result = -1;
  uint8_t result_state = 0;
  char *result_error = NULL;
  FILE *f = NULL;
  uint8_t *raw_data = NULL;
  uint8_t *packed_data = NULL;
  long fsize;
  header_cff_t header;
  bool header2_allocated = false;
  header2_cff_t *header2 = NULL;
  size_t patterns_size;
  void *patterns_data;
  char fname[255+1];

  DBG_ENTER ("load_song_cff");

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
  if (fsize < (long)RAW_DATA_START_CFF) goto _err_format;
  if (fseek (f, 0, SEEK_SET) != 0) goto _err_fread;

  if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;

  result = -2;

  if (memcmp (header.id, id_cff_v1, sizeof (header.id)) != 0) goto _err_format;

  result = -1;  // one more ID to check

  if (progress != NULL) progress->num_steps = 4;

  if (header.packed_flag != 1)  // raw
  {
    // read header
    if (fsize < (long)(RAW_DATA_START_CFF + sizeof (*header2))) goto _err_format;
    if ((header2 = malloc (sizeof (*header2))) == NULL) goto _err_malloc;
    header2_allocated = true;
    if (fseek (f, RAW_DATA_START_CFF, SEEK_SET) != 0) goto _err_fread;
    if (fread (header2, sizeof (*header2), 1, f) == 0) goto _err_fread;
    if (check_header2_cff (header2)) goto _err_format;
    if (progress != NULL) next_progress_step (progress);

    result = -4;

    // read patterns
    patterns_size = fsize - RAW_DATA_START_CFF - sizeof (*header2);
    if (patterns_size != 0)
    {
      if ((raw_data = malloc (patterns_size)) == NULL) goto _err_malloc;
      if (fread (raw_data, patterns_size, 1, f) == 0) goto _err_fread;
    }
    fclose (f);
    f = NULL;
    patterns_data = raw_data;
    if (progress != NULL) next_progress_step (progress);
  }
  else  // packed
  {
    size_t packed_size, raw_limit, raw_size;

    // read packed data
    if (fsize < (long)PACKED_DATA_START_CFF) goto _err_format;
    packed_size = fsize - PACKED_DATA_START_CFF;
    if (packed_size == 0) goto _err_format;
    raw_limit = sizeof (*header2) + sizeof (pat_data_cff_t) * PATTERNS_MAX_CFF;
    if (packed_size > raw_limit) goto _err_format;
    if ((packed_data = malloc (packed_size)) == NULL) goto _err_malloc;
    if (fseek (f, PACKED_DATA_START_CFF, SEEK_SET) != 0) goto _err_fread;
    if (fread (packed_data, packed_size, 1, f) == 0) goto _err_fread;
    fclose (f);
    f = NULL;
    if (progress != NULL) next_progress_step (progress);

    result = -2;

    // unpack
    if ((raw_data = malloc (raw_limit)) == NULL) goto _err_malloc;
    if (unpack_cff (raw_data, raw_limit, &raw_size,
                    packed_data, packed_size, true,
                    &result_error)) goto _exit;
    free (packed_data);
    packed_data = NULL;
    if (progress != NULL) next_progress_step (progress);

    // check format
    if (raw_size < sizeof (*header2)) goto _err_eod;
    header2 = (header2_cff_t *)raw_data;
    if (check_header2_cff (header2)) goto _err_format;

    if (raw_size > sizeof (*header2))
    {
      patterns_size = raw_size - sizeof (*header2);
      patterns_data = &raw_data[sizeof (*header2)];
    }
    else
    {
      patterns_size = 0;
      patterns_data = NULL;
    }

    result = -4;
  }

  // initialize song
  init_songdata (song);
  song->patt_len = PATTERN_LEN_CFF;
  if (adjust_tracks || (song->nm_tracks < CHANNELS_MAX_CFF))
    song->nm_tracks = CHANNELS_MAX_CFF;
  tempo = 51;
  speed = 6;
  song->tempo = tempo;
  song->speed = speed;
  song->common_flag |= 0x02;

  get_title_cff (song, &header2->title);
  get_composer_cff (song, &header2->composer);
  {
    String_t s = NameOnly (_fname);
    CopyString (songdata_title, (String *)&s, sizeof (songdata_title) - 1);
  }

  for (unsigned i = 0; i < INSTRUMENTS_MAX_CFF; i++)
    get_ins_cff (song, i, &header2->instruments[i]);

  if (get_pat_order_cff (song, &header2->pat_order, header2->num_patterns,
                         &result_error)) goto _exit;

  apply_song_flags (song);
  result_state = 2;
  if (progress != NULL) next_progress_step (progress);

  // import patterns
  if (patterns_size != 0)
    get_patterns_cff (song,
                      patterns_data, patterns_size, header2->num_patterns);
  if (progress != NULL) next_progress_step (progress);

  result = 0;

_exit:
  if (f != NULL) fclose (f);
  if ((header2 != NULL) && header2_allocated) free (header2);
  if (raw_data != NULL) free (raw_data);
  if (packed_data != NULL) free (packed_data);
  if (result < 0)
  {
    if (state != NULL) *state = result_state;
    if (error != NULL) *error = result_error;
  }

  DBG_LEAVE (); //EXIT //load_song_cff
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
