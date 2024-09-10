// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure key_on(chan: Byte);
begin
  If NOT (is_4op_chan(chan) and (chan in _4op_tracks_hi)) then
    opl3out($0b0+_chan_n[chan],0)
  else opl3out($0b0+_chan_n[SUCC(chan)],0);
end;
