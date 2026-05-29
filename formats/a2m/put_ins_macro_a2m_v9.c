// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// In: `idx' = 0..INSTRUMENTS_MAX-1
static void put_ins_macro_a2m_v9 (ins_macro_a2m_v9_t *dst, tFIXED_SONGDATA *src, unsigned idx)
{
  tREGISTER_TABLE *p = &src->instr_macros[idx];

  dst->length         = p->length;
  dst->loop_begin     = p->loop_begin;
  dst->loop_length    = p->loop_length;
  dst->keyoff_pos     = p->keyoff_pos;
  dst->arpeggio_table = p->arpeggio_table;
  dst->vibrato_table  = p->vibrato_table;

  for (unsigned i = 0; i < lengthof (dst->data, [0]); i++)
  {
    tREGISTER_TABLE_DEF *src1 = &p->data[i];
    ins_macro_row_a2m_v9_t *dst1 = &dst->data[i];

    put_ins_fm_data_a2m_v1 (&dst1->fm_data, &src1->fm_data);
    dst1->frequency_slide = int16_LE (src1->freq_slide);
    dst1->panning         = src1->panning;
    dst1->duration        = src1->duration;
  }
}
