// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

var
  temp_buf: array[1..32,1..255] of Record
                                     r,g,b: Byte;
                                   end;
procedure screen_saver;

procedure fadeout;

var
  r,g,b: Byte;
  index: Byte;
  depth: Byte;

function min0(val: Integer): Integer;
begin
  If (val <= 0) then min0 := 0
  else min0 := val;
end;

begin
  _last_debug_str_ := _debug_str_;
  _debug_str_ := 'ADT2KEYB.PAS:screen_saver:fadeout';
  For depth := 1 to 32 do
    begin
      For index := 1 to 255 do
        begin
          GetRGBitem(index,r,g,b);
          temp_buf[depth][index].r := r;
          temp_buf[depth][index].g := g;
          temp_buf[depth][index].b := b;
          SetRGBitem(index,min0(r-1),min0(g-1),min0(b-1));
        end;
      WaitRetrace;
      realtime_gfx_poll_proc;
      If (depth MOD 4 = 0) then draw_screen;
      keyboard_reset_buffer;
    end;
end;

procedure fadein;

var
  index: Byte;
  depth: Byte;


begin
  _last_debug_str_ := _debug_str_;
  _debug_str_ := 'ADT2KEYB.PAS:screen_saver:fadein';
  For depth := 32 downto 1 do
    begin
      For index := 1 to 255 do
        SetRGBitem(index,temp_buf[depth][index].r,
                         temp_buf[depth][index].g,
                         temp_buf[depth][index].b);
      If (depth MOD 4 <> 0) then WaitRetrace;
      realtime_gfx_poll_proc;
      If (depth MOD 4 = 0) then draw_screen;
      keyboard_reset_buffer;
    end;
end;

begin
  _last_debug_str_ := _debug_str_;
  _debug_str_ := 'ADT2KEYB.PAS:screen_saver';
  If (ssaver_time = 0) then EXIT;
  fadeout;
  Repeat
    realtime_gfx_poll_proc;
    draw_screen;
  until (seconds_counter = 0);
  fadein;
end;
