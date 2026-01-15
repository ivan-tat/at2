// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024-2025 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

unit Pascal;

{$L pascal.o}
{$L pascal/errno.o}
{$IFDEF GO32V2}
{$L pascal/dos.o}
{$L pascal/dpmi.o}
{$L pascal/go32.o}
{$L pascal/pc.o}
{$ENDIF} {DEFINED(GO32V2)}
{$L pascal/stdio.o}
{$L pascal/stdlib.o}
{$L pascal/string.o}
{$L pascal/time.o}

interface

uses
{$IFDEF GO32V2}
  go32,
{$ENDIF} {NOT DEFINED(GO32V2)}
  dos,
  sysutils;

const
  PUBLIC_PREFIX = {$IF DEFINED(GO32V2) OR (DEFINED(WINDOWS) AND NOT DEFINED(CPU64))} '_' {$ELSE} '' {$ENDIF};

// Check structures shared between C and Pascal

procedure _check_struct_memb (const name, member: PChar; off_C, off: SizeUInt);
procedure _check_struct_size (const name: PChar; size_C, size: SizeUInt);

{ `system' unit }

const
  Pascal_RandSeedPtr: Pointer = @system.RandSeed; cvar;

var
  Pascal_FileRec_size: SizeInt; cvar;
  Pascal_TextRec_size: SizeInt; cvar;
{$IFDEF GO32V2}
  Pascal_LFNSupport: Boolean; cvar;
{$ENDIF} {DEFINED(GO32V2)}
  Pascal_FileMode_ptr: ^Byte; cvar;
  Pascal_Output: Pointer; cvar;
  Pascal_InOutRes_ptr: ^Word; cvar;

procedure Pascal_SetLength (var s: String; count: SizeInt); cdecl;

procedure Pascal_Halt (errnum: Longint); cdecl;

procedure Pascal_FillChar (var x; count: SizeInt; value: Byte); cdecl;
procedure Pascal_FillWord (var x; count: SizeInt; value: Word); cdecl;

procedure Pascal_Move (const src; var dest; n: SizeInt); cdecl;

function Pascal_ParamStr (l: Longint): String; cdecl;

// FPC_ABS_REAL
function Pascal_Abs_Single (x: Single): Single; cdecl;
function Pascal_Abs_Double (x: Double): Double; cdecl;

// FPC_TRUNC_REAL
function Pascal_Trunc_Single (x: Single): Single; cdecl;
function Pascal_Trunc_Double (x: Double): Double; cdecl;

function Pascal_AllocMem (size: PtrUInt): Pointer; cdecl;
function Pascal_FreeMem (p: Pointer): PtrUInt; cdecl;
function Pascal_ReAllocMem (var p: pointer; size: PtrUInt): Pointer; cdecl;

function  Pascal_IOResult: Word; cdecl;
procedure Pascal_AssignFile (var f: File; name: String); cdecl;
procedure Pascal_AssignText (var t: Text; name: String); cdecl;
procedure Pascal_ResetFile (var f: File; l: Longint); cdecl;
procedure Pascal_ResetText (var t: Text); cdecl;
procedure Pascal_RewriteFile (var f: File; l: Longint); cdecl;
procedure Pascal_RewriteText (var t: Text); cdecl;
procedure Pascal_Append (var t: Text); cdecl;
function  Pascal_FileSize (var f: File): Int64; cdecl;
function  Pascal_FilePos (var f: File): Int64; cdecl;
procedure Pascal_Seek (var f: File; pos: Int64); cdecl;
procedure Pascal_BlockRead (var f: File; var buf; count: Longint; var result: Longint); cdecl;
procedure Pascal_BlockWrite (var f: File; var buf; count: Longint; var result: Longint); cdecl;
procedure Pascal_Write_PChar (var t: Text; str: PChar); cdecl;
procedure Pascal_Write_String (var t: Text; str: String); cdecl;
procedure Pascal_Flush (var t: Text); cdecl;
procedure Pascal_EraseFile (var f: File); cdecl;
procedure Pascal_EraseText (var t: Text); cdecl;
procedure Pascal_CloseFile (var f: File); cdecl;
procedure Pascal_CloseText (var t: Text); cdecl;
procedure Pascal_GetDir (drivenr: Byte; var dir: String); cdecl;
procedure Pascal_ChDir (s: String); cdecl;

{$IFDEF CPU64}
function Pascal_Random (l: Int64): Int64; cdecl;
{$ELSE} {NOT DEFINED(CPU64)}
function Pascal_Random (l: Longint): Longint; cdecl;
{$ENDIF} {NOT DEFINED(CPU64)}

{ `strings' unit }

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

{ `sysutils' unit }

function Pascal_Now: TDateTime; cdecl;

{ `dateutils' unit }

function Pascal_DateTimeToUnix (const AValue: TDateTime): Int64; cdecl;

{ `crt' unit }

procedure Pascal_Delay (ms: Word); cdecl;
function Pascal_KeyPressed: Boolean; cdecl;
function Pascal_ReadKey: Char; cdecl;

{ `dos' unit }

var
  Pascal_DosError_ptr: ^Integer; cvar;

// `f' (`t') must have been assigned, but not opened.
procedure Pascal_GetFAttrFile (var f: File; var attr: Word); cdecl;
procedure Pascal_GetFAttrText (var t: Text; var attr: Word); cdecl;
// Under Unix like systems (such as Linux and BeOS) the call `SetFAttr' exists, but is not implemented, i.e. it does nothing.
procedure Pascal_SetFAttrFile (var f: File; attr: Word); cdecl;
procedure Pascal_SetFAttrText (var t: Text; attr: Word); cdecl;

function Pascal_DiskSize (drive: Byte): Int64; cdecl;

procedure Pascal_FindFirst (path: String; attr: Word; var f: SearchRec); cdecl;
procedure Pascal_FindNext (var f: SearchRec); cdecl;

{$IFDEF GO32V2}

{ `go32' unit }

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

{$ENDIF} {DEFINED(GO32V2)}

{$IFDEF WINDOWS}

{ `windows' unit }

function Pascal_GetLogicalDriveStrings (nBufferLength: LongWord; lpBuffer: PChar): LongWord; cdecl;

{$ENDIF} {DEFINED(WINDOWS)}


{$IFDEF GO32V2}
{$I pascal/dos.pas}
{$I pascal/dpmi.pas}
{$I pascal/go32.pas}
{$I pascal/pc.pas}
{$ENDIF} {DEFINED(GO32V2)}
{$I pascal/stdio.pas}
{$I pascal/stdlib.pas}
{$I pascal/string.pas}
{$I pascal/time.pas}

implementation

uses
  crt,
  dateutils,
{$IFNDEF GO32V2}
{$IFNDEF UNIX}
  windows,
{$ENDIF} {NOT DEFINED(UNIX)}
{$ENDIF} {NOT DEFINED(GO32V2)}
  strings;

procedure _check_struct_memb (const name, member: PChar; off_C, off: SizeUInt);
begin
  if (off_C <> off) then
  begin
    WriteLn (stderr, 'Error: There is difference (', off - off_C, ') in the offset of member "', name, '.', member, '" in C (', off_c, ') and Pascal (', off, ')');
    Halt (1);
  end;
end;

procedure _check_struct_size (const name: PChar; size_C, size: SizeUInt);
begin
  if (size_C <> size) then
  begin
    WriteLn (stderr, 'Error: There is difference (', size - size_C, ') in the size of structure "', name, '" in C (', size_C, ') and Pascal (', size, ')');
    Halt (1);
  end;
end;

{ `System' unit }

procedure Pascal_SetLength (var s: String; count: SizeInt); cdecl;
public name PUBLIC_PREFIX + 'Pascal_SetLength';
begin
  system.SetLength (s, count);
end;

procedure Pascal_Halt (errnum: Longint); cdecl;
public name PUBLIC_PREFIX + 'Pascal_Halt';
begin
  system.Halt (errnum);
end;

procedure Pascal_FillChar (var x; count: SizeInt; value: Byte); cdecl;
public name PUBLIC_PREFIX + 'Pascal_FillChar';
begin
  system.FillChar (x, count, value);
end;

procedure Pascal_FillWord (var x; count: SizeInt; value: Word); cdecl;
public name PUBLIC_PREFIX + 'Pascal_FillWord';
begin
  system.FillWord (x, count, value);
end;

procedure Pascal_Move (const src; var dest; n: SizeInt); cdecl;
public name PUBLIC_PREFIX + 'Pascal_Move';
begin
  system.Move (src, dest, n);
end;

function Pascal_ParamStr (l: Longint): String; cdecl;
public name PUBLIC_PREFIX + 'Pascal_ParamStr';
begin
  Pascal_ParamStr := system.ParamStr (l);
end;

function Pascal_Abs_Single (x: Single): Single; cdecl;
public name PUBLIC_PREFIX + 'Pascal_Abs_Single';
begin
  Pascal_Abs_Single := system.Abs (x);
end;

function Pascal_Abs_Double (x: Double): Double; cdecl;
public name PUBLIC_PREFIX + 'Pascal_Abs_Double';
begin
  Pascal_Abs_Double := system.Abs (x);
end;

function Pascal_Trunc_Single (x: Single): Single; cdecl;
public name PUBLIC_PREFIX + 'Pascal_Trunc_Single';
begin
  Pascal_Trunc_Single := system.Trunc (x);
end;

function Pascal_Trunc_Double (x: Double): Double; cdecl;
public name PUBLIC_PREFIX + 'Pascal_Trunc_Double';
begin
  Pascal_Trunc_Double := system.Trunc (x);
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

function Pascal_IOResult: Word; cdecl;
public name PUBLIC_PREFIX + 'Pascal_IOResult';
begin
  Pascal_IOResult := system.IOResult;
end;

procedure Pascal_AssignFile (var f: File; name: String); cdecl;
public name PUBLIC_PREFIX + 'Pascal_AssignFile';
begin
  {$PUSH} {$I-}
  system.Assign (f, name);
  {$POP}
end;

procedure Pascal_AssignText (var t: Text; name: String); cdecl;
public name PUBLIC_PREFIX + 'Pascal_AssignText';
begin
  {$PUSH} {$I-}
  system.Assign (t, name);
  {$POP}
end;

procedure Pascal_ResetFile (var f: File; l: Longint); cdecl;
public name PUBLIC_PREFIX + 'Pascal_ResetFile';
begin
  {$PUSH} {$I-}
  system.Reset (f, l);
  {$POP}
end;

procedure Pascal_ResetText (var t: Text); cdecl;
public name PUBLIC_PREFIX + 'Pascal_ResetText';
begin
  {$PUSH} {$I-}
  system.Reset (t);
  {$POP}
end;

procedure Pascal_RewriteFile (var f: File; l: Longint); cdecl;
public name PUBLIC_PREFIX + 'Pascal_RewriteFile';
begin
  {$PUSH} {$I-}
  system.Rewrite (f, l);
  {$POP}
end;

procedure Pascal_RewriteText (var t: Text); cdecl;
public name PUBLIC_PREFIX + 'Pascal_RewriteText';
begin
  {$PUSH} {$I-}
  system.Rewrite (t);
  {$POP}
end;

procedure Pascal_Append (var t: Text); cdecl;
public name PUBLIC_PREFIX + 'Pascal_Append';
begin
  {$PUSH} {$I-}
  system.Append (t);
  {$POP}
end;

function Pascal_FileSize (var f: File): Int64; cdecl;
public name PUBLIC_PREFIX + 'Pascal_FileSize';
begin
  {$PUSH} {$I-}
  Pascal_FileSize := system.FileSize (f);
  {$POP}
end;

function Pascal_FilePos (var f: File): Int64; cdecl;
public name PUBLIC_PREFIX + 'Pascal_FilePos';
begin
  {$PUSH} {$I-}
  Pascal_FilePos := system.FilePos (f);
  {$POP}
end;

procedure Pascal_Seek (var f: File; pos: Int64); cdecl;
public name PUBLIC_PREFIX + 'Pascal_Seek';
begin
  {$PUSH} {$I-}
  system.Seek (f, pos);
  {$POP}
end;

procedure Pascal_BlockRead (var f: File; var buf; count: Longint; var result: Longint); cdecl;
public name PUBLIC_PREFIX + 'Pascal_BlockRead';
begin
  {$PUSH} {$I-}
  system.BlockRead (f, buf, count, result);
  {$POP}
end;

procedure Pascal_BlockWrite (var f: File; var buf; count: Longint; var result: Longint); cdecl;
public name PUBLIC_PREFIX + 'Pascal_BlockWrite';
begin
  {$PUSH} {$I-}
  system.BlockWrite (f, buf, count, result);
  {$POP}
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

procedure Pascal_EraseFile (var f: File); cdecl;
public name PUBLIC_PREFIX + 'Pascal_EraseFile';
begin
  {$PUSH} {$I-}
  system.Erase (f);
  {$POP}
end;

procedure Pascal_EraseText (var t: Text); cdecl;
public name PUBLIC_PREFIX + 'Pascal_EraseText';
begin
  {$PUSH} {$I-}
  system.Erase (t);
  {$POP}
end;

procedure Pascal_CloseFile (var f: File); cdecl;
public name PUBLIC_PREFIX + 'Pascal_CloseFile';
begin
  {$PUSH} {$I-}
  system.Close (f);
  {$POP}
end;

procedure Pascal_CloseText (var t: Text); cdecl;
public name PUBLIC_PREFIX + 'Pascal_CloseText';
begin
  {$PUSH} {$I-}
  system.Close (t);
  {$POP}
end;

procedure Pascal_GetDir (drivenr: Byte; var dir: String); cdecl;
public name PUBLIC_PREFIX + 'Pascal_GetDir';
begin
  {$PUSH} {$I-}
  system.GetDir (drivenr, dir);
  {$POP}
end;

procedure Pascal_ChDir (s: String); cdecl;
public name PUBLIC_PREFIX + 'Pascal_ChDir';
begin
  {$PUSH} {$I-}
  system.ChDir (s);
  {$POP}
end;

{$IFDEF CPU64}
function Pascal_Random (l: Int64): Int64; cdecl;
{$ELSE} {NOT DEFINED(CPU64)}
function Pascal_Random (l: Longint): Longint; cdecl;
{$ENDIF} {NOT DEFINED(CPU64)}
public name PUBLIC_PREFIX + 'Pascal_Random';
begin
  Pascal_Random := system.Random (l);
end;

{ `strings' unit }

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

{ `sysutils' unit }

function Pascal_Now: TDateTime; cdecl;
public name PUBLIC_PREFIX + 'Pascal_Now';
begin
  Pascal_Now := sysutils.Now;
end;

{ `dateutils' unit }

function Pascal_DateTimeToUnix (const AValue: TDateTime): Int64; cdecl;
public name PUBLIC_PREFIX + 'Pascal_DateTimeToUnix';
begin
  Pascal_DateTimeToUnix := dateutils.DateTimeToUnix (AValue);
end;

{ `crt' unit }

procedure Pascal_Delay (ms: Word); cdecl;
public name PUBLIC_PREFIX + 'Pascal_Delay';
begin
  crt.Delay (ms);
end;

function Pascal_KeyPressed: Boolean; cdecl;
public name PUBLIC_PREFIX + 'Pascal_KeyPressed';
begin
  Pascal_KeyPressed := crt.KeyPressed;
end;

function Pascal_ReadKey: Char; cdecl;
public name PUBLIC_PREFIX + 'Pascal_ReadKey';
begin
  Pascal_ReadKey := crt.ReadKey;
end;

{ `dos' unit }

{$IFDEF GO32V2}

// dos.SearchRec

var
  struct_SearchRec: packed record
    Fill,
    Attr,
    Time,
    //Rreserved, not in DJGPP V2
    Size,
    Name: LongWord;
  end; cvar; external;
var
  struct_SearchRec_size: LongWord; cvar; external;

procedure _check_struct_SearchRec;
const s: PChar = 'SearchRec';
begin
  _check_struct_memb (s, 'Fill', struct_SearchRec.Fill, SizeUInt (@(SearchRec (NIL^).Fill[1])));
  _check_struct_memb (s, 'Attr', struct_SearchRec.Attr, SizeUInt (@(SearchRec (NIL^).Attr)));
  _check_struct_memb (s, 'Time', struct_SearchRec.Time, SizeUInt (@(SearchRec (NIL^).Time)));
  //_check_struct_memb (s, 'Reserved', struct_SearchRec.Reserved, SizeUInt (@(SearchRec (NIL^).Reserved)));
  _check_struct_memb (s, 'Size', struct_SearchRec.Size, SizeUInt (@(SearchRec (NIL^).Size)));
  _check_struct_memb (s, 'Name', struct_SearchRec.Name, SizeUInt (@(SearchRec (NIL^).Name)));
  _check_struct_size (s, struct_SearchRec_size, SizeOf (SearchRec));
end;

{$ENDIF} {DEFINED(GO32V2)}

{$IFDEF UNIX}

// dos.SearchRec

var
  struct_SearchRec: packed record
    SearchPos,
    SearchNum,
    DirPtr,
    SearchType,
    SearchAttr,
    Mode,
    Fill,
    Attr,
    Time,
    Size,
    Reserved,
    Name,
    SearchSpec,
    NamePos: LongWord;
  end; cvar; external;
var
  struct_SearchRec_size: LongWord; cvar; external;

procedure _check_struct_SearchRec;
const s: PChar = 'SearchRec';
begin
  _check_struct_memb (s, 'SearchPos', struct_SearchRec.SearchPos, SizeUInt (@(SearchRec (NIL^).SearchPos)));
  _check_struct_memb (s, 'SearchNum', struct_SearchRec.SearchNum, SizeUInt (@(SearchRec (NIL^).SearchNum)));
  _check_struct_memb (s, 'DirPtr', struct_SearchRec.DirPtr, SizeUInt (@(SearchRec (NIL^).DirPtr)));
  _check_struct_memb (s, 'SearchType', struct_SearchRec.SearchType, SizeUInt (@(SearchRec (NIL^).SearchType)));
  _check_struct_memb (s, 'SearchAttr', struct_SearchRec.SearchAttr, SizeUInt (@(SearchRec (NIL^).SearchAttr)));
  _check_struct_memb (s, 'Fill', struct_SearchRec.Fill, SizeUInt (@(SearchRec (NIL^).Fill[1])));
  _check_struct_memb (s, 'Attr', struct_SearchRec.Attr, SizeUInt (@(SearchRec (NIL^).Attr)));
  _check_struct_memb (s, 'Time', struct_SearchRec.Time, SizeUInt (@(SearchRec (NIL^).Time)));
  _check_struct_memb (s, 'Size', struct_SearchRec.Size, SizeUInt (@(SearchRec (NIL^).Size)));
  _check_struct_memb (s, 'Reserved', struct_SearchRec.Reserved, SizeUInt (@(SearchRec (NIL^).Reserved)));
  _check_struct_memb (s, 'Name', struct_SearchRec.Name, SizeUInt (@(SearchRec (NIL^).Name)));
  _check_struct_memb (s, 'SearchSpec', struct_SearchRec.SearchSpec, SizeUInt (@(SearchRec (NIL^).SearchSpec)));
  _check_struct_memb (s, 'NamePos', struct_SearchRec.NamePos, SizeUInt (@(SearchRec (NIL^).NamePos)));
  _check_struct_size (s, struct_SearchRec_size, SizeOf (SearchRec));
end;

{$ENDIF} {DEFINED(UNIX)}

{$IFDEF WINDOWS}

// dos.TWinFileTime

var
  struct_TWinFileTime: packed record
    dwLowDateTime,
    dwHighDateTime: LongWord;
  end; cvar; external;
var
  struct_TWinFileTime_size: LongWord; cvar; external;

procedure _check_struct_TWinFileTime;
const s: PChar = 'TWinFileTime';
begin
  _check_struct_memb (s, 'dwLowDateTime', struct_TWinFileTime.dwLowDateTime, SizeUInt (@(TWinFileTime (NIL^).dwLowDateTime)));
  _check_struct_memb (s, 'dwHighDateTime', struct_TWinFileTime.dwHighDateTime, SizeUInt (@(TWinFileTime (NIL^).dwHighDateTime)));
  _check_struct_size (s, struct_TWinFileTime_size, SizeOf (TWinFileTime));
end;

// dos.TWinFindData

var
  struct_TWinFindData: packed record
    dwFileAttributes,
    ftCreationTime,
    ftLastAccessTime,
    ftLastWriteTime,
    nFileSizeHigh,
    nFileSizeLow,
    dwReserved0,
    dwReserved1,
    cFileName,
    cAlternateFileName,
    pad: LongWord;
  end; cvar; external;
var
  struct_TWinFindData_size: LongWord; cvar; external;

procedure _check_struct_TWinFindData;
const s: PChar = 'TWinFindData';
begin
  _check_struct_memb (s, 'dwFileAttributes', struct_TWinFindData.dwFileAttributes, SizeUInt (@(TWinFindData (NIL^).dwFileAttributes)));
  _check_struct_memb (s, 'ftCreationTime', struct_TWinFindData.ftCreationTime, SizeUInt (@(TWinFindData (NIL^).ftCreationTime)));
  _check_struct_memb (s, 'ftLastAccessTime', struct_TWinFindData.ftLastAccessTime, SizeUInt (@(TWinFindData (NIL^).ftLastAccessTime)));
  _check_struct_memb (s, 'ftLastWriteTime', struct_TWinFindData.ftLastWriteTime, SizeUInt (@(TWinFindData (NIL^).ftLastWriteTime)));
  _check_struct_memb (s, 'nFileSizeHigh', struct_TWinFindData.nFileSizeHigh, SizeUInt (@(TWinFindData (NIL^).nFileSizeHigh)));
  _check_struct_memb (s, 'nFileSizeLow', struct_TWinFindData.nFileSizeLow, SizeUInt (@(TWinFindData (NIL^).nFileSizeLow)));
  _check_struct_memb (s, 'dwReserved0', struct_TWinFindData.dwReserved0, SizeUInt (@(TWinFindData (NIL^).dwReserved0)));
  _check_struct_memb (s, 'dwReserved1', struct_TWinFindData.dwReserved1, SizeUInt (@(TWinFindData (NIL^).dwReserved1)));
  _check_struct_memb (s, 'cFileName', struct_TWinFindData.cFileName, SizeUInt (@(TWinFindData (NIL^).cFileName)));
  _check_struct_memb (s, 'cAlternateFileName', struct_TWinFindData.cAlternateFileName, SizeUInt (@(TWinFindData (NIL^).cAlternateFileName)));
  _check_struct_memb (s, 'pad', struct_TWinFindData.pad, SizeUInt (@(TWinFindData (NIL^).pad)));
  _check_struct_size (s, struct_TWinFindData_size, SizeOf (TWinFindData));
end;

// dos.SearchRec

var
  struct_SearchRec: packed record
    FindHandle,
    WinFindData,
    ExcludeAttr,
    Time,
    Size,
    Attr,
    Name: LongWord;
  end; cvar; external;
var
  struct_SearchRec_size: LongWord; cvar; external;

procedure _check_struct_SearchRec;
const s: PChar = 'SearchRec';
begin
  _check_struct_memb (s, 'FindHandle', struct_SearchRec.FindHandle, SizeUInt (@(SearchRec (NIL^).FindHandle)));
  _check_struct_memb (s, 'WinFindData', struct_SearchRec.WinFindData, SizeUInt (@(SearchRec (NIL^).WinFindData)));
  _check_struct_memb (s, 'ExcludeAttr', struct_SearchRec.ExcludeAttr, SizeUInt (@(SearchRec (NIL^).ExcludeAttr)));
  _check_struct_memb (s, 'Time', struct_SearchRec.Time, SizeUInt (@(SearchRec (NIL^).Time)));
  _check_struct_memb (s, 'Size', struct_SearchRec.Size, SizeUInt (@(SearchRec (NIL^).Size)));
  _check_struct_memb (s, 'Attr', struct_SearchRec.Attr, SizeUInt (@(SearchRec (NIL^).Attr)));
  _check_struct_memb (s, 'Name', struct_SearchRec.Name, SizeUInt (@(SearchRec (NIL^).Name)));
  _check_struct_size (s, struct_SearchRec_size, SizeOf (SearchRec));
end;

{$ENDIF} {DEFINED(WINDOWS)}

procedure Pascal_GetFAttrFile (var f: File; var attr: Word); cdecl;
public name PUBLIC_PREFIX + 'Pascal_GetFAttrFile';
begin
  dos.GetFAttr (f, attr);
end;

procedure Pascal_GetFAttrText (var t: Text; var attr: Word); cdecl;
public name PUBLIC_PREFIX + 'Pascal_GetFAttrText';
begin
  dos.GetFAttr (t, attr);
end;

procedure Pascal_SetFAttrFile (var f: File; attr: Word); cdecl;
public name PUBLIC_PREFIX + 'Pascal_SetFAttrFile';
begin
  dos.SetFAttr (f, attr);
end;

procedure Pascal_SetFAttrText (var t: Text; attr: Word); cdecl;
public name PUBLIC_PREFIX + 'Pascal_SetFAttrText';
begin
  dos.SetFAttr (t, attr);
end;

function Pascal_DiskSize (drive: Byte): Int64; cdecl;
public name PUBLIC_PREFIX + 'Pascal_DiskSize';
begin
  Pascal_DiskSize := dos.DiskSize (drive);
end;

procedure Pascal_FindFirst (path: String; attr: Word; var f: SearchRec); cdecl;
public name PUBLIC_PREFIX + 'Pascal_FindFirst';
begin
  dos.FindFirst (path, attr, f);
end;

procedure Pascal_FindNext (var f: SearchRec); cdecl;
public name PUBLIC_PREFIX + 'Pascal_FindNext';
begin
  dos.FindNext (f);
end;

{$IFDEF GO32V2}

{ `go32' unit }

function Pascal_allocate_ldt_descriptors (count: Word): Word; cdecl;
public name PUBLIC_PREFIX + 'Pascal_allocate_ldt_descriptors';
begin
  Pascal_allocate_ldt_descriptors := go32.allocate_ldt_descriptors (count);
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

{$IFDEF WINDOWS}

{ `windows' unit }

function Pascal_GetLogicalDriveStrings (nBufferLength: LongWord; lpBuffer: PChar): LongWord; cdecl;
public name PUBLIC_PREFIX + 'Pascal_GetLogicalDriveStrings';
begin
  Pascal_GetLogicalDriveStrings := windows.GetLogicalDriveStrings (nBufferLength, lpBuffer);
end;

{$ENDIF} {DEFINED(WINDOWS)}

{ Initialization and finalization }

var
  OldExitProc: Pointer;

procedure OnExit;
begin
  done_stdlib; // Shuts down all C code
  system.ExitProc := OldExitProc;
end;

begin
{$IFDEF WINDOWS}
  _check_struct_TWinFileTime;
  _check_struct_TWinFindData;
{$ENDIF} {DEFINED(WINDOWS)}
  _check_struct_SearchRec;
  Pascal_FileRec_size := SizeOf (FileRec);
  Pascal_TextRec_size := SizeOf (TextRec);
{$IFDEF GO32V2}
  __dpmi_error_ptr := @go32.int31error;
  Pascal_dosmemselector := go32.dosmemselector;
  Pascal_LFNSupport := system.LFNSupport;
{$ENDIF} {DEFINED(GO32V2)}
  Pascal_FileMode_ptr := @system.FileMode;
  Pascal_Output := @system.Output;
  Pascal_InOutRes_ptr := @system.InOutRes;
  Pascal_DosError_ptr := @dos.DosError;
  OldExitProc := system.ExitProc;
  system.ExitProc := @OnExit;
  // `stdlib' must be first (controls proper exit in C via `atexit' routine)
  init_stdlib;
  init_stdio;
end.
