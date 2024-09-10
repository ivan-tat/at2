// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function _macro_speedup: Word;
begin
  If (macro_speedup > 0) then _macro_speedup := macro_speedup
  else _macro_speedup := macro_speedup+1;
end;
