// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void put_flags_a2m_v9 (uint8_t *dst, UNUSED tFIXED_SONGDATA *src)
{
  *dst = (speed_update    ? 1 << 0 : 0)
       + (lockvol         ? 1 << 1 : 0)
       + (lockVP          ? 1 << 2 : 0)
       + ((tremolo_depth & 1) << 3)
       + ((vibrato_depth & 1) << 4)
       + (panlock         ? 1 << 5 : 0)
       + (percussion_mode ? 1 << 6 : 0)
       + (volume_scaling  ? 1 << 7 : 0);
}
