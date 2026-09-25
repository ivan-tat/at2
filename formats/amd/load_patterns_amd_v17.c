// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// In: `progress' may be NULL.
// Returns `false' on success and `true' on error.
static bool load_patterns_amd_v17 (int patterns, FILE *f,
                                  progress_callback_t *progress, char **error)
{
  bool result = true;
  uint16_t (*track_order)[1][CHANNELS_MAX_AMD] = NULL;
  size_t size = sizeof (*track_order) * patterns;
  tCHUNK (*track)[PATTERN_LEN_AMD] = NULL;
  uint16_t tracks;
  uint_least8_t line = 0, count = 0;
  uint_least16_t pattern = 0xFFFF;
  uint_least8_t channel = 0xFF;

  if ((track_order = malloc (size)) == NULL) goto _err_malloc;
  if ((track = malloc (sizeof (*track))) == NULL) goto _err_malloc;

  if (fread (track_order, size, 1, f) == 0) goto _err_fread;
  if (fread (&tracks, 2, 1, f) == 0) goto _err_fread;
  tracks = uint16_LE (tracks);

  for (uint_least16_t i = 0; i < tracks;)
  {
    if (count == 0)
    {
      uint8_t event[3];

      if (line == 0)
      {
        uint16_t order;

        if (fread (&order, sizeof (order), 1, f) == 0) goto _err_fread;
        order = uint16_LE (order);
        pattern = order / CHANNELS_MAX_AMD;
        channel = order % CHANNELS_MAX_AMD;
      }

      if (fread (event, 1, 1, f) == 0) goto _err_fread;
      if ((event[0] & 0x80) == 0)
      {
        if (pattern < PATTERN_LEN_AMD)
        {
          tCHUNK chunk;

          if (fread (&event[1], 2, 1, f) == 0) goto _err_fread;
          get_pat_event_amd (&chunk, event[2], event[1], event[0]);
          put_chunk (pattern, line, channel + 1, &chunk);
        }
        else
        {
          if (fseek (f, 2, SEEK_CUR) != 0) goto _err_fread;
        }
      }
      else
        count = (event[0] & 0x7F) - 1;
    }
    else count--;

    if (++line == PATTERN_LEN_AMD)
    {
      line = 0;
      count = 0;
      i++;
    }
  }

  for (int dst_pat = 0; dst_pat < patterns; dst_pat++)
  {
    for (int dst_chan = 0; dst_chan < CHANNELS_MAX_AMD; dst_chan++)
    {
      uint16_t order = uint16_LE ((*track_order)[dst_pat][dst_chan]);

      if (order < PATTERN_LEN_AMD * CHANNELS_MAX_AMD)
      {
        pattern = order / CHANNELS_MAX_AMD;
        channel = order % CHANNELS_MAX_AMD;

        for (line = 0; line < PATTERN_LEN_AMD; line++) get_chunk (pattern, line, channel + 1, &(*track)[line]);
        for (line = 0; line < PATTERN_LEN_AMD; line++) put_chunk (dst_pat, line, dst_chan + 1, &(*track)[line]);
      }
    }
    if (progress != NULL) next_progress_step (progress);
  }

  result = false;

_exit:
  if (track_order != NULL) free (track_order);
  if (track != NULL) free (track);

  return result;

_err_malloc:
  *error = "Memory allocation failed";
  goto _exit;

_err_fread:
  *error = "Failed to read input file";
  goto _exit;
}
