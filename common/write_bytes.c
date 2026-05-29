// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024-2026 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

// In: `size' = 1..inf
// Returns `false' on success, `true' on error.
bool write_bytes (const void *src, size_t size, mem_stream_t *stream)
{
  bool result = true;

  if ((size == 0) || ((uint8_t *)stream->curptr + size > (uint8_t *)stream->endptr)) goto _exit;

  if (src == NULL)
    memset (stream->curptr, 0, size);
  else
    memcpy (stream->curptr, src, size);
  stream->curptr = (uint8_t *)stream->curptr + size;
  result = false;

_exit:
  return result;
}
