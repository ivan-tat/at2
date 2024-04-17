// DJGPP support
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef DPMI_H
#define DPMI_H

#include "pascal.h"

#pragma pack(push, 1)

typedef union {
  struct {
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t res;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
  } d;
  struct {
    uint16_t di, di_hi;
    uint16_t si, si_hi;
    uint16_t bp, bp_hi;
    uint16_t res, res_hi;
    uint16_t bx, bx_hi;
    uint16_t dx, dx_hi;
    uint16_t cx, cx_hi;
    uint16_t ax, ax_hi;
    uint16_t flags;
    uint16_t es;
    uint16_t ds;
    uint16_t fs;
    uint16_t gs;
    uint16_t ip;
    uint16_t cs;
    uint16_t sp;
    uint16_t ss;
  } x;
  struct {
    uint8_t edi[4];
    uint8_t esi[4];
    uint8_t ebp[4];
    uint8_t res[4];
    uint8_t bl, bh, ebx_b2, ebx_b3;
    uint8_t dl, dh, edx_b2, edx_b3;
    uint8_t cl, ch, ecx_b2, ecx_b3;
    uint8_t al, ah, eax_b2, eax_b3;
  } h;
} __dpmi_regs;

typedef struct {
  uint32_t offset32;
  uint16_t selector;
} __dpmi_paddr;

typedef struct {
  uint32_t handle;
  uint32_t size; /* or count */
  uint32_t address;
} __dpmi_meminfo;

#pragma pack(pop)

extern uint16_t *__dpmi_error_ptr;
#define __dpmi_error (*__dpmi_error_ptr)

int32_t __dpmi_allocate_ldt_descriptors (int32_t count);
int32_t __dpmi_free_ldt_descriptor (int32_t selector);
int32_t __dpmi_get_selector_increment_value (void);
int32_t __dpmi_set_segment_base_address (int32_t selector, uint32_t address);
int32_t __dpmi_set_segment_limit (int32_t selector, uint32_t limit);

int32_t __dpmi_allocate_dos_memory (int32_t paras, int32_t *ret);
int32_t __dpmi_free_dos_memory (int32_t selector);

int32_t __dpmi_get_protected_mode_interrupt_vector (int32_t vector, __dpmi_paddr *address);
int32_t __dpmi_set_protected_mode_interrupt_vector (int32_t vector, __dpmi_paddr *address);

int32_t __dpmi_simulate_real_mode_interrupt (int32_t vec, __dpmi_regs *regs);

int32_t __dpmi_physical_address_mapping (__dpmi_meminfo *info);

int32_t _go32_dpmi_lock_data (void *lockaddr, uint32_t locksize);
int32_t _go32_dpmi_lock_code (void *lockaddr, uint32_t locksize);

#endif // !defined(DPMI_H)
