// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure draw_screen_proc;
begin
  _update_sdl_screen := FALSE;
  If Compare(screen_ptr,ptr_screen_mirror,(SCREEN_RES_X DIV scr_font_width)*MAX_ROWS*2) then
    EXIT //draw_screen_proc
  else ScreenMemCopy(screen_ptr,ptr_screen_mirror);
  _cursor_blink_factor := ROUND(13/100*sdl_frame_rate);
  _update_sdl_screen := TRUE;

{$IFNDEF CPU64}
  Case program_screen_mode of
    0: draw_SDL_screen_720x480;
    1: draw_SDL_screen_960x800;
    2: draw_SDL_screen_1440x960;
  end;
{$ELSE}
  Case program_screen_mode of
    0: draw_SDL_screen(90,40,720*480);
    1: draw_SDL_screen(120,50,960*800);
    2: draw_SDL_screen(180,60,1440*960);
  end;
{$ENDIF}

  //EXIT //draw_screen_proc
end;
