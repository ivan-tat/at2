// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure set_svga_txtmode_128x48;

var
  crt_address: Word;

begin
  // set VESA gfx mode 104h (1024x768)
  asm
      mov  ax,4f02h
      mov  bx,104h
      int  10h
  end;

  // rerogram CRT controller
  crt_address := MEMW[SEG0040:$63];
  asm
      cli
      mov  dx,crt_address
      // clear write protection for CRT register 0-7
      mov  al,11h        // vertical retrace end register bit 7 reset
      out  dx,al
      inc  dx
      in   al,dx
      and  al,7fh
      out  dx,al
      dec  dx
      mov  al,9
      out  dx,al
      inc  dx
      in   al,dx
      and  al,0e0h       // clear bits 0-4
      or   al,0fh        // set max scan line to 15
      out  dx,al
      dec  dx
      mov  ax,0e0ah
      out  dx,ax
      mov  ax,0f0bh
      out  dx,ax
      mov  al,17h       // mode control register
      out  dx,al
      inc  dx
      in   al,dx
      and  al,not 40h   // set byte mode
      out  dx,al
      dec  dx
      // restore write protection for CRT register 0-7
      mov  al,11h
      out  dx,al
      inc  dx
      in   al,dx
      or   al,80h
      out  dx,al
      dec  dx
      // write sequencer: make planes 2+3 write protected
      mov  dx,3c4h
      mov  al,2
      mov  ah,3
      out  dx,ax
      // set odd/even mode, reset chain 4, more than 64 kB
      mov  dx,3c4h
      mov  al,4
      mov  ah,2
      out  dx,ax
      // write graphics controller
      mov  dx,3ceh
      mov  ax,1005h  // set write mode 0, read mode 0, odd/even addressing
      out  dx,ax
      mov  dx,3ceh
      mov  al,6
      out  dx,al
      inc  dx
      in   al,dx
      and  al,0f0h
      or   al,0eh    // set B800h as base, set text mode, set odd/even
      out  dx,al
      // write attribute controller
      mov  dx,3cch
      in   al,dx
      mov  dx,3dah
      test al,1
      jnz  @@1
      mov  dx,3bah
@@1:  in   al,dx     // reset attribute controller
      mov  dx,3c0h
      mov  al,10h    // select mode register
      out  dx,al
      mov  al,0      // set text mode [bit 0=0]
      out  dx,al
      mov  al,20h    // turn screen on again
      out  dx,al
      sti
  end;

  MaxCol := 128;
  MaxLn := 48;
  MEM[SEG0040:$4a] := MaxCol;
  MEM[SEG0040:$84] := MaxLn-1;

  initialize;
  dosmemput(v_seg,v_ofs,screen_ptr^,MAX_SCREEN_MEM_SIZE);
end;
