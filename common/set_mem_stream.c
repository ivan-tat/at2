// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void set_mem_stream (mem_stream_t *dst, const void *buf, size_t size)
{
  dst->buf = buf;
  dst->size = size;
  dst->curptr = buf;
  dst->endptr = (uint8_t *)buf + size;
}
