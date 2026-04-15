// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static const struct
{
  const char *str; // CP437
  uint8_t attr1, attr2;
} banner[] =
{
  { .attr1 = 0x07, .attr2 = 0x00, .str = "" },
  { .attr1 = 0x09, .attr2 = 0x00, .str = "   subz3ro's" },
  { .attr1 = 0x09, .attr2 = 0x00, .str = "       \xC4\xC2\xC4       \xC4\xC4" },              // "       ─┬─       ──"
  { .attr1 = 0x09, .attr2 = 0x00, .str = "  /\xB4""DLiB\xB3R/\xB4""CK3R \xB3\xB3 PLAYER" },   // "  /┤DLiB│R/┤CK3R ││ PLAYER"
  { .attr1 = 0x09, .attr2 = 0x00, .str = "   \xB3       \xB3     \xC4\xC4   " VERSION },      // "   │       │     ──   "
  { .attr1 = 0x07, .attr2 = 0x00, .str = "" },
  { .str = NULL }
};

/*static*/ void _list_title (void)
{
  if (iVGA ())
    for (int i = 0; banner[i].str != NULL; i++)
    {
      String_t s;

      StrToString ((String *)&s, banner[i].str, sizeof (s) - 1);
      CWriteLn ((String *)&s, banner[i].attr1, banner[i].attr2);
    }
  else
    for (int i = 0; banner[i].str != NULL; i++)
      printf ("%s" NL, banner[i].str);
}
