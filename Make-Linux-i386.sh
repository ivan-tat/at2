#!/bin/sh -e
reset
C=./Make-Linux-i386.conf
B=linux-i386
D=build/linux-i386
test -f $C && . $C
mkdir -p $D
make $MAKEFLAGS -w -C $D -f $PWD/Makefile BUILD=$B $*
