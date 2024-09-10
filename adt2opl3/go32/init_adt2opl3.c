// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void lock_adt2opl3 (bool lock) {
  LOCK_DECL (lock, v, f);

  LOCK_DATA (v, &___ADT2OPL3_CONST_START___, (ptrdiff_t) &___ADT2OPL3_CONST_END___ - (ptrdiff_t) &___ADT2OPL3_CONST_START___);
  LOCK_DATA (v, &___ADT2OPL3_DATA_START___,  (ptrdiff_t) &___ADT2OPL3_DATA_END___  - (ptrdiff_t) &___ADT2OPL3_DATA_START___);
  LOCK_DATA (v, &___ADT2OPL3_BSS_START___,   (ptrdiff_t) &___ADT2OPL3_BSS_END___   - (ptrdiff_t) &___ADT2OPL3_BSS_START___);
  LOCK_CODE (f, (uintptr_t) ___ADT2OPL3_CODE_START___, (ptrdiff_t) ___ADT2OPL3_CODE_END___ - (ptrdiff_t) ___ADT2OPL3_CODE_START___);
}

static void done_adt2opl3 (void);

void init_adt2opl3 (void) {
  lock_adt2opl3 (true);
  memset (_opl_regs_cache, ~0, sizeof (_opl_regs_cache));
  atexit (done_adt2opl3);
}

static void done_adt2opl3 (void) {
  lock_adt2opl3 (false);
}
