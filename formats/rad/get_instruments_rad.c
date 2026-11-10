// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static bool get_instruments_rad (tFIXED_SONGDATA *song,
                                 mem_stream_t *stream,
                                 char **error)
{
  bool result = true;
  char *result_error = NULL;
  uint8_t idx;

  do
  {
    if (read_bytes (&idx, sizeof (idx), stream)) goto _err_eod;

    if ((idx != 0) && ((uint8_t *)stream->curptr + sizeof (ins_fm_data_rad_t) <= (uint8_t *)stream->endptr))
    {
      if (idx > INSTRUMENTS_MAX_RAD) goto _err_format;
      get_ins_data_rad (&song->instr_data[idx - 1], (ins_fm_data_rad_t *)stream->curptr);
      seek_stream (stream, sizeof (ins_fm_data_rad_t), SEEK_CUR);
    }
    else break;
  } while ((idx != 0) && (stream->curptr < stream->endptr));

  result = false;

_exit:
  if (result && (error != NULL)) *error = result_error;

  return result;

_err_eod:
  result_error = "Unexpected end of input data";
  goto _exit;

_err_format:
  result_error = "Unknown file format";
  goto _exit;
}
