// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

unit dpmi;

{$L go32/dpmi.o}

interface

procedure _go32_dpmi_unlock_code; cdecl; external;
procedure _go32_dpmi_unlock_data; cdecl; external;

implementation

uses
  pascal;

end.
