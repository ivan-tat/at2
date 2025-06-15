// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static tCOMPARE_STR_RESULT CompareStr (const String *s1, const String *s2)
{
  if (CompareStrings (s1, (String *)&updir_str) == 0)
    return isLess; //CompareStr
  else if (CompareStrings (s2, (String *)&updir_str) == 0)
    return isMore; //CompareStr
  else
  {
    String_t t, str1, str2;
    String t1[1+1], t2[1+1];
    size_t len1, len2, len;

    t = FilterStr2 (s1, _valid_characters_fname, '_');
    str1 = Upper ((String *)&t);
    t = FilterStr2 (s2, _valid_characters_fname, '_');
    str2 = Upper ((String *)&t);

    len1 = str1.len;
    len2 = str2.len;
    len = len1 > len2 ? len1 : len2;

    SetLength (t1, 1);
    SetLength (t2, 1);

    for (size_t i = 0; i < len; i++)
    {
      char f1[1+1], f2[1+1];

      GetStr (t1)[0] = str1.str[i];
      GetStr (t2)[0] = str2.str[i];
      t = FilterStr2 ((String *)&t1, _valid_characters, '\x01');
      StringToStr ((char *)&f1, (String *)&t, 1);
      t = FilterStr2 ((String *)&t2, _valid_characters, '\x01');
      StringToStr ((char *)&f2, (String *)&t, 1);

      if (strcmp ((char *)&f1, (char *)&f2) > 0)
        return isMore; //CompareStr
      else if (str1.str[i] < str2.str[i])
        return isLess; //CompareStr
    }

    if (len1 < len2)
      return isLess; //EXIT //CompareStr
    else if (len1 > len2)
      return isMore; //EXIT //CompareStr
    else
      return isEqual; //EXIT //CompareStr
  }
}
