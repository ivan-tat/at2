// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#if PATTERN_ORDER_LEN_CFF > PATTERN_ORDER_LEN
#error Pattern order length mismatch
#endif

// In:
//   * `num_patterns' = 0..PATTERNS_MAX_CFF.
//   * `error' may be NULL.
//
// On success:
//   * Return value is 0.
//   * `error' is untouched.
// On error:
//   * Return value is -1.
//   * `error' (if set) is set to error description.
static int8_t get_pat_order_cff (tFIXED_SONGDATA *dst,
                                 pat_order_cff_t *src, unsigned num_patterns,
                                 char **error)
{
  int8_t result = -1;
  char *result_error = NULL;

  for (unsigned i = 0; i < lengthof ((*src), [0]); i++)
  {
    if ((*src)[i] < PATTERNS_MAX_CFF)
    {
      dst->pattern_order[i] = (*src)[i] <= num_patterns ? (*src)[i]
                                                        : PATTERN_ORDER_JUMP + 0; // acts as STOP
    }
    else if ((*src)[i] == PATTERN_ORDER_END)
      dst->pattern_order[i] = PATTERN_ORDER_JUMP + 0; // acts as STOP
    else
      goto _err_format;
  }

  result = 0;

_exit:
  if ((result < 0) && (error != NULL))
    *error = result_error;

  return result;

_err_format:
  result_error = "Unknown file format";
  goto _exit;
}
