// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void lock_adt2unit (bool lock) {
  LOCK_DECL (lock, v, f);

  LOCK_DATA (v, &PAREA_LABEL (CONST, START), PAREA_SIZE (CONST));
  LOCK_DATA (v, &PAREA_LABEL (DATA,  START), PAREA_SIZE (DATA));
  LOCK_DATA (v, &PAREA_LABEL (BSS,   START), PAREA_SIZE (BSS));
  LOCK_CODE (f, (uintptr_t) PAREA_LABEL (CODE, START), PAREA_SIZE (CODE));
}

static void done_adt2unit (void);

void init_adt2unit (void) {
  lock_adt2unit (true);
  atexit (done_adt2unit);
}

static void done_adt2unit (void) {
  lock_adt2unit (false);
}
