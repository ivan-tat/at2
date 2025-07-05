// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// From `MenuLib1' unit

function pstr(item: Word): String;

var
  temp: String;

begin
  _dbg_enter ({$I %FILE%}, 'pstr');

  Move(pBYTE(mnu_data)[(item-1)*(mnu_len+1)],temp,mnu_len+1);
  If NOT solid then pstr := ExpStrR(temp,mnu_len-2,' ')
  else pstr := ExpStrR(temp,mnu_len,' ');

  _dbg_leave; //EXIT //pstr
end;
