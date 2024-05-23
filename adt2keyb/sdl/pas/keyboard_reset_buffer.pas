// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure keyboard_reset_buffer;
var
  event: SDL_Event;

begin
  // flush all unused events
  While (SDL_PollEvent(@event) <> 0) do ;
end;
