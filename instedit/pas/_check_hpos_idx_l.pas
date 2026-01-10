// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Local to `INSTRUMENT_CONTROL_edit' procedure

function _check_hpos_idx_l(vpos: Byte): Byte;

var
  idx: Byte;

begin
  _check_hpos_idx_l := 0;
  For idx := 1 to inst_hpos_max do
    If (inst_hpos[vpos,idx] <> 0) then
      begin
        _check_hpos_idx_l := idx;
        BREAK;
      end;
end;
