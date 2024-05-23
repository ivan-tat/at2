// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

bool alt_pressed (void) {
#if USE_LALT_RALT
  return scankey (SC_LALT) || scankey (SC_RALT);
#else // !USE_LALT_RALT
  return scankey (SC_ALT);
#endif // !USE_LALT_RALT
}
