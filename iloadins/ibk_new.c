// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Returns NULL on error and error description in `error'.
static ibk_t *ibk_new (char **error)
{
  ibk_t *ibk = NULL;

  if ((ibk = malloc (sizeof (*ibk))) != NULL)
    memset (ibk, 0, sizeof (*ibk));
  else
    *error = "Memory allocation failed";

  return ibk;
}
