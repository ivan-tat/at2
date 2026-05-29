// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024-2026 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#include <limits.h>
#include <sys/types.h>
#include "pascal/stdlib.h"

#ifndef SSIZE_MAX
#define SSIZE_MAX (SIZE_MAX/2)
#endif // !defined(SSIZE_MAX)

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

void *custom_malloc (size_t size) {
  void *p = Pascal_AllocMem (size);

  if (p == NULL)
    errno = ENOMEM;

  return p;
}

void *custom_calloc (size_t n, size_t size) {
  void *p;
  size_t x = n * size;

  if ((n != 0) && (x / n == size) && (x <= SSIZE_MAX))
  {
    p = Pascal_AllocMem (x);

    if (p != NULL)
      Pascal_FillChar (p, x, 0);
    else
      errno = ENOMEM;
  }
  else
  {
    p = NULL;
    errno = ERANGE;
  }

  return p;
}

void custom_free (void *ptr) {
  Pascal_FreeMem (ptr);
}

void *custom_realloc (void *ptr, size_t size) {
  void *p = ptr;

  Pascal_ReAllocMem (&p, size);
  if (p == NULL)
    errno = ENOMEM;

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
