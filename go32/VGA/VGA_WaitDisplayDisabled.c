// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void VGA_WaitDisplayDisabled (void) {
  VGA_wait_while_display_disabled (true);
  VGA_wait_while_display_disabled (false);
}
