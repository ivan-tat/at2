// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void keyboard_done (void) {
  DBG_ENTER ("keyboard_done");

  __dpmi_set_protected_mode_interrupt_vector (9, &SysKeyboardOldIRQ);

  lock_SysKeyboardIRQ (false);
  lock_SysKeyboardUserProc (false);

  keyboard_reset_buffer ();
}
