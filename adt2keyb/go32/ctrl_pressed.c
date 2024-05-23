// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

bool ctrl_pressed (void) {
#if USE_LCTRL_RCTRL
  return scankey (SC_LCTRL) || scankey (SC_RCTRL);
#else // !USE_LCTRL_RCTRL
  return scankey (SC_CTRL);
#endif // !USE_LCTRL_RCTRL
}
