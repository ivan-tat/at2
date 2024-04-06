// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

typedef enum {
  fadeFirst,
  fadeOut,
  fadeIn
} tFADE;

typedef enum {
  fast,
  delayed
} tDELAY;

#pragma pack(push, 1)
typedef struct {
  tFADE action;
  struct {
    uint8_t r, g, b;
  } pal0[256], pal1[256]; // HINT: (FPC) start index 0
} tFADE_BUF;
#pragma pack(pop)

extern uint8_t fade_speed;
