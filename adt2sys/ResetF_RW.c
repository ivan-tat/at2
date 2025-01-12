// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void ResetF_RW (void *file)
{
  uint16_t fattr;

  DBG_ENTER ("ResetF_RW");

  Pascal_GetFAttrFile (file, &fattr);
  if (fattr & Pascal_ReadOnly)
    Pascal_SetFAttrFile (file, fattr & ~Pascal_ReadOnly);
  if (Pascal_DosError != 0) {}
  Pascal_FileMode = Pascal_FileMode_ReadWrite;
  Pascal_ResetFile (file, 1);

  DBG_LEAVE (); //EXIT //ResetF_RW
}
