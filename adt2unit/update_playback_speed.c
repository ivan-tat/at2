// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*static*/ void update_playback_speed (int32_t speed_shift)
{
  DBG_ENTER ("update_playback_speed");

  if (speed_shift)
  {
    if (speed_shift > 0)
    {
      // TODO: replace loop with `if' and assignment
      while ((IRQ_freq + IRQ_freq_shift + playback_speed_shift + speed_shift) > MAX_IRQ_FREQ)
        speed_shift--;
    }
    else
    {
      // TODO: replace loop with `if' and assignment
      while ((IRQ_freq + IRQ_freq_shift + playback_speed_shift + speed_shift) < MIN_IRQ_FREQ)
        speed_shift++;
    }

    playback_speed_shift += speed_shift;
    update_timer (tempo);
  }

  DBG_LEAVE (); //update_playback_speed
}
