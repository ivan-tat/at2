// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure init_old_songdata;

{$IFNDEF ADT2PLAY}
var
  temp: Byte;
{$ENDIF} // NOT DEFINED(ADT2PLAY)

begin
  _dbg_enter ({$I %FILE%}, 'init_old_songdata');

  FillChar(old_songdata,SizeOf(old_songdata),0);
  FillChar(old_songdata.pattern_order,SizeOf(old_songdata.pattern_order),$080);
  FillChar(old_songdata.instr_data,SizeOf(old_songdata.instr_data),0);

{$IFNDEF ADT2PLAY}
  For temp := 1 to 250 do
    old_songdata.instr_names[temp] :=
      ' iNS_'+byte2hex(temp)+#247' ';
{$ENDIF} // NOT DEFINED(ADT2PLAY)

  _dbg_leave; //EXIT //init_old_songdata
end;
