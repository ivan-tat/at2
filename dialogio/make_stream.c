// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void make_stream (const String *path, __UNUSED const String *mask, tSTREAM *stream)
{
  String searchpath[Pascal_FileNameLen+1];
  SearchRec_t search;
  uint16_t a = 0, b = 0; // count1, count2

  DBG_ENTER ("make_stream");

#if WIN32 || WIN64 || WINNT
  GetLogicalDriveStrings (sizeof (drive_list), drive_list);
#endif // WIN32 || WIN64 || WINNT

  for (char drive = 'A'; drive <= 'Z'; drive++)
#if GO32
    if (valid_drive (drive))
#else // !GO32
    if (valid_drive (drive, (String *)&stream->stuff[a].info))
#endif // !GO32
    {
      SetLength (stream->stuff[a].name, 1);
      GetStr (stream->stuff[a].name)[0] = drive;
      stream->stuff[a].attr = Pascal_VolumeId;
      stream->stuff[a].size = 0;
      a++;
    }

  // TODO: make a separate flag for this
  SetLength (stream->stuff[a].name, 3);
  GetStr (stream->stuff[a].name)[0] = '~';
  GetStr (stream->stuff[a].name)[1] = charmap.nbsp; // space
  GetStr (stream->stuff[a].name)[2] = '~';
  stream->stuff[a].attr = Pascal_VolumeId;
  a++;

  stream->drive_count = a;

#if GO32
  if (Pascal_DiskSize (UpCase (GetStr (path)[0]) - 'A' + 1) > 0)
  {
#endif // GO32
    CopyString (searchpath, path, Pascal_FileNameLen);
    AppendString (searchpath, WILDCARD_ASTERISK, Pascal_FileNameLen);

    Pascal_FindFirst (searchpath, Pascal_AnyFile - Pascal_VolumeId, &search);

    while ((Pascal_DosError == 0) && (a < MAX_FILES))
    {
      if (!(((search.Attr & Pascal_Directory) != 0) && (CompareStrings (search.Name, ONE_DOT_STR) == 0)))
        if (   ((search.Attr & Pascal_Directory) != 0)
            && !(   (CompareStrings (search.Name, TWO_DOTS_STR) == 0)
#if linux || unix
                 && (Length (path) == 1))) // `<PATHSEP>'
#else // !(linux || unix)
                 && (Length (path) == 3))) // `<disk>:<PATHSEP>'
#endif // !(linux || unix)
        {
          if (CompareStrings (search.Name, TWO_DOTS_STR) == 0)
            CopyString (search.Name, updir_str, Pascal_FileNameLen);

          CopyString (stream->stuff[a].name, search.Name, FILENAME_SIZE);
          stream->stuff[a].attr = search.Attr;
          a++;
        }

      Pascal_FindNext (&search);
    }

#if !GO32
    if ((Length (path) > 3) && (a == stream->drive_count))
    {
      CopyString (stream->stuff[a].name, updir_str, FILENAME_SIZE);
      stream->stuff[a].attr = search.Attr;
      a++;
    }
#endif // !GO32

    Pascal_FindFirst (searchpath, Pascal_AnyFile - Pascal_VolumeId - Pascal_Directory, &search);
    while ((Pascal_DosError == 0) && (a + b < MAX_FILES))
    {
      if (LookUpMask (search.Name))
      {
        String_t s = Lower_filename (search.Name);
        CopyString (search.Name, (String *)&s, Pascal_FileNameLen);
        CopyString (stream->stuff[a + b].name, search.Name, FILENAME_SIZE);
        stream->stuff[a + b].attr = search.Attr;
        stream->stuff[a + b].size = search.Size;
        b++;
      }

      Pascal_FindNext (&search);
    }
#if GO32
  }
#endif // GO32

  QuickSort (stream, stream->drive_count + DRIVE_DIVIDER, a);
  QuickSort (stream, a + DRIVE_DIVIDER, a + b);

  stream->count = a + b;
  stream->match_count = b;

  DBG_LEAVE (); //EXIT //make_stream
}
