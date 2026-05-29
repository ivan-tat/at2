// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// In:
//   * `idx': 0..INSTRUMENTS_MAX-1.
//   * `error' may be NULL.
//
// On success:
//   * Return value: false.
//   * `error' is untouched.
// On error:
//   * Return value: true.
//   * `error' (if set) is set to error description.
static bool put_ins_a2f_v1 (mem_stream_t *stream,
                            tFIXED_SONGDATA *song, unsigned idx,
                            char **error)
{
  bool result = true;
  char *result_error = NULL;

  // instrument data
  {
    ins_data_a2f_v1_t *dst = stream->curptr;

    if (seek_stream (stream, sizeof (*dst), SEEK_CUR)) goto _err_overrun;

    put_ins_data_a2f_v1 (dst, song, idx);
  }

  // instrument name
  {
    tINSTR_NAME *src = &song->instr_names[idx];
    uint8_t len = Length (*src);  // 1 byte

    len = len > 9 ? len - 9 : 0;  // skip first 9 characters
    if (len > INSTRUMENT_NAME_MAX_A2F_V1 - 1)
      len = INSTRUMENT_NAME_MAX_A2F_V1 - 1;
    if (write_bytes (&len, sizeof (len), stream)) goto _err_overrun;
    if ((len != 0) && (write_bytes (&GetStr (*src)[9], len, stream))) goto _err_overrun; // skip first 9 characters
  }

  // instrument FM-register macro
  {
    ins_macro_a2f_v1_t *dst = stream->curptr;

    if (seek_stream (stream, sizeof (*dst), SEEK_CUR)) goto _err_overrun;

    put_ins_macro_a2f_v1 (dst, song, idx);
  }

  // disabled FM-register columns
  {
    dis_fmreg_col_a2f_v1_t *dst = stream->curptr;

    if (seek_stream (stream, sizeof (*dst), SEEK_CUR)) goto _err_overrun;

    put_dis_fmreg_col_a2f_v1 (dst, song, idx);
  }

  result = false;

_exit:
  if (result && (error != NULL)) *error = result_error;

  return result;

_err_overrun:
  result_error = "Output buffer overrun";
  goto _exit;
}
