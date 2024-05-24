// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure update_ksl_tl(p_slot: P_OPL3_SLOT; data: Byte);
begin
  p_slot^.reg_ksl := (data SHR 6) AND 3;
  p_slot^.reg_tl := data AND $3f;
  envelope_update_ksl(p_slot);
end;
