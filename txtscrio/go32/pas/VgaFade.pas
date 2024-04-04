// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

const
  fade_first: Byte = 0;
  fade_last:  Byte = 255;

procedure VgaFade(var data: tFADE_BUF; fade: tFADE; delay: tDELAY);

var
  i,j: Byte;

begin
  If (fade = fadeOut) and (data.action in [first,fadeIn]) then
    begin
      GetPalette(data.pal0,fade_first,fade_last);
      If delay = delayed then
        For i := fade_speed downto 0 do
          begin
            For j := fade_first to fade_last do
              begin
                data.pal1[j].r := data.pal0[j].r * i DIV fade_speed;
                data.pal1[j].g := data.pal0[j].g * i DIV fade_speed;
                data.pal1[j].b := data.pal0[j].b * i DIV fade_speed;
              end;
            SetPalette(data.pal1,fade_first,fade_last);
{$IFDEF ADT2PLAY}
            WaitRetrace;
{$ELSE}
            CRT.Delay(1);
{$ENDIF}
          end
      else
        begin
          FillChar(data.pal1,SizeOf(data.pal1),0);
          SetPalette(data.pal1,fade_first,fade_last);
        end;
      data.action := fadeOut;
    end;

  If (fade = fadeIn) and (data.action = fadeOut) then
    begin
      If delay = delayed then
        For i := 0 to fade_speed do
          begin
            For j := fade_first to fade_last do
              begin
                data.pal1[j].r := data.pal0[j].r * i DIV fade_speed;
                data.pal1[j].g := data.pal0[j].g * i DIV fade_speed;
                data.pal1[j].b := data.pal0[j].b * i DIV fade_speed;
              end;
            SetPalette(data.pal1,fade_first,fade_last);
{$IFDEF ADT2PLAY}
            WaitRetrace;
{$ELSE}
            CRT.Delay(1);
{$ENDIF}
          end
      else
        SetPalette(data.pal0,fade_first,fade_last);
      data.action := fadeIn;
    end;
end;
