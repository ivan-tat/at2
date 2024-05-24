// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function chip_generate(p_chip: P_OPL3_CHIP): Dword;

var
  slot_num: Byte;
  accm: Smallint;
  lr_mix: array[0..1] of Smallint;

begin
  // generate slot data
  For slot_num := 0 to 35 do
    begin
      slot_calc_fb(@p_chip^.slot[slot_num]);
      pg_generate(@p_chip^.slot[slot_num]);
      envelope_calc(@p_chip^.slot[slot_num]);
      slot_generate(@p_chip^.slot[slot_num],WORD_NULL);
    end;

  // rhythm mode
  If (p_chip^.rhy_flag AND $20 <> 0) then
    generate_rhythm_slots(p_chip);

  // update channel mixer
  lr_mix[1] := limit_value(p_chip^.output[1],-$7fff,$7fff);
  lr_mix[0] := limit_value(p_chip^.output[0],-$7fff,$7fff);
  p_chip^.output[0] := 0;
  p_chip^.output[1] := 0;

  // left output
  For slot_num := 0 to 17 do
    begin
      accm := p_chip^.chan[slot_num].p_out[0]^ +
              p_chip^.chan[slot_num].p_out[1]^ +
              p_chip^.chan[slot_num].p_out[2]^ +
              p_chip^.chan[slot_num].p_out[3]^;
      Inc(p_chip^.output[0],SMALLINT(accm AND p_chip^.chan[slot_num].out_l));
      p_chip^.out_l[slot_num] := SMALLINT(accm AND p_chip^.chan[slot_num].out_l);
    end;

  // right output
  For slot_num := 0 to 17 do
    begin
      accm := p_chip^.chan[slot_num].p_out[0]^ +
              p_chip^.chan[slot_num].p_out[1]^ +
              p_chip^.chan[slot_num].p_out[2]^ +
              p_chip^.chan[slot_num].p_out[3]^;
      Inc(p_chip^.output[1],SMALLINT(accm AND p_chip^.chan[slot_num].out_r));
      p_chip^.out_r[slot_num] := SMALLINT(accm AND p_chip^.chan[slot_num].out_r);
    end;

  // update LFO tremolo
  If (p_chip^.timer AND $3f = $3f) then
    begin
      If (p_chip^.trem_dir = 0) then
        begin
          If (p_chip^.trem_pos < 105) then
            Inc(p_chip^.trem_pos)
          else
            begin
              Dec(p_chip^.trem_pos);
              p_chip^.trem_dir := 1;
            end
        end
      else
        begin
          If (p_chip^.trem_pos > 0) then
            Dec(p_chip^.trem_pos)
          else
            begin
              Inc(p_chip^.trem_pos);
              p_chip^.trem_dir := 0;
            end;
        end;

      p_chip^.trem_val := (p_chip^.trem_pos SHR 2) SHR
                          ((1 - p_chip^.dva_bit) SHL 1);
    end;

  // update noise generator
  If (p_chip^.noise AND 1 <> 0) then
    p_chip^.noise := p_chip^.noise XOR NOISE_XOR;

  p_chip^.noise := p_chip^.noise SHR 1;
  Inc(p_chip^.timer);

  chip_generate := (WORD(lr_mix[1]) SHL 16) OR WORD(lr_mix[0]);
end;
