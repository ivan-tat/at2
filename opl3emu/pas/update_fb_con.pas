// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure update_fb_con(p_chan: P_OPL3_CHAN; data: Byte);
begin
  p_chan^.fb := (data AND $0e) SHR 1;
  p_chan^.con := data AND 1;
  p_chan^.alg := p_chan^.con;

  Case p_chan^.ch_type of
    CH_2OP,
    CH_RHYTHM:
      chan_set_alg(p_chan);

    CH_4OP_1:
      begin
        p_chan^.p_chan^.alg := 4 OR
                               (p_chan^.con SHL 1) OR
                               (p_chan^.p_chan^.con);
        chan_set_alg(p_chan^.p_chan);
      end;

    CH_4OP_2:
      begin
        p_chan^.alg := 4 OR
                       (p_chan^.p_chan^.con SHL 1) OR
                       (p_chan^.con);
        chan_set_alg(p_chan);
      end;
  end;

  // trigger output to left
  If ((data SHR 4) AND 1 <> 0) then
    p_chan^.out_l := WORD_NULL
  else p_chan^.out_l := 0;

  // trigger output to right
  If ((data SHR 5) AND 1 <> 0) then
    p_chan^.out_r := WORD_NULL
  else p_chan^.out_r := 0;
end;
