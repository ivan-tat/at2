// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure update_key(p_chan: P_OPL3_CHAN; key_on: Boolean);
begin
  Case p_chan^.ch_type of
    CH_2OP,
    CH_4OP_1,
    CH_RHYTHM:
      begin
        eg_key_on_off(p_chan^.p_slot[0],key_on);
        eg_key_on_off(p_chan^.p_slot[1],key_on);

        If (p_chan^.ch_type = CH_4OP_1) then
          begin
            eg_key_on_off(p_chan^.p_chan^.p_slot[0],key_on);
            eg_key_on_off(p_chan^.p_chan^.p_slot[1],key_on);
          end;
      end;
  end;
end;
