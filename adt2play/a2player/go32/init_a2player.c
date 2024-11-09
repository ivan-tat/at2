// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void lock_a2player (bool lock) {
  LOCK_DECL (lock, v, f);

  LOCK_DATA (v, &___A2PLAYER_CONST_START___, (ptrdiff_t) &___A2PLAYER_CONST_END___ - (ptrdiff_t) &___A2PLAYER_CONST_START___);
  LOCK_DATA (v, &___A2PLAYER_DATA_START___,  (ptrdiff_t) &___A2PLAYER_DATA_END___  - (ptrdiff_t) &___A2PLAYER_DATA_START___);
  LOCK_DATA (v, &___A2PLAYER_BSS_START___,   (ptrdiff_t) &___A2PLAYER_BSS_END___   - (ptrdiff_t) &___A2PLAYER_BSS_START___);
  LOCK_CODE (f, (uintptr_t) ___A2PLAYER_CODE_START___, (ptrdiff_t) ___A2PLAYER_CODE_END___ - (ptrdiff_t) ___A2PLAYER_CODE_START___);
}

static void done_a2player (void);

void init_a2player (void) {
  lock_a2player (true);
  atexit (done_a2player);
}

static void done_a2player (void) {
  lock_a2player (false);
}

