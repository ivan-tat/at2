// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

var
  le76,le77: Byte;
  le6a,le6c,le6e,le70,le72,le74,le78,
  le82a,le82b: Word;

const
  le7a: array[0..4] of Word = ($1ff,$3ff,$7ff,$0fff,$1fff);

var
  stack: array[WORD] of Byte;
  ax,bx,cx,sp: Word;
  edi,td: Dword;

procedure NextCode;

label j2;

begin
    bx := le82a;                    //        mov     bx,le82a
    ax := le82b;                    //        mov     ax,le82b
    td := (ax SHL 16)+bx;           //        add     bx,le78
    td := td + le78;                //        adc     ax,0
    le82a := td AND $ffff;          //        xchg    bx,le82a
    le82b := td SHR 16;             //        xchg    ax,le82b
    cx := bx AND 7;                 //        mov     cx,bx
    td := (ax SHL 16)+bx;           //        and     cx,7
    td := td SHR 1;                 //        shr     ax,1
                                    //        rcr     bx,1
    td := td SHR 1;                 //        shr     ax,1
                                    //        rcr     bx,1
    td := td SHR 1;                 //        shr     ax,1
    bx := td;                       //        rcr     bx,1
    td := input_ptr[bx]+            //        mov     esi,input_ptr
          (input_ptr[bx+1] shl 8)+  //        mov     ax,[ebx+esi]
          (input_ptr[bx+2] shl 16); //        mov     dl,[ebx+esi+2]
    If (cx = 0) then                //        or      cx,cx
      GOTO j2;                      //        jz      @@2
    While (cx <> 0) do
      begin                         //@@1:    shr     dl,1
        td := td SHR 1; Dec(cx);    //        rcr     ax,1
      end;                          //        loop    @@1
j2: bx := le78;                     //@@2:    mov     bx,le78
    Dec(bx,9);                      //        sub     bx,9
                                    //        shl     bx,1
    ax:=td AND le7a[bx];            //        and     ax,[ebx+le7a_0]
end;

procedure LZW_decode;

label
  j1,j2,j3,j4,j5,j7,j8,j9;

begin
    sp := PRED(SizeOf(stack));
    le72 := 0;                                 //        mov     le72,0
    le78 := 9;                                 //        mov     le78,9
    le70 := $102;                              //        mov     le70,102h
    le74 := $200;                              //        mov     le74,200h
    edi := 0;                                  //        mov     edi,output_ptr
    ax := 0;                                   //        xor     eax,eax
    le6a := 0;                                 //        mov     le6a,ax
    le6c := 0;                                 //        mov     le6c,ax
    le6e := 0;                                 //        mov     le6e,ax
    le76 := 0;                                 //        mov     le76,al
    le77 := 0;                                 //        mov     le77,al
    le82a := 0;                                //        mov     le82a,ax
    le82b := 0;                                //        mov     le82b,ax
j1: NextCode;                                  //@@1:    call    NextCode
    If (ax <> $101) then                       //        cmp     ax,101h
      GOTO j2;                                 //        jnz     @@2
    GOTO j9;                                   //        jmp     @@9
j2: If (ax <> $100) then                       //@@2:    cmp     ax,100h
      GOTO j3;                                 //        jnz     @@3
    le78 := 9;                                 //        mov     le78,9
    le74 := $200;                              //        mov     le74,200h
    le70 := $102;                              //        mov     le70,102h
    NextCode;                                  //        caj    NextCode
    le6a := ax;                                //        mov     le6a,ax
    le6c := ax;                                //        mov     le6c,ax
    le77 := ax;                                //        mov     le77,al
    le76 := ax;                                //        mov     le76,al
                                               //        mov     al,le77
    output_ptr[edi] := ax;                     //        mov     byte ptr [edi],al
    Inc(edi);                                  //        inc     edi
    GOTO j1;                                   //        jmp     @@1
j3: le6a := ax;                                //@@3:    mov     le6a,ax
    le6e := ax;                                //        mov     le6e,ax
    If (ax < le70) then                        //        cmp     ax,le70
      GOTO j4;                                 //        jb      @@4
    ax := le6c;                                //        mov     ax,le6c
    le6a := ax;                                //        mov     le6a,ax
    ax := (ax AND $ff00)+le76;                 //        mov     al,le76
    Dec(sp); stack[sp] := ax;                  //        push    eax
    Inc(le72);                                 //        inc     le72
j4: If (le6a <= $ff) then                      //@@4:    cmp     le6a,0ffh
      GOTO j5;                                 //        jbe     @@5
                                               //        mov     esi,work_ptr
                                               //        mov     bx,le6a
    bx := le6a*3;                              //        shl     bx,1
    ax := (ax AND $ff00)+work_ptr[bx+2];       //        add     bx,le6a
    Dec(sp);                                   //        mov     al,[ebx+esi+2]
    stack[sp] := ax;                           //        push    eax
    Inc(le72);                                 //        inc     le72
    ax := work_ptr[bx]+(work_ptr[bx+1] SHL 8); //        mov     ax,[ebx+esi]
    le6a := ax;                                //        mov     le6a,ax
    GOTO j4;                                   //        jmp     @@4
j5: ax := le6a;                                //@@5:    mov     ax,le6a
    le76 := ax;                                //        mov     le76,al
    le77 := ax;                                //        mov     le77,al
    Dec(sp); stack[sp] := ax;                  //        push    eax
    Inc(le72);                                 //        inc     le72
                                               //        xor     ecx,ecx
    cx := le72;                                //        mov     cx,le72
    If (cx = 0) then GOTO j7;                  //        jecxz   @@7
    While (cx <> 0) do                         //
      begin                                    //
        ax := stack[sp]; Inc(sp);              //@@6:    pop     eax
        output_ptr[edi] := ax;                 //        mov     byte ptr [edi],al
        Inc(edi); Dec(cx);                     //        inc     edi
      end;                                     //        loop    @@6
j7: le72 := 0;                                 //@@7:    mov     le72,0
                                               //        push    esi
                                               //        mov     bx,le70
                                               //        shl     bx,1
    bx:=le70*3;                                //        add     bx,le70
                                               //        mov     esi,work_ptr
                                               //        mov     al,le77
    work_ptr[bx+2] := le77;                    //        mov     [ebx+esi+2],al
    work_ptr[bx+1] := le6c SHR 8;              //        mov     ax,le6c
    work_ptr[bx+0] := le6c;                    //        mov     [ebx+esi],ax
    Inc(le70);                                 //        inc     le70
                                               //        pop     esi
    ax := le6e;                                //        mov     ax,le6e
    le6c := ax;                                //        mov     le6c,ax
    bx := le70;                                //        mov     bx,le70
    If (bx < le74) then                        //        cmp     bx,le74
      GOTO j8;                                 //        jl      @@8
    If (le78 = 14) then                        //        cmp     le78,14
      GOTO j8;                                 //        jz      @@8
    Inc(le78);                                 //        inc     le78
    le74 := le74 SHL 1;                        //        shl     le74,1
j8: GOTO j1;                                   //@@8:    jmp     @@1
j9: output_size := edi;                        //@@9:    mov     output_size,ax
end;

function LZW_decompress(var source,dest): Word;
begin
  input_ptr := @source;
  output_ptr := @dest;
  work_ptr := @work_mem;
  LZW_decode;
  LZW_decompress := output_size;
end;
