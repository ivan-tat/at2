// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void ResetF (void *file)
{
  uint16_t fattr;

  DBG_ENTER ("ResetF");

  Pascal_GetFAttrFile (file, &fattr);
  if (fattr & Pascal_ReadOnly)
    Pascal_FileMode = Pascal_FileMode_ReadOnly;
  Pascal_ResetFile (file, 1);

  DBG_LEAVE (); //EXIT //ResetF
}
