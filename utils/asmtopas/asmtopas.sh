#!/bin/bash -e
# SPDX-FileType: SOURCE
# SPDX-FileCopyrightText: 2024 Ivan Tatarinov
# SPDX-License-Identifier: GPL-3.0-or-later

# References:
# * 386DX Microprocessor Programmers Reference Manual 1990
#   https://web.archive.org/web/20140728052748/http://bitsavers.informatik.uni-stuttgart.de/pdf/intel/80386/230985-003_386DX_Microprocessor_Programmers_Reference_Manual_1990.pdf

tmp1=
tmp2=

on_exit() {
	if [[ -n "$tmp1" ]]; then rm -f "$tmp1"; fi
	if [[ -n "$tmp2" ]]; then rm -f "$tmp2"; fi
}

trap on_exit EXIT

if [[ $# -ne 2 ]]; then
	cat <<-EOT
		Helper script for type safe translation of Intel x86 32-bits assembler source code (Intel syntax) into Object Pascal source code.

		Usage:
		  $0 INPUT OUTPUT

		where:
		  INPUT   input file with assembler source code (as found in \`asm ... end;' block)
		  OUTPUT  output file with Pascal source code (to be used in \`begin ... end;' block)
	EOT
	exit
fi

tmp1="$(mktemp)"
tmp2="$(mktemp)"

unexpand -t 8,16 "$1" > "$tmp1"
sed -Ee 's![[:space:]]+$!!;s!@@([0-9A-Za-z_]+)!_L\1!g;s!^(_L[0-9A-Za-z_]+:)!\1\n!' "$tmp1" > "$tmp2"
sed -Ef asmtopas.sed "$tmp2" | expand -t 8,16,50 > "$2"
rm -f "$tmp1" "$tmp2"
tmp1=
tmp2=
