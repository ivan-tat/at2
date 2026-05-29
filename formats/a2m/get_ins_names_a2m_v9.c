// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// In: `count' = 1..INSTRUMENTS_MAX_A2M_V9
static void get_ins_names_a2m_v9 (tFIXED_SONGDATA *dst, ins_name_a2m_v9_t *src, unsigned count)
{
  for (unsigned i = 0; i < count; i++)
    AppendString (dst->instr_names[i], (String *)&src[i], sizeof (dst->instr_names[0]) - 1);

/*
  for (unsigned i = count; i < INSTRUMENTS_MAX; i++)
    SetLength (song->instr_names[i], 0);
*/
}
