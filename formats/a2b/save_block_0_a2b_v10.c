// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// In:
//   * `version': 10.
//   * `progress' and `error' may be NULL.
//
// On success:
//   * Return value: 0.
//   * `crc32' is updated.
//   * `block_size' is set.
//   * `error' is untouched.
// On error:
//   * Return value: -1.
//   * `crc32' and `block_size' are untouched.
//   * `error' (if set) is set to error description.
static int8_t save_block_0_a2b_v10 (uint8_t version, FILE *f,
                                    uint32_t *crc32, uint32_t *block_size,
                                    tFIXED_SONGDATA *song,
                                    progress_callback_t *progress, char **error)
{
  int8_t result = -1;
  char *result_error = NULL;
  block_0_a2b_v10_t *raw_data = NULL;
  void *packed_data = NULL;
  size_t raw_size, packed_limit, packed_size;

  raw_size = sizeof (*raw_data);
  if ((raw_data = calloc (1, raw_size)) == NULL) goto _err_malloc;
  packed_limit = calc_packed_limit (raw_size);
  if ((packed_data = malloc (packed_limit)) == NULL) goto _err_malloc;

  put_ins_names_a2b_v9 (raw_data->ins_names, song, lengthof (raw_data->ins_names, [0]));

  for (unsigned i = 0; i < lengthof (raw_data->ins_data, [0]); i++)
    put_ins_data_a2b_v9 (&raw_data->ins_data[i], song, i);

  put_ins_4op_data_a2b_v10 (&raw_data->ins_4op_data, song);

  if (pack_a2b (version, packed_data, packed_limit, &packed_size, raw_data, raw_size,
                progress, &result_error) != 0) goto _exit;

  free (raw_data);
  raw_data = NULL;

  if (fwrite (packed_data, packed_size, 1, f) == 0) goto _err_fwrite;

  *block_size = uint32_LE (packed_size);
  *crc32 = Update32 (block_size, /*sizeof (*block_size)*/ 2, *crc32); // it's not a bug - it's a feature
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
