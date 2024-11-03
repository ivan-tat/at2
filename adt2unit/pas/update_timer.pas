// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure update_timer(Hz: Word);
begin
  _dbg_enter ({$I %FILE%}, 'update_timer');

  If (Hz = 0) then
    begin
      TimerSetup(18);
      _dbg_leave; EXIT; //update_timer
    end
  else tempo := Hz;
  If (tempo = 18) and timer_fix then IRQ_freq := TRUNC((tempo+0.2)*20)
  else IRQ_freq := 250;
  While (IRQ_freq MOD (tempo*_macro_speedup) <> 0) do Inc(IRQ_freq);
  If (IRQ_freq > MAX_IRQ_FREQ) then IRQ_freq := MAX_IRQ_FREQ;
  While (IRQ_freq+IRQ_freq_shift+playback_speed_shift > MAX_IRQ_FREQ) and
        (playback_speed_shift > 0) do
    Dec(playback_speed_shift);
  While (IRQ_freq+IRQ_freq_shift+playback_speed_shift > MAX_IRQ_FREQ) and
        (IRQ_freq_shift > 0) do
    Dec(IRQ_freq_shift);
  TimerSetup(max(IRQ_freq+IRQ_freq_shift+playback_speed_shift,MAX_IRQ_FREQ));

  _dbg_leave; //EXIT //update_timer
end;
