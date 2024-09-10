// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef PASCAL_TIME_H
#define PASCAL_TIME_H

typedef unsigned custom_time_t;

#define time_t custom_time_t

time_t custom_time (time_t *tloc);

// Aliases
#define time custom_time

#endif // !defined(PASCAL_TIME_H)
