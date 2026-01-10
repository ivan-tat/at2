// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void fib_free (fib_t *fib)
{
  if (fib->instruments != NULL) free (fib->instruments);
  free (fib);
}
