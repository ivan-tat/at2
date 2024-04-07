#!/bin/sh -e
reset
C=./Make-Windows-i386.conf
B=win32-i386
V=$(cat adtrack2.windows)
D=build/windows-i386
test -f $C && . $C
mkdir -p $D
make $MAKEFLAGS -w -C $D -f $PWD/Makefile BUILD=$B AT2_VERSION=$V $*
