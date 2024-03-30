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

unit AdT2data;
{$S-,Q-,R-,V-,B-,X+}
{$PACKRECORDS 1}
{$L adt2data.o}
interface

var
  font8x16: array[0..4095] of Byte; cvar; external;
  vga_font8x16: array[0..4095] of Byte; cvar; external;
{$IFNDEF GO32V2}
  adt2_icon_bitmap: array[0..3125] of Byte; cvar; external;
{$ENDIF}

implementation

end.
