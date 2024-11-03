// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void _dbg_printf (const char *format, ...)
{
  va_list ap;

  if (_debug_ && (_dbg_verbose >= 1))
  {
    va_start (ap, format);
    vprintf (format, ap);
    va_end (ap);
  }
}
