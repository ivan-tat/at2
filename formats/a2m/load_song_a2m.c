// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*****************************************************************************

  AdLib Tracker II module file loader
  Supported file format versions: 1..14
  Filename extension: .a2m

*****************************************************************************/

// In:
//   * `progress', `state' and `error' may be NULL.
//
// On success:
//   * Return value:
//     0: file format version is 1..4.
//     1: file format version >= 5.
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
int8_t load_song_a2m (const String *_fname, progress_callback_t *progress, uint8_t *state, char **error)
{
  tFIXED_SONGDATA *song = &songdata;
  int8_t result = -1;
  uint8_t result_state = 0;
  char *result_error = NULL;
  FILE *f = NULL;
  header_a2m_t main_header;
  unsigned pat_blocks;
#if !ADT2PLAY
  uint8_t ins_marks[(INSTRUMENTS_MAX_A2M_V9+7)/8];
  uint8_t pat_marks[(PATTERNS_MAX_A2M_V9+7)/8];
#endif // !ADT2PLAY
  char fname[255+1];

  DBG_ENTER ("load_song_a2m");

  StringToStr (fname, _fname, sizeof (fname) - 1);
  if ((f = fopen (fname, "rb")) == NULL) goto _err_fopen;

  result = -2;

  if (fread (&main_header, sizeof (main_header), 1, f) == 0) goto _err_fread;

  if (memcmp (main_header.id, id_a2m, sizeof (main_header.id)) != 0) goto _err_format;

  result = -3;

  if ((main_header.version < 1) || (main_header.version > 14)) goto _err_version;

  result = -4;

  if (progress != NULL)
  {
    snprintf (progress->msg, sizeof (progress->msg), "%s",
      is_packed_a2m (main_header.version) ? "Decompressing module data..."
                                          : "Reading module data...");
    progress->update (progress, -1, -1);
  }

#if !ADT2PLAY
  _save_instrument_marks (song, ins_marks, INSTRUMENTS_MAX_A2M_V9);
  _save_pattern_marks (song, pat_marks, PATTERNS_MAX_A2M_V9);
#endif // !ADT2PLAY

  if (main_header.version <= 4)
  {
    header_a2m_v1_t header;

    if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;

    pat_blocks = ((main_header.num_patterns > PATTERNS_MAX_A2M_V1 ? PATTERNS_MAX_A2M_V1
                                                                  : main_header.num_patterns) + 16 - 1) / 16; // 0..4
    if (progress != NULL) progress->num_steps = 1 + pat_blocks + 1; // 1 + 0..4 + 1

    if (check_crc32_a2m (main_header.crc32, true,
                         16, 16, header.block_sizes, lengthof (header.block_sizes, [0]), 1 + pat_blocks,
                         f,
                         &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);

    if (fseek (f, sizeof (main_header) + sizeof (header), SEEK_SET) != 0) goto _err_fread;

    init_songdata (song);
    song->patt_len = PATTERN_LEN_A2M_V1;
    if (adjust_tracks || song->nm_tracks < CHANNELS_MAX_A2M_V1) song->nm_tracks = CHANNELS_MAX_A2M_V1;
    result_state = 1;

    if (load_block_0_a2m_v1 (main_header.version, song,
                             uint16_LE (header.block_sizes[0]),
                             f,
                             progress, &result_state, &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);

    if (main_header.num_patterns != 0)
    {
      size_t block_sizes[4];

      for (unsigned i = 0; i < lengthof (block_sizes, [0]); i++)
        block_sizes[i] = uint16_LE (header.block_sizes[1 + i]);

      if (load_patterns_a2m_v1 (main_header.version, song,
                                block_sizes, main_header.num_patterns,
                                f,
                                progress, &result_state, &result_error) != 0) goto _exit;
    }

    song->common_flag = 0;
    apply_song_flags (song);
  }
  else if (main_header.version <= 8)
  {
    header_a2m_v5_t header;

    if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;

    pat_blocks = ((main_header.num_patterns > PATTERNS_MAX_A2M_V1 ? PATTERNS_MAX_A2M_V1
                                                                  : main_header.num_patterns) + 7) / 8; // 0..8
    if (progress != NULL) progress->num_steps = 1 + pat_blocks + 1; // 1 + 0..8 + 1

    if (check_crc32_a2m (main_header.crc32, true,
                         16, 16, header.block_sizes, lengthof (header.block_sizes, [0]), 1 + pat_blocks,
                         f,
                         &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);

    if (fseek (f, sizeof (main_header) + sizeof (header), SEEK_SET) != 0) goto _err_fread;

    init_songdata (song);
    song->patt_len = PATTERN_LEN_A2M_V1;
    if (adjust_tracks || (song->nm_tracks < CHANNELS_MAX_A2M_V5)) song->nm_tracks = CHANNELS_MAX_A2M_V5;
    result_state = 1;

    if (load_block_0_a2m_v5 (main_header.version, song,
                             uint16_LE (header.block_sizes[0]),
                             f,
                             progress, &result_state, &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);

    if (main_header.num_patterns != 0)
    {
      size_t block_sizes[8];

      for (unsigned i = 0; i < lengthof (block_sizes, [0]); i++)
        block_sizes[i] = uint16_LE (header.block_sizes[1 + i]);

      if (load_patterns_a2m_v5 (main_header.version, song,
                                block_sizes, main_header.num_patterns,
                                f,
                                progress, &result_state, &result_error) != 0) goto _exit;
    }

    apply_song_flags (song);
  }
  else if (main_header.version == 9)
  {
    header_a2m_v9_t header;

    if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;

    pat_blocks = ((main_header.num_patterns > PATTERNS_MAX_A2M_V9 ? PATTERNS_MAX_A2M_V9
                                                                  : main_header.num_patterns) + 7) / 8; // 0..16
    if (progress != NULL) progress->num_steps = 1 + pat_blocks + 1; // 1 + 0..16 + 1

    if (check_crc32_a2m (main_header.crc32, true,
                         32, 16, header.block_sizes, lengthof (header.block_sizes, [0]), 1 + pat_blocks,
                         f,
                         &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);

    if (fseek (f, sizeof (main_header) + sizeof (header), SEEK_SET) != 0) goto _err_fread;

    init_songdata (song);
    result_state = 1;

    if (load_block_0_a2m_v9 (main_header.version, song,
                             uint32_LE (header.block_sizes[0]),
                             f,
                             progress, &result_state, &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);

    if (main_header.num_patterns != 0)
    {
      size_t block_sizes[16];

      for (unsigned i = 0; i < lengthof (block_sizes, [0]); i++)
        block_sizes[i] = uint32_LE (header.block_sizes[1 + i]);

      if (load_patterns_a2m_v9 (main_header.version, song,
                                block_sizes, main_header.num_patterns,
                                f,
                                progress, &result_state, &result_error) != 0) goto _exit;
    }

    //apply_song_flags (song);
  }
  else if (main_header.version == 10)
  {
    header_a2m_v9_t header;

    if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;

    pat_blocks = ((main_header.num_patterns > PATTERNS_MAX_A2M_V9 ? PATTERNS_MAX_A2M_V9
                                                                  : main_header.num_patterns) + 7) / 8; // 0..16
    if (progress != NULL) progress->num_steps = 1 + pat_blocks + 1; // 1 + 0..16 + 1

    if (check_crc32_a2m (main_header.crc32, true,
                         32, 16, header.block_sizes, lengthof (header.block_sizes, [0]), 1 + pat_blocks,
                         f,
                         &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);

    if (fseek (f, sizeof (main_header) + sizeof (header), SEEK_SET) != 0) goto _err_fread;

    init_songdata (song);
    result_state = 1;

    if (load_block_0_a2m_v10 (main_header.version, song,
                              uint32_LE (header.block_sizes[0]),
                              f,
                              progress, &result_state, &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);

    if (main_header.num_patterns != 0)
    {
      size_t block_sizes[16];

      for (unsigned i = 0; i < lengthof (block_sizes, [0]); i++)
        block_sizes[i] = uint32_LE (header.block_sizes[1 + i]);

      if (load_patterns_a2m_v9 (main_header.version, song,
                                block_sizes, main_header.num_patterns,
                                f,
                                progress, &result_state, &result_error) != 0) goto _exit;
    }

    //apply_song_flags (song);
  }
  else if (main_header.version == 11)
  {
    header_a2m_v9_t header;

    if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;

    pat_blocks = ((main_header.num_patterns > PATTERNS_MAX_A2M_V9 ? PATTERNS_MAX_A2M_V9
                                                                  : main_header.num_patterns) + 7) / 8; // 0..16
    if (progress != NULL) progress->num_steps = 1 + pat_blocks + 1; // 1 + 0..16 + 1

    if (check_crc32_a2m (main_header.crc32, true,
                         32, 16, header.block_sizes, lengthof (header.block_sizes, [0]), 1 + pat_blocks,
                         f,
                         &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);

    if (fseek (f, sizeof (main_header) + sizeof (header), SEEK_SET) != 0) goto _err_fread;

    init_songdata (song);
    result_state = 1;

    if (load_block_0_a2m_v11 (main_header.version, song,
                              uint32_LE (header.block_sizes[0]),
                              f,
                              progress, &result_state, &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);

    if (main_header.num_patterns != 0)
    {
      size_t block_sizes[16];

      for (unsigned i = 0; i < lengthof (block_sizes, [0]); i++)
        block_sizes[i] = uint32_LE (header.block_sizes[1 + i]);

      if (load_patterns_a2m_v9 (main_header.version, song,
                                block_sizes, main_header.num_patterns,
                                f,
                                progress, &result_state, &result_error) != 0) goto _exit;
    }

    //apply_song_flags (song);
  }
  else if (main_header.version <= 13)
  {
    header_a2m_v9_t header;

    if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;

    pat_blocks = ((main_header.num_patterns > PATTERNS_MAX_A2M_V9 ? PATTERNS_MAX_A2M_V9
                                                                  : main_header.num_patterns) + 7) / 8; // 0..16
    if (progress != NULL) progress->num_steps = 1 + pat_blocks + 1; // 1 + 0..16 + 1

    if (check_crc32_a2m (main_header.crc32, true,
                         32, 16, header.block_sizes, lengthof (header.block_sizes, [0]), 1 + pat_blocks,
                         f,
                         &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);

    if (fseek (f, sizeof (main_header) + sizeof (header), SEEK_SET) != 0) goto _err_fread;

    init_songdata (song);
    result_state = 1;

    if (load_block_0_a2m_v12 (main_header.version, song,
                              uint32_LE (header.block_sizes[0]),
                              f,
                              progress, &result_state, &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);

    if (main_header.num_patterns != 0)
    {
      size_t block_sizes[16];

      for (unsigned i = 0; i < lengthof (block_sizes, [0]); i++)
        block_sizes[i] = uint32_LE (header.block_sizes[1 + i]);

      if (load_patterns_a2m_v9 (main_header.version, song,
                                block_sizes, main_header.num_patterns,
                                f,
                                progress, &result_state, &result_error) != 0) goto _exit;
    }

    //apply_song_flags (song);
  }
  else // main_header.version == 14
  {
    header_a2m_v9_t header;

    if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;

    pat_blocks = ((main_header.num_patterns > PATTERNS_MAX_A2M_V9 ? PATTERNS_MAX_A2M_V9
                                                                  : main_header.num_patterns) + 7) / 8; // 0..16
    if (progress != NULL) progress->num_steps = 1 + pat_blocks + 1; // 1 + 0..16 + 1

    if (check_crc32_a2m (main_header.crc32, true,
                         32, 16, header.block_sizes, lengthof (header.block_sizes, [0]), 1 + pat_blocks,
                         f,
                         &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);

    if (fseek (f, sizeof (main_header) + sizeof (header), SEEK_SET) != 0) goto _err_fread;

    init_songdata (song);
    result_state = 1;

    if (load_block_0_a2m_v14 (main_header.version, song,
                              uint32_LE (header.block_sizes[0]),
                              f,
                              progress, &result_state, &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);

    if (main_header.num_patterns != 0)
    {
      size_t block_sizes[16];

      for (unsigned i = 0; i < lengthof (block_sizes, [0]); i++)
        block_sizes[i] = uint32_LE (header.block_sizes[1 + i]);

      if (load_patterns_a2m_v9 (main_header.version, song,
                                block_sizes, main_header.num_patterns,
                                f,
                                progress, &result_state, &result_error) != 0) goto _exit;
    }

    //apply_song_flags (song);
  }

#if !ADT2PLAY
  _restore_instrument_marks (song, ins_marks, INSTRUMENTS_MAX_A2M_V9);
  _restore_pattern_marks (song, pat_marks, PATTERNS_MAX_A2M_V9);
#endif // !ADT2PLAY

  speed = song->speed;
  tempo = song->tempo;

  {
    String_t s = NameOnly (_fname);
    CopyString (songdata_title, (String *)&s, sizeof (songdata_title) - 1);
  }

  result = main_header.version <= 4 ? 0 : 1;

_exit:
  if (f != NULL) fclose (f);
  if (result < 0)
  {
    if (state != NULL) *state = result_state;
    if (error != NULL) *error = result_error;
  }

  DBG_LEAVE (); //EXIT //load_song_a2m
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
