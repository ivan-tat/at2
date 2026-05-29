// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// In:
//   * `version': 9.
//   * `progress' and `error' may be NULL.
//
// On success:
//   * Return value: 0.
//   * `error' is untouched.
// On error:
//   * Return value: -1.
//   * `error' (if set) is set to error description.
static int8_t load_block_0_a2i_v9 (uint8_t version, temp_instrument_t *dst,
                                   size_t packed_size,
                                   FILE *f,
                                   progress_callback_t *progress, char **error)
{
  int8_t result = -1;
  char *result_error = NULL;
  void *packed_data = NULL, *raw_data = NULL;
  size_t raw_limit, raw_size;
  mem_stream_t stream;

  if (packed_size == 0) return 0;

  if ((packed_data = malloc (packed_size)) == NULL) goto _err_malloc;

  raw_limit = sizeof (ins_data_a2i_v9_t) + INSTRUMENT_NAME_MAX_A2I_V9;

  if ((raw_data = calloc (1, raw_limit)) == NULL) goto _err_malloc;

  if (fread (packed_data, packed_size, 1, f) == 0) goto _err_fread;

  if (unpack_a2i (version, raw_data, raw_limit, &raw_size, packed_data, packed_size,
                  progress, &result_error) != 0) goto _exit;

  free (packed_data);
  packed_data = NULL;

  set_mem_stream (&stream, raw_data, raw_limit);

  if (get_ins_a2i_v9 (&dst->ins1,
                      &stream,
                      &result_error)) goto _exit;

  dst->four_op = false;
  dst->use_macro = false;

  result = 0;

_exit:
  if (packed_data != NULL) free (packed_data);
  if (raw_data != NULL) free (raw_data);
  if ((result < 0) && (error != NULL)) *error = result_error;

  return result;

_err_malloc:
  result_error = "Memory allocation failed";
  goto _exit;

_err_fread:
  result_error = "Failed to read input file";
  goto _exit;
}
