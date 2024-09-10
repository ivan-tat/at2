// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function is_ins_adsr_data_empty(ins: Byte): Boolean;
begin
  is_ins_adsr_data_empty :=
    (ins_parameter(ins,5) SHR 4 = 0) and
    (ins_parameter(ins,4) SHR 4 = 0) and
    (ins_parameter(ins,5) AND $0f = 0) and
    (ins_parameter(ins,4) AND $0f = 0) and
    (ins_parameter(ins,7) SHR 4 = 0) and
    (ins_parameter(ins,6) SHR 4 = 0) and
    (ins_parameter(ins,7) AND $0f = 0) and
    (ins_parameter(ins,6) AND $0f = 0);
end;
