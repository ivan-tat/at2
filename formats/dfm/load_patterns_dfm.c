// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static int8_t load_patterns_dfm (tFIXED_SONGDATA *dst, int song_len,
                                 size_t packed_size,
                                 FILE *f,
                                 char **error)
{
  int8_t result = -1;
  char *result_error = NULL;
  void *buffer = NULL;
  mem_stream_t stream;
  int_least16_t last_pat;

  if ((buffer = malloc (packed_size)) == NULL) goto _err_malloc;

  if ((fread (buffer, packed_size, 1, f)) == 0) goto _err_fread;

  set_mem_stream (&stream, buffer, packed_size);
  last_pat = -1;
  do
  {
    uint8_t pat;

    if (read_bytes (&pat, sizeof (pat), &stream)) goto _err_eod;

    if ((last_pat >= pat) || (pat >= PATTERNS_MAX_DFM)) goto _err_format;
    last_pat = pat;

    for (uint_least8_t line = 0; line < PATTERN_LEN_DFM; line++)
      for (uint_least8_t chan = 0; chan < CHANNELS_MAX_DFM; chan++)
      {
        tCHUNK chunk;
        uint8_t note, fx;

        if (read_bytes (&note, sizeof (note), &stream)) goto _err_eod;

        if ((note & 0x80) != 0)
        {
          note &= ~0x80;
          if (read_bytes (&fx, sizeof (fx), &stream)) goto _err_eod;
        }
        else
          fx = 0;

        get_pat_event_dfm (&chunk, note, fx);
        put_chunk (pat, line, chan + 1, &chunk);
      }
  } while (stream.curptr < stream.endptr);

  free (buffer);
  buffer = NULL;

  if (song_len > 0)
  {
    uint8_t ins_cache[CHANNELS_MAX_DFM];
    uint8_t processed[(PATTERNS_MAX_DFM+7)/8];

    memset (ins_cache, 0, sizeof (ins_cache));
    memset (processed, 0, sizeof (processed));

    for (uint_least8_t pos = 0; pos < song_len; pos++)
    {
      uint_least8_t pat = dst->pattern_order[pos];

      if (!bit_test (processed, pat))
      {
        for (uint_least8_t line = 0; line < PATTERN_LEN_DFM; line++)
          for (uint_least8_t chan = 0; chan < CHANNELS_MAX_DFM; chan++)
          {
            tCHUNK chunk;

            get_chunk (pat, line, chan + 1, &chunk);

            if (chunk.instr_def != INS_EMPTY)
            {
              chunk.effect_def = ef_Extended;
              chunk.effect     = (ef_ex_ExtendedCmd2 << 4) + ef_ex_cmd2_ResetVol;
              ins_cache[chan] = chunk.instr_def;
              if (!accurate_conv && ((chunk.note == NOTE_EMPTY) || (chunk.note > NOTE_PACK (NOTE_MAX))))
                chunk.instr_def = INS_EMPTY;
            }
            else if (!accurate_conv && (chunk.note >= NOTE_PACK (0)) && (chunk.note <= NOTE_PACK (NOTE_MAX)))
              chunk.instr_def = ins_cache[chan];

            put_chunk (pat, line, chan + 1, &chunk);
          }

        bit_set (processed, pat);
      }
    }
  }

  result = 0;

_exit:
  if (buffer != NULL) free (buffer);
  if ((result < 0) && (error != NULL)) *error = result_error;

  return result;

_err_malloc:
  result_error = "Memory allocation failed";
  goto _exit;

_err_fread:
  result_error = "Failed to read input file";
  goto _exit;

_err_format:
  result_error = "Unknown file format";
  goto _exit;

_err_eod:
  result_error = "Unexpected end of input data";
  goto _exit;
}
