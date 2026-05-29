// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void get_ins_macro_a2m_v9 (tREGISTER_TABLE *dst, ins_macro_a2m_v9_t *src)
{
  dst->length         = src->length;
  dst->loop_begin     = src->loop_begin;
  dst->loop_length    = src->loop_length;
  dst->keyoff_pos     = src->keyoff_pos;
  dst->arpeggio_table = src->arpeggio_table;
  dst->vibrato_table  = src->vibrato_table;

  for (unsigned i = 0; i < lengthof (src->data, [0]); i++)
  {
    ins_macro_row_a2m_v9_t *src1 = &src->data[i];
    tREGISTER_TABLE_DEF *dst1 = &dst->data[i];

    get_ins_fm_data_a2m_v1 (&dst1->fm_data, &src1->fm_data);
    dst1->freq_slide = int16_LE (src1->frequency_slide);
    dst1->panning    = src1->panning;
    dst1->duration   = src1->duration;
  }
}
