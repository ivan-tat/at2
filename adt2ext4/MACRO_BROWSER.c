// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void MACRO_BROWSER (bool instrBrowser, bool updateCurInstr)
{
  tFIXED_SONGDATA *song = &songdata;

  DBG_ENTER ("MACRO_BROWSER");

  songdata_crc = Update32 (song, sizeof (*song), 0); // FIXME: why not CRC32_INITVAL?

  a2w_file_loader (false, !instrBrowser, true, false, updateCurInstr, NULL); // browse internal A2W data

  if (Update32 (song, sizeof (*song), 0) != songdata_crc) // FIXME: why not CRC32_INITVAL?
    module_archived = false;

  DBG_LEAVE (); //EXIT //MACRO_BROWSER
}
