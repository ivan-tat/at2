// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure set_ins_volume(modulator,carrier,chan: Byte);

var
  temp: Byte;

begin
{$IFNDEF GO32V2}
  // ** OPL3 emulation workaround **
  // force muted instrument volume with missing channel ADSR data
  // when there is additionally no FM-reg macro defined for this instrument
  If is_chan_adsr_data_empty(chan) and
     NOT (songdata.instr_macros[voice_table[chan]].length <> 0) then
    begin
      modulator := 63;
      carrier := 63;
    end;
{$ENDIF}

  If (modulator <> BYTE_NULL) then
    begin
      temp := modulator;
      If volume_scaling then
        If (ins_parameter(voice_table[chan],10) AND 1 = 1) or
           (percussion_mode and (chan in [17..20])) then
          modulator := scale_volume(ins_parameter(voice_table[chan],2) AND $3f,modulator);
      If (ins_parameter(voice_table[chan],10) AND 1 = 1) or
         (percussion_mode and (chan in [17..20])) then
        opl3out(_instr[02]+_chan_m[chan],
                scale_volume(scale_volume(modulator,scale_volume(63-global_volume,63-fade_out_volume)),63-overall_volume)+LO(vscale_table[chan]))
      else
        opl3out(_instr[02]+_chan_m[chan],
                temp+LO(vscale_table[chan]));
      volume_table[chan] := concw(temp,HI(volume_table[chan]));
      If (ins_parameter(voice_table[chan],10) AND 1 = 1) or
         (percussion_mode and (chan in [17..20])) then
        modulator_vol[chan] := 63-scale_volume(modulator,scale_volume(63-global_volume,63-fade_out_volume))
      else modulator_vol[chan] := 63-modulator;
    end;

  If (carrier <> BYTE_NULL) then
    begin
      temp := carrier;
      If volume_scaling then
        carrier := scale_volume(ins_parameter(voice_table[chan],3) AND $3f,carrier);
      opl3out(_instr[03]+_chan_c[chan],
              scale_volume(scale_volume(carrier,scale_volume(63-global_volume,63-fade_out_volume)),63-overall_volume)+HI(vscale_table[chan]));
      volume_table[chan] := concw(LO(volume_table[chan]),temp);
      carrier_vol[chan] := 63-scale_volume(carrier,scale_volume(63-global_volume,63-fade_out_volume));
    end;
end;
