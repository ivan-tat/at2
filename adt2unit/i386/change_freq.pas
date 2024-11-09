// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure change_freq(chan: Byte; freq: Word);
begin
  If is_4op_chan(chan) and (chan in _4op_tracks_hi) then
    begin
      freq_table[SUCC(chan)] := freq_table[chan];
{$IFNDEF ADT2PLAY}
      freqtable2[SUCC(chan)] := freqtable2[chan];
{$ENDIF} // NOT DEFINED(ADT2PLAY)
      chan := SUCC(chan);
    end;

  asm
        xor     ebx,ebx
        mov     bl,chan
        dec     ebx
        shl     ebx,1
        mov     ax,freq
        and     ax,1fffh
        mov     dx,word ptr [freq_table+ebx]
        and     dx,NOT 1fffh
        add     ax,dx
        mov     word ptr [freq_table+ebx],ax
{$IFNDEF ADT2PLAY}
        mov     word ptr [freqtable2+ebx],ax
        shr     ebx,1
        cmp     byte ptr [channel_flag+ebx],TRUE
        jnz     @@1
        shl     ebx,1
{$ENDIF} // NOT DEFINED(ADT2PLAY)
        xor     edx,edx
        mov     dx,word ptr [_chan_n+ebx]
        add     dx,0a0h
        push    edx
        xor     edx,edx
        mov     dl,al
        push    edx
        mov     dx,word ptr [_chan_n+ebx]
        add     dx,0b0h
        push    edx
        xor     edx,edx
        mov     dl,ah
        push    edx
        call    opl3out
        call    opl3out
@@1:
  end;

  If is_4op_chan(chan) then
    begin
      freq_table[PRED(chan)] := freq_table[chan];
{$IFNDEF ADT2PLAY}
      freqtable2[PRED(chan)] := freqtable2[chan];
{$ENDIF} // NOT DEFINED(ADT2PLAY)
    end;
end;
