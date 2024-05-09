// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#include "defines.h"

#if USE_FPC
#include "pascal.h"
#endif // USE_FPC
#include "common.h"

#if !HAVE_INLINE_bit_set
#include "common/bit_set.c"
#endif // !HAVE_INLINE_bit_set

#if !HAVE_INLINE_bit_clear
#include "common/bit_clear.c"
#endif // !HAVE_INLINE_bit_clear

#if !HAVE_INLINE_bit_test
#include "common/bit_test.c"
#endif // !HAVE_INLINE_bit_test

#include "common/bit_set_range.c"
#include "common/bit_clear_range.c"

#include "common/memsetw.c"
