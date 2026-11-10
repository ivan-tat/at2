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

function load_song_a2m (_fname: String; progress: progress_callback_p; var state: Byte; var error: PChar): Shortint; cdecl; external;
function load_song_a2t (_fname: String; progress: progress_callback_p; var state: Byte; var error: PChar): Shortint; cdecl; external;
{$IFNDEF ADT2PLAY}
function load_pattern_a2p (_fname: String; progress: progress_callback_p; var state: Byte; var error: PChar): Shortint; cdecl; external;
{$ENDIF}
function load_song_amd (_fname: String; progress: progress_callback_p; var state: Byte; var error: PChar): Shortint; cdecl; external;
function load_song_cff (_fname: String; progress: progress_callback_p; var state: Byte; var error: PChar): Shortint; cdecl; external;
function load_song_dfm (_fname: String; progress: progress_callback_p; var state: Byte; var error: PChar): Shortint; cdecl; external;
function load_song_fmk (_fname: String; var desc: Pointer; progress: progress_callback_p; var state: Byte; var error: PChar): Shortint; cdecl; external;
function load_song_hsc (_fname: String; progress: progress_callback_p; var state: Byte; var error: PChar): Shortint; cdecl; external;
function load_song_mtk (_fname: String; progress: progress_callback_p; var state: Byte; var error: PChar): Shortint; cdecl; external;
function load_song_rad (_fname: String; var desc: Pointer; progress: progress_callback_p; var state: Byte; var error: PChar): Shortint; cdecl; external;
function s3m_file_loader (_fname: String; progress: progress_callback_p; var state: Byte; var error: PChar): Shortint; cdecl; external;
function sat_file_loader (_fname: String; progress: progress_callback_p; var state: Byte; var error: PChar): Shortint; cdecl; external;
