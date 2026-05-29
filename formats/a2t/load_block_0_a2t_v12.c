// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// In:
//   * `version': 12..13.
//   * `progress', `state' and `error' may be NULL.
//
// On success:
//   * Return value: 0.
//   * `state' (if set): 2 (partly loaded).
//   * `error' is untouched.
// On error:
//   * Return value: -1.
//   * `state' (if set) may be set to 2 (partly loaded).
//   * `error' (if set) is set to error description.
static int8_t load_block_0_a2t_v12 (uint8_t version, tFIXED_SONGDATA *song,
                                    size_t packed_size,
                                    FILE *f,
                                    progress_callback_t *progress, uint8_t *state, char **error)
{
  int8_t result = -1;
  uint8_t result_state = state != NULL ? *state : 0;
  char *result_error = NULL;
  void *packed_data = NULL;
  block_0_a2t_v12_t *raw_data = NULL;
  ins_data_a2t_v9_t (*ins_data)[INSTRUMENTS_MAX_A2T_V9] = NULL;
  size_t raw_limit, raw_size, ins_size;

  if (packed_size == 0) return 0;

  if ((packed_data = malloc (packed_size)) == NULL) goto _err_malloc;

  raw_limit = sizeof (*raw_data) + sizeof (*ins_data);
  if ((raw_data = malloc (raw_limit)) == NULL) goto _err_malloc;

  if (fread (packed_data, packed_size, 1, f) == 0) goto _err_fread;

  if (unpack_a2t (version, raw_data, raw_limit, &raw_size, packed_data, packed_size,
                  progress, &result_error) != 0) goto _exit;

  free (packed_data);
  packed_data = NULL;

  if (raw_size < sizeof (*raw_data)) goto _err_format;

  get_ins_4op_data_a2t_v12 (song, &raw_data->ins_4op_data);

  ins_size = raw_size - sizeof (*raw_data);
  if (ins_size != 0)
  {
    unsigned count = ins_size / sizeof ((*ins_data)[0]);

    if (sizeof ((*ins_data)[0]) * count != ins_size) goto _err_format;

    ins_data = (void *)((uint8_t *)raw_data + sizeof (*raw_data));

    for (unsigned i = 0; i < count; i++)
      get_ins_data_a2t_v9 (&song->instr_data[i], &(*ins_data)[i]);
    for (unsigned i = count; i < INSTRUMENTS_MAX; i++)
      memset (&song->instr_data[i], 0, sizeof (song->instr_data[0]));
  }

  free (raw_data);
  raw_data = NULL;

  result_state = 2;

  result = 0;

_exit:
  if (packed_data != NULL) free (packed_data);
  if (raw_data != NULL) free (raw_data);
  if (state != NULL) *state = result_state;
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
}
