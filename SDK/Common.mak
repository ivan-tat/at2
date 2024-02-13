#!/bin/make -f
# SPDX-FileType: SOURCE
# SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
# SPDX-License-Identifier: GPL-3.0-or-later

# Variables

SHELL=bash
INSTALL?=install
INSTALL_PROGRAM?=$(INSTALL) -m 755
INSTALL_DATA?=$(INSTALL) -m 644

MAKECMDGOALS?=all

buildroot:=$(realpath .)/build

AT2SDK:=$(patsubst %/,%,$(realpath $(dir $(lastword $(MAKEFILE_LIST)))))

# Host binary executable file extension
EH=

ifeq ($(OS),Windows_NT)

 OS_HOST:=$(OS)
 CPU_HOST:=$(PROCESSOR_ARCHITECTURE)
 ifeq ($(CPU_HOST),X86)
  AT2SDK_BUILD_SUBDIR=windows-i386
  OS_SOURCE=win32
  CPU_SOURCE=i386
 else ifneq ($(findstring $(CPU_HOST),AMD64 EM64T),)
  AT2SDK_BUILD_SUBDIR=windows-x86_64
  OS_SOURCE=win64
  CPU_SOURCE=x86_64
 else
  AT2SDK_BUILD_SUBDIR=windows-unknown
  OS_SOURCE?=windows
  CPU_SOURCE?=$(CPU_HOST)
  $(warning !!! Unknown host CPU architecture: using CPU_SOURCE=$(CPU_SOURCE))
 endif
 EH=.exe
 DATEPROG?=date
 EVPathDelim=;

else ifneq ($(DJGPP),)

 AT2SDK_BUILD_SUBDIR=go32v2-i386
 OS_HOST=DJGPP
 CPU_HOST=i386
 OS_SOURCE=go32v2
 CPU_SOURCE=i386
 EH=.exe
 DATEPROG?=$(patsubst %djgpp.env,%bin/date.exe,$(DJGPP))
 EVPathDelim=;

else

 OS_HOST:=$(shell uname -s 2>/dev/null)

 ifeq ($(OS_HOST),Linux)
  OS_SOURCE=linux
  CPU_HOST:=$(shell uname -m)
  ifneq ($(findstring $(CPU_HOST),i386 i486 i586 i686),)
   AT2SDK_BUILD_SUBDIR=linux-i386
   CPU_SOURCE=i386
  else ifeq ($(CPU_HOST),x86_64)
   AT2SDK_BUILD_SUBDIR=linux-x86_64
   CPU_SOURCE=x86_64
  else
   AT2SDK_BUILD_SUBDIR=linux-unknown
   CPU_SOURCE=$(CPU_HOST)
   $(warning !!! Unknown host CPU architecture: using CPU_SOURCE=$(CPU_SOURCE))
  endif
  EH=
  DATEPROG?=date
  EVPathDelim=:

 else

  AT2SDK_BUILD_SUBDIR=unknown
  ifneq ($(OS_HOST),)
   OS_SOURCE=$(OS_HOST)
   CPU_HOST:=$(shell uname -m)
   ifneq ($(CPU_HOST),)
    CPU_SOURCE=$(CPU_HOST)
   else
    CPU_HOST=unknown
    CPU_SOURCE=unknown
   endif
  else
   OS_HOST=unknown
   OS_SOURCE=unknown
   CPU_HOST=unknown
   CPU_SOURCE=unknown
  endif
  EVPathDelim?=:

  $(warning !!! Running unknown OS:)
  $(warning !!! ...using OS_SOURCE=$(OS_SOURCE))
  $(warning !!! ...using CPU_SOURCE=$(CPU_SOURCE))
  $(warning !!! ...using EVPathDelim=$(EVPathDelim))

 endif
endif

AT2SDK_PREFIX:=$(AT2SDK)/$(AT2SDK_BUILD_SUBDIR)

# AT2SDK_ENVSET (if set acts as a flag of the properly configured environment)

ifndef AT2SDK_ENVSET
 export AT2SDK_ENVSET=1

 # PATH (set it only once on first inclusion of "Common.mak" file)
 export PATH:=$(AT2SDK_PREFIX)/bin$(EVPathDelim)$(PATH)
endif

# Target binary executable file extension
E=

BUILD:=$(if $(BUILD),$(BUILD),native)
ifeq ($(BUILD),native)
 OS_TARGET=$(OS_SOURCE)
 CPU_TARGET=$(CPU_SOURCE)
 E=$(EH)
else ifeq ($(BUILD),go32v2-i386)
 OS_TARGET=go32v2
 CPU_TARGET=i386
 E=.exe
else ifeq ($(BUILD),linux-i386)
 OS_TARGET=linux
 CPU_TARGET=i386
 E=
else ifeq ($(BUILD),linux-x86_64)
 OS_TARGET=linux
 CPU_TARGET=x86_64
 E=
else ifeq ($(BUILD),win32-i386)
 OS_TARGET=win32
 CPU_TARGET=i386
 E=.exe
else ifeq ($(BUILD),win64-x86_64)
 OS_TARGET=win64
 CPU_TARGET=x86_64
 E=.exe
else
 $(error Unknown build type: $(BUILD))
endif

FULL_SOURCE=$(OS_SOURCE)-$(CPU_SOURCE)
FULL_TARGET=$(OS_TARGET)-$(CPU_TARGET)

# Setup Free Pascal compiler

FPC?=fpc
FPCVERSION:=$(shell $(FPC) -iV)

ifneq ($(OS_TARGET),unknown)
 FPCFLAGS+=-T$(OS_TARGET)
endif

ifneq ($(CPU_TARGET),unknown)
 FPCFLAGS+=-P$(CPU_TARGET)
 ifeq ($(CPU_TARGET),i386)
  FPCFLAGS+=-OpPENTIUM2
 endif
endif

FPCFLAGS+=-O2

DEBUG:=$(if $(DEBUG),$(DEBUG),0)
ifneq ($(DEBUG),0)
 DEBUG=1
 FPCFLAGS+=-g -gh -gl -dDEBUG
endif

ifeq ($(OS_SOURCE),linux)
 $(info Added paths for units: /usr/lib/$(CPU_HOST)-linux-gnu/fpc/$(FPCVERSION)/units/$(CPU_TARGET)-$(OS_TARGET))
  FPCFLAGS+=\
   -Fu/usr/lib/$(CPU_HOST)-linux-gnu/fpc/$(FPCVERSION)/units/$(CPU_TARGET)-$(OS_TARGET)\
   -Fu/usr/lib/$(CPU_HOST)-linux-gnu/fpc/$(FPCVERSION)/units/$(CPU_TARGET)-$(OS_TARGET)/*\
   -Fu/usr/lib/$(CPU_HOST)-linux-gnu/fpc/$(FPCVERSION)/units/$(CPU_TARGET)-$(OS_TARGET)/rtl
endif

# Setup Windows resources compiler

ifneq ($(findstring $(OS_TARGET),win32 win64 windows),)
 WINDRES?=windres
endif
