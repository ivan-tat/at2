// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function GetKey: Word;
var
  result: Word;
  key_c,scan_c: Byte;

begin
  no_status_refresh := FALSE;
  While NOT keypressed do
    begin
      realtime_gfx_poll_proc;
      draw_screen;
      If (seconds_counter >= ssaver_time) then screen_saver;
    end;
  key_c := BYTE(CRT.ReadKey);
  If (key_c = 0) then result := BYTE(CRT.ReadKey) SHL 8
  else begin
         scan_c := inportb($60);
         If (scan_c > $80) then scan_c := scan_c-$80;
         result := key_c+(scan_c SHL 8);
       end;
  GetKey := result;
end;
