// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void ScreenMemCopy (tSCREEN_MEM *src, tSCREEN_MEM *dest) {
  cursor_backup = GetCursor ();
  memmove (dest, src, SCREEN_MEM_SIZE);
}
