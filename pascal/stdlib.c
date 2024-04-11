// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#include <stddef.h>
#include "stdlib.h"

static uint8_t _ExitCount = 0;
static void (*_ExitList[ATEXIT_MAX]) (void);

int custom_atexit (void (*function) (void)) {
  if (_ExitCount < ATEXIT_MAX) {
    _ExitList[_ExitCount++] = function;
    return 0;
  } else
    return -1;
}

void init_stdlib (void) {
}

void done_stdlib (void) {
  while (_ExitCount) {
    void (*p) (void) = _ExitList[--_ExitCount];

    if (p) {
      _ExitList[_ExitCount] = NULL;
      //InOutRes = 0;
      p ();
    }
  }
}
