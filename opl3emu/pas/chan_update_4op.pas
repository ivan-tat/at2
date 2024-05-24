// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure chan_update_4op(p_chip: P_OPL3_CHIP; data: Byte);

var
  bit_num: Byte;

begin
  For bit_num := 0 to 5 do
    If ((data SHR bit_num) AND 1 <> 0) then
      begin
        // set chan. to 4OP
        p_chip^.chan[CH_4OP_IDX[bit_num]].ch_type := CH_4OP_1;
        p_chip^.chan[CH_4OP_IDX[bit_num]+3].ch_type := CH_4OP_2;
      end
    else
      begin
        // reset chan. to 2OP
        p_chip^.chan[CH_4OP_IDX[bit_num]].ch_type := CH_2OP;
        p_chip^.chan[CH_4OP_IDX[bit_num]+3].ch_type := CH_2OP;
      end;
end;
