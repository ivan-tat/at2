// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static bool get_description_fmk (void **dst,
                                 const uint8_t *src, uint8_t input_lines,
                                 char **error)
{
  bool result = true;
  char *result_error = NULL;
  uint8_t *out = NULL;
  size_t out_size = 0;
  int_least16_t first = -1, last = -1;

  // pass 1/2: determine text data size (without NULL-terminators) and non-empty lines range
  for (uint8_t line = 0; line < input_lines; line++)
  {
    const uint8_t *end_ptr = &src[DESC_LINE_SIZE_FMK * (line + 1)];
    uint8_t len = DESC_LINE_SIZE_FMK;

    truncate_string_fmk (&end_ptr, &len);
    if (len != 0)
    {
      out_size += len;
      if (first < 0) first = line;
      last = line;
    }
  }

  if (out_size != 0)
  {
    uint8_t lines_count = last - first + 1;
    fast_str_t *lines;
    uint8_t *line_start;

    // sizeof (song_desc_s) + NULL-terminators for found lines
    out_size += sizeof (size_t) + sizeof (fast_str_t) * lines_count + lines_count;
    if ((out = malloc (out_size)) == NULL) goto _err_malloc;

    *((size_t *)out) = lines_count; // =song_desc_s.count
    lines = (fast_str_t *)(&out[sizeof (size_t)]);  // =&out[offsetof (song_desc_s, lines)]
    line_start = (uint8_t *)&lines[lines_count];  // =&out[sizeof (song_desc_s)]

    // pass 2/2: store lines
    for (uint8_t line = 0; line < lines_count; line++)
    {
      const uint8_t *end_ptr = &src[DESC_LINE_SIZE_FMK * (first + line + 1)];
      uint8_t len = DESC_LINE_SIZE_FMK;

      truncate_string_fmk (&end_ptr, &len);
      if (len != 0) memcpy (line_start, end_ptr - len, len);
      line_start[len] = '\0';
      lines[line].len = len;
      lines[line].data = line_start;
      line_start += len + 1;
    }
  }

  result = false;

_exit:
  if (result)
  {
    if (out != NULL) free (out);
    if (error != NULL) *error = result_error;
  }
  else *dst = out;

  return result;

_err_malloc:
  result_error = "Memory allocation failed";
  goto _exit;
}
