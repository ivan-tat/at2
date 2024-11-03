// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void _dbg_leave (void)
{
  if (_debug_ && (_dbg_verbose >= 2))
  {
    if (   (_dbg_callstackptr >= 0)
        && (_dbg_callstackptr < DBG_CALLSTACK_MAX))
    {
      if (_dbg_callstack[_dbg_callstackptr].label)
        printf ("%d:%s:%s:%s: leave" NL,
          _dbg_callstackptr,
          _dbg_callstack[_dbg_callstackptr].file,
          _dbg_callstack[_dbg_callstackptr].func,
          _dbg_callstack[_dbg_callstackptr].label
        );
      else
        printf ("%d:%s:%s: leave" NL,
          _dbg_callstackptr,
          _dbg_callstack[_dbg_callstackptr].file,
          _dbg_callstack[_dbg_callstackptr].func
        );
    }
    else
      printf ("%d:?:?: leave" NL, _dbg_callstackptr);
  }

  _dbg_callstackptr--;
}
