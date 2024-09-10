// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure update_playback_speed(speed_shift: Longint);
begin
{$IFDEF GO32V2}
  _last_debug_str_ := _debug_str_;
  _debug_str_ := 'ADT2UNIT.PAS:update_playback_speed';
{$ENDIF}
  If (speed_shift = 0) then EXIT
  else If (speed_shift > 0) and (IRQ_freq+playback_speed_shift+speed_shift > MAX_IRQ_FREQ) then
         While (IRQ_freq+IRQ_freq_shift+playback_speed_shift+speed_shift > MAX_IRQ_FREQ) do
           Dec(speed_shift)
       else If (speed_shift < 0) and (IRQ_freq+IRQ_freq_shift+playback_speed_shift+speed_shift < MIN_IRQ_FREQ) then
              While (IRQ_freq+IRQ_freq_shift+playback_speed_shift+speed_shift < MIN_IRQ_FREQ) do
                Inc(speed_shift);
  playback_speed_shift := playback_speed_shift+speed_shift;
  update_timer(tempo);
end;
