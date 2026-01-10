// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Returns NULL on error and error description in `error'.
sbi_bank_t *load_sbi_bank (const String *fname, char **error)
{
  bool status = true; // `false' on success, `true' on error
  ibk_t *ibk = NULL;
  sbi_bank_t *sbi_bank = NULL;

  DBG_ENTER ("load_sbi_bank");

  if ((ibk = load_ibk (fname, error)) == NULL) goto _exit;

  if ((sbi_bank = import_sbi_bank_from_ibk (ibk, error)) == NULL) goto _exit;

  ibk_free (ibk);
  ibk = NULL;

  if (sbi_bank->count == 0) goto _err_empty;

  status = false;

_exit:
  if (ibk != NULL) ibk_free (ibk);
  if (status && (sbi_bank != NULL))
  {
    sbi_bank_free (sbi_bank);
    sbi_bank = NULL;
  }

  DBG_LEAVE (); //EXIT //load_sbi_bank
  return sbi_bank;

_err_empty:
  *error = "No instruments in file";
  goto _exit;
}
