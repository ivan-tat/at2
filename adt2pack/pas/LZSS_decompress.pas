// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

const
  N_BITS = 12;
  F_BITS = 4;
  THRESHOLD = 2;
  N = 1 SHL N_BITS;
  F = (1 SHL F_BITS)+THRESHOLD;

procedure LZSS_decode;

label
  j1,j2,j3,j4,j5;

var
  al,cl,ch,cf: Byte;
  dx: Word;
  ebx,edi: Dword;

procedure GetChar;
begin
  If (ibufCount < ibufSize) then
    begin
      al := input_ptr[ibufCount];
      Inc(ibufCount);
      cf := 0;
    end
  else
    cf := 1;
end;

procedure PutChar;
begin
  output_ptr[output_size] := al;
  Inc(output_size);
end;

begin
    ibufCount := 0;
    ibufSize := input_size;
    output_size := 0;
    ebx := 0;                        //        xor     ebx,ebx
    dx := 0;                         //        xor     edx,edx
    edi := N-F;                      //        mov     edi,N-F
j1: dx := dx SHR 1;                  //@@1:    shr     dx,1
    If (dx SHR 8 <> 0) then          //        or      dh,dh
      GOTO j2;                       //        jnz     @@2
    GetChar;                         //        call    GetChar
    If (cf = 1) then GOTO j5;        //        jc      @@5
    dx := $ff00 OR al;               //        mov     dh,0ffh
                                     //        mov     dl,al
j2: If (dx AND 1 = 0) then           //@@2:    test    dx,1
      GOTO j3;                       //        jz      @@3
    GetChar;                         //        call    GetChar
    If (cf = 1) then GOTO j5;        //        jc      @@5
                                     //        push    esi
    work_ptr[edi] := al;             //        mov     esi,work_ptr
                                     //        add     esi,edi
                                     //        mov     byte ptr [esi],al
                                     //        pop     esi
    edi := (edi+1) AND (N-1);        //        inc     edi
                                     //        and     edi,N-1
    PutChar;                         //        call    PutChar
    GOTO j1;                         //        jmp     @@1
j3: GetChar;                         //@@3:    call    GetChar
    If (cf = 1) then GOTO j5;        //        jc      @@5
    ch := al;                        //        mov     ch,al
    GetChar;                         //        call    GetChar
    If (cf = 1) then GOTO j5;        //        jc      @@5
                                     //        mov     bh,al
                                     //        mov     cl,4
    ebx := (al SHL 4) AND $ff00;     //        shr     bh,cl
    ebx := ebx OR ch;                //        mov     bl,ch
                                     //        mov     cl,al
                                     //        and     cl,0fh
    cl := (al AND $0f)+THRESHOLD;    //        add     cl,THRESHOLD
    Inc(cl);                         //        inc     cl
j4: ebx := ebx AND (N-1);            //@@4:    and     ebx,N-1
                                     //        push    esi
    al := work_ptr[ebx];             //        mov     esi,work_ptr
                                     //        mov     al,byte ptr [esi+ebx]
                                     //        add     esi,edi
    work_ptr[edi] := al;             //        mov     byte ptr [esi],al
                                     //        pop     esi
    Inc(edi);                        //        inc     edi
    edi := edi AND (N-1);            //        and     edi,N-1
    PutChar;                         //        call    PutChar
    Inc(ebx);                        //        inc     ebx
    Dec(cl);                         //        dec     cl
    If (cl <> 0) then GOTO j4;       //        jnz     @@4
    GOTO j1;                         //        jmp     @@1
j5:                                  //@@5:
end;

function LZSS_decompress(var source,dest; size: Word): Word;

begin
  input_ptr := @source;
  output_ptr := @dest;
  work_ptr := @work_mem;
  input_size := size;
  FillChar(work_ptr^,WORKMEM_SIZE,0);
  LZSS_decode;
  LZSS_decompress := output_size;
end;
