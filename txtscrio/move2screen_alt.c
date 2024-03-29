// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void move2screen_alt (void) {
  if (move_to_screen_data) {
    unsigned char *a = move_to_screen_data;
    unsigned char *b = ptr_temp_screen2;
    uint_least8_t y;

    memmove (temp_screen2, screen_ptr, SCREEN_MEM_SIZE);

    for (y = move_to_screen_area[1]; y <= move_to_screen_area[3]; y++) {
      uint_least8_t x;

      for (x = move_to_screen_area[0]; x <= move_to_screen_area[2]; x++) {
        PosChar (x, y); // set `absolute_pos'
        *((uint16_t *) (b+absolute_pos)) = *((uint16_t *) (a+absolute_pos));
      }
    }

    memmove (screen_ptr, temp_screen2, SCREEN_MEM_SIZE);
  }
}
