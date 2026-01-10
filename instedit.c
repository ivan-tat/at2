// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*static*/ bool _ADSR_preview_flag = true;

/*static*/ bool _instedit_operator_enabled[4] = { true, true, true, true }; // HINT: (FPC) start index 1
//static uint8_t _carrier_flag[2] = { 2, 1 }; // HINT: (FPC) boolean index

#define _INS_MARK_CHR charmap.black_right_triangle
#define _ins_mark_chr _INS_MARK_CHR // TODO: remove

/*static*/ const PASCALSTRING String _4op_connection_str[4][5+1] = // HINT: (FPC) start index 0
{
  "\x05" "FM/FM",
  "\x05" "FM/AM",
  "\x05" "AM/FM",
  "\x05" "AM/AM"
};

#include "instedit/_1op_preview_active.c" // static, used in `INSTRUMENT_test'
#include "instedit/INSTRUMENT_test_output_note.c" // static, used in `INSTRUMENT_test'
#include "instedit/INSTRUMENT_test.c"

#include "instedit/reset_instrument_preview.c"

#include "instedit/_1st_marked.c"
#include "instedit/_2nd_marked.c"
#include "instedit/marked_instruments.c"
#include "instedit/reset_marked_instruments.c"
#include "instedit/get_4op_to_test.c"
#include "instedit/check_4op_flag.c"
#include "instedit/check_4op_instrument.c"
#include "instedit/check_4op_to_test.c"
#include "instedit/reset_4op_flag.c"
#include "instedit/set_4op_flag.c"
#include "instedit/update_4op_flag_marks.c"

#undef _INS_MARK_CHR
