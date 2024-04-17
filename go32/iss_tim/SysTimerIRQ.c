// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static __LOCK_FUNC __NAKED void SysTimerIRQ (void) {
  __asm__ __volatile__ (
    "cli" "\n\t"
    "pushl   %ds" "\n\t"
    "pushl   %es" "\n\t"
    "pushl   %fs" "\n\t"
    "pushl   %gs" "\n\t"
    "pusha" "\n\t"
    "movzwl  %cs:_" "__djgpp_ds_alias,%eax" "\n\t"
    "movw    %ax,%ds" "\n\t"
    "movw    %ax,%es" "\n\t"

#if USE_FPC
    "movzwl  _" "_dos_ds,%eax" "\n\t"
    "movw    %ax,%fs" "\n\t"
#endif // USE_FPC

    "call    _" "UpdateUserTimers" "\n\t"

    "movl    _" "TimerSpeed,%eax" "\n\t"
    "movl    $_" "OldTimerCnt,%ebx" "\n\t"
    "addl    %eax,(%ebx)" "\n\t"
    "movl    $0x10000,%eax" "\n\t" // PIT_LATENCY_MAX
    "cmpl    %eax,(%ebx)" "\n\t"
    "jb      L_skip" "\n\t"

      "subl    %eax,(%ebx)" "\n\t"
#if !USE_FPC
      "movzwl  _" "_dos_ds,%eax" "\n\t"
      "movw    %ax,%fs" "\n\t"
#endif // !USE_FPC
      "movl    $1573042,%eax" "\n\t"
      // (24 * 60 * 60 * PIT_FREQ_MAX / PIT_LATENCY_MAX = 1573042)
      "movl    $0x46C,%ebx" "\n\t"
      "incl    %fs:(%ebx)" "\n\t" // Timer tick counter
      // (count of (PIT_LATENCY_MAX / PIT_FREQ_MAX * 1000) ms ticks since CPU
      // reset)
      "cmpl    %eax,%fs:(%ebx)" "\n\t"
      "jbe     L_skip" "\n\t"
      "subl    %eax,%fs:(%ebx)" "\n\t"
      "incb    %fs:4(%ebx)" "\n" // Timer overflow flag
      // (timer has rolled over 24 hours)
  "L_skip:" "\n\t"
    "movb    $0x20,%al" "\n\t"
    "outb    %al,$0x20" "\n\t" // Interrupt request acknowledge
    "popa" "\n\t"
    "popl    %gs" "\n\t"
    "popl    %fs" "\n\t"
    "popl    %es" "\n\t"
    "popl    %ds" "\n\t"
    "iretl"
  );
}

END_OF_STATIC_FUNCTION (SysTimerIRQ)

static void lock_SysTimerIRQ (bool lock) {
  LOCK_DECL (lock, v, f);

  LOCK_VARIABLE (v, __djgpp_ds_alias);
  LOCK_VARIABLE (v, _dos_ds);
  LOCK_VARIABLE (v, ISS_TimersData);
  LOCK_VARIABLE (v, TimerSpeed);
  LOCK_VARIABLE (v, OldTimerCnt);
  LOCK_FUNCTION (f, SysTimerIRQ);

  lock_UpdateUserTimers (lock);
}
