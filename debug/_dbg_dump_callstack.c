// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void _dbg_dump_callstack (void)
{
  if (_dbg_callstackptr == -1)
    printf ("Call stack is empty." NL);
  else
    if (_dbg_callstackptr < -1)
      printf ("Call stack underflow (%d). No stack trace available." NL,
        _dbg_callstackptr);
    else
    {
      unsigned n, i;

      if (_dbg_callstackptr < DBG_CALLSTACK_MAX)
        n = _dbg_callstackptr + 1;
      else
      {
        n = DBG_CALLSTACK_MAX;
        printf ("Call stack overflow (%d). First %u calls only:" NL,
          _dbg_callstackptr, n);
      }

      for (i = 0; i < n; i++)
      {
        printf ("%u:%s:%s", i, _dbg_callstack[i].file, _dbg_callstack[i].func);

        if (_dbg_callstack[i].label)
          printf (":%s", _dbg_callstack[i].label);

        printf (NL);
      }

      if (_dbg_callstackptr >= DBG_CALLSTACK_MAX)
        printf ("<...>" NL);
    }
}
