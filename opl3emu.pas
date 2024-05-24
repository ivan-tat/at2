//  This file is part of Adlib Tracker II (AT2).
//
//  AT2 is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  AT2 is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with AT2.  If not, see <http://www.gnu.org/licenses/>.
//
//  ------------------------------------------------------------------
//  OPL3 EMULATOR
//  Based on NukedOPL3 1.6 by Nuke.YKT (Alexey Khokholov)
//  Special thanks to insane/Altair for initial C to Pascal conversion
//  ------------------------------------------------------------------

unit OPL3EMU;
{$S-,Q-,R-,V-,B-,X+}
{$PACKRECORDS 1}
{$L opl3emu.o}
interface

procedure OPL3EMU_init; cdecl; external;
procedure OPL3EMU_WriteReg(reg: Word; data: Byte); cdecl; external;
procedure OPL3EMU_PollProc(p_data: pDword; var ch_table); cdecl; external;

implementation

uses
  pascal;

end.
