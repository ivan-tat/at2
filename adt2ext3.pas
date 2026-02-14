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

function a2m_file_loader (_fname: String; progress: progress_callback_p; var state: Byte; var error: PChar): Shortint; cdecl; external;
function a2t_file_loader (_fname: String; progress: progress_callback_p; var state: Byte; var error: PChar): Shortint; cdecl; external;
procedure a2p_file_loader (progress: progress_callback_p);
procedure amd_file_loader;
procedure cff_file_loader;
procedure dfm_file_loader;
procedure fmk_file_loader;
procedure hsc_file_loader;
procedure mtk_file_loader;
procedure rad_file_loader;
procedure s3m_file_loader;
procedure sat_file_loader;
procedure sa2_file_loader;

procedure test_instrument_alt (chan: Byte; fkey: Word; loadMacros: Boolean; bankSelector: Boolean; loadArpVib: Boolean; test_ins1, test_ins2: Byte); cdecl; external;
procedure test_instrument_alt2 (var src: temp_instrument_t; chan: Byte; fkey: Word); cdecl; external;

implementation

uses
  debug,
  DOS,
  AdT2opl3,
  AdT2sys,
  AdT2keyb,
  AdT2extn,
  AdT2ext2,
  AdT2ext4,
  AdT2ext5,
  AdT2text,
  AdT2pack,
  StringIO,
  DialogIO,
  ParserIO,
  TxtScrIO,
  DepackIO;

{$I go32/PIT/pas/PIT_consts.inc}
{$i iloaders.inc}

end.
