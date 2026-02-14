// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*static*/ void adjust_macro_speedup_with_notice (tFIXED_SONGDATA *song, const String *_title)
{
  uint16_t new_macro_speedup = calc_max_speedup (song->tempo);

  if (song->macro_speedup > new_macro_speedup)
  {
    char s[127+1];
    String t[sizeof (s)];
    String_t text; // = sizeof (s)
    String_t keys; // at most 7+1 bytes
    String_t title;

    snprintf (s, sizeof (s),
      "Due to system limitations, ~macro speedup~ value is ~changed~$"
      "Slowdown: ~x%d -> x%d~$",
      song->macro_speedup, new_macro_speedup);
    text = iCASE (StrToString (t, s, sizeof (t) - 1));
    keys = iCASE (StrToString (t, "~O~Kay$", sizeof (t) - 1));
    title = iCASE (_title);
    Dialog ((String *)&text, (String *)&keys, (String *)&title, 1);

    song->macro_speedup = new_macro_speedup;
  }
}
