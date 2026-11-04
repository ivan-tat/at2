// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static bool load_patterns_hsc (tFIXED_SONGDATA *song,
                               size_t load_size,
                               FILE *f,
                               char **error)
{
  bool result = true;
  char *result_error = NULL;
  patterns_hsc_t *patterns = NULL;
  unsigned count = (load_size + sizeof (pat_data_hsc_t) - 1) / sizeof (pat_data_hsc_t);

  if (count != 0)
  {
    size_t patterns_size = count * sizeof (pat_data_hsc_t);

    if ((patterns = malloc (patterns_size)) == NULL) goto _err_malloc;
    if (fread (patterns, load_size, 1, f) == 0) goto _err_fread;
    if (load_size < patterns_size)  // clear tail
      memset ((uint8_t *)patterns + load_size, 0, patterns_size - load_size);

    get_patterns_hsc (song, patterns, count, fix_c_note_bug);
  }

  result = false;

_exit:
  if (patterns != NULL) free (patterns);
  if (result && (error != NULL)) *error = result_error;

  return result;

_err_malloc:
  result_error = "Memory allocation failed";
  goto _exit;

_err_fread:
  result_error = "Failed to read input file";
  goto _exit;
}
