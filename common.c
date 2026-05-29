// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024-2026 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#include "defines.h"

#if USE_FPC
#include "pascal.h"
#endif // USE_FPC
#include <stdio.h>  // SEEK_SET, SEEK_CUR, SEEK_END
#include "common.h"

const char GCC_ATTRIBUTE((nonstring)) HEXDIGITS[16] = { "0123456789ABCDEF" };

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

#include "common/get_max_uint8.c"
#include "common/get_max_uint16_LE.c"
#include "common/get_max_uint32_LE.c"
#include "common/get_max_size.c"

#include "common/set_mem_stream.c"
#include "common/read_bytes.c"
#include "common/write_bytes.c"
#include "common/read_string.c"
#include "common/write_string.c"
#include "common/seek_stream.c"
