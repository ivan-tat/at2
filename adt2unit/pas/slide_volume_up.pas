// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure slide_volume_up(chan,slide: Byte);

var
  limit1,limit2: Byte;
  limit1_4op,limit2_4op: Word;
  _4op_flag: Dword;
  _4op_conn: Byte;
  _4op_ch1,_4op_ch2: Byte;
  _4op_ins1,_4op_ins2: Byte;

begin
  _4op_flag := _4op_data_flag(chan);
  _4op_conn := (_4op_flag SHR 1) AND 3;
  _4op_ch1 := (_4op_flag SHR 3) AND 15;
  _4op_ch2 := (_4op_flag SHR 7) AND 15;
  _4op_ins1 := BYTE(_4op_flag SHR 11);
  _4op_ins2 := BYTE(_4op_flag SHR 19);

  If _4op_vol_valid_chan(chan) then
    begin
      If NOT peak_lock[_4op_ch1] then limit1_4op := 0
      else limit1_4op := ins_parameter(_4op_ins1,3) AND $3f SHL 16 +
                         ins_parameter(_4op_ins1,2) AND $3f;
      If NOT peak_lock[_4op_ch2] then limit2_4op := 0
      else limit2_4op := ins_parameter(_4op_ins2,3) AND $3f SHL 16 +
                         ins_parameter(_4op_ins2,2) AND $3f;
    end
  else begin
         If NOT peak_lock[chan] then limit1 := 0
         else limit1 := ins_parameter(event_table[chan].instr_def,3) AND $3f;
         If NOT peak_lock[chan] then limit2 := 0
         else limit2 := ins_parameter(event_table[chan].instr_def,2) AND $3f;
       end;

  Case volslide_type[chan] of
    0: begin
         If NOT _4op_vol_valid_chan(chan) then
           begin
             slide_carrier_volume_up(chan,slide,limit1);
             If (ins_parameter(voice_table[chan],10) AND 1 = 1) or
                (percussion_mode and (chan in [17..20])) then
               slide_modulator_volume_up(chan,slide,limit2);
           end
         else
           Case _4op_conn of
             // FM/FM
             0: slide_carrier_volume_up(_4op_ch1,slide,HI(limit1_4op));
             // FM/AM
             1: begin
                  slide_carrier_volume_up(_4op_ch1,slide,HI(limit1_4op));
                  slide_modulator_volume_up(_4op_ch2,slide,LO(limit2_4op));
                end;
             // AM/FM
             2: begin
                  slide_carrier_volume_up(_4op_ch1,slide,HI(limit1_4op));
                  slide_carrier_volume_up(_4op_ch2,slide,HI(limit2_4op));
                end;
             // AM/AM
             3: begin
                  slide_carrier_volume_up(_4op_ch1,slide,HI(limit1_4op));
                  slide_modulator_volume_up(_4op_ch1,slide,LO(limit1_4op));
                  slide_modulator_volume_up(_4op_ch2,slide,LO(limit2_4op));
                end;
           end;
       end;
    1: slide_carrier_volume_up(chan,slide,limit1);
    2: slide_modulator_volume_up(chan,slide,limit2);
    3: begin
         slide_carrier_volume_up(chan,slide,limit1);
         slide_modulator_volume_up(chan,slide,limit2);
       end;
  end;
end;
