// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static __LOCK_FUNC __NAKED void SysKeyboardIRQ (void) {
  __asm__ __volatile__ (
    "cli" "\n\t"
    "pushl   %ds" "\n\t"
    "pushl   %es" "\n\t"
    "pushl   %fs" "\n\t"
    "pushl   %gs" "\n\t"
    "pusha" "\n\t"
    "movzwl  %cs:" _ASM_SYM_STR (__djgpp_ds_alias) ",%eax" "\n\t"
    "movw    %ax,%ds" "\n\t"
    "movw    %ax,%es" "\n\t"
#if USE_FPC
    "movzwl  " _ASM_SYM_STR (_dos_ds) ",%eax" "\n\t"
    "movw    %ax,%fs" "\n\t"
#endif // USE_FPC
    "call    *" _ASM_SYM_STR (SysKeyboardCallback) "\n\t"
    "popa" "\n\t"
    "popl    %gs" "\n\t"
    "popl    %fs" "\n\t"
    "popl    %es" "\n\t"
    "popl    %ds" "\n\t"
    "jmp     *%cs:" _ASM_SYM_STR (SysKeyboardOldIRQ)
  );
}

END_OF_STATIC_FUNCTION (SysKeyboardIRQ)

static void lock_SysKeyboardIRQ (bool lock) {
  LOCK_DECL (lock, v, f);

  LOCK_VARIABLE (v, __djgpp_ds_alias);
#if USE_FPC
  LOCK_VARIABLE (v, _dos_ds);
#endif // USE_FPC
  LOCK_VARIABLE (v, SysKeyboardCallback);
  LOCK_FUNCTION (f, SysKeyboardIRQ);
}
