// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function is_scrollable_screen_mode: Boolean;
begin
{$IFDEF GO32V2}
  is_scrollable_screen_mode :=
    (program_screen_mode = 0) or
        ((program_screen_mode = 3) and (comp_text_mode < 2)) or
    (is_VESA_emulated_mode and (get_VESA_emulated_mode_idx in [0,1]));
{$ELSE}
  is_scrollable_screen_mode := (program_screen_mode = 0);
{$ENDIF}
end;
