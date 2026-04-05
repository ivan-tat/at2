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

// This file is included in "adt2ext3.pas" in interface section

function a2m_file_loader (_fname: String; progress: progress_callback_p; var state: Byte; var error: PChar): Shortint; cdecl; external;
function a2t_file_loader (_fname: String; progress: progress_callback_p; var state: Byte; var error: PChar): Shortint; cdecl; external;
{$IFNDEF ADT2PLAY}
function a2p_file_loader (_fname: String; progress: progress_callback_p; var state: Byte; var error: PChar): Shortint; cdecl; external;
{$ENDIF}
function amd_file_loader (_fname: String; progress: progress_callback_p; var state: Byte; var error: PChar): Shortint; cdecl; external;
function cff_file_loader (_fname: String; progress: progress_callback_p; var state: Byte; var error: PChar): Shortint; cdecl; external;
function dfm_file_loader (_fname: String; progress: progress_callback_p; var state: Byte; var error: PChar): Shortint; cdecl; external;
function fmk_file_loader (_fname: String; var desc: Pointer; progress: progress_callback_p; var state: Byte; var error: PChar): Shortint; cdecl; external;
function hsc_file_loader (_fname: String; progress: progress_callback_p; var state: Byte; var error: PChar): Shortint; cdecl; external;
function mtk_file_loader (_fname: String; progress: progress_callback_p; var state: Byte; var error: PChar): Shortint; cdecl; external;
function rad_file_loader (_fname: String; var desc: Pointer; progress: progress_callback_p; var state: Byte; var error: PChar): Shortint; cdecl; external;
function s3m_file_loader (_fname: String; progress: progress_callback_p; var state: Byte; var error: PChar): Shortint; cdecl; external;
function sat_file_loader (_fname: String; progress: progress_callback_p; var state: Byte; var error: PChar): Shortint; cdecl; external;
