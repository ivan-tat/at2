// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void put_composer_a2m_v1 (composer_a2m_v1_t *dst, tFIXED_SONGDATA *src)
{
  put_string_a2m (dst, sizeof (*dst), src->composer, sizeof (src->composer), 0, '\0');
}
