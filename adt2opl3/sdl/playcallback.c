// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void playcallback (__UNUSED void *userdata, uint8_t *stream, int32_t len) {
  int32_t IRQ_freq_val = (!_rewind) ? IRQ_freq : (IRQ_freq * 20);
  uint32_t *buffer_ptr_table[18]; // HINT: (FPC) start index 1
  int32_t counter_idx = 0;
  int32_t counter;

  for (counter = 0; counter < len / 4; counter++) {
    uint8_t i;

    counter_idx++;
    if (counter_idx >= sample_frame_size) {
      counter_idx = 0;
      if (ticklooper > 0)  {
        if ((fast_forward || _rewind) && !replay_forbidden)
          poll_proc ();
      } else if (!replay_forbidden)
        poll_proc ();

      if (macro_ticklooper == 0)
        macro_poll_proc ();

      ticklooper++;
      if (ticklooper >= IRQ_freq_val / tempo)
        ticklooper = 0;

      macro_ticklooper++;
      if (macro_ticklooper >= IRQ_freq_val / (tempo * macro_speedup))
        macro_ticklooper = 0;
    }

    // update partial channel sample buffer pointers
    for (i = 0; i < 18; i++)
      buffer_ptr_table[i] = (uint32_t *) opl3_sample_buffer_chan_ptr[i] + counter;
    // update one step
    OPL3EMU_PollProc ((uint32_t *) opl3_sample_buffer_ptr + counter, buffer_ptr_table);
  }

  // update SDL Audio sample buffer
  memmove (stream, opl3_sample_buffer_ptr, len);
  if (play_status == isStopped) {
    wav_buffer_len = 0;
    return;
  }

  // WAV dumper
  if (sdl_opl3_emulator) {
    // calculate cache buffer size
    ssize_t buf_size = opl3_channel_recording_mode ? WAV_BUFFER_SIZE * get_num_files ()
                                                   : WAV_BUFFER_SIZE;
    uint8_t i;

    if (wav_buffer_len + len > buf_size)
      flush_WAV_data (); // sample buffers full -> flush to disk!

    // update main sample buffer
    memmove (&wav_buffer[0][wav_buffer_len], opl3_sample_buffer_ptr, len);

    // update partial channel sample buffers
    for (i = 0; i < 18; i++)
      memmove (&wav_buffer[i][wav_buffer_len], &opl3_sample_buffer_chan_ptr[i], len);

    wav_buffer_len += len;
  }
}
