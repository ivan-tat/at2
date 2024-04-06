#!/bin/make -f
# SPDX-FileType: SOURCE
# SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
# SPDX-License-Identifier: GPL-3.0-or-later

# Variables

# AT2SDK                Full SDK path
# AT2SDK_HOST_SUBDIR    Sub-directory name for host files (SDK or local)
# AT2SDK_TARGET_SUBDIR  Sub-directory name for target files (SDK or local)
# AT2SDK_PREFIX         Full SDK path for host files
# AT2SDK_ENVSET         Flag if env. var. "PATH" is already updated
# OS_HOST               Host OS (internal usage)
# CPU_HOST              Host CPU (internal usage)
# OS_TARGET             Target OS (internal usage)
# CPU_TARGET            Target CPU (internal usage)
# FILE_OS_HOST          Host OS for filenames and paths
# FILE_CPU_HOST         Host CPU for filenames and paths (valid in Linux)
# FILE_OS_TARGET        Target OS for filenames and paths
# FILE_CPU_TARGET       Target CPU for filenames and paths (valid in Linux)
# EH                    Host binary executable file extension
# E                     Target binary executable file extension
# EVPathDelim           Environment variable "PATH" directories delimiter
# DATEPROG              `date' program
# WINDRES               Windows resources compiler for target
# GCC                   GNU C compiler (GCC) for target
# GCCFLAGS              Flags for GCC
# GCC_OS_HOST           Host OS for GCC (used in filenames and paths)
# GCC_CPU_HOST          Host CPU for GCC (option `-march=')
# GCC_OS_TARGET         Target OS for GCC (used in filenames and paths)
# GCC_CPU_TARGET        Target CPU for GCC (option `-march=')
# FPC                   Free Pascal compiler (FPC) for target
# FPCFLAGS              Flags for FPC
# FPC_OS_HOST           Host OS for FPC (option `-T')
# FPC_CPU_HOST          Host CPU for FPC (option `-P')
# FPC_OS_TARGET         Target OS for FPC (option `-T')
# FPC_CPU_TARGET        Target CPU for FPC (option `-P')

SHELL=bash
INSTALL?=install
INSTALL_PROGRAM?=$(INSTALL) -m 755
INSTALL_DATA?=$(INSTALL) -m 644

MAKECMDGOALS?=all

AT2SDK:=$(patsubst %/,%,$(realpath $(dir $(lastword $(MAKEFILE_LIST)))))

EH=

ifeq ($(OS),Windows_NT)

 OS_HOST=Windows
 FILE_OS_HOST=windows
 GCC_OS_HOST=w64-mingw32
 CPU_HOST:=$(PROCESSOR_ARCHITECTURE)
 ifeq ($(CPU_HOST),X86)
  FILE_CPU_HOST=i386
  GCC_CPU_HOST=i386
  FPC_OS_HOST=win32
  FPC_CPU_HOST=i386
 else ifneq ($(findstring $(CPU_HOST),AMD64 EM64T),)
  FILE_CPU_HOST=x86_64
  GCC_CPU_HOST=x86-64
  FPC_OS_HOST=win64
  FPC_CPU_HOST=x86_64
 else
  FILE_CPU_HOST?=unknown
  GCC_CPU_HOST?=unknown
  FPC_OS_HOST?=windows
  FPC_CPU_HOST?=unknown
  $(warning !!! Unknown host CPU architecture (using defaults))
 endif
 EH=.exe
 EVPathDelim=;
 DATEPROG?=date

else ifneq ($(DJGPP),)

 OS_HOST=DJGPP
 CPU_HOST=i386
 FILE_OS_HOST=go32v2
 FILE_CPU_HOST=i386
 GCC_OS_HOST=pc-msdosdjgpp
 GCC_CPU_HOST=i386
 FPC_OS_HOST=go32v2
 FPC_CPU_HOST=i386
 EH=.exe
 EVPathDelim=;
 DATEPROG?=$(patsubst %djgpp.env,%bin/date.exe,$(DJGPP))

else

 _HostOS:=$(shell uname -s 2>/dev/null)
 _HostCPU:=$(shell uname -m 2>/dev/null)

 ifeq ($(_HostOS),Linux)

  OS_HOST=Linux
  FILE_OS_HOST=linux
  GCC_OS_HOST=linux-gnu
  FPC_OS_HOST=linux
  ifneq ($(findstring $(_HostCPU),i386 i486 i586 i686),)
   CPU_HOST:=$(_HostCPU)
   FILE_CPU_HOST=i386
   GCC_CPU_HOST=i386
   FPC_CPU_HOST=i386
  else ifeq ($(_HostCPU),x86_64)
   CPU_HOST:=$(_HostCPU)
   FILE_CPU_HOST=x86_64
   GCC_CPU_HOST=x86-64
   FPC_CPU_HOST=x86_64
  else
   ifeq ($(CPU_HOST),)
    CPU_HOST:=$(if $(_HostCPU),$(_HostCPU),unknown)
   endif
   FILE_CPU_HOST?=unknown
   GCC_CPU_HOST?=unknown
   FPC_CPU_HOST?=unknown
   $(warning !!! Unknown host CPU architecture (using defaults))
  endif
  EH=
  EVPathDelim=:
  DATEPROG?=date

 else

  ifeq ($(OS_HOST),)
   OS_HOST:=$(if $(_HostOS),$(_HostOS),unknown)
  endif
  ifeq ($(CPU_HOST),)
   CPU_HOST:=$(if $(_HostCPU),$(_HostCPU),unknown)
  endif
  FILE_OS_HOST?=unknown
  FILE_CPU_HOST?=unknown
  GCC_OS_HOST?=unknown
  GCC_CPU_HOST?=unknown
  FPC_OS_HOST?=unknown
  FPC_CPU_HOST?=unknown
  EVPathDelim?=:
  $(warning !!! Running unknown OS (using defaults))

 endif

 # Old `make' has no `undefine' directive
 _HostOS=
 _HostCPU=
endif

ifeq ($(FILE_OS_HOST)-$(FILE_CPU_HOST),unknown-unknown)
 AT2SDK_HOST_SUBDIR?=unknown
else
 ifeq ($(AT2SDK_HOST_SUBDIR),)
  AT2SDK_HOST_SUBDIR:=$(FILE_OS_HOST)-$(FILE_CPU_HOST)
 endif
endif

AT2SDK_PREFIX:=$(AT2SDK)/$(AT2SDK_HOST_SUBDIR)

# AT2SDK_ENVSET (if set acts as a flag of the properly configured environment)

ifndef AT2SDK_ENVSET
 export AT2SDK_ENVSET=1

 # PATH (set it only once on first inclusion of "Common.mak" file)
 export PATH:=$(AT2SDK_PREFIX)/bin$(EVPathDelim)$(PATH)
endif

# Binary executable file extension (target)
E=

BUILD?=native
ifeq ($(BUILD),native)
 OS_TARGET?=$(OS_HOST)
 CPU_TARGET?=$(CPU_HOST)
 FILE_OS_TARGET?=$(FILE_OS_HOST)
 FILE_CPU_TARGET?=$(FILE_CPU_HOST)
 GCC_OS_TARGET?=$(GCC_OS_HOST)
 GCC_CPU_TARGET?=$(GCC_CPU_HOST)
 FPC_OS_TARGET?=$(FPC_OS_HOST)
 FPC_CPU_TARGET?=$(FPC_CPU_HOST)
 AT2SDK_TARGET_SUBDIR?=$(AT2SDK_HOST_SUBDIR)
 E=$(EH)
else ifeq ($(BUILD),go32v2-i386)
 OS_TARGET=DJGPP
 CPU_TARGET=i386
 FILE_OS_TARGET=go32v2
 FILE_CPU_TARGET=i386
 GCC_OS_TARGET=pc-msdosdjgpp
 GCC_CPU_TARGET=i386
 FPC_OS_TARGET=go32v2
 FPC_CPU_TARGET=i386
 E=.exe
else ifeq ($(BUILD),linux-i386)
 OS_TARGET=Linux
 CPU_TARGET=i386
 FILE_OS_TARGET=linux
 FILE_CPU_TARGET=i386
 GCC_OS_TARGET=linux-gnu
 GCC_CPU_TARGET=i386
 FPC_OS_TARGET=linux
 FPC_CPU_TARGET=i386
 E=
else ifeq ($(BUILD),linux-x86_64)
 OS_TARGET=Linux
 CPU_TARGET=x86_64
 FILE_OS_TARGET=linux
 FILE_CPU_TARGET=x86_64
 GCC_OS_TARGET=linux-gnu
 GCC_CPU_TARGET=x86-64
 FPC_OS_TARGET=linux
 FPC_CPU_TARGET=x86_64
 E=
else ifeq ($(BUILD),win32-i386)
 OS_TARGET=Windows
 CPU_TARGET=i386
 FILE_OS_TARGET=windows
 FILE_CPU_TARGET=i386
 GCC_OS_TARGET=w64-mingw32
 GCC_CPU_TARGET=i386
 FPC_OS_TARGET=win32
 FPC_CPU_TARGET=i386
 E=.exe
else ifeq ($(BUILD),win64-x86_64)
 OS_TARGET=Windows
 CPU_TARGET=x86_64
 FILE_OS_TARGET=windows
 FILE_CPU_TARGET=x86_64
 GCC_OS_TARGET=w64-mingw32
 GCC_CPU_TARGET=x86-64
 FPC_OS_TARGET=win64
 FPC_CPU_TARGET=x86_64
 E=.exe
else
 $(error Unknown build type: $(BUILD))
endif

ifeq ($(AT2SDK_TARGET_SUBDIR),)
 AT2SDK_TARGET_SUBDIR:=$(FILE_OS_TARGET)-$(FILE_CPU_TARGET)
endif

# DEBUG flag

DEBUG?=0
ifneq ($(DEBUG),0)
 DEBUG=1
endif

# Setup GNU C compiler (target)

ifeq ($(GCC),)
 ifeq ($(OS_HOST),Linux)
  ifeq ($(OS_TARGET),DJGPP)
   GCC=i386-pc-msdosdjgpp-gcc
  else ifeq ($(OS_TARGET),Windows)
   ifeq ($(CPU_TARGET),i386)
    GCC=i686-w64-mingw32-gcc
   else ifeq ($(CPU_TARGET),x86_64)
    GCC=x86_64-w64-mingw32-gcc
   else
    $(error No GCC defined for target CPU: $(CPU_TARGET))
   endif
  endif
 endif
endif
GCC?=gcc
ifneq ($(GCC_CPU_TARGET),unknown)
 GCCFLAGS+=-march=$(GCC_CPU_TARGET)
endif
ifneq ($(findstring $(GCC_CPU_TARGET),i386 i486 i586 i686),)
 GCCFLAGS+=-m32
else ifeq ($(GCC_CPU_TARGET),x86-64)
 GCCFLAGS+=-m64
endif


GCCFLAGS+=-DUSE_FPC=1\
 -fno-exceptions\
 -fno-asynchronous-unwind-tables\
 -fno-unwind-tables\
 -fno-ident\
 -fno-pie

ifneq ($(DEBUG),0)
 GCCFLAGS+=-g -DDEBUG
endif

# Setup Free Pascal compiler (target)

FPC?=fpc
FPCVERSION:=$(shell $(FPC) -iV)

ifneq ($(FPC_OS_TARGET),unknown)
 FPCFLAGS+=-T$(FPC_OS_TARGET)
endif

ifneq ($(FPC_CPU_TARGET),unknown)
 FPCFLAGS+=-P$(FPC_CPU_TARGET)
 ifeq ($(FPC_CPU_TARGET),i386)
  FPCFLAGS+=-OpPENTIUM2
 endif
endif

ifneq ($(DEBUG),0)
 FPCFLAGS+=-g -gh -gl -dDEBUG
endif

ifeq ($(OS_HOST),Linux)
 $(info Added paths for units: /usr/lib/$(FILE_CPU_HOST)-$(GCC_OS_HOST)/fpc/$(FPCVERSION)/units/$(FPC_CPU_TARGET)-$(FPC_OS_TARGET))
 FPCFLAGS+=\
  -Fu/usr/lib/$(FILE_CPU_HOST)-$(GCC_OS_HOST)/fpc/$(FPCVERSION)/units/$(FPC_CPU_TARGET)-$(FPC_OS_TARGET)\
  -Fu/usr/lib/$(FILE_CPU_HOST)-$(GCC_OS_HOST)/fpc/$(FPCVERSION)/units/$(FPC_CPU_TARGET)-$(FPC_OS_TARGET)/*\
  -Fu/usr/lib/$(FILE_CPU_HOST)-$(GCC_OS_HOST)/fpc/$(FPCVERSION)/units/$(FPC_CPU_TARGET)-$(FPC_OS_TARGET)/rtl
 ifeq ($(OS_TARGET),Linux)
  ifneq ($(CPU_HOST),$(CPU_TARGET))
   ifeq ($(FPC_CPU_TARGET),i386)
    $(info Added paths for libraries: /usr/lib/$(FILE_CPU_TARGET)-$(GCC_OS_TARGET))
    FPCFLAGS+=-Fl/usr/lib/$(FILE_CPU_TARGET)-$(GCC_OS_TARGET)
   endif
  endif
 endif
endif

# Setup Windows resources compiler (target)

ifeq ($(OS_TARGET),Windows)
 ifeq ($(OS_HOST),Linux)
  WINDRES?=i686-w64-mingw32-windres
 else
  WINDRES?=windres
 endif
endif
