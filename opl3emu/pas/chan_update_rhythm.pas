// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure chan_update_rhythm(p_chip: P_OPL3_CHIP; data: Byte);
begin
  p_chip^.rhy_flag := data AND $3f;
  If (p_chip^.rhy_flag AND $20 <> 0) then
    begin
      // BD
      p_chip^.chan[6].ch_type := CH_RHYTHM;
      p_chip^.chan[6].p_out[0] := @p_chip^.chan[6].p_slot[1]^.output;
      p_chip^.chan[6].p_out[1] := @p_chip^.chan[6].p_slot[1]^.output;
      p_chip^.chan[6].p_out[2] := @p_chip^.out_null;
      p_chip^.chan[6].p_out[3] := @p_chip^.out_null;

      chan_set_alg(@p_chip^.chan[6]);
      eg_key_on_off(p_chip^.chan[6].p_slot[0],p_chip^.rhy_flag AND $10 <> 0);
      eg_key_on_off(p_chip^.chan[6].p_slot[1],p_chip^.rhy_flag AND $10 <> 0);

      // HH + SD
      p_chip^.chan[7].ch_type := CH_RHYTHM;
      p_chip^.chan[7].p_out[0] := @p_chip^.chan[7].p_slot[0]^.output;
      p_chip^.chan[7].p_out[1] := @p_chip^.chan[7].p_slot[0]^.output;
      p_chip^.chan[7].p_out[2] := @p_chip^.chan[7].p_slot[1]^.output;
      p_chip^.chan[7].p_out[3] := @p_chip^.chan[7].p_slot[1]^.output;

      eg_key_on_off(p_chip^.chan[7].p_slot[0],p_chip^.rhy_flag AND 1 <> 0);
      eg_key_on_off(p_chip^.chan[7].p_slot[1],p_chip^.rhy_flag AND 8 <> 0);

      // TT + TC
      p_chip^.chan[8].ch_type := CH_RHYTHM;
      p_chip^.chan[8].p_out[0] := @p_chip^.chan[8].p_slot[0]^.output;
      p_chip^.chan[8].p_out[1] := @p_chip^.chan[8].p_slot[0]^.output;
      p_chip^.chan[8].p_out[2] := @p_chip^.chan[8].p_slot[1]^.output;
      p_chip^.chan[8].p_out[3] := @p_chip^.chan[8].p_slot[1]^.output;

      eg_key_on_off(p_chip^.chan[8].p_slot[0],p_chip^.rhy_flag AND 4 <> 0);
      eg_key_on_off(p_chip^.chan[8].p_slot[1],p_chip^.rhy_flag AND 2 <> 0);
    end
  else
    begin
      // reset chan. 6/7/8 to 2OP
      p_chip^.chan[6].ch_type := CH_2OP;
      chan_set_alg(@p_chip^.chan[6]);
      p_chip^.chan[7].ch_type := CH_2OP;
      chan_set_alg(@p_chip^.chan[7]);
      p_chip^.chan[8].ch_type := CH_2OP;
      chan_set_alg(@p_chip^.chan[8]);
    end;
end;
