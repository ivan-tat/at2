// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void _dbg_enter (const char *file, const char *func)
{
  _dbg_callstackptr++;
  if (   (_dbg_callstackptr >= 0)
      && (_dbg_callstackptr < DBG_CALLSTACK_MAX))
  {
    _dbg_callstack[_dbg_callstackptr].file = file;
    _dbg_callstack[_dbg_callstackptr].func = func;
    _dbg_callstack[_dbg_callstackptr].label = NULL;
  }

  if (_debug_ && (_dbg_verbose >= 2))
    printf ("%d:%s:%s: enter" NL, _dbg_callstackptr, file, func);
}
