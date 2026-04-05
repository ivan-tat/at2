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

unit AdT2ext3;
{$S-,Q-,R-,V-,B-,X+}
{$PACKRECORDS 1}
{$MODESWITCH CVAR}
{$L adt2ext3.o}
interface

uses
  AdT2unit;

{$i iloadins.inc}
{$i iloaders.inc}

procedure test_instrument_alt (chan: Byte; fkey: Word; loadMacros: Boolean; bankSelector: Boolean; loadArpVib: Boolean; test_ins1, test_ins2: Byte); cdecl; external;
procedure test_instrument_alt2 (var src: temp_instrument_t; chan: Byte; fkey: Word); cdecl; external;

implementation

end.
