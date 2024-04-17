
{ þ Inquisition's Timer Services Unit þ }

{ þ This file is part of the Inquisition Sound Server for the Free þ }
{ þ Pascal Compiler (http://www.freepascal.org) but also can be    þ }
{ þ be distributed separately. The source code is FREE FOR ANY NON þ }
{ þ COMMERCIAL USAGE.                                              þ }
{ þ You can modify this file, but you musn't distribute the        þ }
{ þ modified file, only the original version. Instead, send your   þ }
{ þ modification to us, so we can add it to the official version.  þ }
{ þ Please note, that we can't quarantee the compatibility with    þ }
{ þ previous versions.                                             þ }
{ þ If we'll stop the development of this unit in the future,      þ }
{ þ the source code will be freely available for any use.          þ }

{ þ You can always download the newest version from our website,  þ }
{ þ http://scenergy.dfmk.hu/inqcoders/                            þ }
{ þ About Inquisition itself, see                                 þ }
{ þ http://scenergy.dfmk.hu/inquisition/                          þ }

{ þ Comments, notes, suggestions, bug reports are welcome.      þ }
{ þ Send your mails to charlie@scenergy.dfmk.hu                 þ }
{ þ Please prefer hungarian or english languages.               þ }

{ þ ISS_TIM - Timer Unit (GO32V2 Only!)      þ }
{ þ Coding Starts     : 10. October. 1998.   þ }
{ þ Last Modification : 01. March. 2001.     þ }

{ þ Note: req. FPC version 1.0.0+ for GO32V2 to compile þ }

Unit ISS_Tim;
{$L go32/iss_tim.o}
Interface

uses
  PIT;

var   ISS_TimerSpeed  : DWord; cvar; external;
const ISS_MaxTimers   = $8; { þ Maximum Number of Timers  þ }

      ISS_TENoFree   = $01; { þ Can't add new timer. All timers locked. þ }
      ISS_TENotFound = $02; { þ Can't find specified Timer, to stop. þ }

Type TTimerStruc = Record
       TSpeed     : DWord;
       TCount     : DWord;     { þ Tick Counter þ }
       TPrevCount : DWord;     { þ Tick Counter state at prev. activity þ }
       TProc      : Pointer;   { þ Procedure To Call Offset þ }
       TActive    : Boolean;   { þ 1 If The Timer Is On þ }
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
{ þ ISS_TIM.PAS - (C) 1998-2001 Charlie/Inquisition þ }

{ þ Changelog : þ }
{ þ 1.1.1 - Some code cleanup for less compiler hacking...                þ }
{ þ       - Webpage and email addresses fixed in the header comment.      þ }
{ þ         [01.march.2001]                                               þ }
{ þ 1.1.0 - Major update, a new IRQ routine which contains less compiler  þ }
{ þ         hacking. Based on the docs of FPC 1.0.2. Not tested with      þ }
{ þ         versions below 1.0.0. GNU AS no longer required to compile.   þ }
{ þ         [03.december.2000]                                            þ }
{ þ 1.0.2 - Header comment fixed.                                         þ }
{ þ         [18.apr.2000]                                                 þ }
{ þ 1.0.1 - Removed a limitation which made smartlinking impossible.      þ }
{ þ         (Reported by Surgi/Terror Opera)                              þ }
{ þ         [13.apr.2000]                                                 þ }
{ þ 1.0.0 - First Public Version                                          þ }
{ þ         [08.jan.2000]                                                 þ }
