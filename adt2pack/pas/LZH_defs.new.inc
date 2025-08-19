// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

const
  { DEFAULT COMPRESSION: }
  WIN_SIZE_DEF = 1 SHL 12; { buffer 4k }
  DIC_SIZE_DEF = 1 SHL 13; { dictionary 8kb }
  { ULTRA COMPRESSION: }
  WIN_SIZE_MAX = 1 SHL 15; { buffer 32k }
  DIC_SIZE_MAX = 1 SHL 14; { dictionary 16kb }

const
  MATCH_BIT = 8;
  MAX_MATCH = 1 SHL MATCH_BIT;
  THRESHOLD = 2;
  PBIT = 14;
  TBIT = 15;
  CBIT = 16;
  DIC_BIT = 14;
  CODE_BIT = 16;
  NC = 255 + MAX_MATCH + 2 - THRESHOLD;
  NP = DIC_BIT + 1;
  NT = CODE_BIT + 3;
  MAX_HASH_VAL = 3 * (1 SHL DIC_BIT) + ((1 SHL DIC_BIT) SHR 9 + 1) * 255;
  PERC_FLAG = 32768;

type
  pWORD = ^tWORD;
  tWORD = array[0..PRED((1 SHL DIC_BIT))] of Integer;
  pBYTE = ^tBYTE;
  tBYTE = array[0..PRED(2*(1 SHL DIC_BIT))] of Byte;

type
  PLZHEncoderState = ^TLZHEncoderState;
  TLZHEncoderState = record
    WIN_SIZE: Word;
    DIC_SIZE: Word;
    l_tree, r_tree: array[0..2*(NC-1)] of Word;
    p_len: array[0..PRED(NT)] of Byte;
    c_len: array[0..PRED(NC)] of Byte;
    heap: array[0..NC] of Word;
    len_count: array[0..16] of Word;
    c_freq: array[0..2*(NC-1)] of Word;
    p_freq: array[0..2*(NP-1)] of Word;
    t_freq: array[0..2*(NT-1)] of Word;
    c_code: array[0..PRED(NC)] of Word;
    p_code: array[0..PRED(NT)] of Word;
    freq, sort_ptr, pos_ptr: pWORD;
    buf, len, stream, child_count, level: pBYTE;
    parent, previous, next: pWORD;
    bits, heap_size, remain: Integer;
    match_len: Integer;
    sbit_buf, bit_count,
    depth, c_pos, pos, out_pos, match_pos: Word;
    out_mask, avail: Word;
    input_buffer, output_buffer: pGENERIC_IO_BUFFER;
    input_buffer_idx, output_buffer_idx: Dword;
    input_buffer_size: Dword;
  end;
