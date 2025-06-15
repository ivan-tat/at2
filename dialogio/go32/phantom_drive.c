// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static bool phantom_drive (char drive)
{
  bool r;
  __dpmi_regs regs;

  DBG_ENTER ("phantom_drive");

  regs.x.ax = 0x440E;
  regs.h.bl = UpCase (drive) - 'A' + 1;

  if (__dpmi_simulate_real_mode_interrupt (0x21, &regs))
    r = false; // failed
  else if (regs.x.flags & 1) // Carry flag
    r = false;
  else if (regs.h.al == 0)
    r = false;
  else
    r = regs.h.al != (UpCase (drive) - 'A' + 1);

  DBG_LEAVE (); //EXIT //phantom_drive
  return r;
}
