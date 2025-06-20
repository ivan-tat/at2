// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function pdes(item: Word): String;

var
  temp: String;

begin
  _dbg_enter ({$I %FILE%}, 'pdes');

  If (mn_environment.descr <> NIL) and (item <= mnu_count) then
    Move(pBYTE(mn_environment.descr)[(item-1)*(mn_environment.descr_len+1)],temp,mn_environment.descr_len+1)
  else temp := '';
  pdes := ExpStrR(temp,mn_environment.descr_len,' ');

  _dbg_leave; //EXIT //pdes
end;
