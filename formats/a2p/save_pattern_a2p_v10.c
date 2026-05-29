// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// In:
//   * `version': 10..11.
//   * `idx': 0..PATTERNS_MAX-1.
//   * `progress' and `error' may be NULL.
//
// On success:
//   * Return value: 0.
//   * `crc32' is updated.
//   * `block_sizes[]' is filled.
//   * `error' is untouched.
// On error:
//   * Return value: -1.
//   * `crc32' and `block_sizes[]' are untouched.
//   * `error' (if set) is set to error description.
static int8_t save_pattern_a2p_v10 (uint8_t version, FILE *f,
                                    uint32_t *crc32, uint32_t *block_size,
                                    tFIXED_SONGDATA *song, unsigned idx,
                                    progress_callback_t *progress, char **error)
{
  int8_t result = -1;
  char *result_error = NULL;
  block_0_a2p_v10_t *raw_data = NULL;
  void *packed_data = NULL;
  size_t raw_size, packed_limit, packed_size;

  raw_size = sizeof (*raw_data);
  packed_limit = calc_packed_limit (raw_size);
  if ((packed_data = malloc (packed_limit)) == NULL) goto _err_malloc;
  if ((raw_data = malloc (raw_size)) == NULL) goto _err_malloc;

  put_pat_data_a2p_v9 (&raw_data->pat_data, get_pattern_data (song, idx));

  put_string_a2p (&raw_data->pat_name, sizeof (raw_data->pat_name),
                  song->pattern_names[idx], sizeof (song->pattern_names[0]), 11, '\0'); // skip first 11 characters

  if (pack_a2p (version, packed_data, packed_limit, &packed_size, raw_data, raw_size,
                progress, &result_error) != 0) goto _exit;

  free (raw_data);
  raw_data = NULL;

  if (fwrite (packed_data, packed_size, 1, f) == 0) goto _err_fwrite;

  *block_size = uint32_LE (packed_size);
  *crc32 = Update32 (packed_data, packed_size, *crc32);

  result = 0;

_exit:
  if (raw_data != NULL) free (raw_data);
  if (packed_data != NULL) free (packed_data);
  if ((result < 0) && (error != NULL)) *error = result_error;

  return result;

_err_malloc:
  result_error = "Memory allocation failed";
  goto _exit;

_err_fwrite:
  result_error = "Failed to write output file";
  goto _exit;
}
