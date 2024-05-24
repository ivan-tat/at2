// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure slot_calc_fb(p_slot: P_OPL3_SLOT);
begin
  If (p_slot^.p_chan^.fb <> 0) then
    p_slot^.fb_out := (p_slot^.prev_out + p_slot^.output) SHR
                      (9 - p_slot^.p_chan^.fb)
  else
    p_slot^.fb_out := 0;
  p_slot^.prev_out := p_slot^.output;
end;
