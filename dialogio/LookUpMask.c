// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static bool LookUpMask (const String *filename)
{
  String_t s, t;

#if GO32
  s = Upper (filename);
#else // !GO32
  t = ExtOnly (filename);
  s = Upper ((String *)&t);
#endif // !GO32

  for (unsigned i = 0; i < _fs.count; i++)
  {
#if GO32
    t = Upper ((String *)&_fs.masks[i]);
    if (SameName ((String *)&t, (String *)&s))
#else // !GO32
    String_t u;
    t = Copy ((String *)&_fs.masks[i], 3, _fs.masks[i].len);
    u = Upper ((String *)&t);
    if (memcmp (&u, &s, s.len) == 0)
#endif // !GO32
      return true;
  }

  return false;
}
