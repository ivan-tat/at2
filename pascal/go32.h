// DJGPP support
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef GO32_H
#define GO32_H

#include <stdint.h>

extern uint16_t *_dos_ds_ptr;
#define _dos_ds (*_dos_ds_ptr)

void custom_dosmemget (uint32_t ofs, size_t size, void *buf);
void custom_dosmemput (const void *buf, size_t size, uint32_t ofs);

// Aliases

#define dosmemget custom_dosmemget
#define dosmemput custom_dosmemput

#endif // !defined(GO32_H)
