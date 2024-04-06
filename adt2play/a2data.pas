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
//  along with AT2. If not, see <http://www.gnu.org/licenses/>.

unit A2data;
{$S-,Q-,R-,V-,B-,X+}
{$PACKRECORDS 1}
{$L a2data.o}
interface

var
  font8x8: array[0..2047] of Byte; cvar; external;
  font8x16: array[0..4095] of Byte; cvar; external;
  picture_palette: array[0..767] of Byte; cvar; external;
  picture_bitmap: array[0..63999] of Byte; cvar; external;

implementation

end.
