// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function _4op_vol_valid_chan(chan: Byte): Boolean;

var
  _4op_flag: Dword;

begin
  _4op_flag := _4op_data_flag(chan);
  _4op_vol_valid_chan := BOOLEAN(_4op_flag AND 1) and vol4op_lock[chan] and
                     (BYTE(_4op_flag SHR 11) <> 0) and
                     (BYTE(_4op_flag SHR 19) <> 0);
end;
