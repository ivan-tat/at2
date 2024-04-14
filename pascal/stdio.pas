// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

var
  custom_stdout: Pointer; cvar; external;

procedure custom_printf; cdecl; external;
procedure custom_vprintf; cdecl; external;
procedure custom_fprintf; cdecl; external;
procedure custom_vfprintf; cdecl; external;
procedure custom_sprintf; cdecl; external;
procedure custom_vsprintf; cdecl; external;
procedure custom_snprintf; cdecl; external;
procedure custom_vsnprintf; cdecl; external;
procedure init_stdio; cdecl; external;
procedure done_stdio; cdecl; external;
