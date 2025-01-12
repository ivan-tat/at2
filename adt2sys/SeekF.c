// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void SeekF (void *file, int32_t fpos)
{
  DBG_ENTER ("SeekF");

  Pascal_Seek (file, fpos);

  DBG_LEAVE (); //EXIT //SeekF
}
