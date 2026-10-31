// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static bool load_patterns_fmk (tFIXED_SONGDATA *song, bool stereo,
                               uint32_t *offsets, unsigned count,
                               FILE *f,
                               char **error)
{
  bool result = true;
  char *result_error = NULL;
  void *packed_data = NULL;
  size_t packed_size = 0;

  for (unsigned pat = 0; pat < count; pat++)
    if (offsets[pat] != 0)
    {
      uint16_t packed_blen;

      if (fseek (f, offsets[pat], SEEK_SET) != 0)  goto _err_fread;
      if (fread (&packed_blen, sizeof (packed_blen), 1, f) == 0) goto _err_fread;
      packed_blen = uint16_LE (packed_blen);
      if (packed_blen == 0) continue;
      if (packed_blen < PATTERN_LEN_FMK) goto _err_format;

      if (touch_pattern (song, pat))
      {
        mem_stream_t stream;
        tRAW_PATTERN_DATA *pat_data;

        if (packed_size < packed_blen)
        {
          if (packed_data != NULL)
          {
            void *p = realloc (packed_data, packed_blen);
            if (p == NULL) goto _err_malloc;
            packed_data = p;
          }
          else if ((packed_data = malloc (packed_blen)) == NULL) goto _err_malloc;

          packed_size = packed_blen;
        }
        if (fread (packed_data, packed_blen, 1, f) == 0) goto _err_fread;

        set_mem_stream (&stream, packed_data, packed_blen);
        pat_data = get_pattern_data (song, pat);
        for (unsigned line = 0; line < PATTERN_LEN_FMK; /*not here*/)
        {
          uint8_t x;

          if (read_bytes (&x, sizeof (x), &stream)) goto _err_eod;

          if (x != 0)
          {
            pat_event_fmk_t ev;

            ev.chan = x & 0x1F; // 1..CHANNELS_MAX_FMK

            if (x & 0x20)
            {
              if (read_bytes (&ev.note, sizeof (ev.note), &stream)) goto _err_eod;
              if (read_bytes (&ev.ins, sizeof (ev.ins), &stream)) goto _err_eod;
            }
            else
            {
              ev.note = NOTE_EMPTY_FMK;
              ev.ins  = INS_EMPTY_FMK;
            }

            if (x & 0x40)
            {
              if (read_bytes (&ev.vol, sizeof (ev.vol), &stream)) goto _err_eod;
            }
            else
              ev.vol = VOL_EMPTY_FMK;

            if (x & 0x80)
            {
              if (read_bytes (&ev.cmd, sizeof (ev.cmd), &stream)) goto _err_eod;
              if (read_bytes (&ev.info, sizeof (ev.info), &stream)) goto _err_eod;
            }
            else
            {
              ev.cmd  = CMD_EMPTY_FMK;
              ev.info = 0;
            }

            if ((ev.chan >= 1) && (ev.chan <= CHANNELS_MAX_FMK))
            {
              tCHUNK a;
              get_pat_event_fmk (&a, &ev, stereo);
              (*pat_data)[ev.chan - 1][line] = a;
            }
          }
          else line++;
        }
      }
      else
        if (fseek (f, packed_blen, SEEK_CUR) != 0) goto _err_fread;
    }

  fix_patterns_fmk (song, max (count, PATTERNS_MAX));

  result = false;

_exit:
  if (packed_data != NULL) free (packed_data);
  if (result && (error != NULL)) *error = result_error;

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
