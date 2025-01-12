// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void BlockWriteF (void *file, void *data, int32_t size, int32_t *bytes_written)
{
  DBG_ENTER ("BlockWriteF");

  Pascal_BlockWrite (file, data, size, bytes_written);
  if (Pascal_IOResult () != 0) *bytes_written = 0;

  DBG_LEAVE (); //EXIT //BlockWriteF
}
