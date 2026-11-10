// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*****************************************************************************

  Reality Adlib Tracker song file loader
  Supported file format versions: 1.0 (16)
  Filename extension: .rad

*****************************************************************************/

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
int8_t load_song_rad (const String *_fname, void **desc, progress_callback_t *progress, uint8_t *state, char **error)
{
  tFIXED_SONGDATA *song = &songdata;
  int8_t result = -1;
  void *result_desc = NULL;
  uint8_t result_state = 0;
  char *result_error = NULL;
  FILE *f = NULL;
  header_rad_t *header = NULL;
  long fsize;
  mem_stream_t stream;
  char fname[255+1];

  DBG_ENTER ("load_song_rad");

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
  if (memcmp (header->id, id_rad, sizeof (header->id)) != 0) goto _err_format;
  result = -3;
  if (header->version != 16) goto _err_version;

  result = -4;

  init_songdata (song);
  song->patt_len = PATTERN_LEN_RAD;
  if (adjust_tracks || (song->nm_tracks < CHANNELS_MAX_RAD)) song->nm_tracks = CHANNELS_MAX_RAD;
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

  if ((header->flags & 0x80) != 0)
    if (get_description_rad (&result_desc, &stream, &result_error)) goto _exit;
  if (progress != NULL) next_progress_step (progress);

  if (stream.curptr < stream.endptr)
  {
    if (get_instruments_rad (song, &stream, &result_error)) goto _exit;
    result_state = 2;
  }
  if (progress != NULL) next_progress_step (progress);

  if (stream.curptr < stream.endptr)
    if (get_pat_order_rad (song, &stream, &result_error)) goto _exit;
  if (progress != NULL) next_progress_step (progress);

  if (stream.curptr < stream.endptr)
    if (get_patterns_rad (song, &stream, fsize, &result_error)) goto _exit;
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

  DBG_LEAVE (); //EXIT //load_song_rad
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
