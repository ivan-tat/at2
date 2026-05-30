// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*****************************************************************************

  AdLib Tracker II tiny module file loader
  Supported file format versions: 1..14
  Filename extension: .a2t

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
int8_t load_song_a2t (const String *_fname, progress_callback_t *progress, uint8_t *state, char **error)
{
  tFIXED_SONGDATA *song = &songdata;
  int8_t result = -1;
  uint8_t result_state = 0;
  char *result_error = NULL;
  FILE *f = NULL;
  header_a2t_t main_header;
  uint8_t pat_blocks;
  char fname[255+1];

  DBG_ENTER ("load_song_a2t");

  StringToStr (fname, _fname, sizeof (fname) - 1);
  if ((f = fopen (fname, "rb")) == NULL) goto _err_fopen;

  result = -2;

  if (fread (&main_header, sizeof (main_header), 1, f) == 0) goto _err_fread;

  if (memcmp (main_header.id, id_a2t, sizeof (main_header.id)) != 0) goto _err_format;

  result = -3;

  if ((main_header.version < 1) || (main_header.version > 14)) goto _err_version;

  result = -4;

  if (progress != NULL)
  {
    snprintf (progress->msg, sizeof (progress->msg), "%s",
      is_packed_a2t (main_header.version) ? "Decompressing tiny module data..."
                                          : "Reading tiny module data...");
    progress->update (progress, -1, -1);
  }

  if (main_header.version <= 4)
  {
    header_a2t_v1_t header;

    if (main_header.num_patterns > PATTERNS_MAX_A2T_V1) goto _err_format;

    if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;

    pat_blocks = (main_header.num_patterns + 16 - 1) / 16;  // 0..4
    if (progress != NULL) progress->num_steps = 2 + pat_blocks + 1; // 2 + 0..4 + 1

    if (check_crc32_a2t (main_header.crc32, true,
                         16, 16, header.block_sizes, lengthof (header.block_sizes, [0]), 2 + pat_blocks,
                         f,
                         &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);

    init_songdata (song);
    song->patt_len = PATTERN_LEN_A2T_V1;
    if (adjust_tracks || song->nm_tracks < CHANNELS_MAX_A2T_V1) song->nm_tracks = CHANNELS_MAX_A2T_V1;
    result_state = 1;

    if (fseek (f, sizeof (main_header) + sizeof (header), SEEK_SET) != 0) goto _err_fread;

    if (load_ins_data_a2t_v1 (main_header.version, song,
                              uint16_LE (header.block_sizes[0]),
                              f,
                              progress, &result_state, &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);

    if (load_pat_order_a2t_v1 (main_header.version, song,
                               uint16_LE (header.block_sizes[1]),
                               f,
                               progress, &result_state, &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);

    if (main_header.num_patterns != 0)
    {
      size_t block_sizes[4];

      for (uint8_t i = 0; i < lengthof (block_sizes, [0]); i++)
        block_sizes[i] = uint16_LE (header.block_sizes[2 + i]);

      if (load_patterns_a2t_v1 (main_header.version, song,
                                block_sizes, main_header.num_patterns,
                                f,
                                progress, &result_state, &result_error) != 0) goto _exit;
    }

    song->common_flag = 0;
    apply_song_flags (song);
  }
  else if (main_header.version <= 8)
  {
    header_a2t_v5_t header;

    if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;

    if (main_header.num_patterns > PATTERNS_MAX_A2T_V1) goto _err_format;

    pat_blocks = (main_header.num_patterns + 8 - 1) / 8;  // 0..8
    if (progress != NULL) progress->num_steps = 2 + pat_blocks + 1; // 2 + 0..8 + 1

    if (check_crc32_a2t (main_header.crc32, true,
                         16, 16, header.block_sizes, lengthof (header.block_sizes, [0]), 2 + pat_blocks,
                         f,
                         &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);

    init_songdata (song);
    song->patt_len = PATTERN_LEN_A2T_V1;
    if (adjust_tracks || song->nm_tracks < CHANNELS_MAX_A2T_V5) song->nm_tracks = CHANNELS_MAX_A2T_V5;
    result_state = 1;

    if (fseek (f, sizeof (main_header) + sizeof (header), SEEK_SET) != 0) goto _err_fread;

    if (load_ins_data_a2t_v5 (main_header.version, song,
                              uint16_LE (header.block_sizes[0]),
                              f,
                              progress, &result_state, &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);

    if (load_pat_order_a2t_v1 (main_header.version, song,
                               uint16_LE (header.block_sizes[1]),
                               f,
                               progress, &result_state, &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);

    if (main_header.num_patterns != 0)
    {
      size_t block_sizes[8];

      for (uint8_t i = 0; i < lengthof (block_sizes, [0]); i++)
        block_sizes[i] = uint16_LE (header.block_sizes[2 + i]);

      if (load_patterns_a2t_v5 (main_header.version, song,
                                block_sizes, main_header.num_patterns,
                                f,
                                progress, &result_state, &result_error) != 0) goto _exit;
    }

    song->tempo = main_header.tempo;
    song->speed = main_header.speed;
    get_flags_a2t_v5 (song, &header.flags);
    apply_song_flags (song);
  }
  else if (main_header.version == 9)
  {
    header_a2t_v9_t header;

    if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;

    if (main_header.num_patterns > PATTERNS_MAX_A2T_V9) goto _err_format;

    pat_blocks = (main_header.num_patterns + 8 - 1) / 8;  // 0..16
    if (progress != NULL) progress->num_steps = 4 + pat_blocks + 1; // 4 + 0..16 + 1

    if (check_crc32_a2t (main_header.crc32, true,
                         32, 16, header.block_sizes, lengthof (header.block_sizes, [0]), 4 + pat_blocks,
                         f,
                         &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);

    init_songdata (song);
    song->patt_len = uint16_LE (header.pat_length);
    song->nm_tracks = header.num_channels;
    result_state = 1;

    if (fseek (f, sizeof (main_header) + sizeof (header), SEEK_SET) != 0) goto _err_fread;

    if (load_ins_data_a2t_v9 (main_header.version, song,
                              uint32_LE (header.block_sizes[0]),
                              f,
                              progress, &result_state, &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);

    if (load_ins_macros_a2t_v9 (main_header.version, song,
                                uint32_LE (header.block_sizes[1]),
                                f,
                                progress, &result_state, &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);

    if (load_macros_a2t_v9 (main_header.version, song,
                            uint32_LE (header.block_sizes[2]),
                            f,
                            progress, &result_state, &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);

    if (load_pat_order_a2t_v1 (main_header.version, song,
                               uint32_LE (header.block_sizes[3]),
                               f,
                               progress, &result_state, &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);

    if (main_header.num_patterns != 0)
    {
      size_t block_sizes[16];

      for (uint8_t i = 0; i < lengthof (block_sizes, [0]); i++)
        block_sizes[i] = uint32_LE (header.block_sizes[4 + i]);

      if (load_patterns_a2t_v9 (main_header.version, song,
                                block_sizes, main_header.num_patterns,
                                f,
                                progress, &result_state, &result_error) != 0) goto _exit;
    }

    song->tempo = main_header.tempo;
    song->speed = main_header.speed;
    song->common_flag = header.flags;
    song->macro_speedup = uint16_LE (header.macro_speedup);
    apply_song_flags (song);
  }
  else if (main_header.version == 10)
  {
    header_a2t_v10_t header;

    if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;

    if (main_header.num_patterns > PATTERNS_MAX_A2T_V9) goto _err_format;

    pat_blocks = (main_header.num_patterns + 8 - 1) / 8;  // 0..16
    if (progress != NULL) progress->num_steps = 4 + pat_blocks + 1; // 4 + 0..16 + 1

    if (check_crc32_a2t (main_header.crc32, true,
                         32, 16, header.block_sizes, lengthof (header.block_sizes, [0]), 4 + pat_blocks,
                         f,
                         &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);

    init_songdata (song);
    song->patt_len = uint16_LE (header.pat_length);
    song->nm_tracks = header.num_channels;
    result_state = 1;

    if (fseek (f, sizeof (main_header) + sizeof (header), SEEK_SET) != 0) goto _err_fread;

    if (load_ins_data_a2t_v9 (main_header.version, song,
                              uint32_LE (header.block_sizes[0]),
                              f,
                              progress, &result_state, &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);

    if (load_ins_macros_a2t_v9 (main_header.version, song,
                                uint32_LE (header.block_sizes[1]),
                                f,
                                progress, &result_state, &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);

    if (load_macros_a2t_v9 (main_header.version, song,
                            uint32_LE (header.block_sizes[2]),
                            f,
                            progress, &result_state, &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);

    if (load_pat_order_a2t_v1 (main_header.version, song,
                               uint32_LE (header.block_sizes[3]),
                               f,
                               progress, &result_state, &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);

    if (main_header.num_patterns != 0)
    {
      size_t block_sizes[16];

      for (uint8_t i = 0; i < lengthof (block_sizes, [0]); i++)
        block_sizes[i] = uint32_LE (header.block_sizes[4 + i]);

      if (load_patterns_a2t_v9 (main_header.version, song,
                                block_sizes, main_header.num_patterns,
                                f,
                                progress, &result_state, &result_error) != 0) goto _exit;
    }

    song->tempo = main_header.tempo;
    song->speed = main_header.speed;
    get_flags_a2t_v9 (song, &header.flags);
    song->macro_speedup = uint16_LE (header.macro_speedup);
    get_chan_4op_flags_a2t_v10 (song, &header.chan_4op_flags);
    get_chan_flags_a2t_v10 (song, &header.chan_flags);
    apply_song_flags (song);
  }
  else // main_header.version == 11..14
  {
    header_a2t_v11_t header;

    if (main_header.num_patterns > PATTERNS_MAX_A2T_V9) goto _err_format;

    if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;

    pat_blocks = (main_header.num_patterns + 8 - 1) / 8;  // 0..16
    if (progress != NULL) progress->num_steps = 5 + pat_blocks + 1; // 5 + 0..16 + 1

    if (check_crc32_a2t (main_header.crc32, true,
                         32, 16, header.block_sizes, lengthof (header.block_sizes, [0]), 5 + pat_blocks,
                         f,
                         &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);

    init_songdata (song);
    song->patt_len = uint16_LE (header.pat_length);
    song->nm_tracks = header.num_channels;
    result_state = 1;

    if (fseek (f, sizeof (main_header) + sizeof (header), SEEK_SET) != 0) goto _err_fread;

    switch (main_header.version)
    {
      case 11:
        if (load_ins_data_a2t_v9 (main_header.version, song,
                                  uint32_LE (header.block_sizes[0]),
                                  f,
                                  progress, &result_state, &result_error) != 0) goto _exit;
        break;

      case 12:
      case 13:
        if (load_block_0_a2t_v12 (main_header.version, song,
                                  uint32_LE (header.block_sizes[0]),
                                  f,
                                  progress, &result_state, &result_error) != 0) goto _exit;
        break;

      case 14:
      default:
        if (load_block_0_a2t_v14 (main_header.version, song,
                                  uint32_LE (header.block_sizes[0]),
                                  f,
                                  progress, &result_state, &result_error) != 0) goto _exit;
        break;
    }
    if (progress != NULL) next_progress_step (progress);

    if (load_ins_macros_a2t_v9 (main_header.version, song,
                                uint32_LE (header.block_sizes[1]),
                                f,
                                progress, &result_state, &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);

    if (load_macros_a2t_v9 (main_header.version, song,
                            uint32_LE (header.block_sizes[2]),
                            f,
                            progress, &result_state, &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);

    if (load_dis_fmreg_col_a2t_v11 (main_header.version, song,
                                    uint32_LE (header.block_sizes[3]),
                                    f,
                                    progress, &result_state, &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);

    if (load_pat_order_a2t_v1 (main_header.version, song,
                               uint32_LE (header.block_sizes[4]),
                               f,
                               progress, &result_state, &result_error) != 0) goto _exit;
    if (progress != NULL) next_progress_step (progress);

    if (main_header.num_patterns != 0)
    {
      size_t block_sizes[16];

      for (uint8_t i = 0; i < lengthof (block_sizes, [0]); i++)
        block_sizes[i] = uint32_LE (header.block_sizes[5 + i]);

      if (load_patterns_a2t_v9 (main_header.version, song,
                                block_sizes, main_header.num_patterns,
                                f,
                                progress, &result_state, &result_error) != 0) goto _exit;
    }

    song->tempo = main_header.tempo;
    song->speed = main_header.speed;
    get_flags_a2t_v9 (song, &header.flags);
    song->macro_speedup = uint16_LE (header.macro_speedup);
    get_chan_4op_flags_a2t_v10 (song, &header.chan_4op_flags);

    if (main_header.version <= 12)
      get_chan_flags_a2t_v10 (song, &header.chan_flags);
    else // main_header.version == 13..14
      get_chan_flags_a2t_v13 (song, &header.chan_flags);

    apply_song_flags (song);
  }

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

  DBG_LEAVE (); //EXIT //load_song_a2t
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
