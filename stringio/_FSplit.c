// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void _FSplit (const String *path, ssize_t *_de, ssize_t *_es) {
  ssize_t pl = Length (path), de = pl /* Dir. end */, es /* Ext. start */;

  // Find the first DRIVE_SEP or DIR_SEP from the end,
  // avoid problems with platforms having DRIVE_SEP == DIR_SEP.
  while ((de > 0)
#if (DRIVE_SEP != '\0') && (DRIVE_SEP != DIR_SEP)
  &&     (GetStr (path) [de - 1] != DRIVE_SEP)
#endif // (DRIVE_SEP != '\0') && (DRIVE_SEP != DIR_SEP)
  &&     (GetStr (path) [de - 1] != DIR_SEP))
    de--;

  // The first `extension' should be returned if LFN support is not available,
  // the last one otherwise.
  if (LFNSupport) {
    es = pl;

    while ((es > de) && (GetStr (path) [es - 1] != '.'))
      es--;

    if ((es == 0) || (GetStr (path) [es - 1] != '.'))
      es = pl + 1;
  } else {
    es = de + 1;

    while ((es <= pl) && (GetStr (path) [es - 1] != '.'))
      es++;
  }

  *_de = de;
  *_es = es;
}
