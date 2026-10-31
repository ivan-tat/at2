// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static bool load_description_fmk (void **dst,
                                  unsigned offset,
                                  FILE *f,
                                  char **error)
{
  bool result = true;
  char *result_error = NULL;
  void *buffer = NULL;
  uint8_t num_lines;

  if (fseek (f, offset, SEEK_SET) != 0) goto _err_fread;
  if (fread (&num_lines, sizeof (num_lines), 1, f) == 0) goto _err_fread;
  if (num_lines != 0)
  {
    size_t size;

    if (num_lines > DESC_LINES_MAX_FMK) goto _err_format;
    size = DESC_LINE_SIZE_FMK * num_lines;
    if ((buffer = malloc (size)) == NULL) goto _err_malloc;
    if (fread (buffer, size, 1, f) == 0) goto _err_fread;
    if (get_description_fmk (dst,
                             buffer, num_lines,
                             &result_error)) goto _exit;
    free (buffer);
    buffer = NULL;
  }

  result = false;

_exit:
  if (buffer != NULL) free (buffer);
  if (result && (error != NULL)) *error = result_error;

  return result;

_err_malloc:
  result_error = "Memory allocation failed";
  goto _exit;

_err_fread:
  result_error = "Failed to read input file";
  goto _exit;

_err_format:
  result_error = "Unknown file format";
  goto _exit;
}
