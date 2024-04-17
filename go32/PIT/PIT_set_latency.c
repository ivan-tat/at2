// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Channel: 0-2
__inline__ void PIT_set_latency (uint8_t channel, uint32_t latency) {
  uint8_t command = (channel << 6) | 0x36;
  // Bits 7-6     - Select channel: 0-2
  // Bits 5-4 (3) - Access mode: LSB/MSB
  // Bits 3-1 (3) - Operating mode: square wave generator
  // Bit 0 (0)    - BCD/Binary mode: 16-bit binary
  uint16_t port = 0x40 + channel; // Channel data port (read/write)

  latency &= 0xFFFF;

  __asm__ __volatile__ (
    "pushfl" "\n\t" // save original IF
    "cli" "\n\t"
    "outb    %%al,$0x43" "\n\t" // Mode/Command register (write only)
    "movb    %%cl,%%al" "\n\t"
    "outb    %%al,(%%dx)" "\n\t" // Set LSB
    "movb    %%ch,%%al" "\n\t"
    "outb    %%al,(%%dx)" "\n\t" // Set MSB
    "popfl" // restore original IF
    :
    : "a" (command), "c" (latency), "d" (port)
  );
}
