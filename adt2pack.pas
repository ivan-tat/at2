//  This file is part of Adlib Tracker II (AT2).
//
//  AT2 is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  AT2 is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with AT2.  If not, see <http://www.gnu.org/licenses/>.

unit AdT2pack;
{$S-,Q-,R-,V-,B-,X+}
{$PACKRECORDS 1}
{$MODESWITCH CVAR}
{$L adt2pack.o}
interface

function LZH_compress(var source,dest; size: Dword): Dword; cdecl;
function LZH_compress_ultra(var source,dest; size: Dword): Dword; cdecl;
function LZH_decompress(var source,dest; size: Dword): Dword; cdecl; external;

implementation

uses
  pascal,
  AdT2sys,
  AdT2extn,
  AdT2unit;

{$I adt2pack/pas/LZH_defs.new.inc}
{$I adt2pack/pas/LZH_compress.new.pas}

// TLZHEncoderState

var
  struct_TLZHEncoderState: packed record
    WIN_SIZE,
    DIC_SIZE,
    l_tree, r_tree,
    p_len,
    c_len,
    heap,
    len_count,
    c_freq,
    p_freq,
    t_freq,
    c_code,
    p_code,
    freq,sort_ptr,pos_ptr,
    buf,len,stream,child_count,level,
    parent,previous,next,
    bits,heap_size,remain,
    match_len,
    sbit_buf,bit_count,
    depth,c_pos,pos,out_pos,match_pos,
    out_mask,avail,
    input_buffer,output_buffer,
    input_buffer_idx,output_buffer_idx,
    input_buffer_size: LongWord;
  end; cvar; external;
var
  struct_TLZHEncoderState_size: LongWord; cvar; external;

procedure _check_struct_TLZHEncoderState;
const s: PChar = 'TLZHEncoderState';
begin
  _check_struct_memb (s, 'WIN_SIZE', struct_TLZHEncoderState.WIN_SIZE, SizeUInt (@(TLZHEncoderState (NIL^).WIN_SIZE)));
  _check_struct_memb (s, 'DIC_SIZE', struct_TLZHEncoderState.DIC_SIZE, SizeUInt (@(TLZHEncoderState (NIL^).DIC_SIZE)));
  _check_struct_memb (s, 'l_tree', struct_TLZHEncoderState.l_tree, SizeUInt (@(TLZHEncoderState (NIL^).l_tree[0])));
  _check_struct_memb (s, 'r_tree', struct_TLZHEncoderState.r_tree, SizeUInt (@(TLZHEncoderState (NIL^).r_tree[0])));
  _check_struct_memb (s, 'p_len', struct_TLZHEncoderState.p_len, SizeUInt (@(TLZHEncoderState (NIL^).p_len[0])));
  _check_struct_memb (s, 'c_len', struct_TLZHEncoderState.c_len, SizeUInt (@(TLZHEncoderState (NIL^).c_len[0])));
  _check_struct_memb (s, 'heap', struct_TLZHEncoderState.heap, SizeUInt (@(TLZHEncoderState (NIL^).heap[0])));
  _check_struct_memb (s, 'len_count', struct_TLZHEncoderState.len_count, SizeUInt (@(TLZHEncoderState (NIL^).len_count[0])));
  _check_struct_memb (s, 'c_freq', struct_TLZHEncoderState.c_freq, SizeUInt (@(TLZHEncoderState (NIL^).c_freq[0])));
  _check_struct_memb (s, 'p_freq', struct_TLZHEncoderState.p_freq, SizeUInt (@(TLZHEncoderState (NIL^).p_freq[0])));
  _check_struct_memb (s, 't_freq', struct_TLZHEncoderState.t_freq, SizeUInt (@(TLZHEncoderState (NIL^).t_freq[0])));
  _check_struct_memb (s, 'c_code', struct_TLZHEncoderState.c_code, SizeUInt (@(TLZHEncoderState (NIL^).c_code[0])));
  _check_struct_memb (s, 'p_code', struct_TLZHEncoderState.p_code, SizeUInt (@(TLZHEncoderState (NIL^).p_code[0])));
  _check_struct_memb (s, 'freq', struct_TLZHEncoderState.freq, SizeUInt (@(TLZHEncoderState (NIL^).freq)));
  _check_struct_memb (s, 'sort_ptr', struct_TLZHEncoderState.sort_ptr, SizeUInt (@(TLZHEncoderState (NIL^).sort_ptr)));
  _check_struct_memb (s, 'pos_ptr', struct_TLZHEncoderState.pos_ptr, SizeUInt (@(TLZHEncoderState (NIL^).pos_ptr)));
  _check_struct_memb (s, 'buf', struct_TLZHEncoderState.buf, SizeUInt (@(TLZHEncoderState (NIL^).buf)));
  _check_struct_memb (s, 'len', struct_TLZHEncoderState.len, SizeUInt (@(TLZHEncoderState (NIL^).len)));
  _check_struct_memb (s, 'stream', struct_TLZHEncoderState.stream, SizeUInt (@(TLZHEncoderState (NIL^).stream)));
  _check_struct_memb (s, 'child_count', struct_TLZHEncoderState.child_count, SizeUInt (@(TLZHEncoderState (NIL^).child_count)));
  _check_struct_memb (s, 'level', struct_TLZHEncoderState.level, SizeUInt (@(TLZHEncoderState (NIL^).level)));
  _check_struct_memb (s, 'parent', struct_TLZHEncoderState.parent, SizeUInt (@(TLZHEncoderState (NIL^).parent)));
  _check_struct_memb (s, 'previous', struct_TLZHEncoderState.previous, SizeUInt (@(TLZHEncoderState (NIL^).previous)));
  _check_struct_memb (s, 'next', struct_TLZHEncoderState.next, SizeUInt (@(TLZHEncoderState (NIL^).next)));
  _check_struct_memb (s, 'bits', struct_TLZHEncoderState.bits, SizeUInt (@(TLZHEncoderState (NIL^).bits)));
  _check_struct_memb (s, 'heap_size', struct_TLZHEncoderState.heap_size, SizeUInt (@(TLZHEncoderState (NIL^).heap_size)));
  _check_struct_memb (s, 'remain', struct_TLZHEncoderState.remain, SizeUInt (@(TLZHEncoderState (NIL^).remain)));
  _check_struct_memb (s, 'match_len', struct_TLZHEncoderState.match_len, SizeUInt (@(TLZHEncoderState (NIL^).match_len)));
  _check_struct_memb (s, 'sbit_buf', struct_TLZHEncoderState.bit_count, SizeUInt (@(TLZHEncoderState (NIL^).bit_count)));
  _check_struct_memb (s, 'depth', struct_TLZHEncoderState.depth, SizeUInt (@(TLZHEncoderState (NIL^).depth)));
  _check_struct_memb (s, 'c_pos', struct_TLZHEncoderState.c_pos, SizeUInt (@(TLZHEncoderState (NIL^).c_pos)));
  _check_struct_memb (s, 'pos', struct_TLZHEncoderState.pos, SizeUInt (@(TLZHEncoderState (NIL^).pos)));
  _check_struct_memb (s, 'out_pos', struct_TLZHEncoderState.out_pos, SizeUInt (@(TLZHEncoderState (NIL^).out_pos)));
  _check_struct_memb (s, 'match_pos', struct_TLZHEncoderState.match_pos, SizeUInt (@(TLZHEncoderState (NIL^).match_pos)));
  _check_struct_memb (s, 'out_mask', struct_TLZHEncoderState.out_mask, SizeUInt (@(TLZHEncoderState (NIL^).out_mask)));
  _check_struct_memb (s, 'avail', struct_TLZHEncoderState.avail, SizeUInt (@(TLZHEncoderState (NIL^).avail)));
  _check_struct_memb (s, 'input_buffer', struct_TLZHEncoderState.input_buffer, SizeUInt (@(TLZHEncoderState (NIL^).input_buffer)));
  _check_struct_memb (s, 'output_buffer', struct_TLZHEncoderState.output_buffer, SizeUInt (@(TLZHEncoderState (NIL^).output_buffer)));
  _check_struct_memb (s, 'input_buffer_idx', struct_TLZHEncoderState.input_buffer_idx, SizeUInt (@(TLZHEncoderState (NIL^).input_buffer_idx)));
  _check_struct_memb (s, 'output_buffer_idx', struct_TLZHEncoderState.output_buffer_idx, SizeUInt (@(TLZHEncoderState (NIL^).output_buffer_idx)));
  _check_struct_memb (s, 'input_buffer_size', struct_TLZHEncoderState.input_buffer_size, SizeUInt (@(TLZHEncoderState (NIL^).input_buffer_size)));
  _check_struct_size (s, struct_TLZHEncoderState_size, SizeOf (TLZHEncoderState));
end;

begin
  _check_struct_TLZHEncoderState;
end.
