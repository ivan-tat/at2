// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// In:
//   * `version': 2.
//   * `progress' and `error' may be NULL.
//
// On success:
//   * Return value: 0.
//   * `error' is untouched.
// On error:
//   * Return value: -1.
//   * `error' (if set) is set to error description.
static int8_t load_block_0_a2f_v2 (uint8_t version, temp_instrument_t *dst,
                                   size_t packed_size,
                                   FILE *f, bool swap_ins,
                                   progress_callback_t *progress, char **error)
{
  int8_t result = -1;
  char *result_error = NULL;
  void *packed_data = NULL, *raw_data = NULL;
  size_t raw_limit, raw_size;
  mem_stream_t stream;

  if (packed_size == 0) return 0;

  if ((packed_data = malloc (packed_size)) == NULL) goto _err_malloc;

  raw_limit = (  sizeof (ins_data_a2f_v1_t)
               + INSTRUMENT_NAME_MAX_A2F_V1
               + sizeof (ins_macro_a2f_v1_t)
               + sizeof (dis_fmreg_col_a2f_v1_t)) * 2;

  if ((raw_data = calloc (1, raw_limit)) == NULL) goto _err_malloc;

  if (fread (packed_data, packed_size, 1, f) == 0) goto _err_fread;

  if (unpack_a2f (version, raw_data, raw_limit, &raw_size, packed_data, packed_size,
                  progress, &result_error) != 0) goto _exit;

  free (packed_data);
  packed_data = NULL;

  set_mem_stream (&stream, raw_data, raw_limit);

  if (get_ins_a2f_v1 (&dst->ins1,
                      &stream,
                      &result_error)) goto _exit;

  if (stream.curptr < stream.endptr)  // more data present => 4-op instrument
  {
    struct temp_instrument_data *dst_ins;

    dst->four_op = true;

    if (swap_ins)
    {
      memcpy (&dst->ins2, &dst->ins1, sizeof (dst->ins2)); // copy to 4-op 2/2
      dst_ins = &dst->ins1; // 4-op 1/2
    }
    else
      dst_ins = &dst->ins2; // 4-op 2/2

    if (get_ins_a2f_v1 (dst_ins,
                        &stream,
                        &result_error)) goto _exit;
  }
  else
    dst->four_op = false;

  dst->use_macro = true;

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
