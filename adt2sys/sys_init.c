// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void sys_init (void)
{
#if !GO32
  vid_Init (); // SDL video
  snd_init (); // SDL sound + opl3 emulation (adt2opl3)
#endif // !GO32
}
