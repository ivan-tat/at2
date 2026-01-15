// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024-2026 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef PASCAL_H
#define PASCAL_H

#include "defines.h"
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h> // compiler may include it's own version of `errno.h' here
#include <sys/types.h>
#if WIN32 || WIN64 || WINNT
#include <windows.h>
#endif // WIN32 || WIN64 || WINNT
#include "pascal/errno.h" // last to fix macro redefinition

#pragma pack(push, 1)

typedef unsigned char Shortstring;

typedef struct {
  uint8_t len;
  Shortstring str[255];
} Shortstring_t;

typedef Shortstring String;

typedef Shortstring_t String_t;

#define Length(s) ((s)[0])
#define SetLength(s, l) Pascal_SetLength (s, l)
#define GetStr(s) (&(s)[1])

#if GO32

// `go32' unit

extern uint16_t __v2prt0_ds_alias;
extern uint16_t Pascal_dosmemselector;

extern uint16_t Pascal_allocate_ldt_descriptors (uint16_t count);
extern bool Pascal_free_ldt_descriptor (uint16_t selector);
extern uint16_t Pascal_get_next_selector_increment_value (void);
extern bool Pascal_set_segment_base_address (uint16_t selector, int32_t address);
extern bool Pascal_set_segment_limit (uint16_t selector, int32_t limit);

extern void Pascal_dosmemget (uint16_t seg, uint16_t ofs, void *data, uint32_t count);
extern void Pascal_dosmemput (uint16_t seg, uint16_t ofs, const void *data, uint32_t count);

extern int32_t Pascal_global_dos_alloc (int32_t bytes);
extern bool Pascal_global_dos_free (uint16_t selector);

extern bool Pascal_get_pm_interrupt (uint8_t vector, void *intaddr);
extern bool Pascal_set_pm_interrupt (uint8_t vector, const void *intaddr);

extern bool Pascal_realintr (uint16_t intnr, void *regs);

extern int32_t Pascal_get_linear_addr (int32_t phys_addr, int32_t size);

extern bool Pascal_lock_data (const void *data, int32_t size);
extern bool Pascal_lock_code (const void *functionaddr, int32_t size);
extern bool Pascal_unlock_data (const void *data, int32_t size);
extern bool Pascal_unlock_code (const void *functionaddr, int32_t size);

#endif // GO32

// `system' unit

typedef double TDateTime;

#if GO32
#if USE_FPC
extern bool Pascal_LFNSupport;
#endif // USE_FPC
#endif // GO32

// `FileMode' determines how untyped files are opened for reading with `Reset'
#define Pascal_FileMode_ReadOnly  0 // open file read only
#define Pascal_FileMode_WriteOnly 1 // open file write only
#define Pascal_FileMode_ReadWrite 2 // open file read and write

extern uint8_t *Pascal_FileMode_ptr;
#define Pascal_FileMode (*Pascal_FileMode_ptr)

extern void *Pascal_Output;

extern uint16_t *Pascal_InOutRes_ptr;
#define Pascal_InOutRes (*Pascal_InOutRes_ptr)

extern ssize_t Pascal_FileRec_size;
extern ssize_t Pascal_TextRec_size;

extern void Pascal_SetLength (String *s, size_t count);

extern void Pascal_Halt (int32_t errnum) __NORETURN;

extern void Pascal_FillChar (void *x, ssize_t count, uint8_t value);
extern void Pascal_FillWord (void *x, ssize_t count, uint16_t value);

extern void Pascal_Move (const void *src, void *dest, ssize_t n);

extern String_t Pascal_ParamStr (int32_t l);

extern float Pascal_Abs_Single (float x);
extern double Pascal_Abs_Double (double x);

extern float Pascal_Trunc_Single (float x);
extern double Pascal_Trunc_Double (double x);

extern void *Pascal_AllocMem (size_t size);
extern size_t Pascal_FreeMem (void *p);
extern void *Pascal_ReAllocMem (void **p, size_t size);

extern uint16_t Pascal_IOResult (void);

extern void Pascal_AssignFile (void *file, const String *name);
extern void Pascal_AssignText (void *text, const String *name);
extern void Pascal_ResetFile (void *file, int32_t l);
extern void Pascal_ResetText (void *text);
extern void Pascal_RewriteFile (void *file, int32_t l);
extern void Pascal_RewriteText (void *text);
extern void Pascal_Append (void *text);
extern int64_t Pascal_FileSize (void *file);
extern int64_t Pascal_FilePos (void *file);
extern void Pascal_Seek (void *file, int64_t pos);
extern void Pascal_BlockRead (void *file, void *buf, int32_t count, int32_t *result);
extern void Pascal_BlockWrite (void *file, void *buf, int32_t count, int32_t *result);
extern void Pascal_Write_PChar (void *text, const char *str);
extern void Pascal_Write_String (void *text, const String *str);
extern void Pascal_Flush (void *text);
extern void Pascal_EraseFile (void *file);
extern void Pascal_EraseText (void *text);
extern void Pascal_CloseFile (void *file);
extern void Pascal_CloseText (void *text);

extern void Pascal_GetDir (uint8_t drivenr, String *dir);
extern void Pascal_ChDir (const String *s);

extern int Pascal_Random (int l);

// `strings' unit

extern size_t Pascal_strlen (const char *s);
extern char *Pascal_strcopy (char *dest, const char *src);
extern char *Pascal_strecopy (char *dest, const char *src);
extern char *Pascal_strlcopy (char *dest, const char *src, ssize_t n);
extern char *Pascal_strcat (char *dest, const char *src);
extern char *Pascal_strlcat (char *dest, const char *src, ssize_t n);
extern ssize_t Pascal_strcomp (const char *s1, const char *s2);
extern ssize_t Pascal_strlcomp (const char *s1, const char *s2, ssize_t n);
extern char *Pascal_strnew (const char *s);
extern char *Pascal_strscan (const char *s, char c);
extern char *Pascal_strrscan (const char *s, char c);
extern char *Pascal_strpos (const char *haystack, const char *needle);
extern char *Pascal_stripos (const char *haystack, const char *needle);

// `crt' unit

extern void Pascal_Delay (uint16_t ms);
extern bool Pascal_KeyPressed (void);
extern char Pascal_ReadKey (void);

// `dos' unit

#define DOSERR_FILE_NOT_FOUND      2
#define DOSERR_PATH_NOT_FOUND      3
#define DOSERR_ACCESS_DENIED       5
#define DOSERR_INVALID_HANDLE      6
#define DOSERR_NOT_ENOUGH_MEMORY   8
#define DOSERR_INVALID_ENVIRONMENT 10
#define DOSERR_INVALID_FORMAT      11
#define DOSERR_NO_MORE_FILES       18

extern int16_t *Pascal_DosError_ptr;
#define Pascal_DosError (*Pascal_DosError_ptr)

#define Pascal_FileNameLen 255

// Bit masks for file attribute
#define Pascal_ReadOnly  0x01 // Read-Only file attribute
#define Pascal_Hidden    0x02 // Hidden file attribute
#define Pascal_SysFile   0x04 // System file attribute
#define Pascal_VolumeId  0x08 // Volume ID file attribute
#define Pascal_Directory 0x10 // Directory file attribute
#define Pascal_Archive   0x20 // Archive file attribute
#define Pascal_AnyFile   0x3F // Match any file attribute

// Under Linux, supported attributes are:
//   * Pascal_Directory
//   * Pascal_ReadOnly: if the current process doesn't have access to the file
//   * Pascal_Hidden: for files whose name starts with a dot ('.')

typedef String ComStr_t[Pascal_FileNameLen+1];
typedef String PathStr_t[Pascal_FileNameLen+1];
typedef String DirStr_t[Pascal_FileNameLen+1];
typedef String NameStr_t[Pascal_FileNameLen+1];
typedef String ExtStr_t[Pascal_FileNameLen+1];

#if GO32

typedef struct {
  uint8_t Fill[21]; // HINT: (FPC) start index 1
  uint8_t Attr;
  int32_t Time;
  //uint16_t reserved; not in DJGPP V2
  int32_t Size;
  String Name[255+1]; // LFN Name, DJGPP uses only [12] but more can't hurt (PFV)
} SearchRec_t;

#elif linux || unix

typedef struct {
  int64_t  SearchPos;  // directory position
  int32_t  SearchNum;  // to track which search this is
  void    *DirPtr;     // directory pointer for reading directory
  uint8_t  SearchType; // 0=normal, 1=open will close, 2=only 1 file
  uint8_t  SearchAttr; // attribute we are searching for
  uint16_t Mode;
  uint8_t  Fill;       // future use
  uint8_t  Attr;       // attribute of found file
  int32_t  Time;       // last modify date of found file
  int32_t  Size;       // file size of found file
  uint16_t Reserved;   // future use
  String   Name[Pascal_FileNameLen+1];       // name of found file
  String   SearchSpec[Pascal_FileNameLen+1]; // search pattern
  uint16_t NamePos;    // end of path, start of name position
} SearchRec_t;

#elif WIN32 || WIN64 || WINNT

#define PASCAL_MAX_PATH 260 // including terminating zero

typedef struct {
  uint32_t dwLowDateTime;
  uint32_t dwHighDateTime;
} TWinFileTime;

typedef struct {
  uint32_t dwFileAttributes;
  TWinFileTime ftCreationTime;
  TWinFileTime ftLastAccessTime;
  TWinFileTime ftLastWriteTime;
  uint32_t nFileSizeHigh;
  uint32_t nFileSizeLow;
  uint32_t dwReserved0;
  uint32_t dwReserved1;
  char cFileName[PASCAL_MAX_PATH]; // HINT: (FPC) start index 0
  char cAlternateFileName[16]; // HINT: (FPC) start index 0
  // The structure should be 320 bytes long...
  int32_t pad;
} TWinFindData;

typedef struct {
  uint32_t FindHandle;
  TWinFindData WinFindData;
  int32_t ExcludeAttr;
  int32_t Time;
  int32_t Size;
  int32_t Attr;
  String Name[255+1];
} SearchRec_t;

#else // !(GO32 || linux || unix || WIN32 || WIN64 || WINNT)

#error Not implemented

#endif // !(GO32 || linux || unix || WIN32 || WIN64 || WINNT)

// `file' (`text') must have been assigned, but not opened.
extern void Pascal_GetFAttrFile (void *file, uint16_t *attr);
extern void Pascal_GetFAttrText (void *text, uint16_t *attr);
// Under Unix like systems (such as Linux and BeOS) the call `SetFAttr'
// exists, but is not implemented, i.e. it does nothing.
extern void Pascal_SetFAttrFile (void *file, uint16_t attr);
extern void Pascal_SetFAttrText (void *text, uint16_t attr);

extern int64_t Pascal_DiskSize (uint8_t drive);

extern void Pascal_FindFirst (const String *path, uint16_t attr, SearchRec_t *f);
extern void Pascal_FindNext (SearchRec_t *f);

// `sysutils' unit

extern TDateTime Pascal_Now (void);

// `dateutils' unit

extern int64_t Pascal_DateTimeToUnix (const TDateTime value);

#if !(GO32 || linux || unix)

// `windows' unit

extern uint32_t Pascal_GetLogicalDriveStrings (uint32_t nBufferLength, char *lpBuffer);

#endif // !(GO32 || linux || unix)

int map_InOutRes_to_errno (int code);

#pragma pack(pop)

#endif // !defined(PASCAL_H)
