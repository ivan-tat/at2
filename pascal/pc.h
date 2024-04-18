// DJGPP support
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef PASCAL_PC_H
#define PASCAL_PC_H

#include <stddef.h>
#include <stdint.h>

__inline__ uint8_t inportb (uint16_t port) {
  uint8_t data;

  __asm__ __volatile__ (
    "inb %[port],%[data]"
    : [data] "=a" (data)
    : [port] "dN" (port)
  );

  return data;
}

__inline__ uint16_t inportw (uint16_t port) {
  uint16_t data;

  __asm__ __volatile__ (
    "inw %[port],%[data]"
    : [data] "=a" (data)
    : [port] "dN" (port)
  );

  return data;
}

void inportsb (uint16_t port, uint8_t *buf, size_t len);

__inline__ void outportb (uint16_t port, uint8_t data) {
  __asm__ __volatile__ (
    "outb %[data],%[port]"
    :
    : [port] "dN" (port), [data] "a" (data)
  );
}

__inline__ void outportw (uint16_t port, uint16_t data) {
  __asm__ __volatile__ (
    "outw %[data],%[port]"
    :
    : [port] "dN" (port), [data] "a" (data)
  );
}

void outportsb (uint16_t port, const uint8_t  *buf, size_t len);

#endif // !defined(PASCAL_PC_H)
