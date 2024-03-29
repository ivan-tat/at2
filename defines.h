// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef DEFINES_H
#define DEFINES_H

#ifndef DJGPP // i386-pc-msdosdjgpp-gcc
#define DJGPP 0
#endif

#ifndef GO32 // i386-pc-msdosdjgpp-gcc
#define GO32 0
#endif

#ifndef MSDOS // i386-pc-msdosdjgpp-gcc
#define MSDOS 0
#endif

#ifndef WIN32 // i686-w64-mingw32-gcc, x86_64-w64-mingw32-gcc
#define WIN32 0
#endif

#ifndef WIN64 // x86_64-w64-mingw32-gcc
#define WIN64 0
#endif

#ifndef WINNT // *-w64-mingw32-gcc
#define WINNT 0
#endif

#ifndef linux // *-linux-gnu-gcc
#define linux 0
#endif

#ifndef unix // *-linux-gnu-gcc
#define unix 0
#endif

#ifndef i386 // i386-pc-msdosdjgpp-gcc, i686-w64-mingw32-gcc
#define i386 0
#endif

#ifndef _X86_ // i686-w64-mingw32-gcc
#define _X86_ 0
#endif

#ifndef __amd64 // x86_64-*-gcc
#define __amd64 0
#endif

#ifndef __x86_64 // x86_64-*-gcc
#define __x86_64 0
#endif

#endif // !defined(DEFINES_H)
