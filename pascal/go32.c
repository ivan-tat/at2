// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pascal.h"
#include "pascal/go32.h"

void custom_dosmemget (uint32_t ofs, size_t size, void *buf) {
  Pascal_dosmemget ((ofs >> 4), ofs & 15, buf, size);
}

void custom_dosmemput (const void *buf, size_t size, uint32_t ofs) {
  Pascal_dosmemput ((ofs >> 4), ofs & 15, buf, size);
}
