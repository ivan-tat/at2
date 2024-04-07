#!/bin/sh -e
reset
C=./Make-DOS.conf
B=go32v2-i386
V=$(cat adt2play.dos)
D=build/DOS
test -f $C && . $C
mkdir -p $D
make $MAKEFLAGS -w -C $D -f $PWD/Makefile BUILD=$B AT2PLAY_VERSION=$V $*
