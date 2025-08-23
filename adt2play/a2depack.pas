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

unit A2depack;
{$S-,Q-,R-,V-,B-,X+}
{$PACKRECORDS 1}
{$L a2depack.o}
interface

// Compression algorithm: RDC
// Algorithm developed by Ed Ross
function RDC_decompress(var source,dest; size: Word): Word; cdecl; external;

// Compression algorithm: LZSS
// Algorithm developed by Lempel-Ziv-Storer-Szymanski
function LZSS_decompress(var source,dest; size: Word): Word; cdecl; external;

// Compression algorithm: LZW
// Algorithm developed by Lempel-Ziv-Welch
function LZW_decompress(var source,dest): Word; cdecl; external;

// Compression algorithm: SixPack
// Algorithm developed by Philip G. Gage
function SIXPACK_decompress(var source,dest; size: Word): Word; cdecl; external;

// Compression algorithm: aPack
// Algorithm developed by Joergen Ibsen
function APACK_decompress(var source,dest): Dword; cdecl; external;

// Compression algorithm: LZH
// Algorithm developed by Haruhiko Okomura & Haruyasu Yoshizaki
function LZH_decompress(var source,dest; size: Dword): Dword; cdecl; external;

implementation

end.
