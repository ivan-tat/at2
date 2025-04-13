// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

type
  pBYTE_ARRAY = ^tBYTE_ARRAY;
  tBYTE_ARRAY = array[0..PRED(SizeOf(DWORD))] of Byte;

procedure draw_SDL_screen(columns,rows: Byte; scr_res: Dword);

var
   bit_pos,bit_mask,byte_pos: Byte;
   pos_x,pos_y,chr,attr: Byte;
   s_ofs,v_ofs,skip: Dword;

begin
  If (_cursor_blink_pending_frames >= _cursor_blink_factor) then
    begin
      _cursor_blink_pending_frames := 0;
      cursor_sync := NOT cursor_sync;
    end;

  s_ofs := 0;
  v_ofs := 0;
  skip := virtual_screen__first_row;

  For pos_y := 1 to rows do
    begin
      For byte_pos := 0 to 15 do
        begin
          For pos_x := 1 to columns do
            begin
              chr := pBYTE_ARRAY(screen_ptr)^[s_ofs];
              attr := pBYTE_ARRAY(screen_ptr)^[s_ofs+1];
              bit_mask := pBYTE_ARRAY(Addr(font8x16))^[chr*16+byte_pos];
              For bit_pos := 8 downto 1 do
                begin
                  If (skip <> 0) then
                    Dec(skip)
                  else If (v_ofs < scr_res) then
                         begin
                           If cursor_sync and
                              (pos_x = LO(virtual_cur_pos)) and (pos_y = HI(virtual_cur_pos)) and
                              (byte_pos >= HI(virtual_cur_shape)) and (byte_pos <= LO(virtual_cur_shape)) then
                             pBYTE_ARRAY(_FrameBuffer)^[v_ofs] := attr AND $0f
                           else If (bit_mask OR (1 SHL bit_pos) SHR 1 = bit_mask) then
                                  pBYTE_ARRAY(_FrameBuffer)^[v_ofs] := attr AND $0f
                                else pBYTE_ARRAY(_FrameBuffer)^[v_ofs] := attr SHR 4;
                           Inc(v_ofs);
                         end;
                end;
              Inc(s_ofs,2);
            end;
          Dec(s_ofs,columns*2);
        end;
      Inc(s_ofs,columns*2);
    end;
end;
