// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function shift_pressed: Boolean;
begin
  shift_pressed := scankey(SC_LSHIFT) or scankey(SC_RSHIFT);
end;
