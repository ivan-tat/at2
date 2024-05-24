// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure update_ar_dr(p_slot: P_OPL3_SLOT; data: Byte);
begin
  p_slot^.reg_ar := (data SHR 4) AND $0f;
  p_slot^.reg_dr := data AND $0f;
  envelope_update_rate(p_slot);
end;
