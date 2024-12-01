// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure show_bar(chan,level,xpos,from_pos,to_pos,attr: Byte);

var
  temp: Byte;

begin
  For temp := from_pos to to_pos do
    show_vcstr(xpos+temp-1+PRED(chan)*15,MAX_PATTERN_ROWS+14,
               ExpStrL('~'+_decay_bar_str(Round(105/63*level))+'~',9,#224),
               analyzer_bckg+analyzer,
               analyzer_bckg+attr SHR 4);
end;
