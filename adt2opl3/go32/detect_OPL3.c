// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

bool detect_OPL3 (void) {
  union {
    uint8_t b[2];
    uint16_t w;
  } x;

  DBG_ENTER ("detect_OPL3");

  opl2out (0x04, 0x80);
  WaitRetrace ();
  opl2out (0x04, 0x60);
  WaitRetrace ();

  x.b[0] = inportb (opl3port) & 0xE0;

  opl2out (0x04, 0x21);
  WaitRetrace ();
  opl2out (0x02, 0xFF);
  WaitRetrace ();

  x.b[1] = inportb (opl3port) & 0xE0;

  if (x.w != 0xC000)
    return false;

  opl2out (0x04, 0x80);
  WaitRetrace ();
  opl2out (0x04, 0x60);
  WaitRetrace ();

  return (inportb (opl3port) & 0x06) == 0;
}
