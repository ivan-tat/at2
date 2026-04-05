// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void lock_a2player (bool lock) {
  LOCK_DECL (lock, v, f);

  LOCK_DATA (v, &PAREA_LABEL (CONST, START), PAREA_SIZE (CONST));
  LOCK_DATA (v, &PAREA_LABEL (DATA,  START), PAREA_SIZE (DATA));
  LOCK_DATA (v, &PAREA_LABEL (BSS,   START), PAREA_SIZE (BSS));
  LOCK_CODE (f, (uintptr_t) PAREA_LABEL (CODE, START), PAREA_SIZE (CODE));
}

static void done_a2player (void);

void init_a2player (void) {
  lock_a2player (true);
// adt2opl3 {
  memset (_opl_regs_cache, ~0, sizeof (_opl_regs_cache));
// } adt2opl3
  atexit (done_a2player);
}

static void done_a2player (void) {
  lock_a2player (false);
}
