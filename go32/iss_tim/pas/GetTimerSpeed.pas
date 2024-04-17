// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

Function GetTimerSpeed : DWord;
Var Counter  : DWord;
    TFastest : DWord;
Begin
 TFastest:=PIT_LATENCY_MAX;
 For Counter:=1 To ISS_MaxTimers Do Begin
   If ISS_TimersData[Counter].TActive And
      (ISS_TimersData[Counter].TSpeed < TFastest) Then
     TFastest:=ISS_TimersData[Counter].TSpeed;
  End;
 GetTimerSpeed:=TFastest;
End;
