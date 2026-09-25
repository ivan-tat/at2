// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*****************************************************************************

  Amusic module file format
  Supported file format versions: 16, 17
  Filename extensions: .amd, .xms
  All values are little-endian.

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

  if (progress != NULL) progress->num_steps = header.patterns + 1;

  init_songdata (song);
  song->patt_len = PATTERN_LEN_AMD;
  if (adjust_tracks || (song->nm_tracks < CHANNELS_MAX_AMD))
    song->nm_tracks = CHANNELS_MAX_AMD;
  tempo = 50;
  speed = 6;
  song->tempo = tempo;
  song->speed = speed;
  result_state = 1;

  // import patterns order
  if (header.length > PATTERN_ORDER_LEN_AMD) header.length = PATTERN_ORDER_LEN_AMD;
  for (unsigned i = 0; i < header.length; i++)
    if (header.order[i] <= header.patterns)
    {
      song->pattern_order[i] = header.order[i];
      result_state = 2;
    }

  // import instrument names
  for (int i = 0; i < INSTRUMENTS_MAX_AMD; i++)
  {
    String_t s, t;

    get_ins_data_amd (&song->instr_data[i], &header.instruments[i]);
    s.len = sizeof (header.instruments[0].name);
    memcpy (s.str, header.instruments[i].name, sizeof (header.instruments[0].name));
    t = truncate_string ((String *)&s);
    s = Copy (song->instr_names[i], 1, 9);
    AppendString ((String *)&s, (String *)&t, sizeof (s) - 1);
    CopyString (song->instr_names[i], (String *)&s, sizeof (song->instr_names[0]));
  }
  result_state = 2;

  // import patterns
  switch (header.version)
  {
    case 16:
      if (load_patterns_amd_v16 (header.patterns + 1, f,
                                 progress, &result_error)) goto _exit;
      break;

    case 17:
      if (load_patterns_amd_v17 (header.patterns + 1, f,
                                 progress, &result_error)) goto _exit;
      break;

    default: break;
  }

  song->common_flag |= 0x80;
  {
    String_t s, t;

    StrToString ((String *)&s, header.title, sizeof (header.title));
    t = asciiz_string ((String *)&s);
    s = CutStr ((String *)&t);
    CopyString (song->songname, (String *)&s, sizeof (song->songname) - 1);

    StrToString ((String *)&s, header.composer, sizeof (header.composer));
    t = asciiz_string ((String *)&s);
    s = CutStr ((String *)&t);
    CopyString (song->composer, (String *)&s, sizeof (song->composer) - 1);
  }
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
