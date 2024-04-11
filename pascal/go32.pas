// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

var
  _dos_ds_ptr: ^Word;

procedure custom_dosmemget; cdecl; external;
procedure custom_dosmemput; cdecl; external;
