// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure envelope_update_ksl(p_slot: P_OPL3_SLOT);

var
  ksl: Smallint;

begin
  ksl := (KSL_VAL[p_slot^.p_chan^.fnum SHR 6] SHL 2) -
         (8 - p_slot^.p_chan^.block) SHL 5;
  p_slot^.eg_ksl := limit_value(ksl,0,255);
end;
