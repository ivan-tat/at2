// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pascal.h"
#include <time.h>

time_t custom_time (time_t *tloc) {
  time_t r = Pascal_DateTimeToUnix (Pascal_Now ());

  if (tloc)
    *tloc = r;

  return r;
}
