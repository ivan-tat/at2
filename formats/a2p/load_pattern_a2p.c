// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*****************************************************************************

  AdLib Tracker II pattern file loader
  Supported file format versions: 1..11
  Filename extension: .a2p

*****************************************************************************/

// In:
//   * `progress', `state' and `error' may be NULL.
//
// On success:
//   * Return value: 0.
//   * `state' and `error' are untouched.
// On error:
//   * Return value:
//     -1: open/read error.
//     -2: unknown file format.
//     -3: unsupported file format version.
//     -4: format is supported.
//   * `state' (if set):
//     0: pattern(s) is(are) untouched.
//     1: pattern(s) is(are) cleared.
//     2: pattern(s) is(are) partly loaded.
//   * `error' (if set) is set to error description.
int8_t load_pattern_a2p (const String *_fname, progress_callback_t *progress, uint8_t *state, char **error)
{
  tFIXED_SONGDATA *song = &songdata;
  uint8_t _pattern = pattern2use != UINT8_NULL ? pattern2use : pattern_patt;
  FILE *f = NULL;
  int8_t result = -1;
  uint8_t result_state = 0;
  char *result_error = NULL;
  header_a2p_t main_header;
  char fname[255+1];
  String_t onlyname;

  DBG_ENTER ("load_pattern_a2p");

  StringToStr (fname, _fname, sizeof (fname) - 1);
  {
    String_t s = NameOnly (_fname);
    onlyname = Lower ((String *)&s);
  }

  if ((f = fopen (fname, "rb")) == NULL) goto _err_fopen;
  if (fread (&main_header, sizeof (main_header), 1, f) == 0) goto _err_fread;

  result = -2;

  if (memcmp (main_header.id, id_a2p, sizeof (main_header.id)) != 0) goto _err_format;

  result = -3;

  if ((main_header.version < 1) || (main_header.version > 11)) goto _err_version;

  result = -4;

  if (main_header.version <= 8)
  {
    header_a2p_v1_t header;

    if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;

    if (progress != NULL) progress->num_steps = 1 + 1;

    if (check_crc32_a2p (main_header.crc32, true,
                         16, 16, &header.block_size, 1, 1,
                         f,
                         &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);

    if (fseek (f, sizeof (main_header) + sizeof (header), SEEK_SET) != 0) goto _err_fread;

    if (main_header.version <= 4)
    {
      if (load_pattern_a2p_v1 (main_header.version, song, _pattern, (String *)&onlyname,
                               uint16_LE (header.block_size),
                               f,
                               progress, &result_state, &result_error) != 0) goto _exit;
    }
    else  // main_header.version == 5..8
      if (load_pattern_a2p_v5 (main_header.version, song, _pattern, (String *)&onlyname,
                               uint16_LE (header.block_size),
                               f,
                               progress, &result_state, &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);
  }
  else // main_header.version == 9..11
  {
    header_a2p_v9_t header;

    if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;

    if (progress != NULL) progress->num_steps = 1 + 1;

    if (check_crc32_a2p (main_header.crc32, true,
                         32, 16, &header.block_size, 1, 1,
                         f,
                         &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);

    if (fseek (f, sizeof (main_header) + sizeof (header), SEEK_SET) != 0) goto _err_fread;

    if (main_header.version == 9)
    {
      if (load_pattern_a2p_v9 (main_header.version, song, _pattern, (String *)&onlyname,
                               uint32_LE (header.block_size),
                               f,
                               progress, &result_state, &result_error) != 0) goto _exit;
    }
    else  // main_header.version == 10..11
      if (load_pattern_a2p_v10 (main_header.version, song, _pattern, (String *)&onlyname,
                                uint32_LE (header.block_size),
                                f,
                                progress, &result_state, &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);
  }

  result = 0;

_exit:
#if !ADT2PLAY
  if (result_state != 0) module_archived = false;
#endif // !ADT2PLAY
  if (f != NULL) fclose (f);
  if (result < 0)
  {
    if (state != NULL) *state = result_state;
    if (error != NULL) *error = result_error;
  }

  DBG_LEAVE (); //EXIT //load_pattern_a2p
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
