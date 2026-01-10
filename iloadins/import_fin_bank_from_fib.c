// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Returns `true' on error and error description in `error'.
static fin_bank_t *import_fin_bank_from_fib (const fib_t *fib, char **error)
{
  bool status = true; // `false' on success, `true' on error
  fin_bank_t *fin_bank = NULL;

  DBG_ENTER ("import_fin_bank_from_fib");

  if ((fin_bank = fin_bank_new (fib->capacity, error)) == NULL) goto _exit;

  fin_bank->count = 0; // do not rely on `fib->count' - it's undefined

  for (uint_least16_t i = 0; i < fib->capacity; i++)
  {
    fin_bank->items[i].available = !Empty (&fib->instruments[i].idata, sizeof (fib->instruments[0].idata));

    // DOS file name
    strncpy (fin_bank->items[i].dname, fib->instruments[i].dname, min (sizeof (fin_bank->items[0].dname) - 1, sizeof (fib->instruments[0].dname)));
    fin_bank->items[i].dname[sizeof (fin_bank->items[0].dname) - 1] = '\0'; // terminate string

    // instrument name
    strncpy (fin_bank->items[i].iname, fib->instruments[i].iname, min (sizeof (fin_bank->items[0].iname) - 1, sizeof (fib->instruments[0].iname)));
    fin_bank->items[i].iname[sizeof (fin_bank->items[0].iname) - 1] = '\0'; // terminate string

    if (fin_bank->items[i].available)
    {
      // data
      memset (&fin_bank->items[i].ins, 0, sizeof (fin_bank->items[0].ins));
      import_standard_instrument_alt (&fin_bank->items[i].ins, &fib->instruments[i].idata);
      fin_bank->items[i].ins.perc_voice = (fib->instruments[i].perc_voice >= 6) && (fib->instruments[i].perc_voice <= 10) ? fib->instruments[i].perc_voice - 6 + 1 : 0;

      fin_bank->count++;
    }
  }

  status = false;

_exit:
  if (status)
    if (fin_bank != NULL)
    {
      fin_bank_free (fin_bank);
      fin_bank = NULL;
    }

  DBG_LEAVE (); //EXIT //import_fin_bank_from_fib
  return fin_bank;
}
