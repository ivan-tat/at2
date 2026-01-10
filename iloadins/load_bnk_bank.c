// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Returns NULL on error and error description in `error'.
bnk_bank_t *load_bnk_bank (const String *fname, char **error)
{
  bool status = true; // `false' on success, `true' on error
  bnk_t *bnk = NULL;
  bnk_bank_t *bnk_bank = NULL;

  DBG_ENTER ("load_bnk_bank");

  if ((bnk = load_bnk (fname, error)) == NULL) goto _exit;

  if ((bnk_bank = import_bnk_bank_from_bnk (bnk, error)) == NULL) goto _exit;

  bnk_free (bnk);
  bnk = NULL;

  if (bnk_bank->count == 0) goto _err_empty;

  status = false;

_exit:
  if (bnk != NULL) bnk_free (bnk);
  if (status && (bnk_bank != NULL))
  {
    bnk_bank_free (bnk_bank);
    bnk_bank = NULL;
  }

  DBG_LEAVE (); //EXIT //load_bnk_bank
  return bnk_bank;

_err_empty:
  *error = "No instruments in file";
  goto _exit;
}
