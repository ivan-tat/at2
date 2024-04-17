// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

__inline__ void PIT_disable_IRQ (void) {
  __asm__ __volatile__ (
    "inb     $0x21,%%al" "\n\t" // Get interrupts 0-7 mask
    "orb     $0x01,%%al" "\n\t" // Bit 0 (1): disable IRQ 0 (timer)
    "outb    %%al,$0x21" // Set interrupts 0-7 mask
    :
    :
    : "al", "cc"
  );
}
