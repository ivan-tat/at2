// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void EraseF (void *file)
{
  DBG_ENTER ("EraseF");

  Pascal_EraseFile (file);
  if (Pascal_IOResult () != 0) {}

  DBG_LEAVE (); //EXIT //EraseF
}
