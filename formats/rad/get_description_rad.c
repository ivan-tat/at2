// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static bool get_description_rad (void **dst,
                                 mem_stream_t *stream,
                                 char **error)
{
  bool result = true;
  char *result_error = NULL;
  const uint8_t *in = stream->curptr;
  size_t in_size = 0;
  uint8_t *out = NULL;
  size_t out_size = 0, lines_count = 0;
  size_t len = 0; // current line length

  for (uint8_t x = 1; x != 0; in_size++)
    if (read_bytes (&x, sizeof (x), stream)) goto _err_eod;

  // pass 1/2: determine text data size (without NULL-terminators) and total lines count
  for (size_t pos = 0; (pos < in_size) && (in[pos] != '\0'); pos++)
  {
    uint8_t c = in[pos];

    if (c == 1) // end of line
    {
      out_size += len;
      len = 0;
      lines_count++;
    }
    else if (c <= 31) len += c; // add spaces (2..31) to current line
    else len++; // add single character to current line
  }
  // add remaining characters as line
  if (len != 0)
  {
    out_size += len;
    lines_count++;
  }

  if ((out_size != 0) && (lines_count != 0))
  {
    fast_str_t *lines;
    uint8_t *data;
    uint8_t *line_start;
    size_t line;

    // sizeof (song_desc_s) + NULL-terminators for all lines
    out_size += sizeof (size_t) + sizeof (fast_str_t) * lines_count + lines_count;
    if ((out = malloc (out_size)) == NULL) goto _err_malloc;

    *((size_t *)out) = lines_count; // =song_desc_s.count
    lines = (fast_str_t *)(&out[sizeof (size_t)]);  // =&out[offsetof (song_desc_s, lines)]
    data = (uint8_t *)&lines[lines_count];  // =&out[sizeof (song_desc_s)]
    line_start = data;

    // pass 2/2: store lines
    line = 0;
    len = 0;
    for (size_t pos = 0; (pos < in_size) && (in[pos] != '\0'); pos++)
    {
      uint8_t c = in[pos];

      if (c == 1) // end of line
      {
        truncate_string_rad ((const uint8_t **)&data, &len);
        *data = '\0';
        data++;
        lines[line].len = len;
        lines[line].data = line_start;
        line_start = data;
        line++;
        len = 0;
      }
      else if (c <= 31) // add spaces (2..31) to current line
      {
        memset (data, ' ', c);
        data += c;
        len += c;
      }
      else  // add single character to current line
      {
        *data = c;
        data++;
        len++;
      }
    }
    // add remaining characters as line
    if (len != 0)
    {
      truncate_string_rad ((const uint8_t **)&data, &len);
      *data = '\0';
      data++;
      lines[line].len = len;
      lines[line].data = line_start;
      line_start = data;
    }
    // shrink memory block if needed
    if (line_start < out + out_size)
    {
      void *p = realloc (out, line_start - out);
      if (p == NULL) goto _err_malloc;
      out = p;
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

_err_eod:
  result_error = "Unexpected end of input data";
  goto _exit;
}
