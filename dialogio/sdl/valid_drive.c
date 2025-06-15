// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static bool valid_drive (__UNUSED char drive, String *info)
{
  SetLength (info, 0);

#if GO32 || WIN32 || WIN64 || WINNT
  for (uint8_t i = 0; i < 129; i++)
    if (drive_list[i] == drive)
    {
      CopyString (info, (String *)"\x05" "DRiVE", 255);
      break;
    }
#endif // GO32 || WIN32 || WIN64 || WINNT

  return Length (info) != 0;
}
