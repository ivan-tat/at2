// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void bnk_bank_free (bnk_bank_t *bnk_bank)
{
  if (bnk_bank->items != NULL) free (bnk_bank->items);
  free (bnk_bank);
}
