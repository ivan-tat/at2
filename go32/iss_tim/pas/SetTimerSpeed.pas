// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

Procedure SetTimerSpeed(NewTimerSpeed : DWord);
Begin
 If NewTimerSpeed<>TimerSpeed Then Begin
   Asm
    PUSH EAX
    CLI
    MOV  AL,00110110B
    OUT  43H,AL
    MOV  EAX,NEWTIMERSPEED
    OUT  40H,AL
    MOV  AL,AH
    OUT  40H,AL
    STI
    POP  EAX
   End;
   TimerSpeed:=NewTimerSpeed;
  End;
End;
