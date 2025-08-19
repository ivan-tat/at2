// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function ReadDataBlock(ptr: Pointer; size: Word): Word;

var
  result: Word;

begin
  If (input_buffer_size-input_buffer_idx >= size) then
    result := size
  else result := input_buffer_size-input_buffer_idx;
  Move(input_buffer^[input_buffer_idx],ptr^,result);
  Inc(input_buffer_idx,result);
  ReadDataBlock := result;
{$IFNDEF ADT2PLAY}
  If NOT really_no_status_refresh then
    show_progress2(input_buffer_idx,3);
{$ENDIF} // NOT DEFINED(ADT2PLAY)
end;

procedure PutBits(bits: Integer; xbits: Word);
begin
  If (bits < bit_count) then
    begin
      Dec(bit_count,bits);
      sbit_buf := sbit_buf OR (xbits SHL bit_count);
    end
  else begin
         Dec(bits,bit_count);
         output_buffer^[output_buffer_idx] := sbit_buf OR (xbits SHR bits);
         Inc(output_buffer_idx);
         If (bits < 8) then
           begin
             bit_count := 8-bits;
             sbit_buf := xbits SHL bit_count;
           end
         else begin
                output_buffer^[output_buffer_idx] := xbits SHR (bits-8);
                Inc(output_buffer_idx);
                bit_count := 16-bits;
                sbit_buf := xbits SHL bit_count;
              end;
       end;
end;

procedure CountLen(idx: Integer);
begin
  If (idx < bits) then
    If (depth < 16) then
      Inc(len_count[depth])
    else Inc(len_count[16])
  else begin
         Inc(depth);
         CountLen(l_tree[idx]);
         CountLen(r_tree[idx]);
         Dec(depth);
       end;
end;

procedure MakeLen(root: Integer);

var
  idx,idx2: Integer;
  sum: Word;

begin
  FillChar(len_count,SizeOf(len_count),0);
  CountLen(root);
  sum := 0;
  For idx := 16 downto 1 do
    Inc(sum,len_count[idx] SHL (16-idx));
  While (sum <> 0) do
    begin
      Dec(len_count[16]);
      For idx := 15 downto 1 do
        If (len_count[idx] <> 0) then
          begin
            Dec(len_count[idx]);
            Inc(len_count[SUCC(idx)],2);
            BREAK;
          end;
      Dec(sum);
    end;
  For idx := 16 downto 1 do
    begin
      idx2 := PRED(len_count[idx]);
      While (idx2 >= 0) do
        begin
          Dec(idx2);
          len^[sort_ptr^[0]] := idx;
          sort_ptr := Addr(sort_ptr^[1]);
        end;
    end;
end;

procedure DownHeap(idx: Integer);

var
  idx2,idx3: Integer;

begin
  idx2 := heap[idx];
  idx3 := idx SHL 1;
  While (idx3 <= heap_size) do
    begin
      If (idx3 < heap_size) and
         (freq^[heap[idx3]] > freq^[heap[SUCC(idx3)]]) then
        Inc(idx3);
      If (freq^[idx2] <= freq^[heap[idx3]]) then
        BREAK;
      heap[idx] := heap[idx3];
      idx := idx3;
      idx3 := idx SHL 1;
    end;
  heap[idx] := idx2;
end;

procedure MakeCode(bits: Integer; len: pBYTE; code: pWORD);

var
  idx,idx2: Integer;
  start: array[0..17] of Word;

begin
  start[1] := 0;
  For idx := 1 to 16 do
    start[SUCC(idx)] := (start[idx]+len_count[idx]) SHL 1;
  For idx := 0 to PRED(bits) do
    begin
      idx2 := len^[idx];
      code^[idx] := start[idx2];
      Inc(start[idx2]);
    end;
end;

function MakeTree(n_par: Integer;
                  f_par: pWORD;
                  l_par: pBYTE;
                  c_par: pWORD): Integer;
var
  idx,idx2,idx3,avail: Integer;

begin
  bits := n_par;
  freq := f_par;
  len := l_par;
  avail := bits;
  heap_size := 0;
  heap[1] := 0;

  For idx := 0 to PRED(bits) do
    begin
      len^[idx] := 0;
      If (freq^[idx] <> 0) then
        begin
          Inc(heap_size);
          heap[heap_size] := idx;
        end;
    end;

  If (heap_size < 2) then
    begin
      c_par^[heap[1]] := 0;
      MakeTree := heap[1];
      EXIT; //MakeTree
    end;

  For idx := (heap_size DIV 2) downto 1 do
    DownHeap(idx);
  sort_ptr := c_par;
  Repeat
    idx := heap[1];
    If (idx < bits) then
      begin
        sort_ptr^[0] := idx;
        sort_ptr := Addr(sort_ptr^[1]);
      end;
    heap[1] := heap[heap_size];
    Dec(heap_size);
    DownHeap(1);
    idx3 := heap[1];
    If (idx3 < bits) then
      begin
        sort_ptr^[0] := idx3;
        sort_ptr := Addr(sort_ptr^[1]);
      end;
    idx2 := avail;
    Inc(avail);
    freq^[idx2] := freq^[idx]+freq^[idx3];
    heap[1] := idx2;
    DownHeap(1);
    l_tree[idx2] := idx;
    r_tree[idx2] := idx3;
  until (heap_size <= 1);

  sort_ptr := c_par;
  MakeLen(idx2);
  MakeCode(n_par,l_par,c_par);
  MakeTree := idx2;

  //EXIT //MakeTree
end;

procedure CountFreq;

var
  idx,idx2,bits,count: Integer;

begin
  For idx := 0 to PRED(NT) do
    t_freq[idx] := 0;
  bits := NC;
  While (bits > 0) and
        (c_len[PRED(bits)] = 0) do
    Dec(bits);
  idx := 0;
  While (idx < bits) do
    begin
      idx2 := c_len[idx];
      Inc(idx);
      If (idx2 = 0) then
        begin
          count := 1;
          While (idx < bits) and
                (c_len[idx] = 0) do
            begin
              Inc(idx);
              Inc(count);
            end;
          If (count <= 2) then
            Inc(t_freq[0],count)
          else If (count <= 18) then
                 Inc(t_freq[1])
                else If (count = 19) then
                       begin
                         Inc(t_freq[0]);
                         Inc(t_freq[1]);
                       end
                     else Inc(t_freq[2]);
        end
      else Inc(t_freq[idx2+2]);
    end;
end;

procedure WritePtrLen(bits,n_bit,s_bit: Integer);

var
  idx,idx2: Integer;

begin
  While (bits > 0) and
        (p_len[PRED(bits)] = 0) do
    Dec(bits);
  PutBits(n_bit,bits);
  idx := 0;
  While (idx < bits) do
    begin
      idx2 := p_len[idx];
      Inc(idx);
      If (idx2 <= 6) then
        PutBits(3,idx2)
      else begin
             Dec(idx2,3);
             PutBits(idx2,(1 SHL idx2)-2);
           end;
      If (idx = s_bit) then
        begin
          While (idx < 6) and
                (p_len[idx] = 0) do
            Inc(idx);
          PutBits(2,(idx-3) AND 3);
        end;
    end;
end;

procedure WriteCharLen;

var
  idx,idx2,bits,count: Integer;

begin
  bits := NC;
  While (bits > 0) and
        (c_len[PRED(bits)] = 0) do
    Dec(bits);
  PutBits(CBIT,bits);
  idx := 0;
  While (idx < bits) do
    begin
      idx2 := c_len[idx];
      Inc(idx);
      If (idx2 = 0) then
        begin
          count := 1;
          While (idx < bits) and
                (c_len[idx] = 0) do
            begin
              Inc(idx);
              Inc(count);
            end;
          If (count <= 2) then
            For idx2 := 0 to PRED(count) do
              PutBits(p_len[0],p_code[0])
          else If (count <= 18) then
                 begin
                   PutBits(p_len[1],p_code[1]);
                   PutBits(4,Count-3);
                 end
               else If (count = 19) then
                      begin
                        PutBits(p_len[0],p_code[0]);
                        PutBits(p_len[1],p_code[1]);
                        PutBits(4,15);
                      end
                    else begin
                           PutBits(p_len[2],p_code[2]);
                           PutBits(CBIT,count-20);
                         end;
        end
      else PutBits(p_len[idx2+2],p_code[idx2+2]);
    end;
end;

procedure EncodePtr(ptr: Word);

var
  idx,idx2: Word;

begin
  idx := 0;
  idx2 := ptr;
  While (idx2 <> 0) do
    begin
      idx2 := idx2 SHR 1;
      Inc(idx);
    end;
  PutBits(p_len[idx],p_code[idx]);
  If (idx > 1) then
    PutBits(PRED(idx),ptr AND (PRED(1 SHL 16) SHR (17-idx)));
end;

procedure SendBlock;

var
  idx,idx2,flags,
  root,pos,size: Word;

begin
  root := MakeTree(NC,@c_freq,@c_len,@c_code);
  Size := c_freq[root];
  PutBits(16,size);
  If (root >= NC) then
    begin
      CountFreq;
      root := MakeTree(NT,@t_freq,@p_len,@p_code);
      If (root >= NT) then
        WritePtrLen(NT,TBIT,3)
      else begin
             PutBits(TBIT,0);
             PutBits(TBIT,root);
           end;
      WriteCharLen;
    end
  else begin
         PutBits(TBIT,0);
         PutBits(TBIT,0);
         PutBits(CBIT,0);
         PutBits(CBIT,root);
       end;
  root := MakeTree(NP,@p_freq,@p_len,@p_code);
  If (root >= NP) then
    WritePtrLen(NP,PBIT,-1)
  else begin
         PutBits(PBIT,0);
         PutBits(PBIT,root);
       end;
  pos := 0;
  For idx := 0 to PRED(size) do
    begin
      If (idx AND 7 = 0) then
        begin
          flags := buf^[pos];
          Inc(pos);
        end
      else flags:=flags SHL 1;
      If (flags AND (1 SHL 7) <> 0) then
        begin
          idx2 := buf^[pos]+(1 SHL 8);
          Inc(pos);
          PutBits(c_len[idx2],c_code[idx2]);
          idx2 := buf^[pos] SHL 8;
          Inc(pos);
          Inc(idx2,buf^[pos]);
          Inc(pos);
          EncodePtr(idx2);
        end
      else begin
             idx2 := buf^[pos];
             Inc(pos);
             PutBits(c_len[idx2],c_code[idx2]);
           end;
    end;
  For idx := 0 to PRED(NC) do
    c_freq[idx] := 0;
  For idx := 0 to PRED(NP) do
    p_freq[idx] := 0;
end;

procedure Output(code,c_ptr: Word);
begin
  out_mask := out_mask SHR 1;
  If (out_mask = 0) then
    begin
      out_mask := 1 SHL 7;
      If (out_pos >= WIN_SIZE-24) then
        begin
          SendBlock;
          out_pos := 0;
        end;
      c_pos := out_pos;
      Inc(out_pos);
      buf^[c_pos] := 0;
    end;
  buf^[out_pos] := code;
  Inc(out_pos);
  Inc(c_freq[code]);
  If (code >= 1 SHL 8) then
    begin
      buf^[c_pos] := buf^[c_pos] OR out_mask;
      buf^[out_pos] := c_ptr SHR 8;
      Inc(out_pos);
      buf^[out_pos] := c_ptr;
      Inc(out_pos);
      code := 0;
      While (c_ptr <> 0) do
        begin
          c_ptr := c_ptr SHR 1;
          Inc(code);
        end;
      Inc(p_freq[code]);
    end;
end;

procedure InitSlide;

var
  idx: Word;

begin
  For idx := DIC_SIZE to (DIC_SIZE+255) do
    begin
      level^[idx] := 1;
      pos_ptr^[idx] := 0;
    end;
  For idx := DIC_SIZE to PRED(2*DIC_SIZE) do
    parent^[idx] := 0;
  avail := 1;
  For idx := 1 to DIC_SIZE-2 do
    next^[idx] := SUCC(idx);
  next^[PRED(DIC_SIZE)] := 0;
  For idx := (2*DIC_SIZE) to MAX_HASH_VAL do
    next^[idx] := 0;
end;

function Child(pnode: Integer; chr: Byte): Integer;

var
  node: Integer;

begin
  node := next^[pnode+(chr SHL (DIC_BIT-9))+2*DIC_SIZE];
  parent^[0] := pnode;
  While (parent^[node] <> pnode) do
    node := next^[node];
  Child := node;
end;

procedure MakeChild(p_node: Integer;
                    chr: Byte;
                    c_node: Integer);
var
  idx,idx2: Integer;

begin
  idx := p_node+(chr SHL (DIC_BIT-9))+2*DIC_SIZE;
  idx2 := next^[idx];
  next^[idx] := c_node;
  next^[c_node] := idx2;
  previous^[idx2] := c_node;
  previous^[c_node] := idx;
  parent^[c_node] := p_node;
  Inc(child_count^[p_node]);
end;

procedure SplitTree(old: Integer);

var
  new,idx: Integer;

begin
  new := avail;
  avail := next^[new];
  child_count^[new] := 0;
  idx := previous^[old];
  previous^[new] := idx;
  next^[idx] := new;
  idx := next^[old];
  next^[new] := idx;
  previous^[idx] := new;
  parent^[new] := parent^[old];
  level^[new] := match_len;
  pos_ptr^[new] := pos;
  MakeChild(new,stream^[match_pos+match_len],old);
  MakeChild(new,stream^[pos+match_len],pos);
end;

procedure InsertNode;

var
  idx,idx2,idx3,idx4: Integer;
  chr: Byte;
  ptr1,ptr2: pCHAR;

begin
  If (match_len >= 4) then
    begin
      Dec(match_len);
      idx2 := SUCC(match_pos) OR DIC_SIZE;
      idx := parent^[idx2];
      While (idx = 0) do
        begin
          idx2 := next^[idx2];
          idx := parent^[idx2];
        end;
      While (level^[idx] >= match_len) do
        begin
          idx2 := idx;
          idx :=parent^[idx];
        end;
      idx4 := idx;
      While (pos_ptr^[idx4] < 0) do
        begin
          pos_ptr^[idx4] := pos;
          idx4 := parent^[idx4];
        end;
      If (idx4 < DIC_SIZE) then
        pos_ptr^[idx4] := pos OR PERC_FLAG;
    end
  else begin
         idx := stream^[pos]+DIC_SIZE;
         chr := stream^[SUCC(pos)];
         idx2 := Child(idx,chr);
         If (idx2 = 0) then
           begin
             MakeChild(idx,chr,pos);
             match_len := 1;
             EXIT; //InsertNode
           end;
         match_len := 2;
       end;

  Repeat
    If (idx2 >= DIC_SIZE) then
      begin
        idx3 := MAX_MATCH;
        match_pos := idx2;
      end
    else begin
           idx3 := level^[idx2];
           match_pos := pos_ptr^[idx2] AND NOT (1 SHL 15);
         end;
    If (match_pos >= pos) then
      Dec(match_pos,DIC_SIZE);
    ptr1 := Addr(stream^[pos+match_len]);
    ptr2 := Addr(stream^[match_pos+match_len]);
    While (match_len < idx3) do
      begin
        If (ptr1^ <> ptr2^) then
          begin
            SplitTree(idx2);
            EXIT; //InsertNode
          end;
        Inc(match_len);
        Inc(ptr1);
        Inc(ptr2);
      end;
    If (match_len >= MAX_MATCH) then
      BREAK;
    pos_ptr^[idx2] := pos;
    idx := idx2;
    idx2 := Child(idx,ORD(ptr1^));
    If (idx2 = 0) then
      begin
        MakeChild(idx,ORD(ptr1^),pos);
         EXIT; //InsertNode
      end;
    Inc(match_len);
  until FALSE;

  idx4 := previous^[idx2];
  previous^[pos] := idx4;
  next^[idx4] := pos;
  idx4 := next^[idx2];
  next^[pos] := idx4;
  previous^[idx4] := pos;
  parent^[pos] := idx;
  parent^[idx2] := 0;
  next^[idx2] := pos;

  //EXIT //InsertNode
end;

procedure DeleteNode;

var
  idx,idx2,idx3,idx4: Integer;
  perc_idx: Integer;

begin
  If (parent^[pos] = 0) then
    EXIT; //DeleteNode
  idx := previous^[pos];
  idx2 := next^[pos];
  next^[idx] := idx2;
  previous^[idx2] := idx;
  idx := parent^[pos];
  parent^[pos] := 0;

  Dec(child_count^[idx]);
  If (idx >= DIC_SIZE) or
     (child_count^[idx] > 1) then
    EXIT; //DeleteNode
  idx3 := pos_ptr^[idx] AND NOT PERC_FLAG;
  If (idx3 >= pos) then
    Dec(idx3,DIC_SIZE);

  idx2 := idx3;
  perc_idx := parent^[idx];
  idx4 := pos_ptr^[perc_idx];
  While (idx4 AND PERC_FLAG <> 0) do
    begin
      idx4 := idx4 AND NOT PERC_FLAG;
      If (idx4 >= pos) then
      Dec(idx4,DIC_SIZE);
      If (idx4 > idx2) then
        idx2 := idx4;
      pos_ptr^[perc_idx] := idx2 OR DIC_SIZE;
      perc_idx := parent^[perc_idx];
      idx4 := pos_ptr^[perc_idx];
    end;
  If (perc_idx < DIC_SIZE) then
    begin
      If (idx4 >= pos) then
        Dec(idx4,DIC_SIZE);
      If (idx4 > idx2) then
        idx2 := idx4;
      pos_ptr^[perc_idx] := idx2 OR DIC_SIZE OR PERC_FLAG;
    end;

  idx2 := Child(idx,stream^[idx3+level^[idx]]);
  idx3 := previous^[idx2];
  idx4 := next^[idx2];
  next^[idx3] := idx4;
  previous^[idx4] := idx3;
  idx3 := previous^[idx];
  next^[idx3] := idx2;
  previous^[idx2] := idx3;
  idx3 := next^[idx];
  previous^[idx3] := idx2;
  next^[idx2] := idx3;
  parent^[idx2] := parent^[idx];
  parent^[idx] := 0;
  next^[idx] := avail;
  avail := idx;

  //EXIT //DeleteNode
end;

procedure GetNextMatch;

var
  bits: Integer;

begin
  Dec(remain);
  Inc(pos);
  If (pos = 2*DIC_SIZE) then
    begin
      Move(stream^[DIC_SIZE],stream^[0],DIC_SIZE+MAX_MATCH);
      bits := ReadDataBlock(Addr(stream^[DIC_SIZE+MAX_MATCH]),DIC_SIZE);
      Inc(remain,bits);
      pos := DIC_SIZE;
    end;
  DeleteNode;
  InsertNode;
end;

function LZH_compress(var source,dest; size: Dword): Dword;

var
  last_match_len,last_match_pos: Integer;

begin
  LZH_compress := 0;
  input_buffer := Addr(source);
  input_buffer_idx := 0;
  input_buffer_size := size;
  output_buffer := Addr(dest);
  output_buffer_idx := 0;
  output_buffer^[input_buffer_idx] := 0; // set 'default' compression flag
  Inc(output_buffer_idx);
  Move(size,output_buffer^[output_buffer_idx],SizeOf(size));
  Inc(output_buffer_idx,SizeOf(size));
  progress_old_value := BYTE_NULL;
  progress_value := size;

  WIN_SIZE := WIN_SIZE_DEF;
  DIC_SIZE := DIC_SIZE_DEF;
  GetMem(stream,2*DIC_SIZE+MAX_MATCH);
  GetMem(level,DIC_SIZE+256);
  GetMem(child_count,DIC_SIZE+256);
  GetMem(pos_ptr,(DIC_SIZE+256) SHL 1);
  GetMem(parent,(DIC_SIZE*2) SHL 1);
  GetMem(previous,(DIC_SIZE*2) SHL 1);
  GetMem(next,(MAX_HASH_VAL+1) SHL 1);

  depth := 0;
  InitSlide;
  GetMem(buf,WIN_SIZE);
  buf^[0] := 0;
  FillChar(c_freq,SizeOf(c_freq),0);
  FillChar(p_freq,SizeOf(p_freq),0);
  out_pos := 0;
  out_mask := 0;
  bit_count := 8;
  sbit_buf := 0;
  remain := ReadDataBlock(Addr(stream^[DIC_SIZE]),DIC_SIZE+MAX_MATCH);
  match_len := 0;
  pos := DIC_SIZE;
  InsertNode;
  If (match_len > remain) then
    match_len := remain;

  While (remain > 0) do
    begin
      last_match_len := match_len;
      last_match_pos := match_pos;
      GetNextMatch;
      If (match_len > remain) then
        match_len := remain;
      If (match_len > last_match_len) or
         (last_match_len < THRESHOLD) then
        Output(stream^[PRED(pos)],0)
      else begin
             Output(last_match_len+(256-THRESHOLD),(pos-last_match_pos-2) AND PRED(DIC_SIZE));
             Dec(last_match_len);
             While (last_match_len > 0) do
               begin
                 GetNextMatch;
                 Dec(last_match_len);
               end;
             If (match_len > remain) then
               match_len := remain;
           end;
    end;

  SendBlock;
  PutBits(7,0);
  FreeMem(buf,WIN_SIZE);
  FreeMem(next,(MAX_HASH_VAL+1) SHL 1);
  FreeMem(previous,(DIC_SIZE*2) SHL 1);
  FreeMem(parent,(DIC_SIZE*2) SHL 1);
  FreeMem(pos_ptr,(DIC_SIZE+256) SHL 1);
  FreeMem(child_count,DIC_SIZE+256);
  FreeMem(level,DIC_SIZE+256);
  FreeMem(stream,2*DIC_SIZE+MAX_MATCH);
  LZH_compress := output_buffer_idx;
end;

function LZH_compress_ultra(var source,dest; size: Dword): Dword;

var
  last_match_len,last_match_pos: Integer;

begin
  LZH_compress_ultra := 0;
  input_buffer := Addr(source);
  input_buffer_idx := 0;
  input_buffer_size := size;
  output_buffer := Addr(dest);
  output_buffer_idx := 0;
  output_buffer^[input_buffer_idx] := 1; // set 'ultra' compression flag
  Inc(output_buffer_idx);
  Move(size,output_buffer^[output_buffer_idx],SizeOf(size));
  Inc(output_buffer_idx,SizeOf(size));
  progress_old_value := BYTE_NULL;
  progress_value := size;

  WIN_SIZE := WIN_SIZE_MAX;
  DIC_SIZE := DIC_SIZE_MAX;
  GetMem(stream,2*DIC_SIZE+MAX_MATCH);
  GetMem(level,DIC_SIZE+256);
  GetMem(child_count,DIC_SIZE+256);
  GetMem(pos_ptr,(DIC_SIZE+256) SHL 1);
  GetMem(parent,(DIC_SIZE*2) SHL 1);
  GetMem(previous,(DIC_SIZE*2) SHL 1);
  GetMem(next,(MAX_HASH_VAL+1) SHL 1);

  depth := 0;
  InitSlide;
  GetMem(buf,WIN_SIZE);
  buf^[0] := 0;
  FillChar(c_freq,SizeOf(c_freq),0);
  FillChar(p_freq,SizeOf(p_freq),0);
  out_pos := 0;
  out_mask := 0;
  bit_count := 8;
  sbit_buf := 0;
  remain := ReadDataBlock(Addr(stream^[DIC_SIZE]),DIC_SIZE+MAX_MATCH);
  match_len := 0;
  pos := DIC_SIZE;
  InsertNode;
  If (match_len > remain) then
    match_len := remain;

  While (remain > 0) do
    begin
      last_match_len := match_len;
      last_match_pos := match_pos;
      GetNextMatch;
      If (match_len > remain) then
        match_len := remain;
      If (match_len > last_match_len) or
         (last_match_len < THRESHOLD) then
        Output(stream^[PRED(pos)],0)
      else begin
             Output(last_match_len+(256-THRESHOLD),(pos-last_match_pos-2) AND PRED(DIC_SIZE));
             Dec(last_match_len);
             While (last_match_len > 0) do
               begin
                 GetNextMatch;
                 Dec(last_match_len);
               end;
             If (match_len > remain) then
               match_len := remain;
           end;
    end;

  SendBlock;
  PutBits(7,0);
  FreeMem(buf,WIN_SIZE);
  FreeMem(next,(MAX_HASH_VAL+1) SHL 1);
  FreeMem(previous,(DIC_SIZE*2) SHL 1);
  FreeMem(parent,(DIC_SIZE*2) SHL 1);
  FreeMem(pos_ptr,(DIC_SIZE+256) SHL 1);
  FreeMem(child_count,DIC_SIZE+256);
  FreeMem(level,DIC_SIZE+256);
  FreeMem(stream,2*DIC_SIZE+MAX_MATCH);
  LZH_compress_ultra := output_buffer_idx;
end;
