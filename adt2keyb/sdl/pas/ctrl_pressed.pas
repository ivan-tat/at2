// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function ctrl_pressed: Boolean;
begin
  ctrl_pressed := scankey(SC_LCTRL) or scankey(SC_RCTRL);
end;
