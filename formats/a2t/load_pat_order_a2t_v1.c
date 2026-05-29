// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// In:
//   * `version': 1..14.
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
static int8_t load_pat_order_a2t_v1 (uint8_t version, tFIXED_SONGDATA *song,
                                     size_t packed_size,
                                     FILE *f,
                                     progress_callback_t *progress, uint8_t *state, char **error)
{
  int8_t result = -1;
  uint8_t result_state = state != NULL ? *state : 0;
  char *result_error = NULL;
  void *packed_data = NULL;
  pat_order_a2t_v1_t *raw_data = NULL;
  size_t raw_limit;

  if (packed_size == 0) return 0;

  if ((packed_data = malloc (packed_size)) == NULL) goto _err_malloc;

  raw_limit = sizeof (*raw_data);
  if ((raw_data = calloc (1, raw_limit)) == NULL) goto _err_malloc;

  if (fread (packed_data, packed_size, 1, f) == 0) goto _err_fread;

  if (unpack_a2t (version, raw_data, raw_limit, NULL, packed_data, packed_size,
                  progress, &result_error) != 0) goto _exit;

  free (packed_data);
  packed_data = NULL;

  get_pat_order_a2t_v1 (song, raw_data);

  result_state = 2;

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
