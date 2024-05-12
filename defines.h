// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef DEFINES_H
#define DEFINES_H

// Various macros defined by various GCC versions

// i386-pc-msdosdjgpp-gcc
#ifndef DJGPP
#define DJGPP 0
#endif // !defined(DJGPP)

// i386-pc-msdosdjgpp-gcc
#ifndef GO32
#define GO32 0
#endif // !defined(GO32)

// i386-pc-msdosdjgpp-gcc
#ifndef MSDOS
#define MSDOS 0
#endif // !defined(MSDOS)

// i686-w64-mingw32-gcc, x86_64-w64-mingw32-gcc
#ifndef WIN32
#define WIN32 0
#endif // !defined(WIN32)

// x86_64-w64-mingw32-gcc
#ifndef WIN64
#define WIN64 0
#endif // !defined(WIN64)

// *-w64-mingw32-gcc
#ifndef WINNT
#define WINNT 0
#endif // !defined(WINNT)

// *-linux-gnu-gcc
#ifndef linux
#define linux 0
#endif // !defined(linux)

// *-linux-gnu-gcc
#ifndef unix
#define unix 0
#endif // !defined(unix)

// i386-pc-msdosdjgpp-gcc, i686-w64-mingw32-gcc
#ifndef i386
#define i386 0
#endif // !defined(i386)

// i686-w64-mingw32-gcc
#ifndef _X86_
#define _X86_ 0
#endif // !defined(_X86_)

// x86_64-*-gcc
#ifndef __amd64
#define __amd64 0
#endif // !defined(__amd64)

// x86_64-*-gcc
#ifndef __x86_64
#define __x86_64 0
#endif // !defined(__x86_64)

// Project-specific defines

#ifndef ADT2PLAY
#define ADT2PLAY 0
#endif // !defined(ADT2PLAY)

// GCC-specific defines

#ifndef GCC_VERSION
#define GCC_VERSION (__GNUC__ * 1000 + __GNUC_MINOR__)
#endif // !defined(GCC_VERSION)

#if GCC_VERSION >= 2007
# define GCC_ATTRIBUTE(x) __attribute__(x)
#else // GCC_VERSION < 2007
# define GCC_ATTRIBUTE(x)
#endif // GCC_VERSION < 2007

#ifdef __GNUC__

// Common function, variable and field attributes

#if GCC_VERSION >= 3000
# define __ALIGNED     __attribute__((aligned))
# define __ALIGNED_(x) __attribute__((aligned(x))) // x is a power of 2
#else // GCC_VERSION < 3000
# define __ALIGNED
# define __ALIGNED_(x)
#endif // GCC_VERSION < 3000

#define __UNUSED GCC_ATTRIBUTE((unused))

// Common function attributes

#if GCC_VERSION >= 5000
# define __NO_REORDER __attribute__((no_reorder))
#else // GCC_VERSION < 5000
# define __NO_REORDER
#endif // GCC_VERSION < 5000

#define __NORETURN GCC_ATTRIBUTE((noreturn))

// Target-specific function attributes

// This is critical so do not check GCC version for it and do not guard it:
#define __NAKED __attribute__((naked))

#if GCC_VERSION >= 7000 && defined(__has_attribute)
# if __has_attribute(naked)
#  define __NAKED_RELAXED __attribute__((naked))
# else // !__has_attribute(naked)
#  define __NAKED_RELAXED
# endif // !__has_attribute(naked)
#else // GCC_VERSION < 7000 || !defined(__has_attribute)
# define __NAKED_RELAXED
#endif // GCC_VERSION < 7000 || !defined(__has_attribute)

// Statement attributes

// From `gcc/system.h' of GCC:
#if GCC_VERSION >= 7000 && defined(__has_attribute)
# if __has_attribute(fallthrough)
#  define __FALLTHROUGH __attribute__((fallthrough));
# else // !__has_attribute(fallthrough)
#  define __FALLTHROUGH
# endif // !__has_attribute(fallthrough)
#else // GCC_VERSION < 7000 || !defined(__has_attribute)
# define __FALLTHROUGH
#endif // GCC_VERSION < 7000 || !defined(__has_attribute)

#else // !defined(__GNUC__)

#define __ALIGNED
#define __ALIGNED_(x)
#define __UNUSED
#define __NO_REORDER
#define __NORETURN
#define __NAKED
#define __NAKED_RELAXED
#define __FALLTHROUGH

#endif // !defined(__GNUC__)

#endif // !defined(DEFINES_H)
