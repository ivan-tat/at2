s!byte[[:space:]]+ptr[[:space:]]*\[([^][]+)\][[:space:]]*,[[:space:]]*!Byte(Pointer(\1)^){byte [\1]},!
s!byte[[:space:]]+ptr[[:space:]]*\[([^][]+)\]$!Byte(Pointer(\1)^){byte [\1]}!
s!word[[:space:]]+ptr[[:space:]]*\[([^][]+)\][[:space:]]*,[[:space:]]*!Word(Pointer(\1)^){word [\1]},!
s!word[[:space:]]+ptr[[:space:]]*\[([^][]+)\]$!Word(Pointer(\1)^){word [\1]}!
s!dword[[:space:]]+ptr[[:space:]]*\[([^][]+)\][[:space:]]*,[[:space:]]*!Longword(Pointer(\1)^){dword [\1]},!
s!dword[[:space:]]+ptr[[:space:]]*\[([^][]+)\]$!Longword(Pointer(\1)^){dword [\1]}!
s!qword[[:space:]]+ptr[[:space:]]*\[([^][]+)\][[:space:]]*,[[:space:]]*!QWord(Pointer(\1)^){qword [\1]},!
s!qword[[:space:]]+ptr[[:space:]]*\[([^][]+)\]$!QWord(Pointer(\1)^){qword [\1]}!

s!^([[:space:]]+)clc$!\1CF := 0;\t//\0!
s!^([[:space:]]+)stc$!\1CF := 1;\t//\0!
s!^([[:space:]]+)cmc$!\1CF := 1-CF;\t//\0!
s!^([[:space:]]+)cld$!\1DF := 0;\t//\0!
s!^([[:space:]]+)std$!\1DF := 1;\t//\0!
s!^([[:space:]]+)cli$!\1go32.disable;\t//\0!
s!^([[:space:]]+)sti$!\1go32.enable;\t//\0!

s!^([[:space:]]+)lahf$!\1ah := (SF shl 7)+(ZF shl 6)+(AF shl 4)+(PF shl 2)+CF;\t//\0!
s!^([[:space:]]+)sahf$!\1SF := (ah shr 7) and 1;\t//\0\
\1ZF := (ah shr 6) and 1;\
\1AF := (ah shr 4) and 1;\
\1PF := (ah shr 2) and 1;\
\1CF := ah and 1;!

s!^([[:space:]]+)pushf$!\1PushFlags (16);\t//\0!
s!^([[:space:]]+)popf$!\1PopFlags (16);\t//\0!
s!^([[:space:]]+)pushfd$!\1PushFlags (32);\t//\0!
s!^([[:space:]]+)popfd$!\1PopFlags (32);\t//\0!

s!^([[:space:]]+)mov[[:space:]]+([^,]+)[[:space:]]*,[[:space:]]*(.+)!\1\2 := \3;\t//\0!

s!^([[:space:]]+)movzx[[:space:]]+(ax|cx|dx|bx|sp|bp|si|di)[[:space:]]*,[[:space:]]*(.+)!\1\2 := Byte(\3);\t//\0!
s!^([[:space:]]+)movzx[[:space:]]+(eax|ecx|edx|ebx|esp|ebp|esi|edi)[[:space:]]*,[[:space:]]*(al|cl|dl|bl|ah|ch|dh|bh)!\1\2 := Byte(\3);\t//\0!
s!^([[:space:]]+)movzx[[:space:]]+(eax|ecx|edx|ebx|esp|ebp|esi|edi)[[:space:]]*,[[:space:]]*(ax|cx|dx|bx|sp|bp|si|di)!\1\2 := Word(\3);\t//\0!
s!^([[:space:]]+)movzx[[:space:]]+(eax|ecx|edx|ebx|esp|ebp|esi|edi)[[:space:]]*,[[:space:]]*(.+)!// for 8-bits operand:\t//\0\
\1\2 := Byte(\3);\t// \3: Byte\
// for 16-bits operand:\
//\1\2 := Word(\3);\t// \3: Word!

s!^([[:space:]]+)movsx[[:space:]]+(ax|cx|dx|bx|sp|bp|si|di)[[:space:]]*,[[:space:]]*(.+)!\1\2 := Shortint(\3);\t//\0\
\1// or: \2 := Smallint(Byte(\3) xor $80)-$80;\t// \3: Byte!
s!^([[:space:]]+)movsx[[:space:]]+(eax|ecx|edx|ebx|esp|ebp|esi|edi)[[:space:]]*,[[:space:]]*(al|cl|dl|bl|ah|ch|dh|bh)!\1\2 := Shortint(\3);\t//\0\
\1// or: \2 := Longint(Byte(\3) xor $80)-$80;\t// \3: Byte!
s!^([[:space:]]+)movsx[[:space:]]+(eax|ecx|edx|ebx|esp|ebp|esi|edi)[[:space:]]*,[[:space:]]*(ax|cx|dx|bx|sp|bp|si|di)!\1\2 := Smallint(\3);\t//\0\
\1// or: \2 := Longint(Word(\3) xor $8000)-$8000;\t// \3: Word!
s!^([[:space:]]+)movsx[[:space:]]+(eax|ecx|edx|ebx|esp|ebp|esi|edi)[[:space:]]*,[[:space:]]*(.+)!// for 8-bits operand:\t//\0\
\1\2 := Shortint(\3);\
\1// or: \2 := Longint(Byte(\3) xor $80)-$80;\t// \3: Byte\
// for 16-bits operand:\
//\1\2 := Smallint(\3);\
//\1// or: \2 := Longint(Word(\3) xor $8000)-$8000;\t// \3: Word!

s!^([[:space:]]+)lea[[:space:]]+([^,]+)[[:space:]]*,[[:space:]]*(.+)!\1\2 := Addr (\3);\t//\0!

s!^([[:space:]]+)xchg[[:space:]]+([^,]+)[[:space:]]*,[[:space:]]*(.+)!\1tmp := \2;\t//\0\
\1\2 := \3;\
\1\3 := tmp;!

s!^([[:space:]]+)xlat$!\1al := Byte((Pointer(ebx)+Byte(al))^);\t//\0!

s!^([[:space:]]+)not[[:space:]]+(.+)!\1\2 := not \2;\t//\0!

s!^([[:space:]]+)bt[[:space:]]+([^,]+)[[:space:]]*,[[:space:]]*(.+)!\1CF := ((\2) shr (\3)) and 1;\t//\0\
\1// or: CF := Ord((\2) and (1 shl (\3)) <> 0);!
s!^([[:space:]]+)btc[[:space:]]+([^,]+)[[:space:]]*,[[:space:]]*(.+)!\1CF := 1-(((\2) shr (\3)) and 1);\t//\0\
\1// or: CF := Ord((\2) and (1 shl (\3)) = 0);!
s!^([[:space:]]+)btr[[:space:]]+([^,]+)[[:space:]]*,[[:space:]]*(.+)!\1CF := ((\2) shr (\3)) and 1;\t//\0\
\1// or: CF := Ord((\2) and (1 shl (\3)) <> 0);\
\1\2 := (\2) and not (1 shl (\3));!
s!^([[:space:]]+)bts[[:space:]]+([^,]+)[[:space:]]*,[[:space:]]*(.+)!\1CF := ((\2) shr (\3)) and 1;\t//\0\
\1// or: CF := Ord((\2) and (1 shl (\3)) <> 0);\
\1\2 := (\2) or (1 shl (\3));!

s!^([[:space:]]+)test[[:space:]]+([^,]+)[[:space:]]*,[[:space:]]*\2!\1//OF := 0;\t//\0\
\1//AF := 0;\
\1//CF := 0;\
\1//SF := Get_SF (?{BITS}, \2);\
\1//ZF := Ord(\2 = 0);\
\1//PF := Get_PF (?{BITS}, \2);\
\1//if (\2 ? 0) then ?;!
s!^([[:space:]]+)test[[:space:]]+([^,]+)[[:space:]]*,[[:space:]]*(.+)!\1//tmp := (\2) and (\3);\t//\0\
\1//OF := 0;\
\1//AF := 0;\
\1//CF := 0;\
\1//SF := Get_SF (?{BITS}, tmp);\
\1//ZF := Ord(tmp = 0);\
\1//PF := Get_PF (?{BITS}, tmp);\
\1//if ((\2) and (\3) ? 0) then ?;!

s!^([[:space:]]+)and[[:space:]]+([^,]+)[[:space:]]*,[[:space:]]*\2!\1//OF := 0;\t//\0\
\1//AF := 0;\
\1//CF := 0;\
\1//SF := Get_SF (?{BITS}, \2);\
\1//ZF := Ord(\2 = 0);\
\1//PF := Get_PF (?{BITS}, \2);\
\1//if (\2 ? 0) then ?;!
s!^([[:space:]]+)and[[:space:]]+([^,]+)[[:space:]]*,[[:space:]]*(.+)!\1\2 := (\2) and (\3);\t//\0\
\1//OF := 0;\
\1//AF := 0;\
\1//CF := 0;\
\1//SF := Get_SF (?{BITS}, \2);\
\1//ZF := Ord(\2 = 0);\
\1//PF := Get_PF (?{BITS}, \2);\
\1//if (\2 ? 0) then ?;!

s!^([[:space:]]+)or[[:space:]]+([^,]+)[[:space:]]*,[[:space:]]*\2!\1//OF := 0;\t//\0\
\1//AF := 0;\
\1//CF := 0;\
\1//SF := Get_SF (?{BITS}, \2);\
\1//ZF := Ord(\2 = 0);\
\1//PF := Get_PF (?{BITS}, \2);\
\1//if (\2 ? 0) then ?;!
s!^([[:space:]]+)or[[:space:]]+([^,]+)[[:space:]]*,[[:space:]]*(.+)!\1\2 := (\2) or (\3);\t//\0\
\1//OF := 0;\
\1//AF := 0;\
\1//CF := 0;\
\1//SF := Get_SF (?{BITS}, \2);\
\1//ZF := Ord(\2 = 0);\
\1//PF := Get_PF (?{BITS}, \2);\
\1//if (\2 ? 0) then ?;!

s!^([[:space:]]+)xor[[:space:]]+([^,]+)[[:space:]]*,[[:space:]]*\2!\1\2 := 0;\t//\0\
\1//OF := 0;\
\1//AF := 0;\
\1//CF := 0;\
\1//SF := 0;\
\1//ZF := 1;\
\1//PF := 1;!
s!^([[:space:]]+)xor[[:space:]]+([^,]+)[[:space:]]*,[[:space:]]*(.+)!\1\2 := (\2) xor (\3);\t//\0\
\1//OF := 0;\
\1//AF := 0;\
\1//CF := 0;\
\1//SF := Get_SF (?{BITS}, \2);\
\1//ZF := Ord(\2 = 0);\
\1//PF := Get_PF (?{BITS}, \2);!

s!^([[:space:]]+)(sal|shl)[[:space:]]+(al|cl|dl|bl|ah|ch|dh|bh)[[:space:]]*,[[:space:]]*(.+)!\1_n := (\4) mod 32;\t//\0\
\1if (_n <> 0) then begin\
\1//  if (_n = 1) then\
\1//    OF := ((Byte(\3) shr 6) xor (Byte(\3) shr 7)) and 1;\
\1//  AF := 1;\
\1//  if (_n <= 8) then\
\1//    CF := (Byte(\3) shr (8-_n)) and 1;\
\1//  else\
\1//    CF := 0;\
\1  \3 := Byte(Byte(\3) shl _n);\t// \3: Byte (or Shortint)\
\1//  SF := Get_SF (8, \3);\
\1//  ZF := Ord(\3 = 0);\
\1//  PF := Get_PF (8, \3);\
\1end;!
s!^([[:space:]]+)(sal|shl)[[:space:]]+(ax|cx|dx|bx|sp|bp|si|di)[[:space:]]*,[[:space:]]*(.+)!\1_n := (\4) mod 32;\t//\0\
\1if (_n <> 0) then begin\
\1//  if (_n = 1) then\
\1//    OF := ((Word(\3) shr 14) xor (Word(\3) shr 15)) and 1;\
\1//  AF := 1;\
\1//  if (_n <= 16) then\
\1//    CF := (Word(\3) shr (16-_n)) and 1;\
\1//  else\
\1//    CF := 0;\
\1  \3 := Word(Word(\3) shl _n);\t// \3: Word (or Smallint)\
\1//  SF := Get_SF (16, \3);\
\1//  ZF := Ord(\3 = 0);\
\1//  PF := Get_PF (16, \3);\
\1end;!
s!^([[:space:]]+)(sal|shl)[[:space:]]+(eax|ecx|edx|ebx|esp|ebp|esi|edi)[[:space:]]*,[[:space:]]*(.+)!\1_n := (\4) mod 32;\t//\0\
\1if (_n <> 0) then begin\
\1//  if (_n = 1) then\
\1//    OF := ((Longword(\3) shr 30) xor (Longword(\3) shr 31)) and 1;\
\1//  AF := 1;\
\1//  CF := (Longword(\3) shr (32-_n)) and 1;\
\1  \3 := Longword(Longword(\3) shl _n);\t// \3: Longword (or Longint)\
\1//  SF := Get_SF (32, \3);\
\1//  ZF := Ord(\3 = 0);\
\1//  PF := Get_PF (32, \3);\
\1end;!
s!^([[:space:]]+)(sal|shl)[[:space:]]+([^,]+)[[:space:]]*,[[:space:]]*(.+)!\1_n := (\4) mod 32;\t//\0\
// for 8-bits operand:\
\1if (_n <> 0) then begin\
\1//  if (_n = 1) then\
\1//    OF := ((Byte(\3) shr 6) xor (Byte(\3) shr 7)) and 1;\
\1//  AF := 1;\
\1//  if (_n <= 8) then\
\1//    CF := (Byte(\3) shr (8-_n)) and 1;\
\1//  else\
\1//    CF := 0;\
\1  \3 := Byte(Byte(\3) shl _n);\t// \3: Byte (or Shortint)\
\1//  SF := Get_SF (8, \3);\
\1//  ZF := Ord(\3 = 0);\
\1//  PF := Get_PF (8, \3);\
\1end;\
// for 16-bits operand:\
//\1if (_n <> 0) then begin\
//\1//  if (_n = 1) then\
//\1//    OF := ((Word(\3) shr 14) xor (Word(\3) shr 15)) and 1;\
//\1//  AF := 1;\
//\1//  if (_n <= 16) then\
//\1//    CF := (Word(\3) shr (16-_n)) and 1;\
//\1//  else\
//\1//    CF := 0;\
//\1  \3 := Word(Word(\3) shl _n);\t// \3: Word (or Smallint)\
//\1//  SF := Get_SF (16, \3);\
//\1//  ZF := Ord(\3 = 0);\
//\1//  PF := Get_PF (16, \3);\
//\1end;\
// for 32-bits operand:\
//\1if (_n <> 0) then begin\
//\1//  if (_n = 1) then\
//\1//    OF := ((Longword(\3) shr 30) xor (Longword(\3) shr 31)) and 1;\
//\1//  AF := 1;\
//\1//  CF := (Longword(\3) shr (32-_n)) and 1;\
//\1  \3 := Longword(Longword(\3) shl _n);\t// \3: Longword (or Longint)\
//\1//  SF := Get_SF (32, \3);\
//\1//  ZF := Ord(\3 = 0);\
//\1//  PF := Get_PF (32, \3);\
//\1end;!

s!^([[:space:]]+)shr[[:space:]]+(al|cl|dl|bl|ah|ch|dh|bh)[[:space:]]*,[[:space:]]*(.+)!\1_n := (\3) mod 32;\t//\0\
\1if (_n <> 0) then begin\
\1//  if (_n = 1) then\
\1//    OF := 0;\
\1//  AF := 1;\
\1//  CF := (Byte(\2) shr (_n-1)) and 1;\
\1  \2 := Byte(\2) shr _n;\t// \2: Byte\
\1//  SF := Get_SF (8, \2);\
\1//  ZF := Ord(\2 = 0);\
\1//  PF := Get_PF (8, \2);\
\1end;!
s!^([[:space:]]+)shr[[:space:]]+(ax|cx|dx|bx|sp|bp|si|di)[[:space:]]*,[[:space:]]*(.+)!\1_n := (\3) mod 32;\t//\0\
\1if (_n <> 0) then begin\
\1//  if (_n = 1) then\
\1//    OF := 0;\
\1//  AF := 1;\
\1//  CF := (Word(\2) shr (_n-1)) and 1;\
\1  \2 := Word(\2) shr _n;\t// \2: Word\
\1//  SF := Get_SF (16, \2);\
\1//  ZF := Ord(\2 = 0);\
\1//  PF := Get_PF (16, \2);\
\1end;!
s!^([[:space:]]+)shr[[:space:]]+(eax|ecx|edx|ebx|esp|ebp|esi|edi)[[:space:]]*,[[:space:]]*(.+)!\1_n := (\3) mod 32;\t//\0\
\1if (_n <> 0) then begin\
\1//  if (_n = 1) then\
\1//    OF := 0;\
\1//  AF := 1;\
\1//  CF := (Longword(\2) shr (_n-1)) and 1;\
\1  \2 := Longword(\2) shr _n;\t// \2: Longword\
\1//  SF := Get_SF (32, \2);\
\1//  ZF := Ord(\2 = 0);\
\1//  PF := Get_PF (32, \2);\
\1end;!
s!^([[:space:]]+)shr[[:space:]]+([^,]+)[[:space:]]*,[[:space:]]*(.+)!\1_n := (\3) mod 32;\t//\0\
// for 8-bits operand:\
\1if (_n <> 0) then begin\
\1//  if (_n = 1) then\
\1//    OF := 0;\
\1//  AF := 1;\
\1//  CF := (Byte(\2) shr (_n-1)) and 1;\
\1  \2 := Byte(\2) shr _n;\t// \2: Byte\
\1//  SF := Get_SF (8, \2);\
\1//  ZF := Ord(\2 = 0);\
\1//  PF := Get_PF (8, \2);\
\1end;\
// for 16-bits operand:\
//\1if (_n <> 0) then begin\
//\1//  if (_n = 1) then\
//\1//    OF := 0;\
//\1//  AF := 1;\
//\1//  CF := (Word(\2) shr (_n-1)) and 1;\
//\1  \2 := Word(\2) shr _n;\t// \2: Word\
//\1//  SF := Get_SF (16, \2);\
//\1//  ZF := Ord(\2 = 0);\
//\1//  PF := Get_PF (16, \2);\
//\1end;\
// for 32-bits operand:\
//\1if (_n <> 0) then begin\
//\1//  if (_n = 1) then\
//\1//    OF := 0;\
//\1//  AF := 1;\
//\1//  CF := (Longword(\2) shr (_n-1)) and 1;\
//\1  \2 := Longword(\2) shr _n;\t// \2: Longword\
//\1//  SF := Get_SF (32, \2);\
//\1//  ZF := Ord(\2 = 0);\
//\1//  PF := Get_PF (32, \2);\
//\1end;!

s!^([[:space:]]+)sar[[:space:]]+(al|cl|dl|bl|ah|ch|dh|bh)[[:space:]]*,[[:space:]]*(.+)!\1_n := (\3) mod 32;\t//\0\
\1if (_n <> 0) then begin\
\1//  if (_n = 1) then\
\1//    OF := 0;\
\1//  AF := 1;\
\1//  CF := (Longint(Shortint(\2)) shr (_n-1)) and 1;\
\1  \2 := Shortint(Int64(Shortint(\2)) shr _n);\t// \2: Shortint\
\1//  SF := Get_SF (8, \2);\
\1//  ZF := Ord(\2 = 0);\
\1//  PF := Get_PF (8, \2);\
\1end;!
s!^([[:space:]]+)sar[[:space:]]+(ax|cx|dx|bx|sp|bp|si|di)[[:space:]]*,[[:space:]]*(.+)!\1_n := (\3) mod 32;\t//\0\
\1if (_n <> 0) then begin\
\1//  if (_n = 1) then\
\1//    OF := 0;\
\1//  AF := 1;\
\1//  CF := (Longint(Smallint(\2)) shr (_n-1)) and 1;\
\1  \2 := Smallint(Int64(Smallint(\2)) shr _n);\t// \2: Smallint\
\1//  SF := Get_SF (16, \2);\
\1//  ZF := Ord(\2 = 0);\
\1//  PF := Get_PF (16, \2);\
\1end;!
s!^([[:space:]]+)sar[[:space:]]+(eax|ecx|edx|ebx|esp|ebp|esi|edi)[[:space:]]*,[[:space:]]*(.+)!\1_n := (\3) mod 32;\t//\0\
\1if (_n <> 0) then begin\
\1//  if (_n = 1) then\
\1//    OF := 0;\
\1//  AF := 1;\
\1//  CF := Longint(\2) shr (_n-1)) and 1;\
\1  \2 := Longint(Int64(Longint(\2)) shr _n);\t// \2: Longint\
\1//  SF := Get_SF (32, \2);\
\1//  ZF := Ord(\2 = 0);\
\1//  PF := Get_PF (32, \2);\
\1end;!
s!^([[:space:]]+)sar[[:space:]]+([^,]+)[[:space:]]*,[[:space:]]*(.+)!\1_n := (\3) mod 32;\t//\0\
// for 8-bits operand:\
\1if (_n <> 0) then begin\
\1//  if (_n = 1) then\
\1//    OF := 0;\
\1//  AF := 1;\
\1//  CF := (Longint(Shortint(\2)) shr (_n-1)) and 1;\
\1  \2 := Shortint(Int64(Shortint(\2)) shr _n);\t// \2: Shortint\
\1//  SF := Get_SF (8, \2);\
\1//  ZF := Ord(\2 = 0);\
\1//  PF := Get_PF (8, \2);\
\1end;\
// for 16-bits operand:\
//\1if (_n <> 0) then begin\
//\1//  if (_n = 1) then\
//\1//    OF := 0;\
//\1//  AF := 1;\
//\1//  CF := (Longint(Smallint(\2)) shr (_n-1)) and 1;\
//\1  \2 := Smallint(Int64(Smallint(\2)) shr _n);\t// \2: Smallint\
//\1//  SF := Get_SF (16, \2);\
//\1//  ZF := Ord(\2 = 0);\
//\1//  PF := Get_PF (16, \2);\
//\1end;\
// for 32-bits operand:\
//\1if (_n <> 0) then begin\
//\1//  if (_n = 1) then\
//\1//    OF := 0;\
//\1//  AF := 1;\
//\1//  CF := Longint(\2) shr (_n-1)) and 1;\
//\1  \2 := Longint(Int64(Longint(\2)) shr _n);\t// \2: Longint\
//\1//  SF := Get_SF (32, \2);\
//\1//  ZF := Ord(\2 = 0);\
//\1//  PF := Get_PF (32, \2);\
//\1end;!

s!^([[:space:]]+)rol[[:space:]]+(al|cl|dl|bl|ah|ch|dh|bh)[[:space:]]*,[[:space:]]*(.+)!\1_n := (\3) mod 8;\t//\0\
\1\2 := Byte((Byte(\2) shl _n) + (Byte(\2) shr (8-_n)));\t// \2: Byte\
\1//if ((\3) = 1) then\
\1//  OF := (Byte(\2) xor (Byte(\2) shr 7)) and 1;\
\1//if ((\3) <> 0) then\
\1//  CF := Byte(\2) and 1;!
s!^([[:space:]]+)rol[[:space:]]+(ax|cx|dx|bx|sp|bp|si|di)[[:space:]]*,[[:space:]]*(.+)!\1_n := (\3) mod 16;\t//\0\
\1\2 := Word((Word(\2) shl _n) + (Word(\2) shr (16-_n)));\t// \2: Word\
\1//if ((\3) = 1) then\
\1//  OF := (Word(\2) xor (Word(\2) shr 15)) and 1;\
\1//if ((\3) <> 0) then\
\1//  CF := Word(\2) and 1;!
s!^([[:space:]]+)rol[[:space:]]+(eax|ecx|edx|ebx|esp|ebp|esi|edi)[[:space:]]*,[[:space:]]*(.+)!\1_n := (\3) mod 32;\t//\0\
\1\2 := Longword((Longword(\2) shl _n) + (Longword(\2) shr (32-_n)));\t// \2: Longword\
\1//if ((\3) = 1) then\
\1//  OF := (Longword(\2) xor (Longword(\2) shr 31)) and 1;\
\1//if ((\3) <> 0) then\
\1//  CF := Longword(\2) and 1;!
s!^([[:space:]]+)rol[[:space:]]+([^,]+)[[:space:]]*,[[:space:]]*(.+)!// for 8-bits operand:\t//\0\
\1_n := (\3) mod 8;\
\1\2 := Byte((Byte(\2) shl _n) + (Byte(\2) shr (8-_n)));\t// \2: Byte\
\1//if ((\3) = 1) then\
\1//  OF := (Byte(\2) xor (Byte(\2) shr 7)) and 1;\
\1//if ((\3) <> 0) then\
\1//  CF := Byte(\2) and 1;\
// for 16-bits operand:\
//\1_n := (\3) mod 16;\
//\1\2 := Word((Word(\2) shl _n) + (Word(\2) shr (16-_n)));\t// \2: Word\
//\1//if ((\3) = 1) then\
//\1//  OF := (Word(\2) xor (Word(\2) shr 15)) and 1;\
//\1//if ((\3) <> 0) then\
//\1//  CF := Word(\2) and 1;\
// for 32-bits operand:\
//\1_n := (\3) mod 32;\
//\1\2 := Longword((Longword(\2) shl _n) + (Longword(\2) shr (32-_n)));\t// \2: Longword\
//\1//if ((\3) = 1) then\
//\1//  OF := (Longword(\2) xor (Longword(\2) shr 31)) and 1;\
//\1//if ((\3) <> 0) then\
//\1//  CF := Longword(\2) and 1;\
!

s!^([[:space:]]+)rcl[[:space:]]+(al|cl|dl|bl|ah|ch|dh|bh)[[:space:]]*,[[:space:]]*(.+)!\1_n := (\3) mod 9;\t//\0\
\1_a := Word(Byte(\2) + (CF shl 8));\t// _a: Word\
\1_a := Word((Word(_a) shl _n) + (Word(_a) shr (9-_n)));\
\1\2 := _a and $FF;\t// \2: Byte\
\1//if ((\3) = 1) then\
\1//  OF := ((Word(_a) shr 7) xor (Word(_a) shr 8)) and 1;\
\1//if ((\3) <> 0) then\
\1//  CF := (Word(_a) shr 8) and 1;!
s!^([[:space:]]+)rcl[[:space:]]+(ax|cx|dx|bx|sp|bp|si|di)[[:space:]]*,[[:space:]]*(.+)!\1_n := (\3) mod 17;\t//\0\
\1_a := Longword(Word(\2) + (CF shl 16));\t// _a: Longword\
\1_a := Longword((Longword(_a) shl _n) + (Longword(_a) shr (17-_n)));\
\1\2 := _a and $FFFF;\t// \2: Word\
\1//if ((\3) = 1) then\
\1//  OF := ((Longword(_a) shr 15) xor (Longword(_a) shr 16)) and 1;\
\1//if ((\3) <> 0) then\
\1//  CF := (Longword(_a) shr 16) and 1;!
s!^([[:space:]]+)rcl[[:space:]]+(eax|ecx|edx|ebx|esp|ebp|esi|edi)[[:space:]]*,[[:space:]]*(.+)!\1_n := (\3) mod 33;\t//\0\
\1_a := QWord(Longword(\2) + (CF shl 32));\t// _a: QWord\
\1_a := QWord((QWord(_a) shl _n) + (QWord(_a) shr (33-_n)));\
\1\2 := _a and $FFFFFFFF;\t// \2: Longword\
\1//if ((\3) = 1) then\
\1//  OF := ((QWord(_a) shr 31) xor (QWord(_a) shr 32)) and 1;\
\1//if ((\3) <> 0) then\
\1//  CF := (QWord(_a) shr 32) and 1;!
s!^([[:space:]]+)rcl[[:space:]]+([^,]+)[[:space:]]*,[[:space:]]*(.+)!// for 8-bits operand:\t//\0\
\1_n := (\3) mod 9;\
\1_a := Word(Byte(\2) + (CF shl 8));\t// _a: Word\
\1_a := Word((Word(_a) shl _n) + (Word(_a) shr (9-_n)));\
\1\2 := _a and $FF;\t// \2: Byte\
\1//if ((\3) = 1) then\
\1//  OF := ((Word(_a) shr 7) xor (Word(_a) shr 8)) and 1;\
\1//if ((\3) <> 0) then\
\1//  CF := (Word(_a) shr 8) and 1;\
// for 16-bits operand:\
//\1_n := (\3) mod 17;\
//\1_a := Longword(Word(\2) + (CF shl 16));\t// _a: Longword\
//\1_a := Longword((Longword(_a) shl _n) + (Longword(_a) shr (17-_n)));\
//\1\2 := _a and $FFFF;\t// \2: Word\
//\1//if ((\3) = 1) then\
//\1//  OF := ((Longword(_a) shr 15) xor (Longword(_a) shr 16)) and 1;\
//\1//if ((\3) <> 0) then\
//\1//  CF := (Longword(_a) shr 16) and 1;\
// for 32-bits operand:\
//\1_n := (\3) mod 33;\
//\1_a := QWord(Longword(\2) + (CF shl 32));\t// _a: QWord\
//\1_a := QWord((QWord(_a) shl _n) + (QWord(_a) shr (33-_n)));\
//\1\2 := _a and $FFFFFFFF;\t// \2: Longword\
//\1//if ((\3) = 1) then\
//\1//  OF := ((QWord(_a) shr 31) xor (QWord(_a) shr 32)) and 1;\
//\1//if ((\3) <> 0) then\
//\1//  CF := (QWord(_a) shr 32) and 1;\
!

s!^([[:space:]]+)ror[[:space:]]+(al|cl|dl|bl|ah|ch|dh|bh)[[:space:]]*,[[:space:]]*(.+)!\1_n := (\3) mod 8;\t//\0\
\1\2 := Byte((Byte(\2) shr _n) + (Byte(\2) shl (8-_n)));\t// \2: Byte\
\1//if ((\3) = 1) then\
\1//  OF := ((Byte(\2) shr 6) xor (Byte(\2) shr 7)) and 1;\
\1//if ((\3) <> 0) then\
\1//  CF := (Byte(\2) shr 7) and 1;!
s!^([[:space:]]+)ror[[:space:]]+(ax|cx|dx|bx|sp|bp|si|di)[[:space:]]*,[[:space:]]*(.+)!\1_n := (\3) mod 16;\t//\0\
\1\2 := Word((Word(\2) shr _n) + (Word(\2) shl (16-_n)));\t// \2: Word\
\1//if ((\3) = 1) then\
\1//  OF := ((Word(\2) shr 14) xor (Word(\2) shr 15)) and 1;\
\1//if ((\3) <> 0) then\
\1//  CF := (Word(\2) shr 15) and 1;!
s!^([[:space:]]+)ror[[:space:]]+(eax|ecx|edx|ebx|esp|ebp|esi|edi)[[:space:]]*,[[:space:]]*(.+)!\1_n := (\3) mod 32;\t//\0\
\1\2 := Longword((Longword(\2) shr _n) + (Longword(\2) shl (32-_n)));\t// \2: Longword\
\1//if ((\3) = 1) then\
\1//  OF := ((Longword(\2) shr 30) xor (Longword(\2) shr 31)) and 1;\
\1//if ((\3) <> 0) then\
\1//  CF := (Longword(\2) shr 31) and 1;!
s!^([[:space:]]+)ror[[:space:]]+([^,]+)[[:space:]]*,[[:space:]]*(.+)!// for 8-bits operand:\t//\0\
\1_n := (\3) mod 8;\
\1\2 := Byte((Byte(\2) shr _n) + (Byte(\2) shl (8-_n)));\t// \2: Byte\
\1//if ((\3) = 1) then\
\1//  OF := ((Byte(\2) shr 6) xor (Byte(\2) shr 7)) and 1;\
\1//if ((\3) <> 0) then\
\1//  CF := (Byte(\2) shr 7) and 1;\
// for 16-bits operand:\
//\1_n := (\3) mod 16;\
//\1\2 := Word((Word(\2) shr _n) + (Word(\2) shl (16-_n)));\t// \2: Word\
//\1//if ((\3) = 1) then\
//\1//  OF := ((Word(\2) shr 14) xor (Word(\2) shr 15)) and 1;\
//\1//if ((\3) <> 0) then\
//\1//  CF := (Word(\2) shr 15) and 1;\
// for 32-bits operand:\
//\1_n := (\3) mod 32;\
//\1\2 := Longword((Longword(\2) shr _n) + (Longword(\2) shl (32-_n)));\t// \2: Longword\
//\1//if ((\3) = 1) then\
//\1//  OF := ((Longword(\2) shr 30) xor (Longword(\2) shr 31)) and 1;\
//\1//if ((\3) <> 0) then\
//\1//  CF := (Longword(\2) shr 31) and 1;\
!

s!^([[:space:]]+)rcr[[:space:]]+(al|cl|dl|bl|ah|ch|dh|bh)[[:space:]]*,[[:space:]]*(.+)!\1_n := (\3) mod 9;\t//\0\
\1_a := Word(Byte(\2) + (CF shl 8));\t// _a: Word\
\1_a := Word((Word(_a) shr _n) + (Word(_a) shl (9-_n)));\
\1\2 := _a and $FF;\t// \2: Byte\
\1//if ((\3) = 1) then\
\1//  OF := ((Word(_a) shr 6) xor (Word(_a) shr 7)) and 1;\
\1//if ((\3) <> 0) then\
\1//  CF := (Word(_a) shr 8) and 1;!
s!^([[:space:]]+)rcr[[:space:]]+(ax|cx|dx|bx|sp|bp|si|di)[[:space:]]*,[[:space:]]*(.+)!\1_n := (\3) mod 17;\t//\0\
\1_a := Longword(Word(\2) + (CF shl 16));\t// _a: Longword\
\1_a := Longword((Longword(_a) shr _n) + (Longword(_a) shl (17-_n)));\
\1\2 := _a and $FFFF;\t// \2: Word\
\1//if ((\3) = 1) then\
\1//  OF := ((Longword(_a) shr 14) xor (Longword(_a) shr 15)) and 1;\
\1//if ((\3) <> 0) then\
\1//  CF := (Longword(_a) shr 16) and 1;!
s!^([[:space:]]+)rcr[[:space:]]+(eax|ecx|edx|ebx|esp|ebp|esi|edi)[[:space:]]*,[[:space:]]*(.+)!\1_n := (\3) mod 33;\t//\0\
\1_a := QWord(Longword(\2) + (CF shl 32));\t// _a: QWord\
\1_a := QWord((QWord(_a) shr _n) + (QWord(_a) shl (33-_n)));\
\1\2 := _a and $FFFFFFFF;\t// \2: Longword\
\1//if ((\3) = 1) then\
\1//  OF := ((QWord(_a) shr 30) xor (QWord(_a) shr 31)) and 1;\
\1//if ((\3) <> 0) then\
\1//  CF := (QWord(_a) shr 32) and 1;!
s!^([[:space:]]+)rcr[[:space:]]+([^,]+)[[:space:]]*,[[:space:]]*(.+)!// for 8-bits operand:\t//\0\
\1_n := (\3) mod 9;\
\1_a := Word(Byte(\2) + (CF shl 8));\t// _a: Word\
\1_a := Word((Word(_a) shr _n) + (Word(_a) shl (9-_n)));\
\1\2 := _a and $FF;\t// \2: Byte\
\1//if ((\3) = 1) then\
\1//  OF := ((Word(_a) shr 6) xor (Word(_a) shr 7)) and 1;\
\1//if ((\3) <> 0) then\
\1//  CF := (Word(_a) shr 8) and 1;\
// for 16-bits operand:\
//\1_n := (\3) mod 17;\
//\1_a := Longword(Word(\2) + (CF shl 16));\t// _a: Longword\
//\1_a := Longword((Longword(_a) shr _n) + (Longword(_a) shl (17-_n)));\
//\1\2 := _a and $FFFF;\t// \2: Word\
//\1//if ((\3) = 1) then\
//\1//  OF := ((Longword(_a) shr 14) xor (Longword(_a) shr 15)) and 1;\
//\1//if ((\3) <> 0) then\
//\1//  CF := (Longword(_a) shr 16) and 1;\
// for 32-bits operand:\
//\1_n := (\3) mod 33;\
//\1_a := QWord(Longword(\2) + (CF shl 32));\t// _a: QWord\
//\1_a := QWord((QWord(_a) shr _n) + (QWord(_a) shl (33-_n)));\
//\1\2 := _a and $FFFFFFFF;\t// \2: Longword\
//\1//if ((\3) = 1) then\
//\1//  OF := ((QWord(_a) shr 30) xor (QWord(_a) shr 31)) and 1;\
//\1//if ((\3) <> 0) then\
//\1//  CF := (QWord(_a) shr 32) and 1;\
!

s!^([[:space:]]+)neg[[:space:]]+(al|cl|dl|bl|ah|ch|dh|bh)!\1//OF := Neg_Get_OF (8, \2);\t//\0\
\1//AF := Neg_Get_AF (8, \2);\
\1//CF := Ord(\2 <> 0);\
\1\2 := -\2;\
\1//SF := Get_SF (8, \2);\
\1//ZF := Ord(\2 = 0);\
\1//PF := Get_PF (8, \2);!
s!^([[:space:]]+)neg[[:space:]]+(ax|cx|dx|bx|sp|bp|si|di)!\1//OF := Neg_Get_OF (16, \2);\t//\0\
\1//AF := Neg_Get_AF (16, \2);\
\1//CF := Ord(\2 <> 0);\
\1\2 := -\2;\
\1//SF := Get_SF (16, \2);\
\1//ZF := Ord(\2 = 0);\
\1//PF := Get_PF (16, \2);!
s!^([[:space:]]+)neg[[:space:]]+(eax|ecx|edx|ebx|esp|ebp|esi|edi)!\1//OF := Neg_Get_OF (32, \2);\t//\0\
\1//AF := Neg_Get_AF (32, \2);\
\1//CF := Ord(\2 <> 0);\
\1\2 := -\2;\
\1//SF := Get_SF (32, \2);\
\1//ZF := Ord(\2 = 0);\
\1//PF := Get_PF (32, \2);!
s!^([[:space:]]+)neg[[:space:]]+(.+)!// for 8-bits operand:\t//\0\
\1//OF := Neg_Get_OF (8, \2);\
\1//AF := Neg_Get_AF (8, \2);\
\1//CF := Ord(\2 <> 0);\
\1\2 := -(\2);\
\1//SF := Get_SF (8, \2);\
\1//ZF := Ord(\2 = 0);\
\1//PF := Get_PF (8, \2);\
// for 16-bits operand:\
//\1//OF := Neg_Get_OF (16, \2);\
//\1//AF := Neg_Get_AF (16, \2);\
//\1//CF := Ord(\2 <> 0);\
//\1\2 := -(\2);\
//\1//SF := Get_SF (16, \2);\
//\1//ZF := Ord(\2 = 0);\
//\1//PF := Get_PF (16, \2);\
// for 32-bits operand:\
//\1//OF := Neg_Get_OF (32, \2);\
//\1//AF := Neg_Get_AF (32, \2);\
//\1//CF := Ord(\2 <> 0);\
//\1\2 := -(\2);\
//\1//SF := Get_SF (32, \2);\
//\1//ZF := Ord(\2 = 0);\
//\1//PF := Get_PF (32, \2);!

s!^([[:space:]]+)cbw$!\1ax := Shortint(al);\t//\0!
s!^([[:space:]]+)cwde$!\1eax := Smallint(ax);\t//\0!
s!^([[:space:]]+)cwd$!\1dx := -(Word(ax) shr 15);\t//\0\
// or one of:\
//\1if (Smallint(ax) < 0) then dx := -1 else dx := 0;\
//\1_a := Smallint(ax);\t// _a: = Longint!
s!^([[:space:]]+)cdq$!\1edx := -(Longword(eax) shr 31);\t//\0\
// or one of:\
//\1if (Longint(eax) < 0) then edx := -1 else edx := 0;\
//\1_a := Longint(eax);\t// _a: Int64!

s!^([[:space:]]+)inc[[:space:]]+(al|cl|dl|bl|ah|ch|dh|bh)!\1//OF := Ord(\2 = $7F);\t//\0\
\1//AF := Inc_Get_AF (8, \2);\
\1// CF is not modified\
\1Inc (\2);\
\1//SF := Get_SF (8, \2);\
\1//ZF := Ord(\2 = 0);\
\1//PF := Get_PF (8, \2);!
s!^([[:space:]]+)inc[[:space:]]+(ax|cx|dx|bx|sp|bp|si|di)!\1//OF := Ord(\2 = $7FFF);\t//\0\
\1//AF := Inc_Get_AF (16, \2);\
\1// CF is not modified\
\1Inc (\2);\
\1//SF := Get_SF (16, \2);\
\1//ZF := Ord(\2 = 0);\
\1//PF := Get_PF (16, \2);!
s!^([[:space:]]+)inc[[:space:]]+(eax|ecx|edx|ebx|esp|ebp|esi|edi)!\1//OF := Ord(\2 = $7FFFFFFF);\t//\0\
\1//AF := Inc_Get_AF (32 \2);\
\1// CF is not modified\
\1Inc (\2);\
\1//SF := Get_SF (32, \2);\
\1//ZF := Ord(\2 = 0);\
\1//PF := Get_PF (32, \2);!
s!^([[:space:]]+)inc[[:space:]]+(.+)!// for 8-bits operand:\t//\0\
\1//OF := Ord(\2 = $7F);\
\1//AF := Inc_Get_AF (8, \2);\
\1// CF is not modified\
\1Inc (\2);\
\1//SF := Get_SF (8, \2);\
\1//ZF := Ord(\2 = 0);\
\1//PF := Get_PF (8, \2);\
// for 16-bits operand:\
//\1//OF := Ord(\2 = $7FFF);\
//\1//AF := Inc_Get_AF (16, \2);\
//\1// CF is not modified\
//\1Inc (\2);\
//\1//SF := Get_SF (16, \2);\
//\1//ZF := Ord(\2 = 0);\
//\1//PF := Get_PF (16, \2);\
// for 32-bits operand:\
//\1//OF := Ord(\2 = $7FFFFFFF);\
//\1//AF := Inc_Get_AF (32, \2);\
//\1// CF is not modified\
//\1Inc (\2);\
//\1//SF := Get_SF (32, \2);\
//\1//ZF := Ord(\2 = 0);\
//\1//PF := Get_PF (32, \2);\
!

s!^([[:space:]]+)dec[[:space:]]+(al|cl|dl|bl|ah|ch|dh|bh)!\1//OF := Ord(Byte(\2) = $80);\t//\0\
\1//AF := Dec_Get_AF (8, \2);\
\1// CF is not modified\
\1Dec (\2);\
\1//SF := Get_SF (8, \2);\
\1//ZF := Ord(\2 = 0);\
\1//PF := Get_PF (8, \2);!
s!^([[:space:]]+)dec[[:space:]]+(ax|cx|dx|bx|sp|bp|si|di)!\1//OF := Ord(Word(\2) = $8000);\t//\0\
\1//AF := Dec_Get_AF (16, \2);\
\1// CF is not modified\
\1Dec (\2);\
\1//SF := Get_SF (16, \2);\
\1//ZF := Ord(\2 = 0);\
\1//PF := Get_PF (16, \2);!
s!^([[:space:]]+)dec[[:space:]]+(eax|ecx|edx|ebx|esp|ebp|esi|edi)!\1//OF := Ord(Longword(\2) = $80000000);\t//\0\
\1//AF := Dec_Get_AF (32, \2);\
\1// CF is not modified\
\1Dec (\2);\
\1//SF := Get_SF (32, \2);\
\1//ZF := Ord(\2 = 0);\
\1//PF := Get_PF (32, \2);!
s!^([[:space:]]+)dec[[:space:]]+(.+)!// for 8-bits operand:\t//\0\
\1//OF := Ord(Byte(\2) = $80);\
\1//AF := Dec_Get_AF (8, \2);\
\1// CF is not modified\
\1Dec (\2);\
\1//SF := Get_SF (8, \2);\
\1//ZF := Ord(\2 = 0);\
\1//PF := Get_PF (8, \2);\
// for 16-bits operand:\
//\1//OF := Ord(Word(\2) = $8000);\
//\1//AF := Dec_Get_AF (16, \2);\
//\1// CF is not modified\
//\1Dec (\2);\
//\1//SF := Get_SF (16, \2);\
//\1//ZF := Ord(\2 = 0);\
//\1//PF := Get_PF (16, \2);\
// for 32-bits operand:\
//\1//OF := Ord(Longword(\2) = $80000000);\
//\1//AF := Dec_Get_AF (32, \2);\
//\1// CF is not modified\
//\1Dec (\2);\
//\1//SF := Get_SF (32, \2);\
//\1//ZF := Ord(\2 = 0);\
//\1//PF := Get_PF (32, \2);\
!

s!^([[:space:]]+)add[[:space:]]+([^,]+)[[:space:]]*,[[:space:]]*(.+)!\1//OF := Add_Get_OF (?{BITS}, \2, \3);\t//\0\
\1//AF := Add_Get_AF (?{BITS}, \2, \3);\
\1//CF := Add_Get_CF (?{BITS}, \2, \3);\
\1\2 := (\2) + (\3);\
\1//SF := Get_SF (?{BITS}, \2);\
\1//ZF := Ord(\2 = 0);\
\1//PF := Get_PF (?{BITS}, \2);!

s!^([[:space:]]+)adc[[:space:]]+([^,]+)[[:space:]]*,[[:space:]]*(.+)!\1_CF := CF;\t//\0\
\1//OF := Adc_Get_OF (?{BITS}, \2, \3);\
\1//AF := Adc_Get_AF (?{BITS}, \2, \3);\
\1//CF := Adc_Get_CF (?{BITS}, \2, \3);\
\1\2 := (\2) + (\3) + _CF;\
\1//SF := Get_SF (?{BITS}, \2);\
\1//ZF := Ord(\2 = 0);\
\1//PF := Get_PF (?{BITS}, \2);!

s!^([[:space:]]+)sub[[:space:]]+([^,]+)[[:space:]]*,[[:space:]]*\2!\1\2 := 0;\t//\0\
\1//OF := 0;\
\1//AF := 0;\
\1//CF := 0;\
\1//SF := 0;\
\1//ZF := 1;\
\1//PF := 1;!
s!^([[:space:]]+)sub[[:space:]]+([^,]+)[[:space:]]*,[[:space:]]*(.+)!\1//OF := Sub_Get_OF (?{BITS}, \2, \3);\t//\0\
\1//AF := Sub_Get_AF (?{BITS}, \2, \3);\
\1//CF := Sub_Get_CF (?{BITS}, \2, \3);\
\1\2 := (\2) - (\3);\
\1//SF := Get_SF (?{BITS}, \2);\
\1//ZF := Ord(\2 = 0);\
\1//PF := Get_PF (?{BITS}, \2);!

s!^([[:space:]]+)sbb[[:space:]]+([^,]+)[[:space:]]*,[[:space:]]*\2!\1\2 := -CF;\t//\0\
\1//OF := 0;\
\1//AF := CF;\
\1// CF is not modified\
\1//SF := CF;\
\1//ZF := 1-CF;\
\1//PF := 1;!
s!^([[:space:]]+)sbb[[:space:]]+([^,]+)[[:space:]]*,[[:space:]]*(.+)!\1_CF := CF;\t//\0\
\1//OF := Sbb_Get_OF (?{BITS}, \2, \3);\
\1//AF := Sbb_Get_AF (?{BITS}, \2, \3);\
\1//CF := Sbb_Get_CF (?{BITS}, \2, \3);\
\1\2 := (\2) - (\3) - _CF;\
\1//SF := Get_SF (?{BITS}, \2);\
\1//ZF := Ord(\2 = 0);\
\1//PF := Get_PF (?{BITS}, \2);!

s!^([[:space:]]+)mul[[:space:]]+(al|cl|dl|bl|ah|ch|dh|bh)!\1ax := Word(Byte(al) * Byte(\2));\t//\0\
\1//OF := Ord(Word(ax) > $FF);\
\1//CF := Ord(Word(ax) > $FF);\
\1// SF, ZF, AF, PF: undefined\
!
s!^([[:space:]]+)mul[[:space:]]+(ax|cx|dx|bx|sp|bp|si|di)!\1_a := Longword(Word(ax) * Word(\2));\t//\0\
\1ax := Longword(_a) and $FFFF;\
\1dx := Longword(_a) shr 16;\
\1//OF := Ord(Longword(_a) > $FFFF);\
\1//CF := Ord(Longword(_a) > $FFFF);\
\1// SF, ZF, AF, PF: undefined\
!
s!^([[:space:]]+)mul[[:space:]]+(eax|ecx|edx|ebx|esp|ebp|esi|edi)!\1_a := QWord(Longword(eax) * Longword(\2));\t//\0\
\1eax := QWord(_a) and $FFFFFFFF;\
\1edx := QWord(_a) shr 32;\
\1//OF := Ord(QWord(_a) > $FFFFFFFF);\
\1//CF := Ord(QWord(_a) > $FFFFFFFF);\
\1// SF, ZF, AF, PF: undefined\
!
s!^([[:space:]]+)mul[[:space:]]+(.+)!// 8-bits operand:\t//\0\
\1ax := Word(Byte(al) * Byte(\2));\t// ax: Word\
//\1//OF := Ord(Word(ax) > $FF);\
//\1//CF := Ord(Word(ax) > $FF);\
//\1// SF, ZF, AF, PF: undefined\
// 16-bits operand:\
//\1_a := Longword(Word(ax) * Word(\2));\t// _a: Longword\
//\1ax := Longword(_a) and $FFFF;\
//\1dx := Longword(_a) shr 16;\
//\1//OF := Ord(Longword(_a) > $FFFF);\
//\1//CF := Ord(Longword(_a) > $FFFF);\
//\1// SF, ZF, AF, PF: undefined\
// 32-bits operand:\
//\1_a := QWord(Longword(eax) * Longword(\2));\t// _a: QWord\
//\1eax := QWord(_a) and $FFFFFFFF;\
//\1edx := QWord(_a) shr 32;\
//\1//OF := Ord(QWord(_a) > $FFFFFFFF);\
//\1//CF := Ord(QWord(_a) > $FFFFFFFF);\
//\1// SF, ZF, AF, PF: undefined\
!

s!^([[:space:]]+)imul[[:space:]]+(ax|cx|dx|bx|sp|bp|si|di)[[:space:]]*,[[:space:]]*([^,]+)[[:space:]]*,[[:space:]]*(.+)!\1_a := Longint(Smallint(\3) * Smallint(Shortint(\4)));\t//\0\
// or:\
\1//_a := Longint(Smallint(\3) * Smallint(\4));\t// _a: Longint\
\1\2 := Longint(_a) and $FFFF;\
\1//if ((Longint(_a) >= -$8000) and (Longint(_a) <= $7FFF)) then begin\
\1//  OF := 0;\
\1//  CF := 0;\
\1//end;\
\1// SF, ZF, AF, PF: undefined\
!
s!^([[:space:]]+)imul[[:space:]]+(eax|ecx|edx|ebx|esp|ebp|esi|edi)[[:space:]]*,[[:space:]]*([^,]+)[[:space:]]*,[[:space:]]*(.+)!\1_a := Int64(Longint(\3) * Longint(Shortint(\4)));\t//\0\
// or:\
\1//_a := Int64(Longint(\3) * Longint(\4));\t// _a: Int64\
\1\2 := Int64(_a) and $FFFFFFFF;\
\1//if ((Int64(_a) >= -$80000000) and (Int64(_a) <= $7FFFFFFF)) then begin\
\1//  OF := 0;\
\1//  CF := 0;\
\1//end;\
\1// SF, ZF, AF, PF: undefined\
!
s!^([[:space:]]+)imul[[:space:]]+(ax|cx|dx|bx|sp|bp|si|di)[[:space:]]*,[[:space:]]*(.+)!\1_a := Longint(Smallint(\2) * Smallint(\3));\t//\0\
\1\2 := Longint(_a) and $FFFF;\
\1//if ((Longint(_a) >= -$8000) and (Longint(_a) <= $7FFF)) then begin\
\1//  OF := 0;\
\1//  CF := 0;\
\1//end;\
\1// SF, ZF, AF, PF: undefined\
!
s!^([[:space:]]+)imul[[:space:]]+(eax|ecx|edx|ebx|esp|ebp|esi|edi)[[:space:]]*,[[:space:]]*(.+)!\1_a := Int64(Longint(\2) * Longint(\3));\t//\0\
\1\2 := Int64(_a) and $FFFFFFFF;\
\1//if ((Int64(_a) >= -$80000000) and (Int64(_a) <= $7FFFFFFF)) then begin\
\1//  OF := 0;\
\1//  CF := 0;\
\1//end;\
\1// SF, ZF, AF, PF: undefined\
!
s!^([[:space:]]+)imul[[:space:]]+(al|cl|dl|bl|ah|ch|dh|bh)!\1ax := Smallint(Shortint(al)) * Smallint(Shortint(\2));\t//\0\
\1//if ((Smallint(ax) >= -$80) and (Smallint(ax) <= $7F)) then begin\
\1//  OF := 0;\
\1//  CF := 0;\
\1//end;\
\1// SF, ZF, AF, PF: undefined\
!
s!^([[:space:]]+)imul[[:space:]]+(ax|cx|dx|bx|sp|bp|si|di)!\1_a := Longint(Smallint(ax) * Smallint(\2));\t//\0\
\1ax := Longint(_a) and $FFFF;\
\1dx := (Longint(_a) shr 16) and $FFFF;\
\1//if ((Longint(_a) >= -$8000) and (Longint(_a) <= $7FFF)) then begin\
\1//  OF := 0;\
\1//  CF := 0;\
\1//end;\
\1// SF, ZF, AF, PF: undefined\
!
s!^([[:space:]]+)imul[[:space:]]+(eax|ecx|edx|ebx|esp|ebp|esi|edi)!\1_a := Int64(Longint(eax) * Longint(\2));\t//\0\
\1eax := Int64(_a) and $FFFFFFFF;\
\1edx := (Int64(_a) shr 32) and $FFFFFFFF;\
\1//if ((Int64(_a) >= -$80000000) and (Int64(_a) <= $7FFFFFFF)) then begin\
\1//  OF := 0;\
\1//  CF := 0;\
\1//end;\
\1// SF, ZF, AF, PF: undefined\
!
s!^([[:space:]]+)imul[[:space:]]+(.+)!// 8-bits operand:\t//\0\
\1//ax := Smallint(Shortint(al)) * Smallint(Shortint((\2)^));\
\1//if ((Smallint(ax) >= -$80) and (Smallint(ax) <= $7F)) then begin\
\1//  OF := 0;\
\1//  CF := 0;\
\1//end;\
\1// SF, ZF, AF, PF: undefined\
// 16-bits operand:\
//\1//_a := Longint(Smallint(ax) * Smallint((\2)^));\
//\1//ax := Longint(_a) and $FFFF;\
//\1//dx := (Longint(_a) shr 16) and $FFFF;\
//\1//if ((Longint(_a) >= -$8000) and (Longint(_a) <= $7FFF)) then begin\
//\1//  OF := 0;\
//\1//  CF := 0;\
//\1//end;\
//\1// SF, ZF, AF, PF: undefined\
// 32-bits operand:\
//\1_a := Int64(Longint(eax) * Longint((\2)^));\
//\1eax := Int64(_a) and $FFFFFFFF;\
//\1edx := (Int64(_a) shr 32) and $FFFFFFFF;\
//\1//if ((Int64(_a) >= -$80000000) and (Int64(_a) <= $7FFFFFFF)) then begin\
//\1//  OF := 0;\
//\1//  CF := 0;\
//\1//end;\
//\1// SF, ZF, AF, PF: undefined\
!

s!^([[:space:]]+)div[[:space:]]+(al|cl|dl|bl|ah|ch|dh|bh)!\1_al := Word(ax) div Byte(\2);\t//\0\
\1_ah := Word(ax) mod Byte(\2);\
\1al := _al;\
\1ah := _ah;\
\1// OF, SF, ZF, AF, PF, CF: undefined!
s!^([[:space:]]+)div[[:space:]]+(ax|cx|dx|bx|sp|bp|si|di)!\1_ax := ((Longword(Word(dx)) shl 16)+Word(ax)) div Word(\2);\t//\0\
\1_dx := ((Longword(Word(dx)) shl 16)+Word(ax)) mod Word(\2);\
\1ax := _ax;\
\1dx := _dx;\
\1// OF, SF, ZF, AF, PF, CF: undefined!
s!^([[:space:]]+)div[[:space:]]+(eax|ecx|edx|ebx|esp|ebp|esi|edi)!\1_eax := ((QWord(Longword(edx)) shl 32)+Longword(eax)) div Longword(\2);\t//\0\
\1_edx := ((QWord(Longword(edx)) shl 32)+Longword(eax)) mod Longword(\2);\
\1eax := _eax;\
\1edx := _edx;\
\1// OF, SF, ZF, AF, PF, CF: undefined!
s!^([[:space:]]+)div[[:space:]]+(.+)!// 8-bits operand:\t//\0\
\1_al := Word(ax) div Byte(\2);\
\1_ah := Word(ax) mod Byte(\2);\
\1al := _al;\
\1ah := _ah;\
\1// OF, SF, ZF, AF, PF, CF: undefined\
// 16-bits operand:\
//\1_ax := ((Longword(Word(dx)) shl 16)+Word(ax)) div Word(\2);\
//\1_dx := ((Longword(Word(dx)) shl 16)+Word(ax)) mod Word(\2);\
//\1ax := _ax;\
//\1dx := _dx;\
//\1// OF, SF, ZF, AF, PF, CF: undefined\
// 32-bits operand:\
//\1_eax := ((QWord(Longword(edx)) shl 32)+Longword(eax)) div Longword(\2);\
//\1_edx := ((QWord(Longword(edx)) shl 32)+Longword(eax)) mod Longword(\2);\
//\1eax := _eax;\
//\1edx := _edx;\
//\1// OF, SF, ZF, AF, PF, CF: undefined\
!

s!^([[:space:]]+)idiv[[:space:]]+(al|cl|dl|bl|ah|ch|dh|bh)!\1_al := Smallint(ax) div Shortint(\2);\t//\0\
\1_ah := Smallint(ax) mod Shortint(\2);\
\1al := _al;\
\1ah := _ah;\
\1// OF, SF, ZF, AF, PF, CF: undefined!
s!^([[:space:]]+)idiv[[:space:]]+(ax|cx|dx|bx|sp|bp|si|di)!\1_ax := ((Longint(Smallint(dx)) shl 16)+Word(ax)) div Smallint(\2);\t//\0\
\1_dx := ((Longint(Smallint(dx)) shl 16)+Word(ax)) mod Smallint(\2);\
\1ax := _ax;\
\1dx := _dx;\
\1// OF, SF, ZF, AF, PF, CF: undefined!
s!^([[:space:]]+)idiv[[:space:]]+(eax|ecx|edx|ebx|esp|ebp|esi|edi)!\1_eax := ((Int64(Longint(edx)) shl 32)+Longword(eax)) div Longint(\2);\t//\0\
\1_edx := ((Int64(Longint(edx)) shl 32)+Longword(eax)) mod Longint(\2);\
\1eax := _eax;\
\1edx := _edx;\
\1// OF, SF, ZF, AF, PF, CF: undefined!

s!^([[:space:]]+)cmp[[:space:]]+([^,]+)[[:space:]]*,[[:space:]]*\2!\1//OF := 0;\t//\0\
\1//AF := 0;\
\1//CF := 0;\
\1//SF := 0;\
\1//ZF := 1;\
\1//PF := 1;!
s!^([[:space:]]+)cmp[[:space:]]+([^,]+)[[:space:]]*,[[:space:]]*(.+)!\1//OF := Sub_Get_OF (?{BITS}, \2, _b);\t//\0\
\1//AF := Sub_Get_AF (?{BITS}, \2, _b);\
\1//CF := Sub_Get_CF (?{BITS}, \2, _b);\
\1//SF := Get_SF (?{BITS}, \2 - _b);\
\1//ZF := Ord(\2 = _b);\
\1//PF := Get_PF (?{BITS}, \2 - _b);\
\1//if (\2 ? \3) then ?;!

s!^([[:space:]]+)push[[:space:]]+(.+)!\1PushReg (\2);\t//\0!
s!^([[:space:]]+)pop[[:space:]]+(.+)!\1PopReg (\2);\t//\0!
s!^([[:space:]]+)pusha$!\1PushAll (16);\t//\0!
s!^([[:space:]]+)popa$!\1PopAll (16);\t//\0!
s!^([[:space:]]+)pushad$!\1PushAll (32);\t//\0!
s!^([[:space:]]+)popad$!\1PopAll (32);\t//\0!

s!^([[:space:]]+)call[[:space:]]+(.+)!\1\2 (?);\t//\0!

s!^([[:space:]]+)int[[:space:]]+(.+)!\1{go32.realintr}dos.intr (\1, Regs);\t//\0!
s!^([[:space:]]+)into$!\1if ( OF = 1 ) then {go32.realintr}dos.intr (4, Regs);\t//\0!

s!^([[:space:]]+)jmp[[:space:]]+(.+)!\1goto \2;\t//\0\
//--\
!
s!^([[:space:]]+)jo[[:space:]]+(.+)!\1if ( OF = 1 ) then goto \2;\t//\0\
!
s!^([[:space:]]+)jno[[:space:]]+(.+)!\1if ( OF = 0 ) then goto \2;\t//\0\
!
s!^([[:space:]]+)(jb|jc|jnae)[[:space:]]+(.+)!\1if ( < {unsigned} ) then goto \3;\t//\0\
\1// jump if (CF = 1)\
!
s!^([[:space:]]+)(jae|jnb|jnc)[[:space:]]+(.+)!\1if ( >= {unsigned} ) then goto \3;\t//\0\
\1// jump if (CF = 0) \
!
s!^([[:space:]]+)(je|jz)[[:space:]]+(.+)!\1if ( = {0} ) then goto \3;\t//\0\
\1// jump if (ZF = 1)\
!
s!^([[:space:]]+)(jne|jnz)[[:space:]]+(.+)!\1if ( <> {0} ) then goto \3;\t//\0\
\1// jump if (ZF = 0)\
!
s!^([[:space:]]+)(jbe|jna)[[:space:]]+(.+)!\1if ( <= {unsigned} ) then goto \3;\t//\0\
\1// jump if ((CF = 1) or (ZF = 1))\
!
s!^([[:space:]]+)(ja|jnbe)[[:space:]]+(.+)!\1if ( > {unsigned} ) then goto \3;\t//\0\
\1// jump if ((CF = 0) and (ZF = 0))\
!
s!^([[:space:]]+)js[[:space:]]+(.+)!\1if ( SF = 1 ) then goto \2;\t//\0\
!
s!^([[:space:]]+)jns[[:space:]]+(.+)!\1if ( SF = 0 ) then goto \2;\t//\0\
!
s!^([[:space:]]+)(jp|jpe)[[:space:]]+(.+)!\1if ( PF = 1 {even} ) then goto \3;\t//\0\
!
s!^([[:space:]]+)(jpo|jnp)[[:space:]]+(.+)!\1if ( PF = 0 {odd} ) then goto \3;\t//\0\
!
s!^([[:space:]]+)(jl|jnge)[[:space:]]+(.+)!\1if ( < {signed} ) then goto \3;\t//\0\
\1// jump if (SF <> OF)\
!
s!^([[:space:]]+)(jge|jnl)[[:space:]]+(.+)!\1if ( >= {signed} ) then goto \3;\t//\0\
\1// jump if (SF = OF)\
!
s!^([[:space:]]+)(jle|jng)[[:space:]]+(.+)!\1if ( <= {signed} ) then goto \3;\t//\0\
\1// jump if ((ZF = 1) or (SF <> OF))\
!
s!^([[:space:]]+)(jg|jnle)[[:space:]]+(.+)!\1if ( > {signed} ) then goto \3;\t//\0\
\1// jump if ((ZF = 0) and (SF = OF))\
!
s!^([[:space:]]+)jcxz[[:space:]]+(.+)!\1if (cx = 0) then goto \2;\t//\0\
!
s!^([[:space:]]+)jecxz[[:space:]]+(.+)!\1if (ecx = 0) then goto \2;\t//\0\
!
s!^([[:space:]]+)loop[[:space:]]+(.+)!\1Dec (ecx);\t//\0\
\1if (ecx <> 0) then goto \2;\
!
s!^([[:space:]]+)(loope|loopz)[[:space:]]+(.+)!\1Dec (ecx);\t//\0\
\1if ((ZF = 1) and (ecx <> 0)) then goto \3;\
!
s!^([[:space:]]+)(loopne|loopnz)[[:space:]]+(.+)!\1Dec (ecx);\t//\0\
\1if ((ZF = 0) and (ecx <> 0)) then goto \3;\
!
s!^([[:space:]]+)lodsb$!\1al := Byte(Pointer(esi)^);\t//\0\
\1if (DF) then\
\1  Dec (Pointer(esi))\
\1else\
\1  Inc (Pointer(esi));!
s!^([[:space:]]+)lodsw$!\1ax := Word(Pointer(esi)^);\t//\0\
\1if (DF) then\
\1  Dec (Pointer(esi), 2)\
\1else\
\1  Inc (Pointer(esi), 2);!
s!^([[:space:]]+)lodsd$!\1eax := Longword(Pointer(esi)^);\t//\0\
\1if (DF) then\
\1  Dec (Pointer(esi), 4)\
\1else\
\1  Inc (Pointer(esi), 4);!

s!^([[:space:]]+)(rep|repe|repz|repne|repnz)[[:space:]]+lodsb$!\1if (DF) then begin\t//\0\
\1  if (ecx) then begin\
\1    al := Byte((Pointer(esi)-ecx+1)^);\
\1    Dec (Pointer(esi), ecx);\
\1  end\
\1end else\
\1  if (ecx) then begin\
\1    al := Byte((Pointer(esi)+ecx-1)^);\
\1    Inc (Pointer(esi), ecx);\
\1  end;\
\1ecx := 0;!
s!^([[:space:]]+)(rep|repe|repz|repne|repnz)[[:space:]]+lodsw$!\1if (DF) then begin\t//\0\
\1  if (ecx) then begin\
\1    ax := Word((Pointer(esi)-(ecx-1)*2)^);\
\1    Dec (Pointer(esi), ecx*2);\
\1  end\
\1end else\
\1  if (ecx) then begin\
\1    ax := Word((Pointer(esi)+(ecx-1)*2)^);\
\1    Inc (Pointer(esi), ecx*2);\
\1  end;\
\1ecx := 0;!
s!^([[:space:]]+)(rep|repe|repz|repne|repnz)[[:space:]]+lodsd$!\1if (DF) then begin\t//\0\
\1  if (ecx) then begin\
\1    eax := Word((Pointer(esi)-(ecx-1)*4)^);\
\1    Dec (Pointer(esi), ecx*4);\
\1  end\
\1end else\
\1  if (ecx) then begin\
\1    eax := Word((Pointer(esi)+(ecx-1)*4)^);\
\1    Inc (Pointer(esi), ecx*4);\
\1  end;\
\1ecx := 0;!

s!^([[:space:]]+)stosb$!\1Byte(Pointer(edi)^) := al;\t//\0\
\1if (DF) then\
\1  Dec (Pointer(edi))\
\1else\
\1  Inc (Pointer(edi));!
s!^([[:space:]]+)stosw$!\1Word(Pointer(edi)^) := ax;\t//\0\
\1if (DF) then\
\1  Dec (Pointer(edi), 2)\
\1else\
\1  Inc (Pointer(edi), 2);!
s!^([[:space:]]+)stosd$!\1Longword(Pointer(edi)^) := eax;\t//\0\
\1if (DF) then\
\1  Dec (Pointer(edi), 4)\
\1else\
\1  Inc (Pointer(edi), 4);!

s!^([[:space:]]+)(rep|repe|repz|repne|repnz)[[:space:]]+stosb$!\1if (DF) then\t//\0\
\1  FillChar ((Pointer(edi)-ecx+1)^, ecx, al)\
\1else\
\1  FillChar (Pointer(edi)^, ecx, al);!
s!^([[:space:]]+)(rep|repe|repz|repne|repnz)[[:space:]]+stosw$!\1if (DF) then\t//\0\
\1  FillWord ((Pointer(edi)-ecx*2+2)^, ecx, ax)\
\1else\
\1  FillWord (Pointer(edi)^, ecx, ax);!
s!^([[:space:]]+)(rep|repe|repz|repne|repnz)[[:space:]]+stosd$!\1if (DF) then\t//\0\
\1  FillDWord ((Pointer(edi)-ecx*4+4)^, ecx, eax)\
\1else\
\1  FillDWord (Pointer(edi)^, ecx, eax);!

s!^([[:space:]]+)movsb$!\1Byte(Pointer(edi)^) := Byte(Pointer(esi)^);\t//\0\
\1if (DF) then begin\
\1  Dec (Pointer(esi));\
\1  Dec (Pointer(edi));\
\1end else begin\
\1  Inc (Pointer(esi));\
\1  Inc (Pointer(edi));\
\1end;!
s!^([[:space:]]+)movsw$!\1Word(Pointer(edi)^) := Word(Pointer(esi)^);\t//\0\
\1if (DF) then begin\
\1  Dec (Pointer(esi), 2);\
\1  Dec (Pointer(edi), 2);\
\1end else begin\
\1  Inc (Pointer(esi), 2);\
\1  Inc (Pointer(edi), 2);\
\1end;!
s!^([[:space:]]+)movsd$!\1Longword(Pointer(edi)^) := Longword(Pointer(esi)^);\t//\0\
\1if (DF) then begin\
\1  Dec (Pointer(esi), 4);\
\1  Dec (Pointer(edi), 4);\
\1end else begin\
\1  Inc (Pointer(esi), 4);\
\1  Inc (Pointer(edi), 4);\
\1end;!
s!^([[:space:]]+)(rep|repe|repz|repne|repnz)[[:space:]]+movsb$!\1if (DF) then\t//\0\
\1  while (ecx) do begin\
\1    Byte(Pointer(edi)^) := Byte(Pointer(esi)^);\
\1    Dec (Pointer(esi));\
\1    Dec (Pointer(edi));\
\1    Dec (ecx);\
\1  end\
\1else\
\1  Move (Pointer(esi)^, Pointer(edi)^, ecx);!
s!^([[:space:]]+)(rep|repe|repz|repne|repnz)[[:space:]]+movsw$!\1if (DF) then\t//\0\
\1  while (ecx) do begin\
\1    Word(Pointer(edi)^) := Word(Pointer(esi)^);\
\1    Dec (Pointer(esi), 2);\
\1    Dec (Pointer(edi), 2);\
\1    Dec (ecx);\
\1  end\
\1else\
\1  Move (Pointer(esi)^, Pointer(edi)^, ecx*2);!
s!^([[:space:]]+)(rep|repe|repz|repne|repnz)[[:space:]]+movsd$!\1if (DF) then\t//\0\
\1  while (ecx) do begin\
\1    Longword(Pointer(edi)^) := Longword(Pointer(esi)^);\
\1    Dec (Pointer(esi), 4);\
\1    Dec (Pointer(edi), 4);\
\1    Dec (ecx);\
\1  end\
\1else\
\1  Move (Pointer(esi)^, Pointer(edi)^, ecx*4);!

s!^([[:space:]]+)scasb!\1_b := Byte(Pointer(edi)^);\t//\0\
\1//OF := Sub_Get_OF (8, al, _b);\
\1//AF := Sub_Get_AF (8, al, _b);\
\1//CF := Sub_Get_CF (8, al, _b);\
\1//SF := Get_SF (8, al - _b);\
\1//ZF := Ord(al = _b);\
\1//PF := Get_PF (8, al - _b);\
\1if (DF) then\
\1  Dec (Pointer(edi))\
\1else\
\1  Inc (Pointer(edi));\
\1//if (al ? _b) then ?;!
s!^([[:space:]]+)scasw!\1_b := Word(Pointer(edi)^);\t//\0\
\1//OF := Sub_Get_OF (16, ax, _b);\
\1//AF := Sub_Get_AF (16, ax, _b);\
\1//CF := Sub_Get_CF (16, ax, _b);\
\1//SF := Get_SF (16, ax - _b);\
\1//ZF := Ord(ax = _b);\
\1//PF := Get_PF (16, ax - _b);\
\1if (DF) then\
\1  Dec (Pointer(edi))\
\1else\
\1  Inc (Pointer(edi));\
\1//if (ax ? _b) then ?;!
s!^([[:space:]]+)scasd!\1_b := Longword(Pointer(edi)^);\t//\0\
\1//OF := Sub_Get_OF (32, eax, _b);\
\1//AF := Sub_Get_AF (32, eax, _b);\
\1//CF := Sub_Get_CF (32, eax, _b);\
\1//SF := Get_SF (32, eax - _b);\
\1//ZF := Ord(eax = _b);\
\1//PF := Get_PF (32, eax - _b);\
\1if (DF) then\
\1  Dec (Pointer(edi))\
\1else\
\1  Inc (Pointer(edi));\
\1//if (eax ? _b) then ?;!

s!^([[:space:]]+)(rep|repe|repz)[[:space:]]+scasb!\1while (ecx <> 0) do begin\t//\0\
\1  _b := Byte(Pointer(edi)^);\
\1  //OF := Sub_Get_OF (8, al, _b);\
\1  //AF := Sub_Get_AF (8, al, _b);\
\1  //CF := Sub_Get_CF (8, al, _b);\
\1  //SF := Get_SF (8, al - _b);\
\1  //ZF := Ord(al = _b);\
\1  //PF := Get_PF (8, al - _b);\
\1  if (DF) then\
\1    Dec (Pointer(edi))\
\1  else\
\1    Inc (Pointer(edi));\
\1  Dec (ecx);\
\1  if (al = _b) then break;\
\1end;!
s!^([[:space:]]+)(rep|repe|repz)[[:space:]]+scasw!\1while (ecx <> 0) do begin\t//\0\
\1  _b := Word(Pointer(edi)^);\
\1  //OF := Sub_Get_OF (16, ax, _b);\
\1  //AF := Sub_Get_AF (16, ax, _b);\
\1  //CF := Sub_Get_CF (16, ax, _b);\
\1  //SF := Get_SF (16, ax - _b);\
\1  //ZF := Ord(ax = _b);\
\1  //PF := Get_PF (16, ax - _b);\
\1  if (DF) then\
\1    Dec (Pointer(edi), 2)\
\1  else\
\1    Inc (Pointer(edi), 2);\
\1  Dec (ecx);\
\1  if (ax = _b) then break;\
\1end;!
s!^([[:space:]]+)(rep|repe|repz)[[:space:]]+scasd!\1while (ecx <> 0) do begin\t//\0\
\1  _b := Longword(Pointer(edi)^);\
\1  //OF := Sub_Get_OF (32, eax, _b);\
\1  //AF := Sub_Get_AF (32, eax, _b);\
\1  //CF := Sub_Get_CF (32, eax, _b);\
\1  //SF := Get_SF (32, eax - _b);\
\1  //ZF := Ord(eax = _b);\
\1  //PF := Get_PF (32, eax - _b);\
\1  if (DF) then\
\1    Dec (Pointer(edi), 4)\
\1  else\
\1    Inc (Pointer(edi), 4);\
\1  Dec (ecx);\
\1  if (eax = _b) then break;\
\1end;!

s!^([[:space:]]+)(repne|repnz)[[:space:]]+scasb!\1while (ecx <> 0) do begin\t//\0\
\1  _b := Byte(Pointer(edi)^);\
\1  //OF := Sub_Get_OF (8, al, _b);\
\1  //AF := Sub_Get_AF (8, al, _b);\
\1  //CF := Sub_Get_CF (8, al, _b);\
\1  //SF := Get_SF (8, al - _b);\
\1  //ZF := Ord(al = _b);\
\1  //PF := Get_PF (8, al - _b);\
\1  if (DF) then\
\1    Dec (Pointer(edi))\
\1  else\
\1    Inc (Pointer(edi));\
\1  Dec (ecx);\
\1  if (al <> _b) then break;\
\1end;!
s!^([[:space:]]+)(repne|repnz)[[:space:]]+scasw!\1while (ecx <> 0) do begin\t//\0\
\1  _b := Word(Pointer(edi)^);\
\1  //OF := Sub_Get_OF (16, ax, _b);\
\1  //AF := Sub_Get_AF (16, ax, _b);\
\1  //CF := Sub_Get_CF (16, ax, _b);\
\1  //SF := Get_SF (16, ax - _b);\
\1  //ZF := Ord(ax = _b);\
\1  //PF := Get_PF (16, ax - _b);\
\1  if (DF) then\
\1    Dec (Pointer(edi), 2)\
\1  else\
\1    Inc (Pointer(edi), 2);\
\1  Dec (ecx);\
\1  if (ax <> _b) then break;\
\1end;!
s!^([[:space:]]+)(repne|repnz)[[:space:]]+scasd!\1while (ecx <> 0) do begin\t//\0\
\1  _b := Longword(Pointer(edi)^);\
\1  //OF := Sub_Get_OF (32, eax, _b);\
\1  //AF := Sub_Get_AF (32, eax, _b);\
\1  //CF := Sub_Get_CF (32, eax, _b);\
\1  //SF := Get_SF (32, eax - _b);\
\1  //ZF := Ord(eax = _b);\
\1  //PF := Get_PF (32, eax - _b);\
\1  if (DF) then\
\1    Dec (Pointer(edi), 4)\
\1  else\
\1    Inc (Pointer(edi), 4);\
\1  Dec (ecx);\
\1  if (eax <> _b) then break;\
\1end;!

s!^([[:space:]]+)cmpsb!\1_a := Byte(Pointer(esi)^);\t//\0\
\1_b := Byte(Pointer(edi)^);\
\1//OF := Sub_Get_OF (8, _a, _b);\
\1//AF := Sub_Get_AF (8, _a, _b);\
\1//CF := Sub_Get_CF (8, _a, _b);\
\1//SF := Get_SF (8, _a - _b);\
\1//ZF := Ord(_a = _b);\
\1//PF := Get_PF (8, _a - _b);\
\1if (DF) then begin\
\1  Dec (Pointer(esi));\
\1  Dec (Pointer(edi));\
\1end else begin\
\1  Inc (Pointer(esi));\
\1  Inc (Pointer(edi));\
\1end;\
\1if (_a ? _b) then ?;!
s!^([[:space:]]+)cmpsw!\1_a := Word(Pointer(esi)^);\t//\0\
\1_b := Word(Pointer(edi)^);\
\1//OF := Sub_Get_OF (16, _a, _b);\
\1//AF := Sub_Get_AF (16, _a, _b);\
\1//CF := Sub_Get_CF (16, _a, _b);\
\1//SF := Get_SF (16, _a - _b);\
\1//ZF := Ord(_a = _b);\
\1//PF := Get_PF (16, _a - _b);\
\1if (DF) then begin\
\1  Dec (Pointer(esi));\
\1  Dec (Pointer(edi));\
\1end else begin\
\1  Inc (Pointer(esi));\
\1  Inc (Pointer(edi));\
\1end;\
\1if (_a ? _b) then ?;!
s!^([[:space:]]+)cmpsd!\1_a := Longword(Pointer(esi)^);\t//\0\
\1_b := Longword(Pointer(edi)^);\
\1//OF := Sub_Get_OF (32, _a, _b);\
\1//AF := Sub_Get_AF (32, _a, _b);\
\1//CF := Sub_Get_CF (32, _a, _b);\
\1//SF := Get_SF (32, _a - _b);\
\1//ZF := Ord(_a = _b);\
\1//PF := Get_PF (32, _a - _b);\
\1if (DF) then begin\
\1  Dec (Pointer(esi));\
\1  Dec (Pointer(edi));\
\1end else begin\
\1  Inc (Pointer(esi));\
\1  Inc (Pointer(edi));\
\1end;\
\1if (_a ? _b) then ?;!

s!^([[:space:]]+)(rep|repe|repz)[[:space:]]+cmpsb!\1while (ecx <> 0) do begin\t//\0\
\1  _a := Byte(Pointer(esi)^);\
\1  _b := Byte(Pointer(edi)^);\
\1  //OF := Sub_Get_OF (8, _a, _b);\
\1  //AF := Sub_Get_AF (8, _a, _b);\
\1  //CF := Sub_Get_CF (8, _a, _b);\
\1  //SF := Get_SF (8, _a - _b);\
\1  //ZF := Ord(_a = _b);\
\1  //PF := Get_PF (8, _a - _b);\
\1  if (DF) then begin\
\1    Dec (Pointer(esi));\
\1    Dec (Pointer(edi));\
\1  end else begin\
\1    Inc (Pointer(esi));\
\1    Inc (Pointer(edi));\
\1  end;\
\1  Dec (ecx);\
\1  if (_a = _b) then break;\
\1end;!
s!^([[:space:]]+)(rep|repe|repz)[[:space:]]+cmpsw!\1while (ecx <> 0) do begin\t//\0\
\1  _a := Word(Pointer(esi)^);\
\1  _b := Word(Pointer(edi)^);\
\1  //OF := Sub_Get_OF (16, _a, _b);\
\1  //AF := Sub_Get_AF (16, _a, _b);\
\1  //CF := Sub_Get_CF (16, _a, _b);\
\1  //SF := Get_SF (16, _a - _b);\
\1  //ZF := Ord(_a = _b);\
\1  //PF := Get_PF (16, _a - _b);\
\1  if (DF) then begin\
\1    Dec (Pointer(esi), 2);\
\1    Dec (Pointer(edi), 2);\
\1  end else begin\
\1    Inc (Pointer(esi), 2);\
\1    Inc (Pointer(edi), 2);\
\1  end;\
\1  Dec (ecx);\
\1  if (_a = _b) then break;\
\1end;!
s!^([[:space:]]+)(rep|repe|repz)[[:space:]]+cmpsd!\1while (ecx <> 0) do begin\t//\0\
\1  _a := Longword(Pointer(esi)^);\
\1  _b := Longword(Pointer(edi)^);\
\1  //OF := Sub_Get_OF (32, _a, _b);\
\1  //AF := Sub_Get_AF (32, _a, _b);\
\1  //CF := Sub_Get_CF (32, _a, _b);\
\1  //SF := Get_SF (32, _a - _b);\
\1  //ZF := Ord(_a = _b);\
\1  //PF := Get_PF (32, _a - _b);\
\1  if (DF) then begin\
\1    Dec (Pointer(esi), 4);\
\1    Dec (Pointer(edi), 4);\
\1  end else begin\
\1    Inc (Pointer(esi), 4);\
\1    Inc (Pointer(edi), 4);\
\1  end;\
\1  Dec (ecx);\
\1  if (_a = _b) then break;\
\1end;!
s!^([[:space:]]+)(repne|repnz)[[:space:]]+cmpsb!\1while (ecx <> 0) do begin\t//\0\
\1  _a := Byte(Pointer(esi)^);\
\1  _b := Byte(Pointer(edi)^);\
\1  //OF := Sub_Get_OF (8, _a, _b);\
\1  //AF := Sub_Get_AF (8, _a, _b);\
\1  //CF := Sub_Get_CF (8, _a, _b);\
\1  //SF := Get_SF (8, _a - _b);\
\1  //ZF := Ord(_a = _b);\
\1  //PF := Get_PF (8, _a - _b);\
\1  if (DF) then begin\
\1    Dec (Pointer(esi));\
\1    Dec (Pointer(edi));\
\1  end else begin\
\1    Inc (Pointer(esi));\
\1    Inc (Pointer(edi));\
\1  end;\
\1  Dec (ecx);\
\1  if (_a <> _b) then break;\
\1end;!
s!^([[:space:]]+)(repne|repnz)[[:space:]]+cmpsw!\1while (ecx <> 0) do begin\t//\0\
\1  _a := Word(Pointer(esi)^);\
\1  _b := Word(Pointer(edi)^);\
\1  //OF := Sub_Get_OF (16, _a, _b);\
\1  //AF := Sub_Get_AF (16, _a, _b);\
\1  //CF := Sub_Get_CF (16, _a, _b);\
\1  //SF := Get_SF (16, _a - _b);\
\1  //ZF := Ord(_a = _b);\
\1  //PF := Get_PF (16, _a - _b);\
\1  if (DF) then begin\
\1    Dec (Pointer(esi), 2);\
\1    Dec (Pointer(edi), 2);\
\1  end else begin\
\1    Inc (Pointer(esi), 2);\
\1    Inc (Pointer(edi), 2);\
\1  end;\
\1  Dec (ecx);\
\1  if (_a <> _b) then break;\
\1end;!
s!^([[:space:]]+)(repne|repnz)[[:space:]]+cmpsd!\1while (ecx <> 0) do begin\t//\0\
\1  _a := Longword(Pointer(esi)^);\
\1  _b := Longword(Pointer(edi)^);\
\1  //OF := Sub_Get_OF (32, _a, _b);\
\1  //AF := Sub_Get_AF (32, _a, _b);\
\1  //CF := Sub_Get_CF (32, _a, _b);\
\1  //SF := Get_SF (32, _a - _b);\
\1  //ZF := Ord(_a = _b);\
\1  //PF := Get_PF (32, _a - _b);\
\1  if (DF) then begin\
\1    Dec (Pointer(esi), 4);\
\1    Dec (Pointer(edi), 4);\
\1  end else begin\
\1    Inc (Pointer(esi), 4);\
\1    Inc (Pointer(edi), 4);\
\1  end;\
\1  Dec (ecx);\
\1  if (_a <> _b) then break;\
\1end;!

s!^([[:space:]]+)in[[:space:]]+al[[:space:]]*,[[:space:]]*(.+)!\1al := go32.inportb(\2);\t//\0!
s!^([[:space:]]+)in[[:space:]]+ax[[:space:]]*,[[:space:]]*(.+)!\1ax := go32.inportw(\2);\t//\0!
s!^([[:space:]]+)in[[:space:]]+eax[[:space:]]*,[[:space:]]*(.+)!\1eax := go32.inportl(\2);\t//\0!

s!^([[:space:]]+)out[[:space:]]+([^,]+)[[:space:]]*,[[:space:]]*al$!\1go32.outportb(\2, al);\t//\0!
s!^([[:space:]]+)out[[:space:]]+([^,]+)[[:space:]]*,[[:space:]]*ax$!\1go32.outportw(\2, ax);\t//\0!
s!^([[:space:]]+)out[[:space:]]+([^,]+)[[:space:]]*,[[:space:]]*eax$!\1go32.outportl(\2, eax);\t//\0!
