// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static bnk_t *bnk_new (uint16_t capacity, char **error)
{
  bool status = true; // `false' on success, `true' on error
  bnk_t *bnk = NULL;

  if ((bnk = malloc (sizeof (*bnk))) == NULL) goto _err_malloc;

  bnk->capacity = 0;
  bnk->count = 0;
  bnk->names = NULL;
  bnk->data = NULL;

  if (capacity != 0)
  {
    if ((bnk->names = malloc (capacity * sizeof (bnk->names[0]))) == NULL) goto _err_malloc;
    if ((bnk->data = malloc (capacity * sizeof (bnk->data[0]))) == NULL) goto _err_malloc;

    bnk->capacity = capacity;
  }

  status = false;

_exit:
  if (status)
    if (bnk != NULL)
    {
      if (bnk->names != NULL) free (bnk->names);
      free (bnk);
      bnk = NULL;
    }

  return bnk;

_err_malloc:
  *error = "Memory allocation failed";
  goto _exit;
}
