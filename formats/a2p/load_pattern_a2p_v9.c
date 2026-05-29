// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// In:
//   * `version': 9.
//   * `idx': 0..PATTERNS_MAX-1
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
static int8_t load_pattern_a2p_v9 (uint8_t version, tFIXED_SONGDATA *song, unsigned idx, String *def_name,
                                   size_t packed_size,
                                   FILE *f,
                                   progress_callback_t *progress, uint8_t *state, char **error)
{
  int8_t result = -1;
  uint8_t result_state = state != NULL ? *state : 0;
  char *result_error = NULL;
  pat_data_a2p_v9_t *raw_data = NULL;
  void *packed_data = NULL;
  tRAW_PATTERN_DATA *raw_pat_data = NULL;
  size_t raw_limit, raw_size;

  if (packed_size == 0) return 0;

  raw_limit = sizeof (*raw_data);
  if ((raw_data = calloc (1, raw_limit)) == NULL) goto _err_malloc;
  if ((packed_data = malloc (packed_size)) == NULL) goto _err_malloc;

  if (fread (packed_data, packed_size, 1, f) == 0) goto _err_fread;

  if (unpack_a2p (version, raw_data, raw_limit, &raw_size, packed_data, packed_size,
                  progress, &result_error) != 0) goto _exit;

  free (packed_data);
  packed_data = NULL;

  if ((raw_pat_data = malloc (sizeof (*raw_pat_data))) == NULL) goto _err_malloc;

  if ((pattern2use != UINT8_NULL) && (_patts_marked () != 0))
  {
    for (unsigned pat = 0; pat < max_patterns; pat++)
      if (is_pattern_marked (song, pat))
      {
        get_pat_data_a2p_v9 (raw_pat_data, raw_data);
        set_pattern_data (song, pat, raw_pat_data);
        set_pattern_name (song, pat, NULL, def_name);
        result_state = 2;
      }
  }
  else
  {
    if (idx < max_patterns)
    {
      get_pat_data_a2p_v9 (raw_pat_data, raw_data);
      set_pattern_data (song, idx, raw_pat_data);
      set_pattern_name (song, idx, NULL, def_name);
      result_state = 2;
    }
    else
      limit_exceeded = true;
  }

  result = 0;

_exit:
  if (raw_data != NULL) free (raw_data);
  if (packed_data != NULL) free (packed_data);
  if (raw_pat_data != NULL) free (raw_pat_data);
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
