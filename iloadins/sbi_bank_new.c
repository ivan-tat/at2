// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Returns NULL on error and error description in `error'.
sbi_bank_t *sbi_bank_new (char **error)
{
  sbi_bank_t *sbi_bank = NULL;

  if ((sbi_bank = malloc (sizeof (*sbi_bank))) != NULL)
  {
    sbi_bank->count = 0;
    memset (sbi_bank->items, 0, sizeof (sbi_bank->items));
  }
  else
    *error = "Memory allocation failed";

  return sbi_bank;
}
