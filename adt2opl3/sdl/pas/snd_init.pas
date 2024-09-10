// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure snd_init;

var
  idx: Byte;

begin
  GetMem(opl3_sample_buffer_ptr,sdl_sample_buffer*4);
  For idx := 1 to 18 do GetMem(opl3_sample_buffer_chan_ptr[idx],sdl_sample_buffer*4);
  sample_frame_size := ROUND(sdl_sample_rate/50*(1+sdl_timer_slowdown/100));;

  opl3_init;

  sdl_audio_spec.freq := sdl_sample_rate;
  sdl_audio_spec.format := AUDIO_S16;
  sdl_audio_spec.channels := 2;
  sdl_audio_spec.samples := sdl_sample_buffer;
  @sdl_audio_spec.callback := @playcallback;
  sdl_audio_spec.userdata := NIL;

  If (SDL_Openaudio(@sdl_audio_spec,NIL) < 0) then
    begin
      WriteLn('SDL: Audio initialization error');
      HALT(1);
    end;

  WriteLn('  Sample buffer size: ',sdl_audio_spec.samples,' samples (requested ',sdl_sample_buffer,')');
  WriteLn('  Sampling rate: ',sdl_audio_spec.freq,' Hz (requested ',sdl_sample_rate,')');

  sdl_sample_rate := sdl_audio_spec.freq;
  sdl_sample_buffer := sdl_audio_spec.samples;

  SDL_PauseAudio(0);
end;
