// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// size: 1..inf
// Returns `false' on success, `true' on error.
static bool read_string (String *dst, size_t size, mem_stream_t *stream)
{
  bool result = true; // `false' on success, `true' on error
  uint_least8_t len;

  if ((size == 0) || ((uint8_t *)stream->curptr + 1 > (uint8_t *)stream->endptr)) goto _exit;

  len = *(uint8_t *)stream->curptr;

  if ((uint8_t *)stream->curptr + 1 + len > (uint8_t *)stream->endptr) goto _exit;

  if (dst != NULL)
  {
    uint_least8_t str_len = len <= (size - 1) ? len : (size - 1);

    SetLength (dst, str_len);
    if (str_len != 0) memcpy (GetStr (dst), (uint8_t *)stream->curptr + 1, str_len);
  }
  stream->curptr = (uint8_t *)stream->curptr + 1 + len;
  result = false;

_exit:
  return result;
}
