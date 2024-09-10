// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#include <stddef.h>
#include <limits.h>
#include "stdlib.h"

static uint8_t _ExitCount = 0;
static void (*_ExitList[ATEXIT_MAX]) (void);

extern int32_t *Pascal_RandSeedPtr;

int custom_abs (int i) {
  return (i < 0) ? -i : i;
}

int custom_atexit (void (*function) (void)) {
  if (_ExitCount < ATEXIT_MAX) {
    _ExitList[_ExitCount++] = function;
    return 0;
  } else
    return -1;
}

void custom_free (void *ptr) {
  Pascal_FreeMem (ptr);
}

void *custom_realloc (void *ptr, size_t size) {
  void *p = ptr;

  Pascal_ReAllocMem (&p, size);
  return p;
}

int custom_rand (void) {
  return Pascal_Random (RAND_MAX);
}

void custom_srand (unsigned seed) {
  *Pascal_RandSeedPtr = seed;
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
