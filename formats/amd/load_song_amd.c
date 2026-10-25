// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*****************************************************************************

  Amusic module file loader
  Supported file format versions: 16, 17
  Filename extensions: .amd, .xms

*****************************************************************************/

// In:
//   * `progress', `state' and `error' may be NULL.
//
// On success:
//   * Return value:
//     0: ID is AMD.
//     1: ID is XMS.
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
int8_t load_song_amd (const String *_fname,
                      progress_callback_t *progress, uint8_t *state, char **error)
{
  tFIXED_SONGDATA *song = &songdata;
  int8_t result = -1;
  uint8_t result_state = 0;
  char *result_error = NULL;
  FILE *f = NULL;
  header_amd_t header;
  unsigned num_patterns;
  bool is_amd;
  char fname[255+1];

  DBG_ENTER ("load_song_amd");

  if (progress != NULL)
  {
    snprintf (progress->msg, sizeof (progress->msg), "Reading file...");
    progress->value = 1;
    progress->update (progress, 0, -1);
  }

  StringToStr (fname, _fname, sizeof (fname) - 1);
  if ((f = fopen (fname, "rb")) == NULL) goto _err_fopen;

  if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;

  result = -2;

  is_amd = memcmp (header.id, id_amd, sizeof (header.id)) == 0;
  if (!is_amd && (memcmp (header.id, id_xms, sizeof (header.id)) != 0)) goto _err_format;

  result = -3;

  if ((header.version != 16) && (header.version != 17)) goto _err_version;

  result = -4;

  num_patterns = header.num_patterns + 1;
  if (progress != NULL) progress->num_steps = num_patterns;

  init_songdata (song);
  song->patt_len = PATTERN_LEN_AMD;
  if (adjust_tracks || (song->nm_tracks < CHANNELS_MAX_AMD))
    song->nm_tracks = CHANNELS_MAX_AMD;
  tempo = 50;
  speed = 6;
  song->tempo = tempo;
  song->speed = speed;
  result_state = 1;

  get_pat_order_amd (song,
                     &header.pat_order, header.pat_order_len,
                     num_patterns,
                     &result_state);

  for (int i = 0; i < INSTRUMENTS_MAX_AMD; i++)
  {
    get_ins_name_amd (&song->instr_names[i], &header.instruments[i].name);
    get_ins_data_amd (&song->instr_data[i], &header.instruments[i].fm_data);
  }
  result_state = 2;

  switch (header.version)
  {
    case 16:
      if (load_patterns_amd_v16 (num_patterns, f,
                                 progress, &result_error)) goto _exit;
      break;

    case 17:
      if (load_patterns_amd_v17 (num_patterns, f,
                                 progress, &result_error)) goto _exit;
      break;

    default: break;
  }

  song->common_flag |= 0x80;
  get_title_amd (song, &header.title);
  get_composer_amd (song, &header.composer);
  apply_song_flags (song);
  {
    String_t s = NameOnly (_fname);
    CopyString (songdata_title, (String *)&s, sizeof (songdata_title) - 1);
  }
  result = is_amd ? 0 : 1;

_exit:
  if (f != NULL) fclose (f);
  if (result < 0)
  {
    if (state != NULL) *state = result_state;
    if (error != NULL) *error = result_error;
  }

  DBG_LEAVE (); //EXIT //load_song_amd
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

_err_version:
  result_error = "Unsupported file format version";
  goto _exit;
}
