// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void print_error (const char *format, ...)
{
  va_list ap;
  va_start (ap, format);
  _list_title ();
  printf ("ERROR: ");
  vprintf (format, ap);
  va_end (ap);
}
