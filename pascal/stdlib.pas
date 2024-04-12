// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

procedure custom_atexit; cdecl; external;
procedure custom_free; cdecl; external;
procedure custom_realloc; cdecl; external;
procedure init_stdlib; cdecl; external;
procedure done_stdlib; cdecl; external;
