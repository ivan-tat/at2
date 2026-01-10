#!/bin/bash
# SPDX-FileType: SOURCE
# SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
# SPDX-License-Identifier: GPL-3.0-or-later

if [[ $# -eq 1 ]]; then
  FileName="$1"
  FontHeight=16
elif [[ $# -eq 2 ]]; then
  FileName="$1"
  FontHeight="$2"
else
  cat <<-EOT
	Usage:
	  $0  filename [font_height]
	where:
	  filename    - file name of PBM font in P1 text format
	  font_height - number in range [6..16] (default is 16)
EOT
  exit 1
fi

if [[ "$FontHeight" -lt 6 || "$FontHeight" -gt 16 ]]; then
  echo 'Bad font height'
  exit 1
fi

FontWidth=8
Filler='▒'
#Filler='█'

readarray Lines < "$FileName"

if [[ "${Lines[0]}" -ne 'P1' ]]; then
  echo 'Bad file format'
  exit 1
fi

i=1
while [[ $i -lt ${#Lines[@]} ]]; do
  s="${Lines[$i]}"
  if [[ -n "$s" && "${s:0:1}" != "#" ]]; then break; fi
  let i=i+1
done

read Width Height<<<"${Lines[$i]}"
let i=i+1

if [[ $Width -eq 0 || $((Width % FontWidth)) -ne 0 ]]; then
  echo 'Bad input image width'
  exit 1
fi
if [[ $Height -eq 0 || $((Height % FontHeight)) -ne 0 ]]; then
  echo 'Bad input image height'
  exit 1
fi

Quads=' ▘▝▀▖▌▞▛▗▚▐▜▄▙▟█'
#Quads=' 𜰡𜰢𜰣𜰤𜰥𜰦𜰧𜰨𜰩𜰪𜰫𜰬𜰭𜰮𜰯'

num=0
y=0
while [[ "$y" -lt $Height ]]; do
  if [[ $((y % FontHeight)) -eq 0 ]]; then
    count=$((Width / FontWidth))
    while [[ "$count" -gt 0 ]]; do
      if [[ $num -lt 256 ]]; then
        printf "$Filler$Filler%02X$Filler" $num
      else
        printf "$Filler%03X$Filler" $num
      fi
      let num=num+1
      let count=count-1
    done
    echo "$Filler"
  fi

  s0="${Lines[$i]}"
  s1="${Lines[$((i+1))]}"
  x=0
  while [[ "$x" -lt $Width ]]; do
    if [[ $((x % FontWidth)) -eq 0 ]]; then echo -n "$Filler"; fi
    c=0
    if [[ "${s0:$x:1}"       = 1 ]]; then let c=c+1; fi
    if [[ "${s0:$((x+1)):1}" = 1 ]]; then let c=c+2; fi
    if [[ "${s1:$x:1}"       = 1 ]]; then let c=c+4; fi
    if [[ "${s1:$((x+1)):1}" = 1 ]]; then let c=c+8; fi
    let x=x+2
    echo -n "${Quads:$c:1}"
  done

  echo "$Filler"

  let i=i+2
  let y=y+2
done

count=$((Width / 2 + Width / FontWidth + 1))
s=''
while [[ "$count" -gt 0 ]]; do
  s="$s$Filler"
  let count=count-1
done
echo "$s"
