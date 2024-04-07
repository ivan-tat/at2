#!/bin/sh -e
reset
C=./Make-Linux-i386.conf
B=linux-i386
V=$(cat adtrack2.linux)
D=build/linux-i386
test -f $C && . $C
mkdir -p $D
make $MAKEFLAGS -w -C $D -f $PWD/Makefile BUILD=$B AT2_VERSION=$V $*
