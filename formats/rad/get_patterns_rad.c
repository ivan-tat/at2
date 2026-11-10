// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static bool get_patterns_rad (tFIXED_SONGDATA *song,
                              mem_stream_t *stream,
                              long fsize,
                              char **error)
{
  bool result = true;
  char *result_error = NULL;
  uint16_t offsets[PATTERNS_MAX_RAD];

  if (read_bytes (offsets, sizeof (offsets), stream)) goto _err_eod;

  for (unsigned pat = 0; pat < PATTERNS_MAX_RAD; pat++)
    if (touch_pattern (song, pat))
    {
      tRAW_PATTERN_DATA *pat_data = get_pattern_data (song, pat);
      size_t ofs = uint16_LE (offsets[pat]);

      if ((ofs != 0) && ((long)ofs < fsize))
      {
        uint8_t line;

        seek_stream (stream, ofs, SEEK_SET);
        do
        {
          #pragma pack(push, 1)
          struct { uint8_t chan, byte1, byte2; } x;
          #pragma pack(pop)

          if (read_bytes (&line, sizeof (line), stream)) goto _err_eod;
          do
          {
            uint8_t param;

            if (read_bytes (&x, sizeof (x), stream)) goto _err_eod;
            if ((x.byte2 & 0x0F) != 0)
            {
              if (read_bytes (&param, sizeof (param), stream)) goto _err_eod;
            }
            else param = 0;

            if ((x.chan & 0x0F) < CHANNELS_MAX_RAD)
              get_pat_event_rad (&(*pat_data)[x.chan & 0x0F][line & 0x3F], x.byte1, x.byte2, param);
           } while (((x.chan & 0x80) == 0) && (stream->curptr < stream->endptr));
        } while (((line & 0x80) == 0) && (stream->curptr < stream->endptr));
      }
    }

  result = false;

_exit:
  if (result && (error != NULL)) *error = result_error;

  return result;

_err_eod:
  result_error = "Unexpected end of input data";
  goto _exit;
}
