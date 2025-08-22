// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#if !DEBUG
#define HAVE_INLINE_IsUpper 1

__inline__
#endif
bool IsUpper (uint8_t c)
{
  return (c >= 'A') && (c <= 'Z');
}
