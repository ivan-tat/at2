// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*****************************************************************************

  MPU-401 Trakker song file loader
  Supported file format versions: all
  Filename extension: .mtk

*****************************************************************************/

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
int8_t load_song_mtk (const String *_fname, progress_callback_t *progress, uint8_t *state, char **error)
{
  tFIXED_SONGDATA *song = &songdata;
  int8_t result = -1;
  uint8_t result_state = 0;
  char *result_error = NULL;
  FILE *f = NULL;
  void *packed_buffer = NULL;
  uint8_t *raw_buffer = NULL;
  song_header_mtk_t *song_header;
  long fsize;
  size_t packed_size, unpacked_size, raw_size;
  uint16_t crc;
  header_mtk_t header;
  char fname[255+1];

  DBG_ENTER ("load_song_mtk");

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
      || (fsize > (long)(sizeof (header) + sizeof (song_header_mtk_t) + sizeof (patterns_mtk_t))))
    goto _err_format;
  if (fseek (f, 0, SEEK_SET) != 0) goto _err_fread;

  result = -1;

  if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;

  result = -2;

  if (memcmp (header.id, id_mtk, sizeof (header.id)) != 0) goto _err_format;
  header.crc16 = uint16_LE (header.crc16);
  header.blen = uint16_LE (header.blen);
  if (header.blen < sizeof (song_header_mtk_t)) goto _err_format;

  result = -4;
  if (progress != NULL) next_progress_step (progress);

  // read packed data
  packed_size = fsize - sizeof (header);
  if ((packed_buffer = malloc (packed_size)) == NULL) goto _err_malloc;
  if (fread (packed_buffer, packed_size, 1, f) == 0) goto _err_fread;
  if (progress != NULL) next_progress_step (progress);

  // checksum
  crc = Update16 (packed_buffer, packed_size, /*CRC16_INITVAL*/0);
  if (crc != header.crc16) goto _err_checksum;
  if (progress != NULL) next_progress_step (progress);

  // unpack
  raw_size = sizeof (song_header_mtk_t) + sizeof (patterns_mtk_t);
  if ((raw_buffer = malloc (raw_size)) == NULL) goto _err_malloc;
  unpacked_size = RDC_decompress (packed_buffer, raw_buffer, packed_size);
  if (unpacked_size != header.blen) goto _err_unpack;
  free (packed_buffer);
  packed_buffer = NULL;
  if (unpacked_size < raw_size) // clear tail
    memset (raw_buffer + unpacked_size, 0, raw_size - unpacked_size);
  if (progress != NULL) next_progress_step (progress);

  // initialize song
  song_header = (song_header_mtk_t *)raw_buffer;
  init_songdata (song);
  song->patt_len = PATTERN_LEN_MTK;
  if (adjust_tracks || (song->nm_tracks < CHANNELS_MAX_MTK)) song->nm_tracks = CHANNELS_MAX_MTK;
  tempo = 18;
  speed = 2;
  song->common_flag |= 2;
  song->tempo = tempo;
  song->speed = speed;
  apply_song_flags (song);
  get_title_mtk (song, &song_header->title);
  get_composer_mtk (song, &song_header->composer);
  {
    String_t s = NameOnly (_fname);
    CopyString (songdata_title, (String *)&s, sizeof (songdata_title) - 1);
  }
  get_instruments_mtk (song, song_header->ins_names,
                             song_header->ins_data, INSTRUMENTS_MAX_MTK);
  get_pat_order_mtk (song, song_header->pat_order, PATTERN_ORDER_LEN_MTK);
  result_state = 2;
  if (progress != NULL) next_progress_step (progress);

  if (header.blen > sizeof (song_header_mtk_t))
  {
    size_t patterns_size = header.blen - sizeof (song_header_mtk_t);
    unsigned num_patterns = (patterns_size + sizeof (pat_data_mtk_t) - 1) / sizeof (pat_data_mtk_t);
    get_patterns_mtk (song, raw_buffer + sizeof (song_header_mtk_t), max (num_patterns, PATTERNS_MAX_MTK), false);
  }
  if (progress != NULL) next_progress_step (progress);

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

  DBG_LEAVE (); //EXIT //load_song_mtk
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
