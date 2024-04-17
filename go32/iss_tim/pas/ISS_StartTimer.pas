// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

Function ISS_StartTimer(NewTProc : Pointer; NewTSpeed : DWord) : Boolean;
Var Counter : Word;
    TNumber : Word;
    NewTimerHandler: TSegInfo;
Begin
 Counter:=0; TNumber:=0;
 Repeat
  Inc(Counter);
  If Not ISS_TimersData[Counter].TActive Then TNumber:=Counter;
 Until (TNumber<>0) Or (Counter=ISS_MaxTimers);
 If TNumber=0 Then Begin
   ISS_TimerError:=ISS_TENoFree;
   ISS_StartTimer:=False;
   Exit;
  End;
 If Not NewIRQActive Then Begin
   Lock_Data(ISS_TimersData,SizeOf(ISS_TimersData));
   Lock_Data(DosMemSelector,SizeOf(DosMemSelector));
   Lock_Code(@SysTimerIRQ,DWord(@SysTimerIRQ_Dummy)-DWord(@SysTimerIRQ));
   Lock_Code(@UpdateUserTimers,DWord(@UpdateUserTimers_Dummy)-DWord(@UpdateUserTimers));
   NewTimerHandler.Offset:=@SysTimerIRQ;
   NewTimerHandler.Segment:=Get_CS;
   Get_PM_Interrupt(PIT_IRQ,OldTimer);
   Set_PM_Interrupt(PIT_IRQ,NewTimerHandler);
  End;
 ISS_TimersData[TNumber].TSpeed:=NewTSpeed;
 ISS_TimersData[TNumber].TCount:=0;
 ISS_TimersData[TNumber].TProc:=NewTProc;
 ISS_TimersData[TNumber].TActive:=True;
 SetTimerSpeed(GetTimerSpeed);
 ISS_StartTimer:=True;
End;
