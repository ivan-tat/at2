// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef PLATFORM_H
#define PLATFORM_H

#include "defines.h"
#if GO32
#if USE_FPC
#include "pascal.h"
#endif // USE_FPC
#endif // GO32

#if GO32

#define _ASM_SYM_STR(x) "_" #x

#if USE_FPC
#define LFNSupport Pascal_LFNSupport
#else // !USE_FPC
#define LFNSupport false
#endif // !USE_FPC

#define DRIVE_SEP ':'
#define DIR_SEP '\\'
#define DIR_SEP_STR "\\"

#elif WIN32||WIN64

#define _ASM_SYM_STR(x) "_" #x

#define LFNSupport true

#define DRIVE_SEP ':'
#define DIR_SEP '\\'
#define DIR_SEP_STR "\\"

#elif linux||unix

#define _ASM_SYM_STR(x) "" #x

#define LFNSupport true

#define DRIVE_SEP '\0' // none
#define DIR_SEP '/'
#define DIR_SEP_STR "/"

#endif // linux||unix

#define MAX_FILENAME_LEN 255

#define MAX_PATH_LEN MAX_FILENAME_LEN
#define MAX_DIR_LEN  MAX_FILENAME_LEN
#define MAX_NAME_LEN MAX_FILENAME_LEN
#define MAX_EXT_LEN  MAX_FILENAME_LEN

#endif // !defined(PLATFORM_H)
