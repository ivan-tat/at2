// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function shift_pressed: Boolean;
begin
  shift_pressed := (keyboard_flag OR 1 = keyboard_flag) or
                   (keyboard_flag OR 2 = keyboard_flag);
end;
