// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static bool get_pat_order_rad (tFIXED_SONGDATA *song,
                               mem_stream_t *stream,
                               char **error)
{
  bool result = true;
  char *result_error = NULL;
  uint8_t pat_order_len;

  if (read_bytes (&pat_order_len, sizeof (pat_order_len), stream)) goto _err_eod;
  if (pat_order_len != 0)
  {
    uint8_t pat_order[PATTERN_ORDER_LEN_RAD];
    unsigned len = pat_order_len;

    if ((int32_t)len > max (PATTERN_ORDER_LEN_RAD, PATTERN_ORDER_LEN))
      len = max (PATTERN_ORDER_LEN_RAD, PATTERN_ORDER_LEN);
    if (read_bytes (pat_order, len, stream)) goto _err_eod;
    if (len < pat_order_len)
      if (seek_stream (stream, pat_order_len - len, SEEK_CUR)) goto _err_eod;

    for (unsigned i = 0; i < len; i++)
    {
      uint8_t pat = pat_order[i];

      if (pat & 0x80)
      {
        uint8_t pos = pat & 0x7F;

        if (pos < PATTERN_ORDER_LEN)
          song->pattern_order[i] = PATTERN_ORDER_JUMP + pos;
        else
          song->pattern_order[i] = PATTERN_ORDER_JUMP + 0;  // acts as `stop'
      }
      else
      {
        if (pat < PATTERNS_MAX)
          song->pattern_order[i] = pat;
        else
          song->pattern_order[i] = PATTERN_ORDER_JUMP + 0;  // acts as `stop'
      }
    }
  }

  result = false;

_exit:
  if (result && (error != NULL)) *error = result_error;

  return result;

_err_eod:
  result_error = "Unexpected end of input data";
  goto _exit;
}
