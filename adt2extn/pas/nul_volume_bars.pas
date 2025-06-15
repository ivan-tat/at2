// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure nul_volume_bars;

var
  chan: Byte;

begin
  For chan := chan_pos to chan_pos+MAX_TRACKS-1 do
    If channel_flag[chan] then
      show_str(08+(chan-PRED(chan_pos)-1)*15,MAX_PATTERN_ROWS+11,
               ExpStrR('',14,#205),
               pattern_bckg+pattern_border);
end;
