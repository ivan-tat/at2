// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static bool get_pat_order_fmk (tFIXED_SONGDATA *song,
                               uint8_t *pat_order, unsigned pat_order_len,
                               char **error)
{
  bool result = true;
  char *result_error = NULL;

  for (unsigned i = 0; i < pat_order_len; i++)
  {
    uint8_t pat = pat_order[i];

    if (pat < PATTERNS_MAX_FMK)
      song->pattern_order[i] = pat;
    else if (pat == PATTERN_ORDER_END_FMK)
      song->pattern_order[i] = PATTERN_ORDER_JUMP + 0; // acts as `stop'
    else
      goto _err_format;
  }

  result = false;

_exit:
  if (result && (error != NULL)) *error = result_error;

  return result;

_err_format:
  result_error = "Unknown file format";
  goto _exit;
}
