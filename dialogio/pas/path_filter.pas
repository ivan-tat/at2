// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Local to 'Fselect` function

function path_filter(path: String): String;
begin
  If (Length(path) > 3) and (path[Length(path)] = PATHSEP) then
    Delete(path,Length(path),1);
  path_filter := Upper_filename(path);
end;
