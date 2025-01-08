// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// return string must hold 13+1 bytes (9 printable characters)
static String *_time_playing_str (String *s)
{
  if (play_status != isStopped)
  {
    String_t t; // 4+1 bytes

    // format: " ~##:##.#~ " (11+1 bytes)
    CopyString ((String *)s, (String *)"\x02" " ~", 11);
    t = byte2dec (max (song_timer / 60, 99));
    AppendString ((String *)s, (String *)&t, 11);
    AppendString ((String *)s, (String *)"\x01" ":", 11);
    t = byte2dec (max (song_timer % 60, 99));
    AppendString ((String *)s, (String *)&t, 11);
    t.len = 4;
    t.str[0] = '.';
    t.str[1] = '0' + max (song_timer_tenths / 10, 9);
    t.str[2] = '~';
    t.str[3] = ' ';
    AppendString ((String *)s, (String *)&t, 11);
  }
  else
    CopyString ((String *)s, (String *)"\x0D" " 00~:~00~.~0 ", 13);

  return s;
}
