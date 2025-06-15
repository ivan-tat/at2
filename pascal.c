// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2025 Ivan Tatarinov
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

#pragma pack(pop)
