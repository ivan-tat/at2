// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function APACK_decompress(var source,dest): Dword;

var
  temp,res,swp,eax,ecx: Dword;
  tsi,esi,edi: pByte;
  ncf,cf,dl: Byte;

label
  j1,j2,j3,j4,j5,j6,j7,j8,j9,j10,j11,j12,j13,
  j14,j15,j16,j17,j18,j19,j20,j21,j22,j23,j24,j25;

begin
     esi := @source;                                  //       mov     esi,[source]
     edi := @dest;                                    //       mov     edi,[dest]
     temp := 0; res := 0;                             //       cld
     dl := $80;                                       //       mov     dl,80h
j1:  edi^ := esi^; Inc(esi); Inc(edi); Inc(res);      //@@1:   movsb
j2:  cf := dl SHR 7; dl := dl SHL 1;                  //@@2:   add     dl,dl
     If (dl <> 0) then GOTO j3;                       //       jnz     @@3
     dl := esi^;                                      //       mov     dl,[esi]
     Inc(esi);                                        //       inc     esi
     ncf := dl SHR 7; dl := (dl SHL 1)+cf; cf := ncf; //       adc     dl,dl
j3:  If (cf = 0) then GOTO j1;                        //@@3:   jnc     @@1
     ecx := 0;                                        //       xor     ecx,ecx
     cf := dl SHR 7; dl := dl SHL 1;                  //       add     dl,dl
     If (dl <> 0) then GOTO j4;                       //       jnz     @@4
     dl := esi^;                                      //       mov     dl,[esi]
     Inc(esi);                                        //       inc     esi
     ncf := dl SHR 7; dl := (dl SHL 1)+cf; cf := ncf; //       adc     dl,dl
j4:  If (cf =0 ) then GOTO j8;                        //@@4:   jnc     @@8
     eax := 0;                                        //       xor     eax,eax
     cf := dl SHR 7; dl := dl SHL 1;                  //       add     dl,dl
     If (dl <> 0) then GOTO j5;                       //       jnz     @@5
     dl := esi^;                                      //       mov     dl,[esi]
     Inc(esi);                                        //       inc     esi
     ncf := dl SHR 7; dl := (dl SHL 1)+cf; cf := ncf; //       adc     dl,dl
j5:  If (cf = 0) then GOTO j15;                       //@@5:   jnc     @@15
     Inc(ecx);                                        //       inc     ecx
     eax := $10;                                      //       mov     al,10h
j6:  cf := dl SHR 7; dl := (dl SHL 1);                //@@6:   add     dl,dl
     If (dl <> 0) then GOTO j7;                       //       jnz     @@7
     dl := esi^;                                      //       mov     dl,[esi]
     Inc(esi);                                        //       inc     esi
     ncf := dl SHR 7; dl:= (dl SHL 1)+cf; cf := ncf;  //       adc     dl,dl
j7:  ncf := (eax SHR 7) AND 1;
     eax := (eax AND $ffffff00)+BYTE((eax SHL 1)+cf);
     cf := ncf;                                       //@@7:   adc     al,al
     If (cf = 0) then GOTO j6;                        //       jnc     @@6
     If (eax <> 0) then GOTO j24;                     //       jnz     @@24
     edi^ := eax; Inc(edi); Inc(res);                 //       stosb
     GOTO j2;                                         //       jmp     @@2
j8:  Inc(ecx);                                        //@@8:   inc     ecx
j9:  cf := dl SHR 7; dl := dl SHL 1;                  //@@9:   add     dl,dl
     If (dl <> 0) then GOTO j10;                      //       jnz     @@10
     dl := esi^;                                      //       mov     dl,[esi]
     Inc(esi);                                        //       inc     esi
     ncf := dl SHR 7; dl := (dl SHL 1)+cf; cf := ncf; //       adc     dl,dl
j10: ecx := ecx+ecx+cf;                               //@@10:  adc     ecx,ecx
     cf := dl SHR 7; dl := dl SHL 1;                  //       add     dl,dl
     If (dl <> 0) then GOTO j11;                      //       jnz     @@11
     dl := esi^;                                      //       mov     dl,[esi]
     Inc(esi);                                        //       inc     esi
     ncf := dl SHR 7; dl := (dl SHL 1)+cf; cf := ncf; //       adc     dl,dl
j11: If (cf = 1) then GOTO j9;                        //@@11:  jc      @@9
     Dec(ecx);                                        //       dec     ecx
     Dec(ecx); If (ecx <> 0) then GOTO j16;           //       loop    @@16
     ecx := 0;                                        //       xor     ecx,ecx
     Inc(ecx);                                        //       inc     ecx
j12: cf := dl SHR 7; dl := dl SHL 1;                  //@@12:  add     dl,dl
     If (dl <> 0) then GOTO j13;                      //       jnz     @@13
     dl := esi^;                                      //       mov     dl,[esi]
     Inc(esi);                                        //       inc     esi
     ncf := dl SHR 7; dl := (dl SHL 1)+cf; cf := ncf; //       adc     dl,dl
j13: ecx := ecx+ecx+cf;                               //@@13:  adc     ecx,ecx
     cf := dl SHR 7; dl := dl SHL 1;                  //       add     dl,dl
     If (dl <> 0) then GOTO j14;                      //       jnz     @@14
     dl := esi^;                                      //       mov     dl,[esi]
     Inc(esi);                                        //       inc     esi
     ncf := dl SHR 7; dl := (dl SHL 1)+cf; cf := ncf; //       adc     dl,dl
j14: If (cf = 1) then GOTO j12;                       //@@14:  jc      @@12
     GOTO j23;                                        //       jmp     @@23
j15: eax := esi^; Inc(esi);                           //@@15:  lodsb
     cf := eax AND 1; eax := eax SHR 1;               //       shr     eax,1
     If (eax = 0) then GOTO j25;                      //       jz      @@25
     ecx := ecx+ecx+cf;                               //       adc     ecx,ecx
     GOTO j20;                                        //       jmp     @@20
j16: swp := eax; eax := ecx; ecx := swp;              //@@16:  xchg    eax,ecx
     Dec(eax);                                        //       dec     eax
     eax := (eax SHL 8)+esi^;                         //       shl     eax,8
     Inc(esi);                                        //       lodsb
     ecx := 0;                                        //       xor     ecx,ecx
     Inc(ecx);                                        //       inc     ecx
j17: cf := dl SHR 7; dl := dl SHL 1;                  //@@17:  add     dl,dl
     If (dl <> 0) then GOTO j18;                      //       jnz     @@18
     dl := esi^;                                      //       mov     dl,[esi]
     Inc(esi);                                        //       inc     esi
     ncf := dl SHR 7; dl := (dl SHL 1)+cf; cf := ncf; //       adc     dl,dl
j18: ecx := ecx+ecx+cf;                               //@@18:  adc     ecx,ecx
     cf := dl SHR 7; dl := dl SHL 1;                  //       add     dl,dl
     If (dl <> 0) then GOTO j19;                      //       jnz     @@19
     dl := esi^;                                      //       mov     dl,[esi]
     Inc(esi);                                        //       inc     esi
     ncf := dl SHR 7; dl := (dl SHL 1)+cf; cf := ncf; //       adc     dl,dl
j19: If (cf = 1) then GOTO j17;                       //@@19:  jc      @@17
     If (eax >= 32000) then                           //       cmp     eax,32000
       GOTO j20;                                      //       jae     @@20
     If (eax >= 1280) then                            //       cmp     ah,5
       GOTO j21;                                      //       jae     @@21
     If (eax > 127) then                              //       cmp     eax,7fh
       GOTO j22;                                      //       ja      @@22
j20: Inc(ecx);                                        //@@20:  inc     ecx
j21: Inc(ecx);                                        //@@21:  inc     ecx
j22: swp := temp; temp := eax; eax := swp;            //@@22:  xchg    eax,temp
j23: eax := temp;                                     //@@23:  mov     eax,temp
j24:                                                  //@@24:  push    esi
     tsi := edi;                                      //       mov     esi,edi
     Dec(tsi,eax);                                    //       sub     esi,eax
     While (ecx <> 0) do                              //
       begin                                          //
         edi^ := tsi^;                                //       rep     movsb
         Inc(tsi); Inc(edi); Inc(res);                //
         Dec(ecx);                                    //
       end;                                           //       pop     esi
     GOTO j2;                                         //       jmp     @@2
j25:                                                  //@@25:  sub     edi,[dest]
                                                      //       mov     result,edi
  APACK_decompress := res;
end;
