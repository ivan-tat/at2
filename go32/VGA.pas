// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

unit VGA;

{$L go32/VGA.o}

interface

{$PUSH}
{$PACKRECORDS 1}
type
  VGARegister_t = record
    port: Word;
    idx, val: Byte;
  end;
{$POP}

var
  v_ofs: Word; cvar; external;
  v_seg: Word; cvar; external;

implementation

end.
