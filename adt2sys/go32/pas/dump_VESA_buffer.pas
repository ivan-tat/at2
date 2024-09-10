// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure dump_VESA_buffer(buffer_size: Longint);

var
  dumped_data_size,bank_data_size: Longint;
  current_bank: Byte;

begin
  If NOT _draw_screen_without_vsync then
    WaitRetrace;
  dumped_data_size := 0;
  current_bank := 0;
  While (dumped_data_size < buffer_size) do
    begin
      If (dumped_data_size+65536 <= buffer_size) then
        bank_data_size := 65536
      else bank_data_size := buffer_size-dumped_data_size;
      VESA_SwitchBank(current_bank);
      dosmemput($0a000,0,_FrameBuffer_mirror[dumped_data_size],bank_data_size);
      Inc(dumped_data_size,bank_data_size);
      Inc(current_bank);
    end;
end;
