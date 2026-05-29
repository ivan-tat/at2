// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// In:
//   * `version': 1..4.
//   * `num_patterns': 0..PATTERNS_MAX_A2M_V1.
//   * `progress', `state' and `error' may be NULL.
//
// On success:
//   * Return value: 0.
//   * `state' (if set): 2 (partly loaded).
//   * `error' is untouched.
// On error:
//   * Return value: -1.
//   * `state' (if set) may be set to 2 (partly loaded).
//   * `error' (if set) is set to error description.
static int8_t load_patterns_a2m_v1 (uint8_t version, tFIXED_SONGDATA *song,
                                    size_t *block_sizes, unsigned num_patterns,
                                    FILE *f,
                                    progress_callback_t *progress, uint8_t *state, char **error)
{
  int8_t result = -1;
  uint8_t result_state = state != NULL ? *state : 0;
  char *result_error = NULL;
  void *packed_data = NULL;
  pat_data_a2m_v1_t (*raw_data)[16] = NULL;
  size_t packed_limit, packed_size, raw_limit, raw_size;
  unsigned num_blocks;
  bool adsr_carrier[CHANNELS_MAX_A2M_V1];

  if (num_patterns == 0) return 0;

  num_blocks = ((num_patterns > PATTERNS_MAX_A2M_V1 ? PATTERNS_MAX_A2M_V1 : num_patterns)
                + lengthof ((*raw_data), [0]) - 1) / lengthof ((*raw_data), [0]); // 0..4
  packed_limit = get_max_size (block_sizes, num_blocks);
  if ((packed_data = malloc (packed_limit)) == NULL) goto _err_malloc;
  raw_limit = sizeof (*raw_data);
  if ((raw_data = malloc (raw_limit)) == NULL) goto _err_malloc;

  memset (adsr_carrier, false, sizeof (adsr_carrier));

  for (unsigned i = 0; i < num_blocks; i++)
  {
    packed_size = block_sizes[i];
    if (packed_size != 0)
    {
      unsigned count = lengthof ((*raw_data), [0]) * (i + 1) > num_patterns ? num_patterns % lengthof ((*raw_data), [0])
                                                                            : lengthof ((*raw_data), [0]);

      if (fread (packed_data, packed_size, 1, f) == 0) goto _err_fread;

      memset (raw_data, 0, raw_limit);
      if (unpack_a2m (version, raw_data, raw_limit, &raw_size, packed_data, packed_size,
                      progress, &result_error) != 0) goto _exit;

      for (unsigned j = 0; j < count; j++)
      {
        unsigned idx = lengthof ((*raw_data), [0]) * i + j;

        if (idx < max_patterns)
          get_pat_data_a2m_v1 (get_pattern_data (song, idx), &(*raw_data)[j], &adsr_carrier);
        else
        {
          limit_exceeded = true;
          break;
        }
      }

      result_state = 2;
    }
    if (progress != NULL) next_progress_step (progress);
  }

  free (packed_data);
  packed_data = NULL;

  free (raw_data);
  raw_data = NULL;

  replace_old_adsr_a2m_v1 (song, num_patterns, &adsr_carrier);

  result = 0;

_exit:
  if (packed_data != NULL) free (packed_data);
  if (raw_data != NULL) free (raw_data);
  if (state != NULL) *state = result_state;
  if ((result < 0) && (error != NULL)) *error = result_error;

  return result;

_err_malloc:
  result_error = "Memory allocation failed";
  goto _exit;

_err_fread:
  result_error = "Failed to read input file";
  goto _exit;
}
