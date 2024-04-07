#!/bin/sh -e
reset
C=./Make-Linux-x86_64.conf
B=linux-x86_64
V=$(cat adtrack2.linux)
D=build/linux-x86_64
test -f $C && . $C
mkdir -p $D
make $MAKEFLAGS -w -C $D -f $PWD/Makefile BUILD=$B AT2_VERSION=$V $*
