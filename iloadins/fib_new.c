// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Returns NULL on error and error description in `error'.
static fib_t *fib_new (uint16_t capacity, char **error)
{
  bool status = true; // `false' on success, `true' on error
  fib_t *fib = NULL;

  if ((fib = malloc (sizeof (*fib))) == NULL) goto _err_malloc;

  fib->capacity = 0;
  fib->count = 0;
  fib->instruments = NULL;

  if (capacity != 0)
  {
    if ((fib->instruments = malloc (sizeof (fib->instruments[0]) * capacity)) == NULL) goto _err_malloc;

    fib->capacity = capacity;
  }

  status = false;

_exit:
  if (status)
    if (fib != NULL)
    {
      free (fib);
      fib = NULL;
    }

  return fib;

_err_malloc:
  *error = "Memory allocation failed";
  goto _exit;
}
