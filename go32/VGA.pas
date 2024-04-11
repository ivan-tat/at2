// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

unit VGA;

{$L go32/VGA.o}

interface

procedure VGA_WaitRetrace; cdecl; external;
procedure VGA_GetPaletteEntry; cdecl; external;
procedure VGA_GetPalette; cdecl; external;
procedure VGA_SetPaletteEntry; cdecl; external;
procedure VGA_SetPalette; cdecl; external;
procedure VGA_SplitScreen; cdecl; external;

implementation

end.
