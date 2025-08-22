// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#if !DEBUG
#define HAVE_INLINE_LoCase 1

__inline__
#endif
uint8_t LoCase (uint8_t c)
{
  return IsUpper (c) ? c - 'A' + 'a' : c;
}
