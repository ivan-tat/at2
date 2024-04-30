// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure init_TxtScrIO;
begin
{$IFDEF GO32V2}
  initialize;
{$ENDIF} // DEFINED(GO32V2)
{$IFNDEF ADT2PLAY}
  is_setting.char_filter := _valid_characters;
  is_setting.valid_chars := _valid_characters;
  is_environment.locate_pos := 1;
  is_environment.min_num := 0;
  is_environment.max_num := SizeOf(DWORD);
  is_environment.cur_str := '';
  is_environment.ext_proc := NIL;
{$ENDIF} // NOT DEFINED(ADT2PLAY)
end;
