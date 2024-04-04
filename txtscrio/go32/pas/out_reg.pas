// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure out_reg(reg: VGA_REGISTER);
begin
  Case (reg.port) of
    VGA_ATTR_WRITE_PORT:
      begin
        inportb(VGA_STATUS_1_PORT);
        outportb(VGA_ATTR_WRITE_PORT,reg.idx OR $20);
        outportb(VGA_ATTR_WRITE_PORT,reg.val);
      end;

    VGA_MISC_WRITE_PORT,
    VGA_ENABLE_PORT:
      outportb(reg.port,reg.val);

    else begin
           outportb(reg.port,reg.idx);
           outportb(reg.port+1,reg.val);
         end;
  end;
end;
