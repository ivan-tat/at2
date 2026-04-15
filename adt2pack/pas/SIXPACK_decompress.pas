// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

const
  MAXFREQ       = 2000;
  MINCOPY       = 3;
  MAXCOPY       = 255;
  COPYRANGES    = 6;
  TERMINATE     = 256;
  FIRSTCODE     = 257;
  ROOT          = 1;
  CODESPERRANGE = MAXCOPY-MINCOPY+1;
  MAXCHAR       = FIRSTCODE+COPYRANGES*CODESPERRANGE-1;
  SUCCMAX       = MAXCHAR+1;
  TWICEMAX      = 2*MAXCHAR+1;
  MAXBUF        = PRED(64*1024);
  MAXDISTANCE   = 21389;
  MAXSIZE       = 21389+MAXCOPY;

const
  BitValue: array[1..14] of Word = (1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192);
  CopyBits: array[0..PRED(COPYRANGES)] of Word = (4,6,8,10,12,14);
  CopyMin:  array[0..PRED(COPYRANGES)] of Word = (0,16,80,336,1360,5456);

var
  leftC,rghtC: array[0..MAXCHAR] of Word;
  dad,frq: array[0..TWICEMAX] of Word;
  ibitCount,ibitBuffer,obufCount: Word;

procedure InitTree;

var
  index: Word;

begin
  For index := 2 to TWICEMAX do
    begin
      dad[index] := index DIV 2;
      frq[index] := 1;
    end;

  For index := 1 to MAXCHAR do
    begin
      leftC[index] := 2*index;
      rghtC[index] := 2*index+1;
    end;
end;

procedure UpdateFreq(a,b: Word);
begin
  Repeat
    frq[dad[a]] := frq[a]+frq[b];
    a := dad[a];
    If (a <> ROOT) then
      If (leftC[dad[a]] = a) then b := rghtC[dad[a]]
      else b := leftC[dad[a]];
  until (a = ROOT);

  If (frq[ROOT] = MAXFREQ) then
    For a := 1 to TWICEMAX do frq[a] := frq[a] SHR 1;
end;

procedure UpdateModel(code: Word);

var
  a,b,c,
  code1,code2: Word;

begin
  a := code+SUCCMAX;
  Inc(frq[a]);

  If (dad[a] <> ROOT) then
    begin
      code1 := dad[a];
      If (leftC[code1] = a) then UpdateFreq(a,rghtC[code1])
      else UpdateFreq(a,leftC[code1]);

      Repeat
        code2 := dad[code1];
        If (leftC[code2] = code1) then b := rghtC[code2]
        else b := leftC[code2];

        If (frq[a] > frq[b]) then
          begin
            If (leftC[code2] = code1) then rghtC[code2] := a
            else leftC[code2] := a;

            If (leftC[code1] = a) then
              begin
                leftC[code1] := b;
                c := rghtC[code1];
              end
            else begin
                   rghtC[code1] := b;
                   c := leftC[code1];
                 end;

            dad[b] := code1;
            dad[a] := code2;
            UpdateFreq(b,c);
            a := b;
          end;

        a := dad[a];
        code1 := dad[a];
      until (code1 = ROOT);
    end;
end;

function InputCode(bits: Word): Word;

var
  index,code: Word;

begin
  code := 0;
  For index := 1 to bits do
    begin
      If (ibitCount = 0) then
        begin
          If (ibufCount = MAXBUF) then ibufCount := 0;
          ibitBuffer := pWord(input_ptr)[ibufCount];
          Inc(ibufCount);
          ibitCount := 15;
        end
      else Dec(ibitCount);

      If (ibitBuffer > $7fff) then code := code OR bitValue[index];
      ibitBuffer := ibitBuffer SHL 1;
    end;

  InputCode := code;
end;

function Uncompress: Word;

var
  a: Word;

begin
  a := 1;
  Repeat
    If (ibitCount = 0) then
      begin
        If (ibufCount = MAXBUF) then ibufCount := 0;
        ibitBuffer := pWord(input_ptr)[ibufCount];
        Inc(ibufCount);
        ibitCount := 15;
      end
    else Dec(ibitCount);

    If (ibitBuffer > $7fff) then a := rghtC[a]
    else a := leftC[a];
    ibitBuffer := ibitBuffer SHL 1;
  until (a > MAXCHAR);

  Dec(a,SUCCMAX);
  UpdateModel(a);
  Uncompress := a;
end;

procedure SIXPACK_decode;

var
  i,j,k,t,c,
  count,dist,len,index: Word;

begin
  count := 0;
  InitTree;
  c := Uncompress;

  While (c <> TERMINATE) do
    begin
      If (c < 256) then
        begin
          output_ptr[obufCount] := c;
          Inc(obufCount);
          If (obufCount = MAXBUF) then
            begin
              output_size := MAXBUF;
              obufCount := 0;
            end;

          work_ptr[count] := c;
          Inc(count);
          If (count = MAXSIZE) then count := 0;
        end
      else begin
             t := c-FIRSTCODE;
             index := t DIV CODESPERRANGE;
             len := t+MINCOPY-index*CODESPERRANGE;
             dist := InputCode(CopyBits[index])+len+CopyMin[index];

             j := count;
             k := count-dist;
             If (count < dist) then Inc(k,MAXSIZE);

             For i := 0 to PRED(len) do
               begin
                 output_ptr[obufCount] := work_ptr[k];
                 Inc(obufCount);
                 If (obufCount = MAXBUF) then
                   begin
                     output_size := MAXBUF;
                     obufCount := 0;
                   end;

                 work_ptr[j] := work_ptr[k];
                 Inc(j);
                 Inc(k);
                 If (j = MAXSIZE) then j := 0;
                 If (k = MAXSIZE) then k := 0;
               end;

             Inc(count,len);
             If (count >= MAXSIZE) then Dec(count,MAXSIZE);
           end;

      c := Uncompress;
    end;

  output_size := obufCount;
end;

function SIXPACK_decompress(var source,dest; size: Word): Word;
begin
  input_ptr := @source;
  output_ptr := @dest;
  work_ptr := @work_mem;
  input_size := size;
  ibitCount  := 0;
  ibitBuffer := 0;
  obufCount  := 0;
  ibufCount  := 0;
  SIXPACK_decode;
  SIXPACK_decompress := output_size;
end;
