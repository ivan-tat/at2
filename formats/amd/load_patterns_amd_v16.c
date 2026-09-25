// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// In: `progress' may be NULL.
// Returns `false' on success and `true' on error.
static bool load_patterns_amd_v16 (int patterns, FILE *f,
                                   progress_callback_t *progress, char **error)
{
  bool result = true;
  void *data = NULL;
  size_t size = PATTERN_LEN_AMD * CHANNELS_MAX_AMD * 3 * patterns;
  uint8_t (*event)[3];

  if ((data = malloc (size)) == NULL) goto _err_malloc;
  if (fread (data, size, 1, f) == 0) goto _err_fread;

  event = data;
  for (int pat = 0; pat < patterns; pat++)
  {
    for (int line = 0; line < PATTERN_LEN_AMD; line++)
      for (int chan = 0; chan < CHANNELS_MAX_AMD; chan++)
      {
        tCHUNK chunk;

        get_pat_event_amd (&chunk, (*event)[2], (*event)[1], (*event)[0]);
        put_chunk (pat, line, chan + 1, &chunk);
        event++;
      }
    if (progress != NULL) next_progress_step (progress);
  }

  result = false;

_exit:
  if (data != NULL) free (data);

  return result;

_err_malloc:
  *error = "Memory allocation failed";
  goto _exit;

_err_fread:
  *error = "Failed to read input file";
  goto _exit;
}
