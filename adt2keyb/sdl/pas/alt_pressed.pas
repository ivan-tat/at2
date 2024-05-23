// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function alt_pressed: Boolean;
begin
  alt_pressed := scankey(SC_LALT) or scankey(SC_RALT);
end;
