// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void lock_adt2unit (bool lock) {
  LOCK_DECL (lock, v, f);

  LOCK_DATA (v, &___ADT2UNIT_CONST_START___, (ptrdiff_t) &___ADT2UNIT_CONST_END___ - (ptrdiff_t) &___ADT2UNIT_CONST_START___);
  LOCK_DATA (v, &___ADT2UNIT_DATA_START___,  (ptrdiff_t) &___ADT2UNIT_DATA_END___  - (ptrdiff_t) &___ADT2UNIT_DATA_START___);
  LOCK_DATA (v, &___ADT2UNIT_BSS_START___,   (ptrdiff_t) &___ADT2UNIT_BSS_END___   - (ptrdiff_t) &___ADT2UNIT_BSS_START___);
  LOCK_CODE (f, (uintptr_t) ___ADT2UNIT_CODE_START___, (ptrdiff_t) ___ADT2UNIT_CODE_END___ - (ptrdiff_t) ___ADT2UNIT_CODE_START___);
}

static void done_adt2unit (void);

void init_adt2unit (void) {
  lock_adt2unit (true);
  atexit (done_adt2unit);
}

static void done_adt2unit (void) {
  lock_adt2unit (false);
}
