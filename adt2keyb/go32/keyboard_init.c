// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void keyboard_init (void) {
  __dpmi_paddr p;

  DBG_ENTER ("keyboard_init");

  memset (keydown, false, sizeof (keydown));
  SysKeyboardCallback = SysKeyboardUserProc;

  lock_SysKeyboardIRQ (true);
  lock_SysKeyboardUserProc (true);

  __dpmi_get_protected_mode_interrupt_vector (9, &SysKeyboardOldIRQ);

  p.offset32 = (uint32_t) SysKeyboardIRQ;
  p.selector = _go32_my_cs ();
  __dpmi_set_protected_mode_interrupt_vector (9, &p);

  DBG_LEAVE (); //keyboard_init
}
