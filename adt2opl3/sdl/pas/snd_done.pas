// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure snd_done;

var
  idx: Byte;

begin
  SDL_PauseAudio(1);
  SDL_CloseAudio;
  FreeMem(opl3_sample_buffer_ptr);
  For idx := 1 to 18 do FreeMem(opl3_sample_buffer_chan_ptr[idx]);
  opl3_sample_buffer_ptr := NIL;
end;
