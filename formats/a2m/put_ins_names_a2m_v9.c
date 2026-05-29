// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void put_ins_names_a2m_v9 (ins_name_a2m_v9_t *dst, tFIXED_SONGDATA *src, unsigned count)
{
  for (unsigned i = 0; i < count; i++)
    put_string_a2m (&dst[i], sizeof (dst[0]), src->instr_names[i], sizeof (src->instr_names[0]), 9, '\0');  // skip first 9 characters
}
