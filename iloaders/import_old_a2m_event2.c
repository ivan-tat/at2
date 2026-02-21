// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#define ef_ManualFSlide 22

// patt: 0..max_patterns-1
// line: 0..255
// chan: 1..20
static void import_old_a2m_event2 (uint8_t patt, uint8_t line, uint8_t chan, const tOLD_CHUNK old_chunk)
{
  tCHUNK chunk;

  memset (&chunk, 0, sizeof (chunk));
  chunk.note = old_chunk.note;
  chunk.instr_def = old_chunk.instr_def;

  if (old_chunk.effect_def != ef_ManualFSlide)
  {
    chunk.effect_def = old_chunk.effect_def;
    chunk.effect = old_chunk.effect;
  }
  else if ((old_chunk.effect >> 4) != 0)
  {
    chunk.effect_def = ef_Extended2;
    chunk.effect = (ef_ex2_FineTuneUp << 4) + (old_chunk.effect >> 4);
  }
  else
  {
    chunk.effect_def = ef_Extended2;
    chunk.effect = (ef_ex2_FineTuneDown << 4) + (old_chunk.effect & 0x0F);
  }

  put_chunk (patt, line, chan, &chunk);
}
