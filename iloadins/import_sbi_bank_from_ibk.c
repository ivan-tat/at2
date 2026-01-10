// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Returns NULL on error and error description in `error'.
static sbi_bank_t *import_sbi_bank_from_ibk (const ibk_t *ibk, char **error)
{
  bool status = true; // `false' on success, `true' on error
  sbi_bank_t *sbi_bank = NULL;

  DBG_ENTER ("import_sbi_bank_from_ibk");

  if ((sbi_bank = sbi_bank_new (error)) == NULL) goto _exit;

  sbi_bank->count = 0;

  for (uint_least8_t i = 0; i < SBI_BANK_CAPACITY; i++)
  {
    sbi_bank->items[i].available = !Empty (&ibk->instruments[i], sizeof (ibk->instruments[0]));

    // instrument name
    strncpy (sbi_bank->items[i].name, ibk->names[i], sizeof (sbi_bank->items[0].name));
    sbi_bank->items[i].name[sizeof (sbi_bank->items[0].name) - 1] = '\0'; // terminate string

    if (sbi_bank->items[i].available)
    {
      // instrument data
      memset (&sbi_bank->items[i].ins, 0, sizeof (sbi_bank->items[0].ins));
      import_sbi_instrument_alt (&sbi_bank->items[i].ins, &ibk->instruments[i].idata);
      sbi_bank->items[i].ins.perc_voice = (ibk->instruments[i].perc_voice >= 6) && (ibk->instruments[i].perc_voice <= 10) ? ibk->instruments[i].perc_voice - 6 + 1 : 0;

      sbi_bank->count++;
    }
  }

  status = false;

_exit:
  if (status && (sbi_bank != NULL))
  {
    sbi_bank_free (sbi_bank);
    sbi_bank = NULL;
  }

  DBG_LEAVE (); //EXIT //import_sbi_bank_from_ibk
  return sbi_bank;
}
