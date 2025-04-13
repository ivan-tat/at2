// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure vid_SetVideoMode(do_delay: Boolean);

var
  icon: pSDL_Surface;
  rwop: pSDL_RWops;
  win_title: String;

begin
  If do_delay then SDL_Delay(1000);
  screen := SDL_SetVideoMode(SCREEN_RES_x,SCREEN_RES_y,8,SDL_SWSURFACE);
  If (screen = NIL) then
    begin
      WriteLn('SDL: Couldn''t initialize video mode');
      HALT(1);
    end;

  vid_SetRGBPalette(Addr(rgb_color)^);
  Move(rgb_color,rgb_color_alt,SizeOf(rgb_color));
  _FrameBuffer := screen^.pixels;
  rwop := SDL_RWFromMem(adt2_icon_bitmap,SizeOf(adt2_icon_bitmap));
  icon := SDL_LoadBMP_RW(rwop,TRUE);
  SDL_WM_SetIcon(icon,NIL);
  win_title := '/|DLiB TR/|CK3R ][ SDL'+#0;
  SDL_WM_SetCaption(Addr(win_title[1]),NIL);
end;
