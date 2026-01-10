// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Returns NULL on error and error description in `error'.
fin_bank_t *load_fin_bank (const String *fname, char **error)
{
  bool status = true; // `false' on success, `true' on error
  fib_t *fib = NULL;
  fin_bank_t *fin_bank = NULL;

  DBG_ENTER ("load_fin_bank");

  if ((fib = load_fib (fname, error)) == NULL) goto _exit;

  if ((fin_bank = import_fin_bank_from_fib (fib, error)) == NULL) goto _exit;

  fib_free (fib);
  fib = NULL;

  if (fin_bank->count == 0) goto _err_empty;

  status = false;

_exit:
  if (fib != NULL) fib_free (fib);
  if (status && (fin_bank != NULL))
  {
    fin_bank_free (fin_bank);
    fin_bank = NULL;
  }

  DBG_LEAVE (); //EXIT //load_fin_bank
  return fin_bank;

_err_empty:
  *error = "No instruments in file";
  goto _exit;
}
