// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// This file is included in `adt2ext3.c'

static bool adsr_carrier[9]; // used in "import_old_a2m_event1" and "replace_old_adsr" // HINT: (FPC) start index 1

#include "iloaders/import_old_a2m_event1.c" // static
#include "iloaders/replace_old_adsr.c" // static
#include "iloaders/import_old_a2m_patterns1.c" // static
#include "iloaders/import_old_a2m_event2.c" // static
#include "iloaders/import_old_a2m_patterns2.c" // static
#include "iloaders/import_old_flags.c" // HINT: static
#include "iloaders/import_old_songdata.c" // static
#include "iloaders/adjust_macro_speedup_with_notice.c" // HINT: static
#include "iloaders/a2m_file_loader.c"
#include "iloaders/a2t_file_loader.c"
#if !ADT2PLAY
#include "iloaders/a2p_file_loader.c"
#endif // !ADT2PLAY

static uint8_t dec2hex (uint8_t dec)
{
  return (dec / 10) * 16 + (dec % 10);
}

#include "iloaders/amd_file_loader.c"
