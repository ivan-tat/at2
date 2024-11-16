// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure slide_volume_down(chan,slide: Byte);

var
  _4op_flag: Dword;
  _4op_conn: Byte;
  _4op_ch1,_4op_ch2: Byte;

begin
  _4op_flag := _4op_data_flag(chan);
  _4op_conn := (_4op_flag SHR 1) AND 3;
  _4op_ch1 := (_4op_flag SHR 3) AND 15;
  _4op_ch2 := (_4op_flag SHR 7) AND 15;

  Case volslide_type[chan] of
    0: begin
         If NOT _4op_vol_valid_chan(chan) then
           begin
             slide_carrier_volume_down(chan,slide);
             If (ins_parameter(voice_table[chan],10) AND 1 = 1) or
                (percussion_mode and (chan in [17..20])) then
               slide_modulator_volume_down(chan,slide);
           end
         else
           Case _4op_conn of
             // FM/FM
             0: slide_carrier_volume_down(_4op_ch1,slide);
             // FM/AM
             1: begin
                  slide_carrier_volume_down(_4op_ch1,slide);
                  slide_modulator_volume_down(_4op_ch2,slide);
                end;
             // AM/FM
             2: begin
                  slide_carrier_volume_down(_4op_ch1,slide);
                  slide_carrier_volume_down(_4op_ch2,slide);
                end;
             // AM/AM
             3: begin
                  slide_carrier_volume_down(_4op_ch1,slide);
                  slide_modulator_volume_down(_4op_ch1,slide);
                  slide_modulator_volume_down(_4op_ch2,slide);
                end;
           end;
       end;
    1: slide_carrier_volume_down(chan,slide);
    2: slide_modulator_volume_down(chan,slide);
    3: begin
         slide_carrier_volume_down(chan,slide);
         slide_modulator_volume_down(chan,slide);
       end;
  end;
end;
