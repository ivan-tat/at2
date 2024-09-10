// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure opl3exp(data: Word);
begin
  OPL3EMU_WriteReg((data AND $ff) OR $100,data SHR 8);
end;
