// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function is_VESA_emulated_mode: Boolean;
begin
  is_VESA_emulated_mode := (program_screen_mode = 3) and
                           (comp_text_mode > 1);
end;
