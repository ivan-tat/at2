// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

var
  ibuf_idx,ibuf_end,obuf_idx,obuf_src: pByte;
  ctrl_bits,ctrl_mask,
  command,count,offs: Word;

procedure RDC_decode;
begin
  ctrl_mask := 0;
  ibuf_idx := input_ptr;
  obuf_idx := output_ptr;
  ibuf_end := input_ptr+input_size;

  While (ibuf_idx < ibuf_end) do
    begin
      ctrl_mask := ctrl_mask SHR 1;
      If (ctrl_mask = 0) then
      begin
        ctrl_bits := pWord(ibuf_idx)^;
        Inc(ibuf_idx,2);
        ctrl_mask := $8000;
      end;

      If (ctrl_bits AND ctrl_mask = 0) then
        begin
          obuf_idx^ := ibuf_idx^;
          Inc(ibuf_idx);
          Inc(obuf_idx);
          CONTINUE;
        end;

      command := (ibuf_idx^ SHR 4) AND 15;
      count := ibuf_idx^ AND 15;
      Inc(ibuf_idx);

      Case command Of
        // short RLE
        0: begin
             Inc(count,3);
             FillChar(obuf_idx^,count,ibuf_idx^);
             Inc(ibuf_idx);
             Inc(obuf_idx,count);
           end;
        // long RLE
        1: begin
             Inc(count,ibuf_idx^ SHL 4);
             Inc(ibuf_idx);
             Inc(count,19);
             FillChar(obuf_idx^,count,ibuf_idx^);
             Inc(ibuf_idx);
             Inc(obuf_idx, count);
           end;
        // long pattern
        2: begin
             offs := count+3;
             Inc(offs,ibuf_idx^ SHL 4);
             Inc(ibuf_idx);
             count := ibuf_idx^;
             Inc(ibuf_idx);
             Inc(count,16);
             obuf_src := obuf_idx-offs;
             Move(obuf_src^,obuf_idx^,count);
             Inc(obuf_idx,count);
           end;
        // short pattern
        else begin
               offs := count+3;
               Inc(offs,ibuf_idx^ SHL 4);
               Inc(ibuf_idx);
               obuf_src := obuf_idx-offs;
               Move(obuf_src^,obuf_idx^,command);
               Inc(obuf_idx,command);
             end;
      end;
    end;

  output_size := obuf_idx-output_ptr;
end;

function RDC_decompress(var source,dest; size: Word): Word;
begin
  input_ptr := @source;
  output_ptr := @dest;
  input_size := size;
  RDC_decode;
  RDC_decompress := output_size;
end;
