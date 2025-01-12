// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void CloseF (void *file)
{
  DBG_ENTER ("CloseF");

  Pascal_CloseFile (file);
  if (Pascal_IOResult () != 0) {}

  DBG_LEAVE (); //EXIT //CloseF
}
