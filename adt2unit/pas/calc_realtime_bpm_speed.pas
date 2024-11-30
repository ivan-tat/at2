// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function calc_realtime_bpm_speed(tempo,speed,rows_per_beat: Byte): Real;
begin
{$IFDEF GO32V2}
  calc_realtime_bpm_speed := tempo*60/speed/rows_per_beat*(1+((IRQ_freq_shift+playback_speed_shift)/IRQ_freq)-0.02);
{$ELSE}
  calc_realtime_bpm_speed := tempo*60/speed/rows_per_beat*(1+((IRQ_freq_shift+playback_speed_shift)/IRQ_freq)-sdl_timer_slowdown/100);
{$ENDIF}
end;
