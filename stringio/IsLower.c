// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#if !DEBUG
#define HAVE_INLINE_IsLower 1

__inline__
#endif
bool IsLower (uint8_t c)
{
  return (c >= 'a') && (c <= 'z');
}
