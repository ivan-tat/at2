// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

var
  __v2prt0_ds_alias: Word; cvar; external;

Procedure SysTimerIRQ; Assembler;
Asm
  CLI
  PUSH   DS
  PUSH   ES
  PUSH   FS
  PUSH   GS
  PUSHAD
  MOV    AX,CS:[__v2prt0_ds_alias]
  MOV    DS,AX
  MOV    ES,AX
  MOV    AX,DosMemSelector
  MOV    FS,AX

  CALL   UpdateUserTimers

  MOV    EAX,TimerSpeed
  ADD    OldTimerCnt,EAX
  CMP    OldTimerCnt,$10000
  JB     @NotUpdateClock

    SUB   OldTimerCnt,$10000
    INC   WORD PTR FS:[1132]
    JNZ   @Timer_2
    INC   WORD PTR FS:[1134]
    @Timer_2:
    MOV  AX,$018
    CMP  FS:[1134],AX
    JNZ  @Timer_3
    MOV  AX,$0B0
    CMP  FS:[1132],AX
    JNZ  @Timer_3
    MOV  WORD PTR FS:[1134],$0
    MOV  WORD PTR FS:[1132],$0
    MOV  BYTE PTR FS:[1136],$1
    @Timer_3:

  @NotUpdateClock:
  MOV   DX,$20 { ■ Interrupt request acknowledge ■ }
  MOV   AL,$20
  OUT   DX,AL
  POPAD
  POP   GS
  POP   FS
  POP   ES
  POP   DS
  IRET
End;

Procedure SysTimerIRQ_Dummy; Begin End;
