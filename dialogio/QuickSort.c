// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Local to `make_stream' procedure

// l, r: 1..MAX_FILES
static void QuickSort (tSTREAM *stream, uint16_t l, uint16_t r)
{
  if (l < r)
  {
    uint16_t i = l, j = r;
    String s[FILENAME_SIZE + 1]; // as `name' member of `tSEARCH' structure

    CopyString (s, stream->stuff[(l + r) / 2 - 1].name, FILENAME_SIZE);

    do
    {
#if GO32
      while ((i < r) && (CompareStrings (stream->stuff[i - 1].name, s) < 0))
        i++;
      while ((j > l) && (CompareStrings (stream->stuff[j - 1].name, s) > 0))
        j--;
#else // !GO32
      while ((i < r) && (CompareStr (stream->stuff[i - 1].name, s) == isLess))
        i++;
      while ((j > l) && (CompareStr (stream->stuff[j - 1].name, s) == isMore))
        j--;
#endif // !GO32

      if (i <= j)
      {
        tSEARCH t;
        memcpy (&t, &stream->stuff[i - 1], sizeof (t));
        memcpy (&stream->stuff[i - 1], &stream->stuff[j - 1], sizeof (t));
        memcpy (&stream->stuff[j - 1], &t, sizeof (t));
        i++;
        j--;
      }
    } while (i <= j);

    if (l < j) QuickSort (stream, l, j);
    if (i < r) QuickSort (stream, i, r);
  }

  //EXIT //QuickSort
}
