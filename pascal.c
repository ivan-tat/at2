// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2025-2026 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pascal.h"

#pragma pack(push, 1)

#define _put_memb_off(struc, memb) .memb = __builtin_offsetof (struc, memb)

#if GO32

// SearchRec

const struct {
  uint32_t
    Fill,
    Attr,
    Time,
    //Rreserved, not in DJGPP V2
    Size,
    Name;
} struct_SearchRec = {
  _put_memb_off (SearchRec_t, Fill),
  _put_memb_off (SearchRec_t, Attr),
  _put_memb_off (SearchRec_t, Time),
  //_put_memb_off (SearchRec_t, Reserved),
  _put_memb_off (SearchRec_t, Size),
  _put_memb_off (SearchRec_t, Name)
};

const uint32_t struct_SearchRec_size = sizeof (SearchRec_t);

#elif linux || unix

// SearchRec

const struct {
  uint32_t
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
    NamePos;
} struct_SearchRec = {
  _put_memb_off (SearchRec_t, SearchPos),
  _put_memb_off (SearchRec_t, SearchNum),
  _put_memb_off (SearchRec_t, DirPtr),
  _put_memb_off (SearchRec_t, SearchType),
  _put_memb_off (SearchRec_t, SearchAttr),
  _put_memb_off (SearchRec_t, Mode),
  _put_memb_off (SearchRec_t, Fill),
  _put_memb_off (SearchRec_t, Attr),
  _put_memb_off (SearchRec_t, Time),
  _put_memb_off (SearchRec_t, Size),
  _put_memb_off (SearchRec_t, Reserved),
  _put_memb_off (SearchRec_t, Name),
  _put_memb_off (SearchRec_t, SearchSpec),
  _put_memb_off (SearchRec_t, NamePos)
};

const uint32_t struct_SearchRec_size = sizeof (SearchRec_t);

#elif WIN32 || WIN64 || WINNT

// TWinFileTime

const struct {
  uint32_t
    dwLowDateTime,
    dwHighDateTime;
} struct_TWinFileTime = {
  _put_memb_off (TWinFileTime, dwLowDateTime),
  _put_memb_off (TWinFileTime, dwHighDateTime)
};

const uint32_t struct_TWinFileTime_size = sizeof (TWinFileTime);

// TWinFindData

const struct {
  uint32_t
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
    pad;
} struct_TWinFindData = {
  _put_memb_off (TWinFindData, dwFileAttributes),
  _put_memb_off (TWinFindData, ftCreationTime),
  _put_memb_off (TWinFindData, ftLastAccessTime),
  _put_memb_off (TWinFindData, ftLastWriteTime),
  _put_memb_off (TWinFindData, nFileSizeHigh),
  _put_memb_off (TWinFindData, nFileSizeLow),
  _put_memb_off (TWinFindData, dwReserved0),
  _put_memb_off (TWinFindData, dwReserved1),
  _put_memb_off (TWinFindData, cFileName),
  _put_memb_off (TWinFindData, cAlternateFileName),
  _put_memb_off (TWinFindData, pad)
};

const uint32_t struct_TWinFindData_size = sizeof (TWinFindData);

// SearchRec

const struct {
  uint32_t
    FindHandle,
    WinFindData,
    ExcludeAttr,
    Time,
    Size,
    Attr,
    Name;
} struct_SearchRec = {
  _put_memb_off (SearchRec_t, FindHandle),
  _put_memb_off (SearchRec_t, WinFindData),
  _put_memb_off (SearchRec_t, ExcludeAttr),
  _put_memb_off (SearchRec_t, Time),
  _put_memb_off (SearchRec_t, Size),
  _put_memb_off (SearchRec_t, Attr),
  _put_memb_off (SearchRec_t, Name)
};

const uint32_t struct_SearchRec_size = sizeof (SearchRec_t);

#else // !(GO32 || linux || unix || WIN32 || WIN64 || WINNT)

#error Not implemented

#endif // !(GO32 || linux || unix || WIN32 || WIN64 || WINNT)

int map_InOutRes_to_errno (int code)
{
  int result = -1; // unknown error

  switch (code)
  {
    case 0: result = 0; break;
    // DOS errors
    case 1: result = ENOMEM; break;       // Out of memory
    case 2: result = ENOENT; break;       // File not found
    case 3: result = ENOENT; break;       // Path not found (Invalid file name)
    case 4: result = EMFILE; break;       // Too many open files
    case 5: result = EACCES; break;       // Access denied
    case 6: result = EBADF; break;        // Invalid file handle
    case 12: result = EBADF; break;       // Invalid file-access mode
    case 15: result = ENODEV; break;      // Invalid disk number (Invalid drive)
    case 16: result = ENOTEMPTY; break;   // Cannot remove current directory
    case 17: result = EXDEV; break;       // Cannot rename across volumes
    // I/O errors
    case 100: result = EIO; break;        // Error when reading from disk (End of file)
    case 101: result = EIO; break;        // Error when writing to disk (Disk full)
    case 102: result = EBADF; break;      // File not assigned
    case 103: result = EBADF; break;      // File not opened
    case 104: result = EPERM; break;      // File not opened for input
    case 105: result = EPERM; break;      // File not opened for output
    case 106: result = EDOM; break;       // Invalid input
    // Fatal errors
    case 150: result = EPERM; break;      // Disk is write protected
    case 151: result = ENODEV; break;     // Unknown device
    case 152: result = EBUSY; break;      // Drive not ready
    case 153: result = ENOSYS; break;     // Unknown command
    case 154: result = ERANGE; break;     // CRC check failed
    case 155: result = EFAULT; break;     // Invalid drive specified
    case 156: result = EIO; break;        // Seek error on disk
    case 157: result = ENOTTY; break;     // Invalid media type
    case 158: result = EIO; break;        // Sector not found
    case 159: result = ENOSPC; break;     // Printer out of paper
    case 160: result = EIO; break;        // Error when writing to device
    case 161: result = EIO; break;        // Error when reading from device
    case 162: result = EIO; break;        // Hardware failure
    default: break;
  }

  return result;
}

#pragma pack(pop)
