// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure envelope_update_rate(p_slot: P_OPL3_SLOT);
begin
  Case p_slot^.eg_state of
    EG_OFF:     p_slot^.eg_rate := 0;
    EG_ATTACK:  p_slot^.eg_rate := envelope_calc_rate(p_slot,p_slot^.reg_ar);
    EG_DECAY:   p_slot^.eg_rate := envelope_calc_rate(p_slot,p_slot^.reg_dr);
    EG_SUSTAIN,
    EG_RELEASE: p_slot^.eg_rate := envelope_calc_rate(p_slot,p_slot^.reg_rr);
  end;
end;
