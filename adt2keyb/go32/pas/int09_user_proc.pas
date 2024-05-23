{$ASMMODE INTEL}

procedure int09_user_proc; assembler;
asm
        push    eax
        push    ebx
        push    es
        push    ds
        call    process_global_keys
        pop     ds
        pop     es
        mov     dword ptr [seconds_counter],0
        in      al,60h
        xor     ebx,ebx
        mov     bx,ax
        and     bx,007fh
        and     al,80h
        jz      @@4
@@1:    mov     byte ptr keydown[ebx],0
        cmp     ebx,1dh // [Ctrl]
        jnz     @@3
        cmp     byte ptr [_ctrl_pressed],1
        jnz     @@2
        mov     byte ptr [_2x_ctrl_pressed],1
@@2:    mov     byte ptr [_ctrl_pressed],1
@@3:    jmp     @@5
@@4:    mov     byte ptr keydown[ebx],1
        cmp     ebx,1dh // [Ctrl]
        jz      @@5
        mov     byte ptr [_ctrl_pressed],0
        mov     byte ptr [_2x_ctrl_pressed],0
@@5:    cmp     keyboard_sleep,1
        jz      @@10
        cmp     byte ptr keydown[1dh],1 // [Ctrl]
        jnz     @@6
        cmp     byte ptr keydown[38h],1 // [Alt]
        jnz     @@6
        cmp     byte ptr keydown[4ah],1 // *[-]
        jz      @@10
        cmp     byte ptr keydown[4eh],1 // *[+]
        jz      @@10
@@6:    cmp     byte ptr keydown[1dh],1 // [Ctrl]
        jnz     @@7
        cmp     byte ptr keydown[38h],1 // [Alt]
        jnz     @@7
        cmp     byte ptr keydown[53h],1 // [Del]
        jz      @@9
@@7:    cmp     byte ptr keydown[1dh],1 // [Ctrl]
        jnz     @@8
        cmp     byte ptr keydown[02h],1 // *[1]
        jz      @@10
        cmp     byte ptr keydown[03h],1 // *[2]
        jz      @@10
        cmp     byte ptr keydown[04h],1 // *[3]
        jz      @@10
        cmp     byte ptr keydown[05h],1 // *[4]
        jz      @@10
        cmp     byte ptr keydown[06h],1 // *[5]
        jz      @@10
        cmp     byte ptr keydown[07h],1 // *[6]
        jz      @@10
        cmp     byte ptr keydown[08h],1 // *[7]
        jz      @@10
        cmp     byte ptr keydown[09h],1 // *[8]
        jz      @@10
@@8:    jmp     @@11
@@9:    mov     CTRL_ALT_DEL_pressed,1
@@10:   in      al,61h
        mov     ah,al
        or      al,80h
        out     61h,al
        xchg    ah,al
        out     61h,al
        mov     al,20h
        out     20h,al
        pop     ebx
        pop     eax
        jmp     @@12
@@11:   pop     ebx
        pop     eax
@@12:
end;

procedure int09_user_proc_end; begin end;
