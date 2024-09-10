// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void update_timer (uint16_t Hz) {
  DBG_ENTER ("update_timer");

  if (!Hz)
    TimerSetup (PIT_FREQ_FLOOR);
  else {
    tempo = Hz;

    if ((tempo == PIT_FREQ_FLOOR) && timer_fix)
      IRQ_freq = trunc ((tempo + 0.2) * 20);
    else
      IRQ_freq = 250;

    while (IRQ_freq % (tempo * _macro_speedup ()))
      IRQ_freq++;

    if (IRQ_freq > MAX_IRQ_FREQ)
      IRQ_freq = MAX_IRQ_FREQ;

    while ((IRQ_freq + IRQ_freq_shift + playback_speed_shift > MAX_IRQ_FREQ)
          && (playback_speed_shift > 0))
      playback_speed_shift--;

    while ((IRQ_freq + IRQ_freq_shift + playback_speed_shift > MAX_IRQ_FREQ)
          && (IRQ_freq_shift > 0))
      IRQ_freq_shift--;

    TimerSetup (max (IRQ_freq + IRQ_freq_shift + playback_speed_shift, MAX_IRQ_FREQ));
  }
}
