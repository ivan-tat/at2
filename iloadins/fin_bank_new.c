// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Returns NULL on error and error description in `error'.
fin_bank_t *fin_bank_new (uint16_t capacity, char **error)
{
  bool status = true; // `false' on success, `true' on error
  fin_bank_t *fin_bank = NULL;

  if ((fin_bank = malloc (sizeof (*fin_bank))) == NULL) goto _err_malloc;

  fin_bank->capacity = 0;
  fin_bank->count = 0;
  fin_bank->items = NULL;

  if (capacity != 0)
  {
    if ((fin_bank->items = malloc (sizeof (fin_bank->items[0]) * capacity)) == NULL) goto _err_malloc;

    fin_bank->capacity = capacity;
  }

  status = false;

_exit:
  if (status && (fin_bank != NULL))
  {
    free (fin_bank);
    fin_bank = NULL;
  }

  return fin_bank;

_err_malloc:
  *error = "Memory allocation failed";
  goto _exit;
}
