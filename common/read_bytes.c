// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// size: 1..inf
// Returns `false' on success, `true' on error.
bool read_bytes (void *dst, size_t size, mem_stream_t *stream)
{
  bool result = true; // `false' on success, `true' on error

  if ((size == 0) || ((uint8_t *)stream->curptr + size > (uint8_t *)stream->endptr)) goto _exit;

  if (dst != NULL) memcpy (dst, stream->curptr, size);
  stream->curptr = (uint8_t *)stream->curptr + size;
  result = false;

_exit:
  return result;
}
