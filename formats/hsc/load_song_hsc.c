// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*****************************************************************************

  HSC AdLib Composer / HSC-Tracker song file loader
  Supported file format versions: all
  Filename extension: .hsc

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
int8_t load_song_hsc (const String *_fname, progress_callback_t *progress, uint8_t *state, char **error)
{
  tFIXED_SONGDATA *song = &songdata;
  int8_t result = -1;
  uint8_t result_state = 0;
  char *result_error = NULL;
  FILE *f = NULL;
  header_hsc_t *header = NULL;
  long fsize;
  char fname[255+1];

  DBG_ENTER ("load_song_hsc");

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
  if (   (fsize < (long)sizeof (header_hsc_t))
      || (fsize > (long)(sizeof (header_hsc_t) + sizeof (patterns_hsc_t))))
    goto _err_format;
  if (fseek (f, 0, SEEK_SET) != 0) goto _err_fread;

  if ((header = malloc (sizeof (*header))) == NULL) goto _err_malloc;

  if (fread (header, sizeof (*header), 1, f) == 0) goto _err_fread;

  result = -4;

  if (progress != NULL) progress->num_steps = 3;

  // initialize song
  init_songdata (song);
  song->patt_len = PATTERN_LEN_HSC;
  if (adjust_tracks || (song->nm_tracks < CHANNELS_MAX_HSC)) song->nm_tracks = CHANNELS_MAX_HSC;
  tempo = 18;
  speed = 2;
  song->common_flag |= 2;
  song->tempo = tempo;
  song->speed = speed;
  {
    String_t s = NameOnly (_fname);
    CopyString (songdata_title, (String *)&s, sizeof (songdata_title) - 1);
  }
  apply_song_flags (song);
  result_state = 1;
  if (progress != NULL) next_progress_step (progress);

  get_instruments_hsc (song, header->ins_data, INSTRUMENTS_MAX_HSC);
  result_state = 2;
  get_pat_order_hsc (song, header->pat_order, PATTERN_ORDER_LEN_HSC);

  if (progress != NULL) next_progress_step (progress);

  if (fsize > (long)sizeof (*header))
    if (load_patterns_hsc (song,
                           fsize - sizeof (*header),
                           f,
                           &result_error)) goto _exit;
  if (progress != NULL) next_progress_step (progress);

  result = 0;

_exit:
  if (f != NULL) fclose (f);
  if (header != NULL) free (header);
  if (result < 0)
  {
    if (state != NULL) *state = result_state;
    if (error != NULL) *error = result_error;
  }

  DBG_LEAVE (); //EXIT //load_song_hsc
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
