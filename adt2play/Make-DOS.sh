#!/bin/sh -e
reset
C=./Make-DOS.conf
S="$(pwd)"
D=build/DOS
B=go32v2-i386
test -f $C && . $C
mkdir -p $D
make $MAKEFLAGS -w -C $D -f "$S/Makefile" BUILD=$B $*
