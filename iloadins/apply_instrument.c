// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// idx: 0..254
static void copy_ins_data (tFIXED_SONGDATA *song, uint8_t idx, struct temp_instrument_data *src, bool copy_macro)
{
  String_t s;

  memcpy (&song->instr_data[idx], &src->fm, sizeof (song->instr_data[0]));
  if (copy_macro)
  {
    memcpy (&song->instr_macros[idx], &src->macro, sizeof (song->instr_macros[0]));
    memcpy (&song->dis_fmreg_col[idx], &src->dis_fmreg_col, sizeof (song->dis_fmreg_col[0]));
  }
  else
  {
    memset (&song->instr_macros[idx], 0, sizeof (song->instr_macros[0]));
    memset (&song->dis_fmreg_col[idx], 0, sizeof (song->dis_fmreg_col[0]));
  }

  s = Copy ((String *)song->instr_names[idx], 1, 9);
  AppendString ((String *)&s, (String *)src->name, sizeof (src->name) - 1);
  CopyString ((String *)song->instr_names[idx], (String *)&s, sizeof (song->instr_names[0]) - 1);
}

// idx: 0..254
void apply_instrument (uint8_t idx, temp_instrument_t *src)
{
  if (src->four_op)
  {
    if (idx == 254) idx--; // prevent index overflow

    set_4op_flag (idx + 1);
    update_4op_flag_marks ();
  }
  else
  {
    uint16_t ins_4op = check_4op_instrument (idx + 1); // was `check_4op_to_test()'

    if (ins_4op != 0)
    {
      reset_4op_flag (ins_4op >> 8);
      update_4op_flag_marks ();
    }
  }

  copy_ins_data (&songdata, idx, &src->ins1, src->use_macro); // also 4OP 1/2

  if (src->four_op)
    copy_ins_data (&songdata, idx + 1, &src->ins2, src->use_macro); // 4OP 2/2
}
