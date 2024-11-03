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

unit debug;
{$S-,Q-,R-,V-,B-,X+}
{$PACKRECORDS 1}
{$MODESWITCH CVAR}
{$L debug.o}
interface

var
  _debug_: Boolean; cvar; external;
  _dbg_verbose: Byte; cvar; external;

procedure _dbg_enter (_file: PChar; _func: PChar); cdecl; external;
procedure _dbg_label (_label: PChar); cdecl; external;
procedure _dbg_printf (format: PChar); cdecl; varargs; external;
procedure _dbg_leave; cdecl; external;
procedure _dbg_dump_callstack; cdecl; external;

implementation

uses
  pascal;

end.
