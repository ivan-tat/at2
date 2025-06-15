// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure new_fselect_external_proc; cdecl;
begin
  mn_environment.curr_item := fstream.stuff[mn_environment.curr_pos].name;
  If (@old_fselect_external_proc <> NIL) then old_fselect_external_proc;
end;
