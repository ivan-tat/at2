// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure fade_out_playback(fade_screen: Boolean);

var
{$IFDEF GO32V2}
  idx,idx2: Byte;
  fade_buf: tFADE_BUF;
{$ELSE}
  temp: Byte;
  temp2,temp3: Byte;
  factor: Byte;
{$ENDIF}

begin
  _dbg_enter ({$I %FILE%}, 'fade_out_playback');

{$IFDEF GO32V2}

  If fade_screen then
    begin
      draw_screen;
      fade_buf.action := first;
      fade_speed := 32;
      GetPalette(fade_buf.pal0,0,255);

      For idx := fade_speed downto 0 do
        begin
          For idx2 := 0 to 255 do
            begin
              fade_buf.pal1[idx2].r := fade_buf.pal0[idx2].r * idx DIV fade_speed;
              fade_buf.pal1[idx2].g := fade_buf.pal0[idx2].g * idx DIV fade_speed;
              fade_buf.pal1[idx2].b := fade_buf.pal0[idx2].b * idx DIV fade_speed;
            end;

          SetPalette(fade_buf.pal1,0,255);
          If (play_status <> isStopped) then
            begin
              fade_out_volume := idx*2;
              set_global_volume;
              CRT.Delay(fade_out_volume DIV 32);
            end
          else CRT.Delay(1);

          realtime_gfx_poll_proc;
          draw_screen;
          keyboard_reset_buffer;
        end;
      fade_buf.action := fadeIn;
    end
  else If (play_status <> isStopped) then
         For idx := 63 downto 0 do
           begin
             fade_out_volume := idx;
             set_global_volume;
             CRT.Delay(fade_out_volume DIV 32);
             realtime_gfx_poll_proc;
             keyboard_reset_buffer;
           end;
{$ELSE}

  If fade_screen then factor := 255
  else factor := 63;

  If (global_volume > 0) then temp2 := factor DIV global_volume
  else temp2 := 0;
  temp3 := 0;
  fade_out_volume := 63;

  If (play_status <> isStopped) then
    For temp := 1 to factor do
      begin
        Inc(temp3);
        If (temp3 > temp2) then
          begin
            temp3 := 0;
            Dec(fade_out_volume);
            set_global_volume;
            If fade_screen or (temp MOD 5 = 0) then
              begin
                _draw_screen_without_delay := TRUE;
                draw_screen;
                keyboard_reset_buffer;
              end;
          end;
        If fade_screen then
          begin
            vid_FadeOut;
            SDL_Delay(1);
          end;
      end
  else
    For fade_out_volume := 1 to 255 do
      If fade_screen then
        begin
          vid_FadeOut;
          SDL_Delay(1);
        end;

{$ENDIF}

  _dbg_leave; //EXIT //fade_out_playback
end;
