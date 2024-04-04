// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

unit Pascal;

{$IFDEF GO32V2}
{$L pascal/dos.o}
{$L pascal/go32.o}
{$L pascal/pc.o}
{$ENDIF}
{$L pascal/string.o}

interface

const
  PUBLIC_PREFIX = {$IF DEFINED(GO32V2) OR DEFINED(WINDOWS)} '_' {$ELSE} '' {$ENDIF};

procedure Pascal_FillChar (var x; count: SizeInt; value: Byte); cdecl;
procedure Pascal_FillWord (var x; count: SizeInt; value: Word); cdecl;

procedure Pascal_Move (const src; var dest; n: SizeInt); cdecl;

function Pascal_strlen (const s: PChar): SizeInt; cdecl;

procedure Pascal_Delay (ms: Word); cdecl;

{$IFDEF GO32V2}

function Pascal_dosmemselector: Word; cdecl;
procedure Pascal_dosmemget (seg: Word; ofs: Word; var data; count: Longint); cdecl;
procedure Pascal_dosmemput (seg: Word; ofs: Word; var data; count: Longint); cdecl;

{$ENDIF}

{$I pascal/dos.pas}
{$I pascal/go32.pas}
{$I pascal/pc.pas}
{$I pascal/string.pas}

implementation

uses
{$IFDEF GO32V2}
  go32,
{$ENDIF}
  crt,
  strings;

procedure Pascal_FillChar (var x; count: SizeInt; value: Byte); cdecl;
public name PUBLIC_PREFIX + 'Pascal_FillChar';
begin
  FillChar (x, count, value);
end;

procedure Pascal_FillWord (var x; count: SizeInt; value: Word); cdecl;
public name PUBLIC_PREFIX + 'Pascal_FillWord';
begin
  FillWord (x, count, value);
end;

procedure Pascal_Move (const src; var dest; n: SizeInt); cdecl;
public name PUBLIC_PREFIX + 'Pascal_Move';
begin
  Move (src, dest, n);
end;

function Pascal_strlen (const s: PChar): SizeInt; cdecl;
public name PUBLIC_PREFIX + 'Pascal_strlen';
begin
  Pascal_strlen := strings.strlen (s);
end;

procedure Pascal_Delay (ms: Word); cdecl;
public name PUBLIC_PREFIX + 'Pascal_Delay';
begin
  crt.Delay (ms);
end;

{$IFDEF GO32V2}

function Pascal_dosmemselector: Word; cdecl;
public name PUBLIC_PREFIX + 'Pascal_dosmemselector';
begin
  Pascal_dosmemselector := go32.dosmemselector;
end;

procedure Pascal_dosmemget (seg: Word; ofs: Word; var data; count: Longint); cdecl;
public name PUBLIC_PREFIX + 'Pascal_dosmemget';
begin
  go32.dosmemget (seg, ofs, data, count);
end;

procedure Pascal_dosmemput (seg: Word; ofs: Word; var data; count: Longint); cdecl;
public name PUBLIC_PREFIX + 'Pascal_dosmemput';
begin
  go32.dosmemput (seg, ofs, data, count);
end;

{$ENDIF}

end.
