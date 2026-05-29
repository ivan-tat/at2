// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// In:
//   * `version': 9..14.
//   * `num_patterns': 0..PATTERNS_MAX_A2M_V9.
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
static int8_t save_patterns_a2m_v9 (uint8_t version, FILE *f,
                                    uint32_t *crc32, uint32_t *block_sizes, unsigned num_patterns,
                                    tFIXED_SONGDATA *song,
                                    progress_callback_t *progress, char **error)
{
  int8_t result = -1;
  char *result_error = NULL;
  pat_data_a2m_v9_t (*raw_data)[8] = NULL;
  void *packed_data = NULL;
  size_t raw_limit, raw_size, packed_limit, packed_size;
  uint8_t num_blocks, count;

  memset (block_sizes, 0, sizeof (block_sizes[0]) * 16);

  if (num_patterns == 0) return 0;

  if (num_patterns > PATTERNS_MAX_A2M_V9)
    num_patterns = PATTERNS_MAX_A2M_V9;

  count = num_patterns > lengthof ((*raw_data), [0]) ? lengthof ((*raw_data), [0])
                                                     : num_patterns;
  raw_limit = sizeof ((*raw_data)[0]) * count;
  if ((raw_data = malloc (raw_limit)) == NULL) goto _err_malloc;

  packed_limit = calc_packed_limit (raw_limit);
  if ((packed_data = malloc (packed_limit)) == NULL) goto _err_malloc;

  num_blocks = (num_patterns + lengthof ((*raw_data), [0]) - 1) / lengthof ((*raw_data), [0]); // 0..16

  for (uint8_t i = 0; i < num_blocks; i++)
  {
    count = lengthof ((*raw_data), [0]) * (i + 1) > num_patterns ? num_patterns % lengthof ((*raw_data), [0])
                                                                 : lengthof ((*raw_data), [0]);

    raw_size = sizeof ((*raw_data)[0]) * count;

    for (uint8_t j = 0; j < count; j++)
      put_pat_data_a2m_v9 (&(*raw_data)[j], get_pattern_data (song, lengthof ((*raw_data), [0]) * i + j));

    if (pack_a2m (version, packed_data, packed_limit, &packed_size, raw_data, raw_size,
                  progress, &result_error) != 0) goto _exit;

    if (fwrite (packed_data, packed_size, 1, f) == 0) goto _err_fwrite;

    block_sizes[i] = uint32_LE (packed_size);
    *crc32 = Update32 (packed_data, packed_size, *crc32);

    if (progress != NULL) next_saver_step (progress);
  }

  free (raw_data);
  raw_data = NULL;

  free (packed_data);
  packed_data = NULL;

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
