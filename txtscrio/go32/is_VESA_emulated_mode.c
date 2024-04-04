// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

bool is_VESA_emulated_mode (void) {
  return (program_screen_mode == 3) && (comp_text_mode > 1);
}
