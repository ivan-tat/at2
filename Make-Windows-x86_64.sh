#!/bin/sh -e
reset
C=./Make-Windows-x86_64.conf
B=win64-x86_64
V=$(cat adtrack2.windows)
D=build/windows-x86_64
test -f $C && . $C
mkdir -p $D
make $MAKEFLAGS -w -C $D -f $PWD/Makefile BUILD=$B AT2_VERSION=$V $*
