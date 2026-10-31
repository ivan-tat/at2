// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static bool load_instruments_fmk (tFIXED_SONGDATA *song,
                                  uint16_t *offsets, unsigned count,
                                  FILE *f,
                                  char **error)
{
  bool result = true;
  char *result_error = NULL;

  for (unsigned i = 0; i < count; i++)
    if (offsets[i] != 0)
    {
      ins_data_fmk_t data;

      if (fseek (f, offsets[i], SEEK_SET) != 0) goto _err_fread;
      if (fread (&data, sizeof (data), 1, f) == 0) goto _err_fread;

      get_ins_name_fmk (&song->instr_names[i], &data);
      get_ins_data_fmk (&song->instr_data[i], &data.fm_data);
    }

  result = false;

_exit:
  if (result && (error != NULL)) *error = result_error;

  return result;

_err_fread:
  result_error = "Failed to read input file";
  goto _exit;
}
