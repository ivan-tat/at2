// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

int32_t GetCursor (void) {
#if GO32
  return (VBIOS_get_cursor_pos (v_page) << 16) + VGA_GetCursorShape ();
#else // !GO32
  return 0;
#endif // !GO32
}
