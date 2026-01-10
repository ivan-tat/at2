// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Local to `INSTRUMENT_test' procedure

function _1op_preview_active: Boolean;

var
  temp,nm_slots: Byte;

begin
  nm_slots := 0;
  For temp := 1 to 4 do
    If _operator_enabled[temp] then
      Inc(nm_slots);
  _1op_preview_active := (nm_slots = 1);
end;
