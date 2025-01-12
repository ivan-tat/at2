// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void BlockReadF (void *file, void *data, int32_t size, int32_t *bytes_read)
{
  DBG_ENTER ("BlockReadF");

  Pascal_BlockRead (file, data, size, bytes_read);
  if (Pascal_IOResult () != 0) *bytes_read = 0;

  DBG_LEAVE (); //EXIT //BlockReadF
}
