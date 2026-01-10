// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void fin_bank_free (fin_bank_t *fin_bank)
{
  if (fin_bank->items != NULL) free (fin_bank->items);
  free (fin_bank);
}
