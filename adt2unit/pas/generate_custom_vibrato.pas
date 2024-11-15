// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure generate_custom_vibrato(value: Byte);

const
  vibtab_size: array[0..15] of Byte = (
    16,16,16,16,32,32,32,32,64,64,64,64,128,128,128,128);

var
  mul_r: Real;
  mul_b: Byte;
  idx,idx2: Byte;

function min0(value: Longint): Longint;
begin
  If (value >= 0) then min0 := value
  else min0 := 0;
end;

begin
  Case value of
    // set default speed table
    0: begin
         vibtrem_table_size := def_vibtrem_table_size;
         Move(def_vibtrem_table,vibtrem_table,SizeOf(vibtrem_table));
       end;

    // set custom speed table (fixed size = 32)
    1..239:
       begin
         vibtrem_table_size := def_vibtrem_table_size;
         mul_r := value/16;
         For idx2 := 0 to 7 do
          begin
            vibtrem_table[idx2*32] := 0;
            For idx := 1 to 16 do
              vibtrem_table[idx2*32+idx] := ROUND(idx*mul_r);
            For idx := 17 to 31 do
              vibtrem_table[idx2*32+idx] := ROUND((32-idx)*mul_r);
          end;
       end;

    // set custom speed table (speed factor = 1-4)
    240..255:
       begin
         vibtrem_speed_factor := SUCC((value-240) MOD 4);
         vibtrem_table_size := 2*vibtab_size[value-240];
         mul_b := 256 DIV (vibtab_size[value-240]);
         For idx2 := 0 to PRED(128 DIV vibtab_size[value-240]) do
           begin
             vibtrem_table[2*vibtab_size[value-240]*idx2] := 0;
             For idx := 1 to vibtab_size[value-240] do
               vibtrem_table[2*vibtab_size[value-240]*idx2+idx] :=
                 min0(idx*mul_b-1);
             For idx := vibtab_size[value-240]+1 to
                        2*vibtab_size[value-240]-1 do
               vibtrem_table[2*vibtab_size[value-240]*idx2+idx] :=
                 min0((2*vibtab_size[value-240]-idx)*mul_b-1);
           end;
       end;
  end;
end;
