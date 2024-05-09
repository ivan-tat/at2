// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function CutStr(str: String): String;
begin
{$IFDEF ADT2PLAY}
  While (str[0] <> #0) and (str[1] in [#00,#32]) do Delete(str,1,1);
  While (str[0] <> #0) and (str[Length(str)] in [#00,#32]) do Delete(str,Length(str),1);
{$ELSE} // NOT DEFINED(ADT2PLAY)
  While (BYTE(str[0]) <> 0) and (str[1] = ' ') do
    Delete(str,1,1);
  While (BYTE(str[0]) <> 0) and (str[BYTE(str[0])] = ' ') do
    Delete(str,BYTE(str[0]),1);
{$ENDIF} // NOT DEFINED(ADT2PLAY)
  CutStr := str;
end;
