// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// In:
//   * `error' may be NULL.
//
// On success:
//   * Return value: false.
//   * `error' is untouched.
// On error:
//   * Return value: true.
//   * `error' (if set) is set to error description.
static bool get_ins_a2f_v1 (struct temp_instrument_data *dest,
                            mem_stream_t *stream,
                            char **error)
{
  bool result = true;
  char *result_error = NULL;

  // instrument data
  {
    ins_data_a2f_v1_t *src = stream->curptr;

    if (seek_stream (stream, sizeof (*src), SEEK_CUR)) goto _err_eod;

    get_ins_data_a2f_v1 (&dest->fm, src);
  }

  // instrument name
  if (read_string (dest->name, sizeof (dest->name), stream)) goto _err_eod;

  // instrument FM-register macro
  {
    ins_macro_a2f_v1_t *src = stream->curptr;

    if (seek_stream (stream, sizeof (*src), SEEK_CUR)) goto _err_eod;

    get_ins_macro_a2f_v1 (&dest->macro, src);
  }

  // disabled FM-register columns
  {
    dis_fmreg_col_a2f_v1_t *src = stream->curptr;

    if (seek_stream (stream, sizeof (*src), SEEK_CUR)) goto _err_eod;

    get_dis_fmreg_col_a2f_v1 (&dest->dis_fmreg_col, src);
  }

  result = false;

_exit:
  if (result && (error != NULL)) *error = result_error;

  return result;

_err_eod:
  result_error = "Unexpected end of unpacked data";
  goto _exit;
}
