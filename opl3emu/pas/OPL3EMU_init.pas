// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure OPL3EMU_init;

var
  slot_num,
  chan_num: Byte;

begin
  // initialize slot data
  For slot_num := 0 to 35 do
    begin
      opl3.slot[slot_num].p_chip := @opl3;
      opl3.slot[slot_num].p_mod := @opl3.out_null;
      opl3.slot[slot_num].eg_rout := $1ff;
      opl3.slot[slot_num].eg_out := $1ff;
      opl3.slot[slot_num].eg_state := EG_OFF;
      opl3.slot[slot_num].p_trem := @opl3.out_null;
    end;

  // initialize chan. data
  For chan_num := 0 to 17 do
    begin
      opl3.out_l[chan_num] := 0;
      opl3.out_r[chan_num] := 0;
      opl3.chan[chan_num].out_l := WORD_NULL;
      opl3.chan[chan_num].out_r := WORD_NULL;
      opl3.chan[chan_num].p_chip := @opl3;
      opl3.chan[chan_num].ch_type := CH_2OP;
      opl3.chan[chan_num].p_out[0] := @opl3.out_null;
      opl3.chan[chan_num].p_out[1] := @opl3.out_null;
      opl3.chan[chan_num].p_out[2] := @opl3.out_null;
      opl3.chan[chan_num].p_out[3] := @opl3.out_null;
      opl3.chan[chan_num].p_slot[0] := @opl3.slot[CH_SLOT_IDX[chan_num]];
      opl3.chan[chan_num].p_slot[1] := @opl3.slot[CH_SLOT_IDX[chan_num]+3];
      opl3.slot[CH_SLOT_IDX[chan_num]].p_chan := @opl3.chan[chan_num];
      opl3.slot[CH_SLOT_IDX[chan_num]+3].p_chan := @opl3.chan[chan_num];

      If (CH_4OP_MASK[chan_num] <> 0) then
        opl3.chan[chan_num].p_chan := @opl3.chan[PRED(CH_4OP_MASK[chan_num])];

      chan_set_alg(@opl3.chan[chan_num]);
  end;

  // initialize chip data
  opl3.noise := NOISE_HASH_VAL;
  opl3.timer := 0;
  opl3.nts_bit := 0;
  opl3.dva_bit := 0;
  opl3.dvb_bit := 0;
  opl3.rhy_flag := 0;
  opl3.vib_pos := 0;
  opl3.trem_dir := 0;
  opl3.trem_pos := 0;
  opl3.trem_val := 0;
  opl3.output[0] := 0;
  opl3.output[1] := 0;
end;
