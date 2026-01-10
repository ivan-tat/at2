// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Returns NULL on error and error description in `error'.
bnk_bank_t *bnk_bank_new (uint16_t capacity, char **error)
{
  bool status = true; // `false' on success, `true' on error
  bnk_bank_t *bnk_bank = NULL;

  if ((bnk_bank = malloc (sizeof (*bnk_bank))) == NULL) goto _err_malloc;

  bnk_bank->capacity = 0;
  bnk_bank->count = 0;
  bnk_bank->items = NULL;

  if (capacity != 0)
  {
    if ((bnk_bank->items = malloc (sizeof (bnk_bank->items[0]) * capacity)) == NULL) goto _err_malloc;

    bnk_bank->capacity = capacity;
  }

  status = false;

_exit:
  if (status && (bnk_bank != NULL))
  {
    free (bnk_bank);
    bnk_bank = NULL;
  }

  return bnk_bank;

_err_malloc:
  *error = "Memory allocation failed";
  goto _exit;
}
