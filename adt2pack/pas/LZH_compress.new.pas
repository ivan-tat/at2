// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function ReadDataBlock (es: PLZHEncoderState; ptr: Pointer; size: Word): Word; cdecl;
{$IF 0}external;{$ELSE}public name PUBLIC_PREFIX + 'ReadDataBlock';

var
  result: Word;

begin
  If (es^.input_buffer_size-es^.input_buffer_idx >= size) then
    result := size
  else result := es^.input_buffer_size-es^.input_buffer_idx;
  Move(es^.input_buffer^[es^.input_buffer_idx],ptr^,result);
  Inc(es^.input_buffer_idx,result);
  ReadDataBlock := result;
{$IFNDEF ADT2PLAY}
  If NOT really_no_status_refresh then
    show_progress2(es^.input_buffer_idx,3);
{$ENDIF} // NOT DEFINED(ADT2PLAY)
end;{$ENDIF}

procedure PutBits (es: PLZHEncoderState; bits: Integer; xbits: Word); cdecl;
{$IF 0}external;{$ELSE}public name PUBLIC_PREFIX + 'PutBits';
begin
  If (bits < es^.bit_count) then
    begin
      Dec(es^.bit_count,bits);
      es^.sbit_buf := es^.sbit_buf OR (xbits SHL es^.bit_count);
    end
  else begin
         Dec(bits,es^.bit_count);
         es^.output_buffer^[es^.output_buffer_idx] := es^.sbit_buf OR (xbits SHR bits);
         Inc(es^.output_buffer_idx);
         If (bits < 8) then
           begin
             es^.bit_count := 8-bits;
             es^.sbit_buf := xbits SHL es^.bit_count;
           end
         else begin
                es^.output_buffer^[es^.output_buffer_idx] := xbits SHR (bits-8);
                Inc(es^.output_buffer_idx);
                es^.bit_count := 16-bits;
                es^.sbit_buf := xbits SHL es^.bit_count;
              end;
       end;
end;{$ENDIF}

procedure CountLen (es: PLZHEncoderState; idx: Integer); cdecl;
{$IF 0}external;{$ELSE}public name PUBLIC_PREFIX + 'CountLen';
begin
  If (idx < es^.bits) then
    If (es^.depth < 16) then
      Inc(es^.len_count[es^.depth])
    else Inc(es^.len_count[16])
  else begin
         Inc(es^.depth);
         CountLen (es, es^.l_tree[idx]);
         CountLen (es, es^.r_tree[idx]);
         Dec(es^.depth);
       end;
end;{$ENDIF}

procedure MakeLen (es: PLZHEncoderState; root: Integer); cdecl;
{$IF 0}external;{$ELSE}public name PUBLIC_PREFIX + 'MakeLen';
var
  idx,idx2: Integer;
  sum: Word;
begin
  FillChar(es^.len_count,SizeOf(es^.len_count),0);
  CountLen (es, root);
  sum := 0;
  For idx := 16 downto 1 do
    Inc(sum,es^.len_count[idx] SHL (16-idx));
  While (sum <> 0) do
    begin
      Dec(es^.len_count[16]);
      For idx := 15 downto 1 do
        If (es^.len_count[idx] <> 0) then
          begin
            Dec(es^.len_count[idx]);
            Inc(es^.len_count[SUCC(idx)],2);
            BREAK;
          end;
      Dec(sum);
    end;
  For idx := 16 downto 1 do
    begin
      idx2 := PRED(es^.len_count[idx]);
      While (idx2 >= 0) do
        begin
          Dec(idx2);
          es^.len^[es^.sort_ptr^[0]] := idx;
          es^.sort_ptr := Addr(es^.sort_ptr^[1]);
        end;
    end;
end;{$ENDIF}

procedure DownHeap (es: PLZHEncoderState; idx: Integer); cdecl;
{$IF 0}external;{$ELSE}public name PUBLIC_PREFIX + 'DownHeap';

var
  i,j: Integer;

begin
  i := es^.heap[idx];
  j := idx SHL 1;
  While (j <= es^.heap_size) do
    begin
      If (j < es^.heap_size) and
         (es^.freq^[es^.heap[j]] > es^.freq^[es^.heap[SUCC(j)]]) then
        Inc(j);
      If (es^.freq^[i] <= es^.freq^[es^.heap[j]]) then
        BREAK;
      es^.heap[idx] := es^.heap[j];
      idx := j;
      j := idx SHL 1;
    end;
  es^.heap[idx] := i;
end;{$ENDIF}

procedure MakeCode (es: PLZHEncoderState; bits: Integer; len: pBYTE; code: pWORD); cdecl;
{$IF 0}external;{$ELSE}public name PUBLIC_PREFIX + 'MakeCode';

var
  idx,idx2: Integer;
  start: array[0..17] of Word;

begin
  start[1] := 0;
  For idx := 1 to 16 do
    start[SUCC(idx)] := (start[idx]+es^.len_count[idx]) SHL 1;
  For idx := 0 to PRED(bits) do
    begin
      idx2 := len^[idx];
      code^[idx] := start[idx2];
      Inc(start[idx2]);
    end;
end;{$ENDIF}

function MakeTree (es: PLZHEncoderState;
                   n_par: Integer;
                   f_par: pWORD;
                   l_par: pBYTE;
                   c_par: pWORD): Integer; cdecl;
{$IF 0}external;{$ELSE}public name PUBLIC_PREFIX + 'MakeTree';
var
  i,result,j,avail: Integer;

begin
  es^.bits := n_par;
  es^.freq := f_par;
  es^.len := l_par;
  avail := es^.bits;
  es^.heap_size := 0;
  es^.heap[1] := 0;

  For i := 0 to PRED(es^.bits) do
    begin
      es^.len^[i] := 0;
      If (es^.freq^[i] <> 0) then
        begin
          Inc(es^.heap_size);
          es^.heap[es^.heap_size] := i;
        end;
    end;

  If (es^.heap_size < 2) then
    begin
      c_par^[es^.heap[1]] := 0;
      MakeTree := es^.heap[1];
      EXIT; //MakeTree
    end;

  For i := (es^.heap_size DIV 2) downto 1 do
    DownHeap (es, i);
  es^.sort_ptr := c_par;
  Repeat
    i := es^.heap[1];
    If (i < es^.bits) then
      begin
        es^.sort_ptr^[0] := i;
        es^.sort_ptr := Addr(es^.sort_ptr^[1]);
      end;
    es^.heap[1] := es^.heap[es^.heap_size];
    Dec(es^.heap_size);
    DownHeap (es, 1);
    j := es^.heap[1];
    If (j < es^.bits) then
      begin
        es^.sort_ptr^[0] := j;
        es^.sort_ptr := Addr(es^.sort_ptr^[1]);
      end;
    result := avail;
    Inc(avail);
    es^.freq^[result] := es^.freq^[i]+es^.freq^[j];
    es^.heap[1] := result;
    DownHeap (es, 1);
    es^.l_tree[result] := i;
    es^.r_tree[result] := j;
  until (es^.heap_size <= 1);

  es^.sort_ptr := c_par;
  MakeLen (es, result);
  MakeCode (es, n_par,l_par,c_par);
  MakeTree := result;

  //EXIT //MakeTree
end;{$ENDIF}

procedure CountFreq (es: PLZHEncoderState); cdecl;
{$IF 0}external;{$ELSE}public name PUBLIC_PREFIX + 'CountFreq';

var
  i,j,bits,count: Integer;

begin
  For i := 0 to PRED(NT) do
    es^.t_freq[i] := 0;
  bits := NC;
  While (bits > 0) and
        (es^.c_len[PRED(bits)] = 0) do
    Dec(bits);
  i := 0;
  While (i < bits) do
    begin
      j := es^.c_len[i];
      Inc(i);
      If (j = 0) then
        begin
          count := 1;
          While (i < bits) and
                (es^.c_len[i] = 0) do
            begin
              Inc(i);
              Inc(count);
            end;
          If (count <= 2) then
            Inc(es^.t_freq[0],count)
          else If (count <= 18) then
                 Inc(es^.t_freq[1])
                else If (count = 19) then
                       begin
                         Inc(es^.t_freq[0]);
                         Inc(es^.t_freq[1]);
                       end
                     else Inc(es^.t_freq[2]);
        end
      else Inc(es^.t_freq[j+2]);
    end;
end;{$ENDIF}

procedure WritePtrLen (es: PLZHEncoderState; bits,n_bit,s_bit: Integer); cdecl;
{$IF 0}external;{$ELSE}public name PUBLIC_PREFIX + 'WritePtrLen';

var
  i,j: Integer;

begin
  While (bits > 0) and
        (es^.p_len[PRED(bits)] = 0) do
    Dec(bits);
  PutBits (es, n_bit,bits);
  i := 0;
  While (i < bits) do
    begin
      j := es^.p_len[i];
      Inc(i);
      If (j <= 6) then
        PutBits (es, 3,j)
      else begin
             Dec(j,3);
             PutBits (es, j,(1 SHL j)-2);
           end;
      If (i = s_bit) then
        begin
          While (i < 6) and
                (es^.p_len[i] = 0) do
            Inc(i);
          PutBits (es, 2,(i-3) AND 3);
        end;
    end;
end;{$ENDIF}

procedure WriteCharLen (es: PLZHEncoderState); cdecl;
{$IF 0}external;{$ELSE}public name PUBLIC_PREFIX + 'WriteCharLen';

var
  i,j,bits,count: Integer;

begin
  bits := NC;
  While (bits > 0) and
        (es^.c_len[PRED(bits)] = 0) do
    Dec(bits);
  PutBits (es, CBIT,bits);
  i := 0;
  While (i < bits) do
    begin
      j := es^.c_len[i];
      Inc(i);
      If (j = 0) then
        begin
          count := 1;
          While (i < bits) and
                (es^.c_len[i] = 0) do
            begin
              Inc(i);
              Inc(count);
            end;
          If (count <= 2) then
            For j := 0 to PRED(count) do
              PutBits (es, es^.p_len[0],es^.p_code[0])
          else If (count <= 18) then
                 begin
                   PutBits (es, es^.p_len[1],es^.p_code[1]);
                   PutBits (es, 4,Count-3);
                 end
               else If (count = 19) then
                      begin
                        PutBits (es, es^.p_len[0],es^.p_code[0]);
                        PutBits (es, es^.p_len[1],es^.p_code[1]);
                        PutBits (es, 4,15);
                      end
                    else begin
                           PutBits (es, es^.p_len[2],es^.p_code[2]);
                           PutBits (es, CBIT,count-20);
                         end;
        end
      else PutBits (es, es^.p_len[j+2],es^.p_code[j+2]);
    end;
end;{$ENDIF}

procedure EncodePtr (es: PLZHEncoderState; ptr: Word); cdecl;
{$IF 0}external;{$ELSE}public name PUBLIC_PREFIX + 'EncodePtr';

var
  i,x: Word;

begin
  i := 0;
  x := ptr;
  While (x <> 0) do
    begin
      x := x SHR 1;
      Inc(i);
    end;
  PutBits (es, es^.p_len[i],es^.p_code[i]);
  If (i > 1) then
    PutBits (es, PRED(i),ptr AND (PRED(1 SHL 16) SHR (17-i)));
end;{$ENDIF}

procedure SendBlock (es: PLZHEncoderState); cdecl;
{$IF 0}external;{$ELSE}public name PUBLIC_PREFIX + 'SendBlock';

var
  idx,idx2,flags,
  root,pos,size: Word;

begin
  root := MakeTree (es, NC, pWORD (@(es^.c_freq)), pBYTE (@(es^.c_len)), pWORD (@(es^.c_code)));
  size := es^.c_freq[root];
  PutBits (es, 16, size);
  If (root >= NC) then
  begin
    CountFreq (es);
    root := MakeTree (es, NT, pWORD (@(es^.t_freq)), pBYTE (@(es^.p_len)), pWORD (@(es^.p_code)));
    If (root >= NT) then
      WritePtrLen (es, NT, TBIT, 3)
    else
    begin
      PutBits (es, TBIT, 0);
      PutBits (es, TBIT, root);
    end;
    WriteCharLen (es);
  end
  else
  begin
    PutBits (es, TBIT,0);
    PutBits (es, TBIT,0);
    PutBits (es, CBIT,0);
    PutBits (es, CBIT,root);
  end;
  root := MakeTree (es, NP, pWORD (@(es^.p_freq)), pBYTE (@(es^.p_len)), pWORD (@(es^.p_code)));
  If (root >= NP) then
    WritePtrLen (es, NP, PBIT, -1)
  else
  begin
    PutBits (es, PBIT,0);
    PutBits (es, PBIT,root);
  end;
  pos := 0;
  For idx := 0 to PRED(size) do
  begin
    If (idx AND 7 = 0) then
    begin
      flags := es^.buf^[pos];
      Inc(pos);
    end
    else
      flags := flags SHL 1;
    If (flags AND (1 SHL 7) <> 0) then
    begin
      idx2 := es^.buf^[pos] + (1 SHL 8);
      Inc(pos);
      PutBits (es, es^.c_len[idx2], es^.c_code[idx2]);
      idx2 := (es^.buf^[pos] SHL 8) + es^.buf^[pos + 1];
      Inc(pos, 2);
      EncodePtr (es, idx2);
    end
    else
    begin
      idx2 := es^.buf^[pos];
      Inc(pos);
      PutBits (es, es^.c_len[idx2], es^.c_code[idx2]);
    end;
  end;
  For idx := 0 to PRED(NC) do es^.c_freq[idx] := 0;
  For idx := 0 to PRED(NP) do es^.p_freq[idx] := 0;
end;{$ENDIF}

procedure Output (es: PLZHEncoderState; code,c_ptr: Word); cdecl;
{$IF 0}external;{$ELSE}public name PUBLIC_PREFIX + 'Output';
begin
  es^.out_mask := es^.out_mask SHR 1;
  If (es^.out_mask = 0) then
    begin
      es^.out_mask := 1 SHL 7;
      If (es^.out_pos >= es^.WIN_SIZE-24) then
        begin
          SendBlock (es);
          es^.out_pos := 0;
        end;
      es^.c_pos := es^.out_pos;
      Inc(es^.out_pos);
      es^.buf^[es^.c_pos] := 0;
    end;
  es^.buf^[es^.out_pos] := code;
  Inc(es^.out_pos);
  Inc(es^.c_freq[code]);
  If (code >= 1 SHL 8) then
    begin
      es^.buf^[es^.c_pos] := es^.buf^[es^.c_pos] OR es^.out_mask;
      es^.buf^[es^.out_pos] := c_ptr SHR 8;
      Inc(es^.out_pos);
      es^.buf^[es^.out_pos] := c_ptr;
      Inc(es^.out_pos);
      code := 0;
      While (c_ptr <> 0) do
        begin
          c_ptr := c_ptr SHR 1;
          Inc(code);
        end;
      Inc(es^.p_freq[code]);
    end;
end;{$ENDIF}

procedure InitSlide (es: PLZHEncoderState); cdecl;
{$IF 0}external;{$ELSE}public name PUBLIC_PREFIX + 'InitSlide';

var
  idx: Word;

begin
  For idx := es^.DIC_SIZE to (es^.DIC_SIZE+255) do
    begin
      es^.level^[idx] := 1;
      es^.pos_ptr^[idx] := 0;
    end;
  For idx := es^.DIC_SIZE to PRED(2*es^.DIC_SIZE) do
    es^.parent^[idx] := 0;
  es^.avail := 1;
  For idx := 1 to es^.DIC_SIZE-2 do
    es^.next^[idx] := SUCC(idx);
  es^.next^[PRED(es^.DIC_SIZE)] := 0;
  For idx := (2*es^.DIC_SIZE) to MAX_HASH_VAL do
    es^.next^[idx] := 0;
end;{$ENDIF}

function Child (es: PLZHEncoderState; pnode: Integer; chr: Byte): Integer; cdecl;
{$IF 0}external;{$ELSE}public name PUBLIC_PREFIX + 'Child';

var
  node: Integer;

begin
  node := es^.next^[pnode+(chr SHL (DIC_BIT-9))+2*es^.DIC_SIZE];
  es^.parent^[0] := pnode;
  While (es^.parent^[node] <> pnode) do
    node := es^.next^[node];
  Child := node;
end;{$ENDIF}

procedure MakeChild (es: PLZHEncoderState;
                     p_node: Integer;
                     chr: Byte;
                     c_node: Integer); cdecl;
{$IF 0}external;{$ELSE}public name PUBLIC_PREFIX + 'MakeChild';

var
  idx,idx2: Integer;

begin
  idx := p_node+(chr SHL (DIC_BIT-9))+2*es^.DIC_SIZE;
  idx2 := es^.next^[idx];
  es^.next^[idx] := c_node;
  es^.next^[c_node] := idx2;
  es^.previous^[idx2] := c_node;
  es^.previous^[c_node] := idx;
  es^.parent^[c_node] := p_node;
  Inc(es^.child_count^[p_node]);
end;{$ENDIF}

procedure SplitTree (es: PLZHEncoderState; old: Integer); cdecl;
{$IF 0}external;{$ELSE}public name PUBLIC_PREFIX + 'SplitTree';

var
  new,idx: Integer;

begin
  new := es^.avail;
  es^.avail := es^.next^[new];
  es^.child_count^[new] := 0;
  idx := es^.previous^[old];
  es^.previous^[new] := idx;
  es^.next^[idx] := new;
  idx := es^.next^[old];
  es^.next^[new] := idx;
  es^.previous^[idx] := new;
  es^.parent^[new] := es^.parent^[old];
  es^.level^[new] := es^.match_len;
  es^.pos_ptr^[new] := es^.pos;
  MakeChild (es, new,es^.stream^[es^.match_pos+es^.match_len],old);
  MakeChild (es, new,es^.stream^[es^.pos+es^.match_len],es^.pos);
end;{$ENDIF}

procedure InsertNode (es: PLZHEncoderState); cdecl;
{$IF 0}external;{$ELSE}public name PUBLIC_PREFIX + 'InsertNode';

var
  idx,idx2,idx3,idx4: Integer;
  chr: Byte;
  ptr1,ptr2: pCHAR;

begin
  If (es^.match_len >= 4) then
    begin
      Dec(es^.match_len);
      idx2 := SUCC(es^.match_pos) OR es^.DIC_SIZE;
      idx := es^.parent^[idx2];
      While (idx = 0) do
        begin
          idx2 := es^.next^[idx2];
          idx := es^.parent^[idx2];
        end;
      While (es^.level^[idx] >= es^.match_len) do
        begin
          idx2 := idx;
          idx :=es^.parent^[idx];
        end;
      idx4 := idx;
      While (es^.pos_ptr^[idx4] < 0) do
        begin
          es^.pos_ptr^[idx4] := es^.pos;
          idx4 := es^.parent^[idx4];
        end;
      If (idx4 < es^.DIC_SIZE) then
        es^.pos_ptr^[idx4] := es^.pos OR PERC_FLAG;
    end
  else begin
         idx := es^.stream^[es^.pos]+es^.DIC_SIZE;
         chr := es^.stream^[SUCC(es^.pos)];
         idx2 := Child (es, idx,chr);
         If (idx2 = 0) then
           begin
             MakeChild (es, idx,chr,es^.pos);
             es^.match_len := 1;
             EXIT; //InsertNode
           end;
         es^.match_len := 2;
       end;

  Repeat
    If (idx2 >= es^.DIC_SIZE) then
      begin
        idx3 := MAX_MATCH;
        es^.match_pos := idx2;
      end
    else begin
           idx3 := es^.level^[idx2];
           es^.match_pos := es^.pos_ptr^[idx2] AND NOT (1 SHL 15);
         end;
    If (es^.match_pos >= es^.pos) then
      Dec(es^.match_pos,es^.DIC_SIZE);
    ptr1 := Addr(es^.stream^[es^.pos+es^.match_len]);
    ptr2 := Addr(es^.stream^[es^.match_pos+es^.match_len]);
    While (es^.match_len < idx3) do
      begin
        If (ptr1^ <> ptr2^) then
          begin
            SplitTree (es, idx2);
            EXIT; //InsertNode
          end;
        Inc(es^.match_len);
        Inc(ptr1);
        Inc(ptr2);
      end;
    If (es^.match_len >= MAX_MATCH) then
      BREAK;
    es^.pos_ptr^[idx2] := es^.pos;
    idx := idx2;
    idx2 := Child (es, idx,ORD(ptr1^));
    If (idx2 = 0) then
      begin
        MakeChild (es, idx,ORD(ptr1^),es^.pos);
         EXIT; //InsertNode
      end;
    Inc(es^.match_len);
  until FALSE;

  idx4 := es^.previous^[idx2];
  es^.previous^[es^.pos] := idx4;
  es^.next^[idx4] := es^.pos;
  idx4 := es^.next^[idx2];
  es^.next^[es^.pos] := idx4;
  es^.previous^[idx4] := es^.pos;
  es^.parent^[es^.pos] := idx;
  es^.parent^[idx2] := 0;
  es^.next^[idx2] := es^.pos;

  //EXIT //InsertNode
end;{$ENDIF}

procedure DeleteNode (es: PLZHEncoderState); cdecl;
{$IF 0}external;{$ELSE}public name PUBLIC_PREFIX + 'DeleteNode';

var
  idx,idx2,idx3,idx4: Integer;
  perc_idx: Integer;

begin
  If (es^.parent^[es^.pos] = 0) then
    EXIT; //DeleteNode
  idx := es^.previous^[es^.pos];
  idx2 := es^.next^[es^.pos];
  es^.next^[idx] := idx2;
  es^.previous^[idx2] := idx;
  idx := es^.parent^[es^.pos];
  es^.parent^[es^.pos] := 0;

  Dec(es^.child_count^[idx]);
  If (idx >= es^.DIC_SIZE) or
     (es^.child_count^[idx] > 1) then
    EXIT; //DeleteNode
  idx3 := es^.pos_ptr^[idx] AND NOT PERC_FLAG;
  If (idx3 >= es^.pos) then
    Dec(idx3,es^.DIC_SIZE);

  idx2 := idx3;
  perc_idx := es^.parent^[idx];
  idx4 := es^.pos_ptr^[perc_idx];
  While (idx4 AND PERC_FLAG <> 0) do
    begin
      idx4 := idx4 AND NOT PERC_FLAG;
      If (idx4 >= es^.pos) then
      Dec(idx4,es^.DIC_SIZE);
      If (idx4 > idx2) then
        idx2 := idx4;
      es^.pos_ptr^[perc_idx] := idx2 OR es^.DIC_SIZE;
      perc_idx := es^.parent^[perc_idx];
      idx4 := es^.pos_ptr^[perc_idx];
    end;
  If (perc_idx < es^.DIC_SIZE) then
    begin
      If (idx4 >= es^.pos) then
        Dec(idx4,es^.DIC_SIZE);
      If (idx4 > idx2) then
        idx2 := idx4;
      es^.pos_ptr^[perc_idx] := idx2 OR es^.DIC_SIZE OR PERC_FLAG;
    end;

  idx2 := Child (es, idx,es^.stream^[idx3+es^.level^[idx]]);
  idx3 := es^.previous^[idx2];
  idx4 := es^.next^[idx2];
  es^.next^[idx3] := idx4;
  es^.previous^[idx4] := idx3;
  idx3 := es^.previous^[idx];
  es^.next^[idx3] := idx2;
  es^.previous^[idx2] := idx3;
  idx3 := es^.next^[idx];
  es^.previous^[idx3] := idx2;
  es^.next^[idx2] := idx3;
  es^.parent^[idx2] := es^.parent^[idx];
  es^.parent^[idx] := 0;
  es^.next^[idx] := es^.avail;
  es^.avail := idx;

  //EXIT //DeleteNode
end;{$ENDIF}

procedure GetNextMatch (es: PLZHEncoderState); cdecl;
{$IF 0}external;{$ELSE}public name PUBLIC_PREFIX + 'GetNextMatch';

var
  bits: Integer;

begin
  Dec(es^.remain);
  Inc(es^.pos);
  If (es^.pos = 2*es^.DIC_SIZE) then
    begin
      Move(es^.stream^[es^.DIC_SIZE],es^.stream^[0],es^.DIC_SIZE+MAX_MATCH);
      bits := ReadDataBlock (es, Addr(es^.stream^[es^.DIC_SIZE+MAX_MATCH]),es^.DIC_SIZE);
      Inc(es^.remain,bits);
      es^.pos := es^.DIC_SIZE;
    end;
  DeleteNode (es);
  InsertNode (es);
end;{$ENDIF}

// ultra: compression flag (FALSE=default, TRUE=ultra)
function LZH_do_compress (var source,dest; size: Dword; ultra: boolean): Dword; cdecl;
{$IF 1}external;{$ELSE}public name PUBLIC_PREFIX + 'LZH_do_compress';
var
  result: Dword;
  es: PLZHEncoderState;
  last_match_len,last_match_pos: Integer;

label
  _exit;

begin
  result := 0;
  GetMem (es, sizeof (TLZHEncoderState)); if (es = NIL) then goto _exit;
  es^.input_buffer := Addr(source);
  es^.input_buffer_idx := 0;
  es^.input_buffer_size := size;
  es^.output_buffer := Addr(dest);
  es^.output_buffer_idx := 0;
  if (ultra) then
    es^.output_buffer^[es^.output_buffer_idx] := 1
  else
    es^.output_buffer^[es^.output_buffer_idx] := 0;
  Inc(es^.output_buffer_idx);
  Move(size,es^.output_buffer^[es^.output_buffer_idx],SizeOf(size));
  Inc(es^.output_buffer_idx,SizeOf(size));
  progress_old_value := BYTE_NULL;
  progress_value := size;
  if (ultra = 0) then
  begin
    es^.WIN_SIZE := WIN_SIZE_DEF;
    es^.DIC_SIZE := DIC_SIZE_DEF;
  end else begin
    es^.WIN_SIZE := WIN_SIZE_MAX;
    es^.DIC_SIZE := DIC_SIZE_MAX;
  end;
  GetMem (es^.stream, 2*es^.DIC_SIZE+MAX_MATCH); if (es^.stream = NIL) then goto _exit;
  GetMem (es^.level, es^.DIC_SIZE+256); if (es^.level = NIL) then goto _exit;
  GetMem (es^.child_count, es^.DIC_SIZE+256); if (es^.child_count = NIL) then goto _exit;
  GetMem (es^.pos_ptr, (es^.DIC_SIZE+256) SHL 1); if (es^.pos_ptr = NIL) then goto _exit;
  GetMem (es^.parent, (es^.DIC_SIZE*2) SHL 1); if (es^.parent = NIL) then goto _exit;
  GetMem (es^.previous, (es^.DIC_SIZE*2) SHL 1); if (es^.previous = NIL) then goto _exit;
  GetMem (es^.next, (MAX_HASH_VAL+1) SHL 1); if (es^.next = NIL) then goto _exit;

  es^.depth := 0;
  InitSlide (es);
  GetMem (es^.buf,es^.WIN_SIZE); if (es^.buf = NIL) then goto _exit;
  es^.buf^[0] := 0;
  FillChar (es^.c_freq, SizeOf(es^.c_freq),0);
  FillChar (es^.p_freq, SizeOf(es^.p_freq),0);
  es^.out_pos := 0;
  es^.out_mask := 0;
  es^.bit_count := 8;
  es^.sbit_buf := 0;
  es^.remain := ReadDataBlock (es, Addr(es^.stream^[es^.DIC_SIZE]),es^.DIC_SIZE+MAX_MATCH);
  es^.match_len := 0;
  es^.pos := es^.DIC_SIZE;
  InsertNode (es);
  If (es^.match_len > es^.remain) then
    es^.match_len := es^.remain;

  While (es^.remain > 0) do
    begin
      last_match_len := es^.match_len;
      last_match_pos := es^.match_pos;
      GetNextMatch (es);
      If (es^.match_len > es^.remain) then
        es^.match_len := es^.remain;
      If (es^.match_len > last_match_len) or
         (last_match_len < THRESHOLD) then
        Output (es, es^.stream^[PRED(es^.pos)],0)
      else begin
             Output (es, last_match_len+(256-THRESHOLD),(es^.pos-last_match_pos-2) AND PRED(es^.DIC_SIZE));
             Dec(last_match_len);
             While (last_match_len > 0) do
               begin
                 GetNextMatch (es);
                 Dec(last_match_len);
               end;
             If (es^.match_len > es^.remain) then
               es^.match_len := es^.remain;
           end;
    end;

  SendBlock (es);
  PutBits (es, 7, 0);
  result := es^.output_buffer_idx;
_exit:
  if (es <> NIL) then
  begin
    if (es^.pos_ptr <> NIL) then FreeMem (es^.pos_ptr, (es^.DIC_SIZE+256) SHL 1);
    if (es^.buf <> NIL) then FreeMem (es^.buf, es^.WIN_SIZE);
    if (es^.stream <> NIL) then FreeMem (es^.stream, 2*es^.DIC_SIZE+MAX_MATCH);
    if (es^.child_count <> NIL) then FreeMem (es^.child_count, es^.DIC_SIZE+256);
    if (es^.level <> NIL) then FreeMem (es^.level, es^.DIC_SIZE+256);
    if (es^.parent <> NIL) then FreeMem (es^.parent, (es^.DIC_SIZE*2) SHL 1);
    if (es^.previous <> NIL) then FreeMem (es^.previous, (es^.DIC_SIZE*2) SHL 1);
    if (es^.next <> NIL) then FreeMem (es^.next, (MAX_HASH_VAL+1) SHL 1);
    FreeMem (es, sizeof (TLZHEncoderState));
  end;
  LZH_do_compress := result;
end;{$ENDIF}

function LZH_compress (var source, dest; size: Dword): Dword; cdecl;
{$IF 1}external;{$ELSE}public name PUBLIC_PREFIX + 'LZH_compress';
begin
  LZH_compress := LZH_do_compress (source, dest, size, false);
end;{$ENDIF}

function LZH_compress_ultra (var source, dest; size: Dword): Dword; cdecl;
{$IF 1}external;{$ELSE}public name PUBLIC_PREFIX + 'LZH_compress_ultra';
begin
  LZH_compress_ultra := LZH_do_compress (source, dest, size, true);
end;{$ENDIF}
