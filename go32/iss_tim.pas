
{ � Inquisition's Timer Services Unit � }

{ � This file is part of the Inquisition Sound Server for the Free � }
{ � Pascal Compiler (http://www.freepascal.org) but also can be    � }
{ � be distributed separately. The source code is FREE FOR ANY NON � }
{ � COMMERCIAL USAGE.                                              � }
{ � You can modify this file, but you musn't distribute the        � }
{ � modified file, only the original version. Instead, send your   � }
{ � modification to us, so we can add it to the official version.  � }
{ � Please note, that we can't quarantee the compatibility with    � }
{ � previous versions.                                             � }
{ � If we'll stop the development of this unit in the future,      � }
{ � the source code will be freely available for any use.          � }

{ � You can always download the newest version from our website,  � }
{ � http://scenergy.dfmk.hu/inqcoders/                            � }
{ � About Inquisition itself, see                                 � }
{ � http://scenergy.dfmk.hu/inquisition/                          � }

{ � Comments, notes, suggestions, bug reports are welcome.      � }
{ � Send your mails to charlie@scenergy.dfmk.hu                 � }
{ � Please prefer hungarian or english languages.               � }

{ � ISS_TIM - Timer Unit (GO32V2 Only!)      � }
{ � Coding Starts     : 10. October. 1998.   � }
{ � Last Modification : 01. March. 2001.     � }

{ � Note: req. FPC version 1.0.0+ for GO32V2 to compile � }

Unit ISS_Tim;
{$L go32/iss_tim.o}
Interface

uses
  PIT;

var   ISS_TimerSpeed  : DWord; cvar; external;
const ISS_MaxTimers   = $8; { � Maximum Number of Timers  � }

      ISS_TENoFree   = $01; { � Can't add new timer. All timers locked. � }
      ISS_TENotFound = $02; { � Can't find specified Timer, to stop. � }

Type TTimerStruc = Record
       TSpeed     : DWord;
       TCount     : DWord;     { � Tick Counter � }
       TPrevCount : DWord;     { � Tick Counter state at prev. activity � }
       TProc      : Pointer;   { � Procedure To Call Offset � }
       TActive    : Boolean;   { � 1 If The Timer Is On � }
      End;

var ISS_TimersData: array[0..ISS_MaxTimers-1] of TTimerStruc; cvar; external;
    ISS_TimerError: DWord; cvar; external;

procedure ISS_DisableTimerIRQ; cdecl; external;
procedure ISS_EnableTimerIRQ; cdecl; external;
function ISS_StartTimer (NewTProc: Pointer; NewTSpeed: Longword): Boolean; cdecl; external;
function ISS_StopTimer (TimerProc: Pointer): Boolean; cdecl; external;
function ISS_GetTimerNumber (TimerProc: Pointer): Longint; cdecl; external;

procedure init_ISS_Timer; cdecl; external;

Implementation

uses
  go32,
  dpmi,
  pascal;

Begin
  init_ISS_Timer;
End.
{ � ISS_TIM.PAS - (C) 1998-2001 Charlie/Inquisition � }

{ � Changelog : � }
{ � 1.1.1 - Some code cleanup for less compiler hacking...                � }
{ �       - Webpage and email addresses fixed in the header comment.      � }
{ �         [01.march.2001]                                               � }
{ � 1.1.0 - Major update, a new IRQ routine which contains less compiler  � }
{ �         hacking. Based on the docs of FPC 1.0.2. Not tested with      � }
{ �         versions below 1.0.0. GNU AS no longer required to compile.   � }
{ �         [03.december.2000]                                            � }
{ � 1.0.2 - Header comment fixed.                                         � }
{ �         [18.apr.2000]                                                 � }
{ � 1.0.1 - Removed a limitation which made smartlinking impossible.      � }
{ �         (Reported by Surgi/Terror Opera)                              � }
{ �         [13.apr.2000]                                                 � }
{ � 1.0.0 - First Public Version                                          � }
{ �         [08.jan.2000]                                                 � }
