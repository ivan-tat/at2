// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

unit Pascal;

{$IFDEF GO32V2}
{$L pascal/dos.o}
{$L pascal/dpmi.o}
{$L pascal/go32.o}
{$L pascal/pc.o}
{$ENDIF}
{$L pascal/stdio.o}
{$L pascal/stdlib.o}
{$L pascal/string.o}

interface

{$IFDEF GO32V2}
uses
  go32;
{$ENDIF}

const
  PUBLIC_PREFIX = {$IF DEFINED(GO32V2) OR DEFINED(WINDOWS)} '_' {$ELSE} '' {$ENDIF};

var
  Pascal_Output: Pointer; cvar;
  Pascal_InOutRes_ptr: ^Word; cvar;

procedure Pascal_Halt (errnum: Longint); cdecl;

procedure Pascal_FillChar (var x; count: SizeInt; value: Byte); cdecl;
procedure Pascal_FillWord (var x; count: SizeInt; value: Word); cdecl;

procedure Pascal_Move (const src; var dest; n: SizeInt); cdecl;

function Pascal_strlen (const s: PChar): SizeInt; cdecl;

procedure Pascal_Delay (ms: Word); cdecl;

// FPC_TRUNC_REAL
function Pascal_Trunc_Single (x: Single): Single; cdecl;
function Pascal_Trunc_Double (x: Double): Double; cdecl;

function Pascal_AllocMem (size: PtrUInt): Pointer; cdecl;
function Pascal_FreeMem (p: Pointer): PtrUInt; cdecl;
function Pascal_ReAllocMem (var p: pointer; size: PtrUInt): Pointer; cdecl;

procedure Pascal_Write_PChar (var t: Text; str: PChar); cdecl;
procedure Pascal_Write_String (var t: Text; str: String); cdecl;
procedure Pascal_Flush (var t: Text); cdecl;

{$IFDEF GO32V2}

function Pascal_allocate_ldt_descriptors (count: Word): Word; cdecl;
function Pascal_free_ldt_descriptor (selector: Word): Boolean; cdecl;
function Pascal_get_next_selector_increment_value: Word; cdecl;
function Pascal_set_segment_base_address (selector: Word; address: Longint): Boolean; cdecl;
function Pascal_set_segment_limit (selector: Word; limit: Longint): Boolean; cdecl;
procedure Pascal_dosmemget (seg: Word; ofs: Word; var data; count: Longint); cdecl;
procedure Pascal_dosmemput (seg: Word; ofs: Word; var data; count: Longint); cdecl;
function Pascal_global_dos_alloc (bytes: Longint): Longint; cdecl;
function Pascal_global_dos_free (selector: Word): Boolean; cdecl;
function Pascal_realintr (intnr: Word; var regs: trealregs): Boolean; cdecl;
function Pascal_get_linear_addr(phys_addr: Longint; size: Longint): Longint; cdecl;

{$I pascal/dos.pas}
{$I pascal/dpmi.pas}
{$I pascal/go32.pas}
{$I pascal/pc.pas}

{$ENDIF}

{$I pascal/stdio.pas}
{$I pascal/stdlib.pas}
{$I pascal/string.pas}

implementation

uses
  crt,
  strings;

procedure Pascal_Halt (errnum: Longint); cdecl;
public name PUBLIC_PREFIX + 'Pascal_Halt';
begin
  Halt (errnum);
end;

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

function Pascal_Trunc_Single (x: Single): Single; cdecl;
public name PUBLIC_PREFIX + 'Pascal_Trunc_Single';
begin
  Pascal_Trunc_Single := Trunc (x);
end;

function Pascal_Trunc_Double (x: Double): Double; cdecl;
public name PUBLIC_PREFIX + 'Pascal_Trunc_Double';
begin
  Pascal_Trunc_Double := Trunc (x);
end;

function Pascal_AllocMem (size: PtrUInt): Pointer; cdecl
public name PUBLIC_PREFIX + 'Pascal_AllocMem';
begin
  Pascal_AllocMem := system.AllocMem (size);
end;

function Pascal_FreeMem (p: Pointer): PtrUInt; cdecl;
public name PUBLIC_PREFIX + 'Pascal_FreeMem';
begin
  Pascal_FreeMem := system.FreeMem (p);
end;

function Pascal_ReAllocMem (var p: pointer; size: PtrUInt): Pointer; cdecl;
public name PUBLIC_PREFIX + 'Pascal_ReAllocMem';
begin
  Pascal_ReAllocMem := system.ReAllocMem (p, size);
end;

procedure Pascal_Write_PChar (var t: Text; str: PChar); cdecl;
public name PUBLIC_PREFIX + 'Pascal_Write_PChar';
begin
  {$PUSH} {$I-}
  system.Write (t, str);
  {$POP}
end;

procedure Pascal_Write_String (var t: Text; str: String); cdecl;
public name PUBLIC_PREFIX + 'Pascal_Write_String';
begin
  {$PUSH} {$I-}
  system.Write (t, str);
  {$POP}
end;

procedure Pascal_Flush (var t: Text); cdecl;
public name PUBLIC_PREFIX + 'Pascal_Flush';
begin
  {$PUSH} {$I-}
  system.Flush (t);
  {$POP}
end;

{$IFDEF GO32V2}

function Pascal_allocate_ldt_descriptors (count: Word): Word; cdecl;
public name PUBLIC_PREFIX + 'Pascal_allocate_ldt_descriptors';
begin
  Pascal_allocate_ldt_descriptors := go32.allocate_ldt_descriptors(count);
end;

function Pascal_free_ldt_descriptor (selector: Word): Boolean; cdecl;
public name PUBLIC_PREFIX + 'Pascal_free_ldt_descriptor';
begin
  Pascal_free_ldt_descriptor := go32.free_ldt_descriptor (selector);
end;

function Pascal_get_next_selector_increment_value: Word; cdecl;
public name PUBLIC_PREFIX + 'Pascal_get_next_selector_increment_value';
begin
  Pascal_get_next_selector_increment_value := go32.get_next_selector_increment_value;
end;

function Pascal_set_segment_base_address (selector: Word; address: Longint): Boolean; cdecl;
public name PUBLIC_PREFIX + 'Pascal_set_segment_base_address';
begin
  Pascal_set_segment_base_address := go32.set_segment_base_address (selector, address);
end;

function Pascal_set_segment_limit (selector: Word; limit: Longint): Boolean; cdecl;
public name PUBLIC_PREFIX + 'Pascal_set_segment_limit';
begin
  Pascal_set_segment_limit := go32.set_segment_limit (selector, limit);
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

function Pascal_global_dos_alloc (bytes: Longint): Longint; cdecl;
public name PUBLIC_PREFIX + 'Pascal_global_dos_alloc';
begin
  Pascal_global_dos_alloc := go32.global_dos_alloc (bytes);
end;

function Pascal_global_dos_free (selector: Word): Boolean; cdecl;
public name PUBLIC_PREFIX + 'Pascal_global_dos_free';
begin
  Pascal_global_dos_free := go32.global_dos_free (selector);
end;

function Pascal_realintr (intnr: Word; var regs: trealregs): Boolean; cdecl;
public name PUBLIC_PREFIX + 'Pascal_realintr';
begin
  Pascal_realintr := go32.realintr (intnr, regs);
end;

function Pascal_get_linear_addr(phys_addr: Longint; size: Longint): Longint; cdecl;
public name PUBLIC_PREFIX + 'Pascal_get_linear_addr';
begin
  Pascal_get_linear_addr := go32.get_linear_addr (phys_addr, size);
end;

{$ENDIF}

var
  OldExitProc: Pointer;

procedure OnExit;
begin
  done_stdio;
  done_stdlib;
  ExitProc := OldExitProc;
end;

begin
{$IFDEF GO32V2}
  __dpmi_error_ptr := @go32.int31error;
  _dos_ds_ptr := @go32.dosmemselector;
{$ENDIF}
  Pascal_Output := @system.Output;
  Pascal_InOutRes_ptr := @system.InOutRes;
  OldExitProc := ExitProc;
  ExitProc := @OnExit;
  init_stdlib;
  init_stdio;
end.
