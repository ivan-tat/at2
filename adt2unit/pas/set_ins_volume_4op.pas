// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure set_ins_volume_4op(volume,chan: Byte);

var
  _4op_flag: Dword;
  _4op_conn: Byte;
  _4op_ch1,_4op_ch2: Byte;

procedure set_volume(modulator,carrier,chan: Byte);

var
  temp: Byte;

begin
{$IFNDEF GO32V2}
  // OPL3 EMULATION WORKAROUND
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
      modulator := scale_volume(ins_parameter(voice_table[chan],2) AND $3f,modulator);
      opl3out(_instr[02]+_chan_m[chan],
              scale_volume(scale_volume(modulator,scale_volume(63-global_volume,63-fade_out_volume)),63-overall_volume)+LO(vscale_table[chan]));
      volume_table[chan] := concw(temp,HI(volume_table[chan]));
      modulator_vol[chan] := 63-scale_volume(modulator,scale_volume(63-global_volume,63-fade_out_volume))
    end;

  If (carrier <> BYTE_NULL) then
    begin
      temp := carrier;
      carrier := scale_volume(ins_parameter(voice_table[chan],3) AND $3f,carrier);
      opl3out(_instr[03]+_chan_c[chan],
              scale_volume(scale_volume(carrier,scale_volume(63-global_volume,63-fade_out_volume)),63-overall_volume)+HI(vscale_table[chan]));
      volume_table[chan] := concw(LO(volume_table[chan]),temp);
      carrier_vol[chan] := 63-scale_volume(carrier,scale_volume(63-global_volume,63-fade_out_volume));
    end;
end;

begin
  _4op_flag := _4op_data_flag(chan);
  _4op_conn := (_4op_flag SHR 1) AND 3;
  _4op_ch1 := (_4op_flag SHR 3) AND 15;
  _4op_ch2 := (_4op_flag SHR 7) AND 15;

  If _4op_vol_valid_chan(chan) then
    Case _4op_conn of
      // FM/FM
      0: If (volume = BYTE_NULL) then
           set_volume(BYTE_NULL,HI(volume_table[_4op_ch1]),_4op_ch1)
         else set_volume(BYTE_NULL,volume,_4op_ch1);
      // FM/AM
      1: If (volume = BYTE_NULL) then
           begin
             set_volume(BYTE_NULL,HI(volume_table[_4op_ch1]),_4op_ch1);
             set_volume(LO(volume_table[_4op_ch2]),BYTE_NULL,_4op_ch2);
           end
         else begin
                set_volume(BYTE_NULL,volume,_4op_ch1);
                set_volume(volume,BYTE_NULL,_4op_ch2);
              end;
      // AM/FM
      2: If (volume = BYTE_NULL) then
           begin
             set_volume(BYTE_NULL,HI(volume_table[_4op_ch1]),_4op_ch1);
             set_volume(BYTE_NULL,HI(volume_table[_4op_ch2]),_4op_ch2);
           end
         else begin
                set_volume(BYTE_NULL,volume,_4op_ch1);
                set_volume(BYTE_NULL,volume,_4op_ch2);
              end;
      // AM/AM
      3: If (volume = BYTE_NULL) then
           begin
             set_volume(LO(volume_table[_4op_ch1]),HI(volume_table[_4op_ch1]),_4op_ch1);
             set_volume(LO(volume_table[_4op_ch2]),BYTE_NULL,_4op_ch2);
           end
         else begin
                set_volume(volume,volume,_4op_ch1);
                set_volume(volume,BYTE_NULL,_4op_ch2);
              end;
    end;
end;
