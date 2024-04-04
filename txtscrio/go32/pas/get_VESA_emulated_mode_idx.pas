// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function get_VESA_emulated_mode_idx: Byte;
begin
  get_VESA_emulated_mode_idx := min(comp_text_mode-2,0);
end;
