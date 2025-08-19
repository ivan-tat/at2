// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure WriteDataBlock(ptr: Pointer; size: Word);
begin
  Move(ptr^,output_buffer^[output_buffer_idx],size);
  Inc(output_buffer_idx,size);
{$IFNDEF ADT2PLAY}
  If NOT really_no_status_refresh then
    show_progress2(output_buffer_idx,3);
{$ENDIF} // NOT DEFINED(ADT2PLAY)
end;

procedure FillBitBuffer(bits: Integer);
begin
  bit_buf := (bit_buf SHL bits);
  While (bits > bit_count) do
    begin
      Dec(bits,bit_count);
      bit_buf := bit_buf OR (sbit_buf SHL bits);
      If (input_buffer_idx <= input_buffer_size) then
        begin
          sbit_buf := input_buffer^[input_buffer_idx];
          Inc(input_buffer_idx);
        end
      else sbit_buf := 0;
      bit_count := 8;
    end;
  Dec(bit_count,bits);
  bit_buf := bit_buf OR (sbit_buf SHR bit_count);
end;

function GetBits(bits: Integer): Word;
begin
  GetBits := bit_buf SHR (16-bits);
  FillBitBuffer(bits);
end;

procedure MakeTable(n_char: Integer;
                    bit_len: pBYTE;
                    bits: Integer;
                    table: pWORD);
var
  count,weight: array[1..16] of Word;
  start: array[1..17] of Word;
  idx,idx2,len,chr,j_bits,avail,next_c,mask: Integer;
  ptr: pWORD;

begin
  FillChar(count,SizeOf(count),0);
  FillChar(weight,SizeOf(weight),0);
  FillChar(start,SizeOf(start),0);

  For idx := 0 to PRED(n_char) do
    Inc(count[bit_len^[idx]]);
  start[1] := 0;
  For idx := 1 to 16 do
    start[SUCC(idx)] := start[idx]+(count[idx] SHL (16-idx));
  j_bits := 16-bits;
  For idx := 1 to bits do
    begin
      start[idx] := start[idx] SHR j_bits;
      weight[idx] := 1 SHL (bits-idx);
    end;
  idx := SUCC(bits);
  While (idx <= 16) do
    begin
      weight[idx] := 1 SHL (16-idx);
      Inc(idx);
    end;
  idx := start[SUCC(bits)] SHR j_bits;
  If (idx <> 0) then
    begin
      idx2 := 1 SHL bits;
      If (idx <> idx2) then
        begin
          FillWord(table^[idx],idx2-idx,0);
          idx := idx2;
        end;
    end;
  avail := n_char;
  mask := 1 SHL (15-bits);
  For chr := 0 to PRED(n_char) do
    begin
      len := bit_len^[chr];
      If (len = 0) then
        CONTINUE;
      idx2 := start[len];
      next_c := idx2+weight[len];
      If (len <= bits) then
        For idx := idx2 to PRED(next_c) do
          table^[idx] := chr
       else begin
              ptr := Addr(table^[WORD(idx2) SHR j_bits]);
              idx := len-bits;
              While (idx <> 0) do
                begin
                  If (ptr^[0] = 0) then
                    begin
                      r_tree[avail] := 0;
                      l_tree[avail] := 0;
                      ptr^[0] := avail;
                      Inc(avail);
                    end;
                  If (idx2 AND mask <> 0) then
                    ptr := Addr(r_tree[ptr^[0]])
                  else ptr := Addr(l_tree[ptr^[0]]);
                  idx2 := idx2 SHL 1;
                  Dec(idx);
                end;
              ptr^[0] := chr;
            end;
      start[len] := next_c;
    end;
end;

procedure ReadPtrLen(n_char,n_bit,i_bit: Integer);

var
  idx,chr,bits: Integer;
  mask: Word;

begin
  bits := GetBits(n_bit);
  If (bits = 0) then
    begin
      chr := GetBits(n_bit);
      FillChar(p_len,SizeOf(p_len),0);
      FillWord(p_table,SizeOf(p_table) DIV 2,chr);
    end
  else begin
         idx := 0;
         While (idx < bits) do
           begin
             chr := bit_buf SHR (16-3);
             If (chr = 7) then
               begin
                 mask := 1 SHL (16-4);
                 While (mask AND bit_buf <> 0) do
                   begin
                     mask := mask SHR 1;
                     Inc(chr);
                   end;
               end;
             If (chr < 7) then
               FillBitBuffer(3)
             else FillBitBuffer(chr-3);
             p_len[idx] := chr;
             Inc(idx);
             If (idx = i_bit) then
               begin
                 chr := PRED(GetBits(2));
                 While (chr >= 0) do
                   begin
                     p_len[idx] := 0;
                     Inc(idx);
                     Dec(chr);
                   end;
               end;
           end;
         If (idx < n_char) then
           begin
             FillWord(p_len[idx],n_char-idx,0);
             idx := n_char;
           end;
         MakeTable(n_char,@p_len,8,@p_table);
       end;
end;

procedure ReadCharLen;

var
  idx,chr,bits: Integer;
  mask: Word;

begin
  bits := GetBits(CBIT);
  If (bits = 0) then
    begin
      chr := GetBits(CBIT);
      FillChar(c_len,SizeOf(c_len),0);
      FillWord(c_table,SizeOf(c_table) DIV 2,chr);
    end
  else begin
         idx := 0;
         While (idx < bits) do
           begin
             chr := p_table[bit_buf SHR (16-8)];
             If (chr >= NT) then
               begin
                 mask := 1 SHL (16-9);
                 Repeat
                   If (bit_buf AND mask <> 0) then
                     chr := r_tree[chr]
                   else chr := l_tree[chr];
                   mask := mask SHR 1;
                 until (chr < NT);
               end;
             FillBitBuffer(p_len[chr]);
             If (chr <= 2) then
               begin
                 If (chr = 1) then
                   chr := 2+GetBits(4)
                 else If (chr = 2) then
                        chr := 19+GetBits(CBIT);
                 While (chr >= 0) do
                   begin
                     c_len[idx] := 0;
                     Inc(idx);
                     Dec(chr);
                   end;
               end
             else begin
                    c_len[idx] := chr-2;
                    Inc(idx);
                  end;
           end;
         While (idx < NC) do
           begin
             c_len[idx] := 0;
             Inc(idx);
           end;
         MakeTable(NC,@c_len,12,@c_table);
       end;
end;

function DecodeChar: Word;

var
  chr,mask: Word;

begin
  If (block_size = 0) then
    begin
      block_size := GetBits(16);
      ReadPtrLen(NT,TBIT,3);
      ReadCharLen;
      ReadPtrLen(NP,PBIT,-1);
    end;
  Dec(block_size);
  chr := c_table[bit_buf SHR (16-12)];
  If (chr >= NC) then
    begin
      mask := 1 SHL (16-13);
      Repeat
        If (bit_buf AND mask <> 0) then
          chr := r_tree[chr]
        else chr := l_tree[chr];
        mask := mask SHR 1;
      until (chr < NC);
    end;
  FillBitBuffer(c_len[chr]);
  DecodeChar := chr;
end;

function DecodePtr: Word;

var
  ptr,mask: Word;

begin
  ptr := p_table[bit_buf SHR (16-8)];
  If (ptr >= NP) then
    begin
      mask := 1 SHL (16-9);
      Repeat
        If (bit_buf AND mask <> 0) then
          ptr := r_tree[ptr]
        else ptr := l_tree[ptr];
        mask := mask SHR 1;
      until (ptr < NP);
    end;
  FillBitBuffer(p_len[ptr]);
  If (ptr <> 0) then
    begin
      Dec(ptr);
      ptr := (1 SHL ptr)+GetBits(ptr);
    end;
  DecodePtr := ptr;
end;

procedure DecodeBuffer(count: Word; buffer: pBYTE);

var
  idx,idx2: Word;

begin
  idx2 := 0;
  Dec(dec_counter);
  While (dec_counter >= 0) do
    begin
      buffer^[idx2] := buffer^[dec_ptr];
      dec_ptr := SUCC(dec_ptr) AND PRED(DIC_SIZE);
      Inc(idx2);
      If (idx2 = count) then
        EXIT; //DecodeBuffer
      Dec(dec_counter);
    end;
  Repeat
    idx := DecodeChar;
    If (idx <= 255) then
      begin
        buffer^[idx2] := idx;
        Inc(idx2);
        If (idx2 = count) then
          EXIT; //DecodeBuffer
      end
    else begin
           dec_counter := idx-(256-THRESHOLD);
           dec_ptr := (idx2-DecodePtr-1) AND PRED(DIC_SIZE);
           Dec(dec_counter);
           While (dec_counter >= 0) do
             begin
               buffer^[idx2] := buffer^[dec_ptr];
               dec_ptr := SUCC(dec_ptr) AND PRED(DIC_SIZE);
               Inc(idx2);
               If (idx2 = count) then
                 EXIT; //DecodeBuffer
               Dec(dec_counter);
             end;
         end;
  until FALSE;
end;

function LZH_decompress(var source,dest; size: Dword): Dword;

var
  ptr: pBYTE;
  size_temp: Dword;
  ultra_compression_flag: Boolean;

begin
  LZH_decompress := 0;
  input_buffer := Addr(source);
  input_buffer_idx := 0;
  ultra_compression_flag := BOOLEAN(input_buffer^[input_buffer_idx]);
  Inc(input_buffer_idx);
  input_buffer_size := size;
  output_buffer := Addr(dest);
  output_buffer_idx := 0;
  Move(input_buffer^[input_buffer_idx],size_unpacked,SizeOf(size_unpacked));
  Inc(input_buffer_idx,SizeOf(size_unpacked));
  size := size_unpacked;
{$IFNDEF ADT2PLAY}
  progress_old_value := BYTE_NULL;
  progress_value := size;
{$ENDIF} // NOT DEFINED(ADT2PLAY)

  If ultra_compression_flag then
    begin
{$IFNDEF ADT2PLAY}
      WIN_SIZE := WIN_SIZE_MAX;
{$ENDIF} // NOT DEFINED(ADT2PLAY)
      DIC_SIZE := DIC_SIZE_MAX;
    end
  else begin
{$IFNDEF ADT2PLAY}
         WIN_SIZE := WIN_SIZE_DEF;
{$ENDIF} // NOT DEFINED(ADT2PLAY)
         DIC_SIZE := DIC_SIZE_DEF;
       end;

  GetMem(ptr,DIC_SIZE);
  bit_buf := 0;
  sbit_buf := 0;
  bit_count := 0;
  FillBitBuffer(16);
  block_size := 0;
  dec_counter := 0;

  While (size > 0) do
    begin
      If (size > DIC_SIZE) then
        size_temp := DIC_SIZE
      else size_temp := size;
      DecodeBuffer(size_temp,ptr);
      WriteDataBlock(ptr,size_temp);
      Dec(size,size_temp);
    end;

  FreeMem(ptr,DIC_SIZE);
  LZH_decompress := size_unpacked;
end;
