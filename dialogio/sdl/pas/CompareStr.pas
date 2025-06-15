// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Local to `make_stream' procedure

type
  tCOMPARE_STR_RESULT = (isLess,isMore,isEqual);

function CompareStr(str1,str2: String): tCOMPARE_STR_RESULT;

var
  idx,len: Byte;
  result: tCOMPARE_STR_RESULT;

begin
  If (str1 = UPDIR_STR) then result := isLess
  else If (str2 = UPDIR_STR) then result := isMore
       else result := isEqual;

  If (result <> isEqual) then
    begin
      CompareStr := result;
      EXIT; //CompareStr
    end;

  str1 := Upper(FilterStr2(str1,_valid_characters_fname,'_'));
  str2 := Upper(FilterStr2(str2,_valid_characters_fname,'_'));

  If (Length(str1) > Length(str2)) then len := Length(str1)
  else len := Length(str2);

  For idx := 1 to len do
    If (FilterStr2(str1[idx],_valid_characters,#01) > FilterStr2(str2[idx],_valid_characters,#01)) then
      begin
        result := isMore;
        BREAK;
      end
    else If (str1[idx] < str2[idx]) then
           begin
             result := isLess;
             BREAK;
           end;

  If (result = isEqual) then
    If (Length(str1) < Length(str2)) then
      result := isLess
    else If (Length(str1) > Length(str2)) then
           result := isMore;

  CompareStr := result;

  //EXIT //CompareStr
end;
