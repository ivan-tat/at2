// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure playcallback(var userdata; stream: pByte; len: Longint); cdecl;
public name PUBLIC_PREFIX + 'playcallback';

const
  counter_idx: Longint = 0;

var
  counter: Longint;
  idx: Byte;
  IRQ_freq_val: Longint;
  buffer_ptr_table: array[1..18] of pDword;
  buf_size: Longint;

begin
  If NOT _rewind then
    IRQ_freq_val := IRQ_freq
  else IRQ_freq_val := IRQ_freq * 20;

  For counter := 0 to PRED(len DIV 4) do
    begin
      Inc(counter_idx);
      If (counter_idx >= sample_frame_size) then
        begin
          counter_idx := 0;
          If (ticklooper > 0) then
            If (fast_forward or _rewind) and NOT replay_forbidden then
              poll_proc
            else
          else If NOT replay_forbidden then
                 poll_proc;

          If (macro_ticklooper = 0) then
            macro_poll_proc;

          Inc(ticklooper);
          If (ticklooper >= IRQ_freq_val DIV tempo) then
            ticklooper := 0;

          Inc(macro_ticklooper);
          If (macro_ticklooper >= IRQ_freq_val DIV (tempo*macro_speedup)) then
            macro_ticklooper := 0;
        end;

      // update partial channel sample buffer pointers
      For idx := 1 to 18 do
        buffer_ptr_table[idx] := opl3_sample_buffer_chan_ptr[idx]+counter*4;
      // update one step
      OPL3EMU_PollProc(opl3_sample_buffer_ptr+counter*4,buffer_ptr_table);
    end;

  // update SDL Audio sample buffer
  Move(opl3_sample_buffer_ptr^,stream^,len);
  If (play_status = isStopped) then
    begin
      wav_buffer_len := 0;
      EXIT; //playcallback
    end;

  // calculate cache buffer size
  If opl3_channel_recording_mode then
    buf_size := WAV_BUFFER_SIZE * get_num_files
  else buf_size := WAV_BUFFER_SIZE;

  // WAV dumper
  If (sdl_opl3_emulator <> 0) then
    If (wav_buffer_len+len <= buf_size) then
      begin
        // update main sample buffer
        Move(opl3_sample_buffer_ptr^,wav_buffer[0][wav_buffer_len],len);
        // update partial channel sample buffers
        For idx := 1 to 18 do
          Move(opl3_sample_buffer_chan_ptr[idx]^,wav_buffer[idx][wav_buffer_len],len);
        Inc(wav_buffer_len,len);
      end
    else
      begin
        // sample buffers full -> flush to disk!
        flush_WAV_data;
        // update main sample buffer
        Move(opl3_sample_buffer_ptr^,wav_buffer[0][wav_buffer_len],len);
        // update partial channel sample buffers
        For idx := 1 to 18 do
          Move(opl3_sample_buffer_chan_ptr[idx]^,wav_buffer[idx][wav_buffer_len],len);
        Inc(wav_buffer_len,len);
      end;

  //EXIT //playcallback
end;
