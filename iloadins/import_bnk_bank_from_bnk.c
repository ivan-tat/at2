// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Returns NULL on error and error description in `error'.
static bnk_bank_t *import_bnk_bank_from_bnk (bnk_t *bnk, char **error)
{
  bool status = true; // `false' on success, `true' on error
  bnk_bank_t *bnk_bank = NULL;

  DBG_ENTER ("import_bnk_bank_from_bnk");

  if ((bnk_bank = bnk_bank_new (bnk->capacity, error)) == NULL) goto _exit;

  bnk_bank->count = 0;

  for (uint_least16_t i = 0; i < bnk->capacity; i++)
  {
    bnk_bank->items[i].available = bnk->names[i].usage_flag != false;

    // instrument name
    strncpy (bnk_bank->items[i].name, bnk->names[i].ins_name, min (sizeof (bnk_bank->items[0].name) - 1, sizeof (bnk->names[0].ins_name)));
    bnk_bank->items[i].name[sizeof (bnk_bank->items[0].name) - 1] = '\0'; // terminate string

    if (bnk_bank->items[i].available)
    {
      // data
      memset (&bnk_bank->items[i].ins, 0, sizeof (bnk_bank->items[0].ins));
      import_bnk_instrument (&bnk_bank->items[i].ins, &bnk->data[i]);

      bnk_bank->count++;
    }
  }

  status = false;

_exit:
  if (status && (bnk_bank != NULL))
  {
    bnk_bank_free (bnk_bank);
    bnk_bank = NULL;
  }

  DBG_LEAVE (); //EXIT //import_bnk_bank_from_bnk
  return bnk_bank;
}
