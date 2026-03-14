// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024-2026 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

// whence: SEEK_SET, SEEK_CUR, or SEEK_END
// Returns `false' on success, `true' on error.
bool seek_stream (mem_stream_t *stream, ssize_t offset, int whence)
{
  bool result = true; // `false' on success, `true' on error

  switch (whence)
  {
    case SEEK_SET:
      if ((offset >= 0) && ((size_t)offset < stream->size))
      {
        stream->curptr = (uint8_t *)stream->buf + offset;
        result = false;
      }
      break;
    case SEEK_CUR:
      offset += (uint8_t *)stream->curptr - (uint8_t *)stream->buf;
      if ((offset >= 0) && ((size_t)offset <= stream->size))
      {
        stream->curptr = (uint8_t *)stream->buf + offset;
        result = false;
      }
      break;
    case SEEK_END:
      stream->curptr = stream->endptr;
      result = false;
      break;
    default:
      break;
  }

  return result;
}
