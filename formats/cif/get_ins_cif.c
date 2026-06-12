// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void get_ins_cif (temp_instrument_t *dst, ins_cif_t *src)
{
  dst->four_op = false;
  dst->use_macro = false;
  get_ins_data_cif (&dst->ins1.fm, &src->ins_data);
  get_ins_name_cif (&dst->ins1.name, &src->ins_name);
}
