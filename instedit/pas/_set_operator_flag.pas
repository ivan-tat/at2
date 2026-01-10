// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Local to `INSTRUMENT_CONTROL_edit' procedure

procedure _set_operator_flag(operator: Byte; toggle: Boolean);

var
  _temp_operator_enabled: array[1..4] of Boolean;

begin
  If (inst^.perc_voice in [2..5]) or
     (NOT (get_4op_to_test <> 0) and NOT (operator in [1..2])) or
     (NOT (operator in [1..4])) then
    EXIT; //_set_operator_flag

  If NOT toggle then
    begin
      FillChar(_operator_enabled,SizeOf(_operator_enabled),FALSE);
      _operator_enabled[operator] := TRUE;
      EXIT; //_set_operator_flag
    end;

  Move(_operator_enabled,_temp_operator_enabled,SizeOf(_temp_operator_enabled));
  If NOT (get_4op_to_test <> 0) and (operator in [1,2]) then
    begin
      _temp_operator_enabled[operator] := NOT _temp_operator_enabled[operator];
      If NOT ((_temp_operator_enabled[1] = FALSE) and
              (_temp_operator_enabled[2] = FALSE)) then
        Move(_temp_operator_enabled,_operator_enabled,SizeOf(_operator_enabled));
    end
  else If (get_4op_to_test <> 0) and (operator in [1,2,3,4]) then
         begin
           _temp_operator_enabled[operator] := NOT _temp_operator_enabled[operator];
           If NOT ((_temp_operator_enabled[1] = FALSE) and
                   (_temp_operator_enabled[2] = FALSE) and
                   (_temp_operator_enabled[3] = FALSE) and
                   (_temp_operator_enabled[4] = FALSE)) then
           Move(_temp_operator_enabled,_operator_enabled,SizeOf(_operator_enabled));
         end;

  //EXIT //_set_operator_flag
end;
