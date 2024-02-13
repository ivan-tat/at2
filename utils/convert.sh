#!/bin/bash -e
# SPDX-FileType: SOURCE
# SPDX-FileCopyrightText: 2024 Ivan Tatarinov
# SPDX-License-Identifier: GPL-3.0-or-later

opt_binary=0
opt_padding=2
INPUT=
IDENT=
LINES=

on_error() {
	echo "$BASH_SOURCE: Error on line $BASH_LINENO. Stopped." >&2
}

show_usage() {
	cat <<-EOT >&2
	Helper script to convert binary data in hexadecimal format written in Object Pascal source code into C source code.

	Usage:
	  $0 [option ...] [INPUT IDENT LINES]

	Options:
	  -b     Binary output format with one byte per line (default is multi-byte hexadecimal output)
	  -p N   Number of spaces to put at the beginning of the line (default is $opt_padding)
	  INPUT  input file with Object Pascal source code
	  IDENT  Pascal identifier (acts as a marker of the beginning of binary data)
	  LINES  number of lines to process after identifier

	If no parameters were specified - show this help.
	EOT
}

trap on_error ERR

if [[ $# -eq 0 ]]; then
	show_usage
	exit
fi

i=0
while [[ $# -ne 0 ]]; do
	case "$1" in
	-b)	opt_binary=1
		;;
	-p)	if [[ $# -eq 1 ]]; then
			echo "Error: Missing argument for \"-p\"." >&2
			exit 1
		fi
		shift 1
		if [[ $1 -eq 0 ]]; then
			opt_padding=
		else
			let opt_padding=$1
		fi
		;;
	*)	case $i in
		0)	INPUT="$1";;
		1)	IDENT=$1;;
		2)	LINES=$1;;
		*)	echo "Error: Too many parameters." >&2
			exit 1;;
		esac
		let i=i+1
		;;
	esac
	shift 1
done

if [[ -z "$INPUT" ]]; then
	echo "Error: No input filename specified." >&2
	exit 1
elif [[ -z "$IDENT" ]]; then
	echo "Error: No identifier specified." >&2
	exit 1
elif [[ -z "$LINES" ]]; then
	echo "Error: No lines count specified." >&2
	exit 1
fi

start=$(grep -nEe '^[[:space:]]*'$IDENT'[[:space:]]*:[[:space:]]*array[[:space:]]*\[[[:space:]]*0[[:space:]]*..[[:space:]]*[0-9]+[[:space:]]*\][[:space:]]*of[[:space:]]+Dword[[:space:]]*=[[:space:]]*\(' "$INPUT" \
| head -n 1 \
| cut -d: -f 1)

pattern='s/^[[:space:]]+/\t/;s/\$0([0-9A-F]{8})/\$\1/g;s/\);//'
if [[ $opt_binary -eq 1 ]]; then
	pattern="$pattern;"'s/\$(..)(..)(..)(..)/\4,\3,\2,\1/g;
s/0/0000/g;s/1/0001/g;s/2/0010/g;s/3/0011/g;s/4/0100/g;s/5/0101/g;s/6/0110/g;s/7/0111/g;
s/8/1000/g;s/9/1001/g;s/A/1010/g;s/B/1011/g;s/C/1100/g;s/D/1101/g;s/E/1110/g;s/F/1111/g;
s/,([01]{8})/,\n\t\1/g;s/([01]{8})/0b\1/g'
else
	pattern="$pattern;"'s/\$(..)(..)(..)(..)/0x\4,0x\3,0x\2,0x\1/g;s/^\t(.{80})/\t\1\n\t/'
fi

if [[ -n "$opt_padding" ]]; then
	tail -n +$(($start+1)) "$INPUT" \
	| head -n $LINES \
	| tr abcdef ABCDEF \
	| sed -Ee "$pattern" \
	| expand -t $opt_padding
else
	tail -n +$(($start+1)) "$INPUT" \
	| head -n $LINES \
	| tr abcdef ABCDEF \
	| sed -Ee "$pattern;s/\t//g"
fi
