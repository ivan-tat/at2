// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024-2026 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

// In: `size' = 1..inf
// Returns `false' on success, `true' on error.
bool write_string (const String *src, size_t size, mem_stream_t *stream)
{
  bool result = true;
  uint_least8_t len, str_len;

  if ((size == 0) || ((uint8_t *)stream->curptr + 1 > (uint8_t *)stream->endptr)) goto _exit;

  len = Length (src);

  if ((uint8_t *)stream->curptr + 1 + len > (uint8_t *)stream->endptr) goto _exit;

  str_len = src == NULL ? 0 : (len <= (size - 1) ? len : (size - 1));
  *(uint8_t *)stream->curptr = str_len;
  if (str_len != 0) memcpy ((uint8_t *)stream->curptr + 1, GetStr (src), str_len);
  stream->curptr = (uint8_t *)stream->curptr + 1 + len;
  result = false;

_exit:
  return result;
}
