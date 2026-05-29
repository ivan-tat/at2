// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*****************************************************************************

  AdLib Tracker II tiny module file saver
  Supported file format version: 14
  Filename extension: .a2t

*****************************************************************************/

// In:
//   * `progress' and `error' may be NULL.
//
// On success:
//   * Return value: 0.
//   * `error' is untouched.
// On error:
//   * Return value: -1.
//   * `error' (if set) is set to error description.
int8_t save_song_a2t (const String *_fname,
                      tFIXED_SONGDATA *song,
                      progress_callback_t *progress, char **error)
{
  int8_t result = -1;
  char *result_error = NULL;
  FILE *f = NULL;
#pragma pack(push, 1)
  struct
  {
    header_a2t_t     main;
    header_a2t_v11_t data;
  } header;
#pragma pack(pop)
  uint8_t num_instruments;
  char fname[255+1];

  DBG_ENTER ("save_song_a2t");

  StringToStr (fname, _fname, sizeof (fname) - 1);
  if ((f = fopen (fname, "wb")) == NULL) goto _err_fopen;

  // save header

  memcpy (header.main.id, id_a2t, sizeof (header.main.id));
  header.main.crc32 = CRC32_INITVAL;  // we'll update it in the end
  header.main.version = 14;
  count_patterns (&header.main.num_patterns);
  if (header.main.num_patterns > PATTERNS_MAX_A2T_V9)
    header.main.num_patterns = PATTERNS_MAX_A2T_V9;
  header.main.tempo = song->tempo;
  header.main.speed = song->speed;
  put_flags_a2t_v9 (&header.data.flags, song);
  header.data.pat_length = uint16_LE (song->patt_len);
  header.data.num_channels = song->nm_tracks;
  header.data.macro_speedup = uint16_LE (song->macro_speedup);
  put_chan_4op_flags_a2t_v10 (&header.data.chan_4op_flags, song);
  put_chan_flags_a2t_v13 (&header.data.chan_flags, song);
  memset (header.data.block_sizes, 0, sizeof (header.data.block_sizes));  // we'll update it in the end

  if (fwrite (&header, sizeof (header), 1, f) == 0) goto _err_fwrite;

  if (progress != NULL)
    progress->num_steps = 5 + (header.main.num_patterns + 7) / 8 + 1; // 5 + 0..16 + 1

  // save data

  if (progress != NULL)
  {
    snprintf (progress->msg, sizeof (progress->msg), "Compressing tiny module data...");
    progress->step = 1;
    progress->update (progress, -1, -1);
  }

  count_instruments (&num_instruments);

  if (save_block_0_a2t_v14 (header.main.version, f,
                            &header.main.crc32, &header.data.block_sizes[0], num_instruments,
                            song,
                            progress, &result_error) != 0) goto _exit;
  if (progress != NULL) next_saver_step (progress);

  if (save_ins_macros_a2t_v9 (header.main.version, f,
                              &header.main.crc32, &header.data.block_sizes[1], num_instruments,
                              song,
                              progress, &result_error) != 0) goto _exit;
  if (progress != NULL) next_saver_step (progress);

  if (save_macros_a2t_v9 (header.main.version, f,
                          &header.main.crc32, &header.data.block_sizes[2], num_instruments,
                          song,
                          progress, &result_error) != 0) goto _exit;
  if (progress != NULL) next_saver_step (progress);

  if (save_dis_fmreg_col_a2t_v11 (header.main.version, f,
                                  &header.main.crc32, &header.data.block_sizes[3], num_instruments,
                                  song,
                                  progress, &result_error) != 0) goto _exit;
  if (progress != NULL) next_saver_step (progress);

  if (save_pat_order_a2t_v1 (header.main.version, f,
                             &header.main.crc32, &header.data.block_sizes[4],
                             song,
                             progress, &result_error) != 0) goto _exit;
  if (progress != NULL) next_saver_step (progress);

  if (save_patterns_a2t_v9 (header.main.version, f,
                            &header.main.crc32, &header.data.block_sizes[5], header.main.num_patterns,
                            song,
                            progress, &result_error) != 0) goto _exit;

  // update header

  _block_sizes_crc32 (&header.main.crc32,
                      32, 16, header.data.block_sizes, lengthof (header.data.block_sizes, [0]));

  header.main.crc32 = uint32_LE (header.main.crc32);

  if (fseek (f, 0, SEEK_SET) != 0) goto _err_fwrite;
  if (fwrite (&header, sizeof (header), 1, f) == 0) goto _err_fwrite;
  if (progress != NULL) next_saver_step (progress);

  result = 0;

_exit:
  if (f != NULL)
  {
    fclose (f);
    if (result < 0) remove (fname);
  }
  if ((result < 0) && (error != NULL)) *error = result_error;

  DBG_LEAVE (); //EXIT //save_song_a2t
  return result;

_err_fopen:
  result_error = "Failed to open output file for writing";
  goto _exit;

_err_fwrite:
  result_error = "Failed to write output file";
  goto _exit;
}
