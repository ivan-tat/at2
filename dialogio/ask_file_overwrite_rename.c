// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Returns: 0=overwrite, 1=rename, -1=cancel
int8_t ask_file_overwrite_rename (const String *_fname, const String *title)
{
  int8_t result = -1;
  FILE *f = NULL;
  char fname[255+1];

  StringToStr (fname, _fname, sizeof (fname) - 1);
  if ((f = fopen (fname, "rb")) != NULL)
  {
    if (quick_cmd)
      result = 0;
    else if (dl_environment.keystroke != kESC)
    {
      String_t s, t;
      char u[255+1], v[255+1];
      uint8_t index;

      s = NameOnly (_fname);
      snprintf (u, sizeof (u), "File \"%s\" already exists in destination directory$", StringToStr (v, (String *)&s, sizeof (v) - 1));
      s = iCASE (StrToString ((String *)&s, u, sizeof (s) - 1));
      t = iCASE (StrToString ((String *)&t, "~O~verwrite$~R~ename$~C~ancel$", sizeof (t) - 1));
      index = Dialog ((String *)&s, (String *)&t, title, 1);

      if ((dl_environment.keystroke != kESC) && (index != 3))
        result = index == 1 ? 0 : 1;
    }
  } else
    result = 0;

  if (f != NULL) fclose (f);
  return result;
}
