// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*****************************************************************************

  Digital-FM Tracker song file loader
  Supported file format versions: all
  Filename extension: .dfm

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
int8_t load_song_dfm (const String *_fname, progress_callback_t *progress, uint8_t *state, char **error)
{
  tFIXED_SONGDATA *song = &songdata;
  int8_t result = -1;
  uint8_t result_state = 0;
  char *result_error = NULL;
  FILE *f = NULL;
  long fsize;
  header_dfm_t header;
  int song_len;
  char fname[255+1];

  DBG_ENTER ("load_song_dfm");

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
      || (fsize > (long)sizeof (header) + (1 + 2 * CHANNELS_MAX_DFM * PATTERN_LEN_DFM) * PATTERNS_MAX_DFM))
    goto _err_format;
  if (fseek (f, 0, SEEK_SET) != 0) goto _err_fread;

  if (fread (&header, sizeof (header), 1, f) == 0)  goto _err_fread;

  result = -2;

  if (memcmp (header.id, id_dfm, sizeof (header.id)) != 0) goto _err_format;

  result = -4;

  if (progress != NULL) progress->num_steps = 4;

  // initialize song
  init_songdata (song);
  song->patt_len = PATTERN_LEN_DFM;
  if (adjust_tracks || (song->nm_tracks < CHANNELS_MAX_DFM)) song->nm_tracks = CHANNELS_MAX_DFM;
  tempo = 135;
  speed = header.tempo + 1; // TODO: check value first
  song->tempo = tempo;
  song->speed = speed;
  song->common_flag |= 1 | 2 | 8 | 0x10;
  apply_song_flags (song);
  get_title_dfm (song, &header.title);
  {
    String_t s = NameOnly (_fname);
    CopyString (songdata_title, (String *)&s, sizeof (songdata_title) - 1);
  }
  result_state = 1;
  if (progress != NULL) next_progress_step (progress);

  for (uint_least8_t i = 0; i < INSTRUMENTS_MAX_DFM; i++)
  {
    get_ins_name_dfm (&song->instr_names[i], &header.ins_names[i]);
    get_ins_data_dfm (&song->instr_data[i], &header.ins_data[i]);
  }
  result_state = 2;
  if (progress != NULL) next_progress_step (progress);

  song_len = get_pat_order_dfm (song, &header.pat_order);
  if (progress != NULL) next_progress_step (progress);

  if (fsize > (long)sizeof (header))
  {
    if (load_patterns_dfm (song, song_len,
                           fsize - sizeof (header),
                           f,
                           &result_error) != 0) goto _exit;
  }
  if (progress != NULL) next_progress_step (progress);

  result = 0;

_exit:
  if (f != NULL) fclose (f);
  if (result < 0)
  {
    if (state != NULL) *state = result_state;
    if (error != NULL) *error = result_error;
  }

  DBG_LEAVE (); //EXIT //load_song_dfm
  return result;

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
