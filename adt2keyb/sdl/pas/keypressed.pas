// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function keypressed: Boolean;
var
  event: SDL_Event;

begin
  keypressed := FALSE;
  Repeat
    keyboard_poll_input;
    If (SDL_PeepEvents(event,1,SDL_PEEKEVENT,SDL_QUITMASK) > 0) then
      begin
        _force_program_quit := TRUE;
        keypressed := TRUE;
        EXIT;
      end;
    If (SDL_PeepEvents(event,1,SDL_PEEKEVENT,SDL_MOUSEEVENTMASK) > 0) then
      begin
        // skip mouse events
        SDL_PeepEvents(event,1,SDL_GETEVENT,SDL_MOUSEEVENTMASK);
        CONTINUE;
      end;
    If (SDL_PeepEvents(event,1,SDL_PEEKEVENT,SDL_KEYDOWNMASK) > 0) then
      If (event.key.keysym.sym >= SDLK_NUMLOCK) then
        begin
          // skip modifier key presses
          SDL_PeepEvents(event,1,SDL_GETEVENT,SDL_KEYDOWNMASK);
          CONTINUE;
        end
      else
        keypressed := TRUE;
    EXIT;
  until FALSE;
end;
