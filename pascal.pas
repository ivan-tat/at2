// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

unit Pascal;

{$IFDEF GO32V2}
{$L pascal/dos.o}
{$L pascal/dpmi.o}
{$L pascal/go32.o}
{$L pascal/pc.o}
{$ENDIF} {DEFINED(GO32V2)}
{$L pascal/stdio.o}
{$L pascal/stdlib.o}
{$L pascal/string.o}

interface

{$IFDEF GO32V2}
uses
  go32;
{$ENDIF} {DEFINED(GO32V2)}

const
  PUBLIC_PREFIX = {$IF DEFINED(GO32V2) OR DEFINED(WINDOWS)} '_' {$ELSE} '' {$ENDIF};

var
{$IFDEF GO32V2}
  Pascal_LFNSupport: Boolean; cvar;
{$ENDIF} {DEFINED(GO32V2)}
  Pascal_Output: Pointer; cvar;
  Pascal_InOutRes_ptr: ^Word; cvar;

procedure Pascal_Halt (errnum: Longint); cdecl;

procedure Pascal_FillChar (var x; count: SizeInt; value: Byte); cdecl;
procedure Pascal_FillWord (var x; count: SizeInt; value: Word); cdecl;

procedure Pascal_Move (const src; var dest; n: SizeInt); cdecl;

function Pascal_strlen (const s: PChar): SizeInt; cdecl;
function Pascal_strcopy (dest, src: PChar): PChar; cdecl;
function Pascal_strecopy (dest, src: PChar): PChar; cdecl;
function Pascal_strlcopy (dest, src: PChar; n: SizeInt): PChar; cdecl;
function Pascal_strcat (dest, src: PChar): PChar; cdecl;
function Pascal_strlcat (dest, src: PChar; n: SizeInt): PChar; cdecl;
function Pascal_strcomp (s1, s2: PChar): SizeInt; cdecl;
function Pascal_strlcomp (s1, s2: PChar; n: SizeInt): SizeInt; cdecl;
function Pascal_strnew (const s: PChar): PChar; cdecl;
function Pascal_strscan (const s: PChar; c: Char): PChar; cdecl;
function Pascal_strrscan (const s: PChar; c: Char): PChar; cdecl;
function Pascal_strpos (const haystack, needle: PChar): PChar; cdecl;
function Pascal_stripos (const haystack, needle: PChar): PChar; cdecl;

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

var
  __v2prt0_ds_alias: Word; cvar; external;
  Pascal_dosmemselector: Word; cvar;

function Pascal_allocate_ldt_descriptors (count: Word): Word; cdecl;
function Pascal_free_ldt_descriptor (selector: Word): Boolean; cdecl;
function Pascal_get_next_selector_increment_value: Word; cdecl;
function Pascal_set_segment_base_address (selector: Word; address: Longint): Boolean; cdecl;
function Pascal_set_segment_limit (selector: Word; limit: Longint): Boolean; cdecl;
procedure Pascal_dosmemget (seg: Word; ofs: Word; var data; count: Longint); cdecl;
procedure Pascal_dosmemput (seg: Word; ofs: Word; var data; count: Longint); cdecl;
function Pascal_global_dos_alloc (bytes: Longint): Longint; cdecl;
function Pascal_global_dos_free (selector: Word): Boolean; cdecl;
function Pascal_get_pm_interrupt (vector: Byte; var intaddr: TSegInfo): Boolean; cdecl;
function Pascal_set_pm_interrupt (vector: Byte; var intaddr: TSegInfo): Boolean; cdecl;
function Pascal_realintr (intnr: Word; var regs: trealregs): Boolean; cdecl;
function Pascal_get_linear_addr (phys_addr: Longint; size: Longint): Longint; cdecl;
function Pascal_lock_data (var data; size: Longint): Boolean; cdecl;
function Pascal_lock_code (functionaddr: Pointer; size: Longint): Boolean; cdecl;
function Pascal_unlock_data (var data; size: Longint): Boolean; cdecl;
function Pascal_unlock_code (functionaddr: Pointer; size: Longint): Boolean; cdecl;

{$I pascal/dos.pas}
{$I pascal/dpmi.pas}
{$I pascal/go32.pas}
{$I pascal/pc.pas}

{$ENDIF} {DEFINED(GO32V2)}

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

function Pascal_strcopy (dest, src: PChar): PChar; cdecl;
public name PUBLIC_PREFIX + 'Pascal_strcopy';
begin
  Pascal_strcopy := strings.strcopy (dest, src);
end;

function Pascal_strecopy (dest, src: PChar): PChar; cdecl;
public name PUBLIC_PREFIX + 'Pascal_strecopy';
begin
  Pascal_strecopy := strings.strecopy (dest, src);
end;

function Pascal_strlcopy (dest, src: PChar; n: SizeInt): PChar; cdecl;
public name PUBLIC_PREFIX + 'Pascal_strlcopy';
begin
  Pascal_strlcopy := strings.strlcopy (dest, src, n);
end;

function Pascal_strcat (dest, src: PChar): PChar; cdecl;
public name PUBLIC_PREFIX + 'Pascal_strcat';
begin
  Pascal_strcat := strings.strcat (dest, src);
end;

function Pascal_strlcat (dest, src: PChar; n: SizeInt): PChar; cdecl;
public name PUBLIC_PREFIX + 'Pascal_strlcat';
begin
  Pascal_strlcat := strings.strlcat (dest, src, n);
end;

function Pascal_strcomp (s1, s2: PChar): SizeInt; cdecl;
public name PUBLIC_PREFIX + 'Pascal_strcomp';
begin
  Pascal_strcomp := strings.strcomp (s1, s2);
end;

function Pascal_strlcomp (s1, s2: PChar; n: SizeInt): SizeInt; cdecl;
public name PUBLIC_PREFIX + 'Pascal_strlcomp';
begin
  Pascal_strlcomp := strings.strlcomp (s1, s2, n);
end;

function Pascal_strnew (const s: PChar): PChar; cdecl;
public name PUBLIC_PREFIX + 'Pascal_strnew';
begin
  Pascal_strnew := strings.strnew (s);
end;

function Pascal_strscan (const s: PChar; c: Char): PChar; cdecl;
public name PUBLIC_PREFIX + 'Pascal_strscan';
begin
  Pascal_strscan := strings.strscan (s, c);
end;

function Pascal_strrscan (const s: PChar; c: Char): PChar; cdecl;
public name PUBLIC_PREFIX + 'Pascal_strrscan';
begin
  Pascal_strrscan := strings.strrscan (s, c);
end;

function Pascal_strpos (const haystack, needle: PChar): PChar; cdecl;
public name PUBLIC_PREFIX + 'Pascal_strpos';
begin
  Pascal_strpos := strings.strpos (haystack, needle);
end;

function Pascal_stripos (const haystack, needle: PChar): PChar; cdecl;
public name PUBLIC_PREFIX + 'Pascal_stripos';
begin
  Pascal_stripos := strings.stripos (haystack, needle);
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

function Pascal_get_pm_interrupt (vector: Byte; var intaddr: TSegInfo): Boolean; cdecl;
public name PUBLIC_PREFIX + 'Pascal_get_pm_interrupt';
begin
  Pascal_get_pm_interrupt := go32.get_pm_interrupt (vector, intaddr);
end;

function Pascal_set_pm_interrupt (vector: Byte; var intaddr: TSegInfo): Boolean; cdecl;
public name PUBLIC_PREFIX + 'Pascal_set_pm_interrupt';
begin
  Pascal_set_pm_interrupt := go32.set_pm_interrupt (vector, intaddr);
end;

function Pascal_realintr (intnr: Word; var regs: trealregs): Boolean; cdecl;
public name PUBLIC_PREFIX + 'Pascal_realintr';
begin
  Pascal_realintr := go32.realintr (intnr, regs);
end;

function Pascal_get_linear_addr (phys_addr: Longint; size: Longint): Longint; cdecl;
public name PUBLIC_PREFIX + 'Pascal_get_linear_addr';
begin
  Pascal_get_linear_addr := go32.get_linear_addr (phys_addr, size);
end;

function Pascal_lock_data (var data; size: Longint): Boolean; cdecl;
public name PUBLIC_PREFIX + 'Pascal_lock_data';
begin
  Pascal_lock_data := go32.lock_data (data, size);
end;

function Pascal_lock_code (functionaddr: Pointer; size: Longint): Boolean; cdecl;
public name PUBLIC_PREFIX + 'Pascal_lock_code';
begin
  Pascal_lock_code := go32.lock_code (functionaddr, size);
end;

function Pascal_unlock_data (var data; size: Longint): Boolean; cdecl;
public name PUBLIC_PREFIX + 'Pascal_unlock_data';
begin
  Pascal_unlock_data := go32.unlock_data (data, size);
end;

function Pascal_unlock_code (functionaddr: Pointer; size: Longint): Boolean; cdecl;
public name PUBLIC_PREFIX + 'Pascal_unlock_code';
begin
  Pascal_unlock_code := go32.unlock_code (functionaddr, size);
end;

{$ENDIF} {DEFINED(GO32V2)}

var
  OldExitProc: Pointer;

procedure OnExit;
begin
  done_stdlib; // Shuts down all C code
  ExitProc := OldExitProc;
end;

begin
{$IFDEF GO32V2}
  __dpmi_error_ptr := @go32.int31error;
  Pascal_dosmemselector := go32.dosmemselector;
  Pascal_LFNSupport := system.LFNSupport;
{$ENDIF} {DEFINED(GO32V2)}
  Pascal_Output := @system.Output;
  Pascal_InOutRes_ptr := @system.InOutRes;
  OldExitProc := ExitProc;
  ExitProc := @OnExit;
  // `stdlib' must be first (controls proper exit in C via `atexit' routine)
  init_stdlib;
  init_stdio;
end.
