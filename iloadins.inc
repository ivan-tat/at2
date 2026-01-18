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

procedure import_standard_instrument_alt (var ins: tADTRACK2_INS; var data); cdecl; external;
procedure import_hsc_instrument_alt (var ins: tADTRACK2_INS; var data); cdecl; external;
procedure apply_instrument (idx: Byte; var src: temp_instrument_t); cdecl; external;

function a2i_file_loader_alt (var dst: temp_instrument_t; fname: String; swap_ins: Boolean; var error: PChar): Shortint; cdecl; external;
function a2f_file_loader_alt (var dst: temp_instrument_t; fname: String; swap_ins: Boolean; var error: PChar): Shortint; cdecl; external;
function cif_file_loader_alt (var dst: temp_instrument_t; fname: String; var error: PChar): Shortint; cdecl; external;
procedure import_fin_instrument_alt (var ins: tADTRACK2_INS; var data); cdecl; external; // FIN
function fin_file_loader_alt (var dst: temp_instrument_t; fname: String; var error: PChar): Shortint; cdecl; external;
procedure import_sat_instrument_alt (var ins: tADTRACK2_INS; var data); cdecl; external; // INS
function ins_file_loader_alt (var dst: temp_instrument_t; fname: String; var error: PChar): Shortint; cdecl; external;
function sbi_file_loader_alt (var dst: temp_instrument_t; fname: String; var error: PChar): Shortint; cdecl; external;
function sgi_file_loader_alt (var dst: temp_instrument_t; fname: String; var error: PChar): Shortint; cdecl; external;
