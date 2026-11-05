// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void get_instruments_mtk (tFIXED_SONGDATA *song,
                                 ins_name_mtk_t *ins_names,
                                 ins_data_mtk_t *ins_data,
                                 unsigned count)
{
  for (unsigned i = 0; i < count; i++)
  {
    get_ins_name_mtk (&song->instr_names[i], &ins_names[i]);
    get_ins_data_mtk (&song->instr_data[i], &ins_data[i]);
  }
}

