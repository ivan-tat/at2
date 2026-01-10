// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void bnk_free (bnk_t *bnk)
{
  if (bnk->names != NULL) free (bnk->names);
  if (bnk->data != NULL) free (bnk->data);
  free (bnk);
}
