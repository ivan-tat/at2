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

unit ParserIO;
{$S-,Q-,R-,V-,B-,X+}
{$PACKRECORDS 1}
{$L parserio.o}
interface

function Scan(var buf; skip,size: Longint; str: String): Longint; cdecl; external;
function SensitiveScan(var buf; skip,size: Longint; str: String): Longint; cdecl; external;
function Compare(var buf1,buf2; size: Longint): Boolean; cdecl; external;
function Empty(var buf; size: Longint): Boolean; cdecl; external;
function CountLines(var buf; size: Longint): Longint; cdecl; external;
function Update16(var buf; size: Longint; crc: Word): Word; cdecl; external;
function Update32(var buf; size: Longint; crc: Longint): Longint; cdecl; external;
procedure make_table_16bit; cdecl; external;
procedure make_table_32bit; cdecl; external;

implementation

begin
  make_table_16bit;
  make_table_32bit;
end.
