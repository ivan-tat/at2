// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

unit platform;
{$S-,Q-,R-,V-,B-,X+}
{$PACKRECORDS 1}
{$MODESWITCH CVAR}
{$L platform.o}
interface

{$IFNDEF ADT2PLAY}

var
  PATHSEP: Char; cvar; external;
  WILDCARD_ASTERISK: String[3]; cvar; external;

{$ENDIF} // NOT DEFINED(ADT2PLAY)

implementation

end.
