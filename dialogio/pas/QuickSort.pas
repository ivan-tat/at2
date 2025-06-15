// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Local to `make_stream' procedure

procedure QuickSort(l,r: Word);

var
  i,j: Word;
  cmp: String;
  tmp: tSEARCH;

begin
  If (l >= r) then
    EXIT; //QuickSort
  cmp := stream.stuff[(l+r) DIV 2].name;
  i := l;
  j := r;

  Repeat
{$IFDEF GO32V2}
    While (i < r) and (stream.stuff[i].name < cmp) do
      Inc(i);
    While (j > l) and (stream.stuff[j].name > cmp) do
      Dec(j);
{$ELSE}
    While (i < r) and
          (CompareStr(stream.stuff[i].name,cmp) = isLess) do
      Inc(i);
    While (j > l) and
          (CompareStr(stream.stuff[j].name,cmp) = isMore) do
      Dec(j);
{$ENDIF}

    If (i <= j) then
      begin
        tmp := stream.stuff[i];
        stream.stuff[i] := stream.stuff[j];
        stream.stuff[j] := tmp;
        Inc(i);
        Dec(j);
      end;
  until (i > j);

  If (l < j) then QuickSort(l,j);
  If (i < r) then QuickSort(i,r);

  //EXIT //QuickSort
end;
