// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// In:
//   * `version': 10.
//   * `idx': 0..INSTRUMENTS_MAX-1.
//   * `progress' and `error' may be NULL.
//
// On success:
//   * Return value: 0.
//   * `crc16' is updated.
//   * `block_size' is set.
//   * `error' is untouched.
// On error:
//   * Return value: -1.
//   * `crc16' and `block_size' are untouched.
//   * `error' (if set) is set to error description.
static int8_t save_block_0_a2i_v10 (uint8_t version, FILE *f,
                                    uint16_t *crc16, uint16_t *block_size,
                                    tFIXED_SONGDATA *song, unsigned idx,
                                    progress_callback_t *progress, char **error)
{
  int8_t result = -1;
  char *result_error = NULL;
  uint16_t ins_4op = check_4op_instrument (idx + 1);
  void *raw_data = NULL, *packed_data = NULL;
  size_t raw_limit, raw_size, packed_limit, packed_size;
  mem_stream_t stream;

  raw_limit = sizeof (ins_data_a2i_v9_t) + INSTRUMENT_NAME_MAX_A2I_V9;
  if (ins_4op != 0) raw_limit *= 2;
  if ((raw_data = malloc (raw_limit)) == NULL) goto _err_malloc;

  set_mem_stream (&stream, raw_data, raw_limit);

  if (ins_4op != 0)
  {
    if (put_ins_a2i_v9 (&stream,
                        song, (ins_4op >> 8) - 1,
                        &result_error)) goto _exit; // 4-op 1/2
    if (put_ins_a2i_v9 (&stream,
                        song, (ins_4op & 0xFF) - 1,
                        &result_error)) goto _exit; // 4-op 2/2
  }
  else
    if (put_ins_a2i_v9 (&stream,
                        song, idx,
                        &result_error)) goto _exit;

  raw_size = (uint8_t *)stream.curptr - (uint8_t *)stream.buf;

  packed_limit = calc_packed_limit (raw_size);
  if ((packed_data = malloc (packed_limit)) == NULL) goto _err_malloc;

  if (pack_a2i (version, packed_data, packed_limit, &packed_size, raw_data, raw_size,
                progress, &result_error) != 0) goto _exit;

  free (raw_data);
  raw_data = NULL;

  if (fwrite (packed_data, packed_size, 1, f) == 0) goto _err_fwrite;

  *block_size = uint16_LE (packed_size);
  *crc16 = Update16 (block_size, /*sizeof (*block_size)*/ 1, *crc16); // it's not a bug - it's a feature
  *crc16 = Update16 (packed_data, packed_size, *crc16);

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
