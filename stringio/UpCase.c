// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#if !DEBUG
#define HAVE_INLINE_UpCase 1

__inline__
#endif
uint8_t UpCase (uint8_t c)
{
  return IsLower (c) ? c - 'a' + 'A' : c;
}
