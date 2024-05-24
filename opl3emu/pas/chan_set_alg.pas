// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure chan_set_alg(p_chan: P_OPL3_CHAN);
begin
  Case p_chan^.ch_type of
    CH_2OP:
      Case (p_chan^.alg AND 1) of
        0: begin
             // FM
             p_chan^.p_slot[0]^.p_mod := @p_chan^.p_slot[0]^.fb_out;
             p_chan^.p_slot[1]^.p_mod := @p_chan^.p_slot[0]^.output;
             p_chan^.p_out[0] := @p_chan^.p_slot[1]^.output;
             p_chan^.p_out[1] := @p_chan^.p_chip^.out_null;
             p_chan^.p_out[2] := @p_chan^.p_chip^.out_null;
             p_chan^.p_out[3] := @p_chan^.p_chip^.out_null;
           end;

        1: begin
             // AM
             p_chan^.p_slot[0]^.p_mod := @p_chan^.p_slot[0]^.fb_out;
             p_chan^.p_slot[1]^.p_mod := @p_chan^.p_chip^.out_null;
             p_chan^.p_out[0] := @p_chan^.p_slot[0]^.output;
             p_chan^.p_out[1] := @p_chan^.p_slot[1]^.output;
             p_chan^.p_out[2] := @p_chan^.p_chip^.out_null;
             p_chan^.p_out[3] := @p_chan^.p_chip^.out_null;
           end;
      end;

    CH_4OP_2:
      begin
        p_chan^.p_chan^.p_out[0] := @p_chan^.p_chip^.out_null;
        p_chan^.p_chan^.p_out[1] := @p_chan^.p_chip^.out_null;
        p_chan^.p_chan^.p_out[2] := @p_chan^.p_chip^.out_null;
        p_chan^.p_chan^.p_out[3] := @p_chan^.p_chip^.out_null;

        Case (p_chan^.alg AND 3) of
          0: begin
               // FM-FM
               p_chan^.p_chan^.p_slot[0]^.p_mod := @p_chan^.p_chan^.p_slot[0]^.fb_out;
               p_chan^.p_chan^.p_slot[1]^.p_mod := @p_chan^.p_chan^.p_slot[0]^.output;
               p_chan^.p_slot[0]^.p_mod := @p_chan^.p_chan^.p_slot[1]^.output;
               p_chan^.p_slot[1]^.p_mod := @p_chan^.p_slot[0]^.output;
               p_chan^.p_out[0] := @p_chan^.p_slot[1]^.output;
               p_chan^.p_out[1] := @p_chan^.p_chip^.out_null;
               p_chan^.p_out[2] := @p_chan^.p_chip^.out_null;
               p_chan^.p_out[3] := @p_chan^.p_chip^.out_null;
             end;

          1: begin
               // FM-AM
               p_chan^.p_chan^.p_slot[0]^.p_mod := @p_chan^.p_chan^.p_slot[0]^.fb_out;
               p_chan^.p_chan^.p_slot[1]^.p_mod := @p_chan^.p_chan^.p_slot[0]^.output;
               p_chan^.p_slot[0]^.p_mod := @p_chan^.p_chip^.out_null;
               p_chan^.p_slot[1]^.p_mod := @p_chan^.p_slot[0]^.output;
               p_chan^.p_out[0] := @p_chan^.p_chan^.p_slot[1]^.output;
               p_chan^.p_out[1] := @p_chan^.p_slot[1]^.output;
               p_chan^.p_out[2] := @p_chan^.p_chip^.out_null;
               p_chan^.p_out[3] := @p_chan^.p_chip^.out_null;
             end;

          2: begin
               // AM-AM
               p_chan^.p_chan^.p_slot[0]^.p_mod := @p_chan^.p_chan^.p_slot[0]^.fb_out;
               p_chan^.p_chan^.p_slot[1]^.p_mod := @p_chan^.p_chip^.out_null;
               p_chan^.p_slot[0]^.p_mod := @p_chan^.p_chan^.p_slot[1]^.output;
               p_chan^.p_slot[1]^.p_mod := @p_chan^.p_slot[0]^.output;
               p_chan^.p_out[0] := @p_chan^.p_chan^.p_slot[0]^.output;
               p_chan^.p_out[1] := @p_chan^.p_slot[1]^.output;
               p_chan^.p_out[2] := @p_chan^.p_chip^.out_null;
               p_chan^.p_out[3] := @p_chan^.p_chip^.out_null;
             end;

          3: begin
               // AM-FM
               p_chan^.p_chan^.p_slot[0]^.p_mod := @p_chan^.p_chan^.p_slot[0]^.fb_out;
               p_chan^.p_chan^.p_slot[1]^.p_mod := @p_chan^.p_chip^.out_null;
               p_chan^.p_slot[0]^.p_mod := @p_chan^.p_chan^.p_slot[1]^.output;
               p_chan^.p_slot[1]^.p_mod := @p_chan^.p_chip^.out_null;
               p_chan^.p_out[0] := @p_chan^.p_chan^.p_slot[0]^.output;
               p_chan^.p_out[1] := @p_chan^.p_slot[0]^.output;
               p_chan^.p_out[2] := @p_chan^.p_slot[1]^.output;
               p_chan^.p_out[3] := @p_chan^.p_chip^.out_null;
             end;
        end;
      end;

    CH_RHYTHM:
      Case (p_chan^.alg AND 1) of
        0: begin
             // FM
             p_chan^.p_slot[0]^.p_mod := @p_chan^.p_slot[0]^.fb_out;
             p_chan^.p_slot[1]^.p_mod := @p_chan^.p_slot[0]^.output;
           end;

        1: begin
             // AM
             p_chan^.p_slot[0]^.p_mod := @p_chan^.p_slot[0]^.fb_out;
             p_chan^.p_slot[1]^.p_mod := @p_chan^.p_chip^.out_null;
           end;
      end;
  end;
end;
