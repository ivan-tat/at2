// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*****************************************************************************

  FM-Kingtracker song file loader
  Supported file format versions: all
  Filename extension: .fmk

*****************************************************************************/

// In:
//   * `desc', `progress', `state' and `error' may be NULL.
//
// On success:
//   * Return value is 0.
//   * `desc' (if set) is set to allocated song description structure.
//   * `state' and `error' are untouched.
// On error:
//   * Return value:
//     -1: open/read error.
//     -2: unknown file format.
//     -3: unsupported file format version.
//     -4: format is supported.
//   * `state' (if set):
//     0: song is untouched.
//     1: song is cleared.
//     2: song is partly loaded.
//   * `error' (if set) is set to error description.
int8_t load_song_fmk (const String *_fname, void **desc, progress_callback_t *progress, uint8_t *state, char **error)
{
  tFIXED_SONGDATA *song = &songdata;
  int8_t result = -1;
  void *result_desc = NULL;
  uint8_t result_state = 0;
  char *result_error = NULL;
  FILE *f = NULL;
  header_fmk_t header;
  var_data_fmk_t *data = NULL;
  bool stereo;
  uint8_t num_channels;
  char fname[255+1];

  DBG_ENTER ("load_song_fmk");

  if (progress != NULL)
  {
    snprintf (progress->msg, sizeof (progress->msg), "Reading file...");
    progress->num_steps = 5;
    progress->value = 1;
    progress->update (progress, 0, -1);
  }

  StringToStr (fname, _fname, sizeof (fname) - 1);
  if ((f = fopen (fname, "rb")) == NULL) goto _err_fopen;

  if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;

  result = -2;

  if ((header.id != ID_FMK) || (header.bytef4 != 0xF4)) goto _err_format;

  result = -3;

  if (   ((header.version != 1) && (header.version != 2))
      || (header.num_instruments > INSTRUMENTS_MAX_FMK)
      || (header.num_patterns > PATTERNS_MAX_FMK)) goto _err_version;

  result = -4;

  if ((data = malloc (sizeof (*data))) == NULL) goto _err_malloc;

  // read variable header data

  // import channels set
  if (fread (data->chan_set, sizeof (data->chan_set[0])
                             * (header.version == 1 ? CHANNELS_MAX_FMK
                                                    : MELODIC_CHANNELS_MAX_FMK), 1, f) == 0) goto _err_fread;
  if (header.version == 2)  // disable last 2 channels
  {
    data->chan_set[CHANNELS_MAX_FMK - 2] = 0xFF;
    data->chan_set[CHANNELS_MAX_FMK - 1] = 0xFF;
  }
  // find playable channels
  for (num_channels = CHANNELS_MAX_FMK; num_channels >= 1; num_channels--)
    if ((data->chan_set[num_channels - 1] & 7) != 7) break;
  if (num_channels == 0) goto _err_chan;

  if (   (header.pat_order_len != 0)
      && (fread (data->pat_order, sizeof (data->pat_order[0]) * header.pat_order_len, 1, f) == 0)) goto _err_fread;

  if (fread (&data->desc_offset, sizeof (data->desc_offset), 1, f) == 0) goto _err_fread;
  data->desc_offset = uint16_LE (data->desc_offset);

  if (header.num_instruments != 0)
  {
    if (fread (data->ins_offsets, sizeof (data->ins_offsets[0]) * header.num_instruments, 1, f) == 0) goto _err_fread;
    for (unsigned i = 0; i < header.num_instruments; i++) data->ins_offsets[i] = uint16_LE (data->ins_offsets[i]);
  }

  if (header.num_patterns != 0)
  {
    if (fread (data->pat_offsets, sizeof (data->pat_offsets[0]) * header.num_patterns, 1, f) == 0) goto _err_fread;
    for (unsigned i = 0; i < header.num_patterns; i++) data->pat_offsets[i] = uint32_LE (data->pat_offsets[i]);
  }

  init_songdata (song);
  stereo = (header.flags & FL_STEREO_FMK) != 0;

  for (unsigned chan = 0; chan < num_channels; chan++)
    if ((data->chan_set[chan] & 7) == 6)
      switch (chan)
      {
        case 0:  case 1:  song->flag_4op |= 0x01; break;
        case 2:  case 3:  song->flag_4op |= 0x02; break;
        case 4:  case 5:  song->flag_4op |= 0x04; break;
        case 9:  case 10: song->flag_4op |= 0x08; break;
        case 11: case 12: song->flag_4op |= 0x10; break;
        case 13: case 14: song->flag_4op |= 0x20; break;
        default: break;
      }

  if (adjust_tracks)
    song->nm_tracks = num_channels;
  else if (song->nm_tracks < MELODIC_CHANNELS_MAX_FMK)
    song->nm_tracks = MELODIC_CHANNELS_MAX_FMK;

  song->patt_len = PATTERN_LEN_FMK;
  speed = header.init_speed != 0 ? header.init_speed : 1;
  tempo = header.base_speed != 0 ? header.base_speed : 50;
  song->tempo = tempo;
  song->speed = speed;
  song->common_flag |= 0x01 | 0x02 | 0x80;
  if (stereo) song->common_flag |= 0x20;
  if (header.flags & FL_TREMOLO_FMK) song->common_flag |= 0x08;
  if (header.flags & FL_VIBRATO_FMK) song->common_flag |= 0x10;
  apply_song_flags (song);
  if (stereo) // must be after a call to `apply_song_flags()'
    for (unsigned chan = 0; chan < MELODIC_CHANNELS_MAX_FMK; chan++)
      song->lock_flags[chan] |= pan_lut_fmk[(header.panning[chan >> 2] >> ((chan & 3) << 1)) & 3];
  get_title_fmk (song, &header.title);
  get_composer_fmk (song, &header.composer);
  {
    String_t s = NameOnly (_fname);
    CopyString (songdata_title, (String *)&s, sizeof (songdata_title) - 1);
  }

  result_state = 1;
  if (progress != NULL) next_progress_step (progress);

  if (get_pat_order_fmk (song,
                         data->pat_order, max (header.pat_order_len, PATTERN_ORDER_LEN),
                         &result_error)) goto _exit;
  result_state = 2;
  if (progress != NULL) next_progress_step (progress);

  if ((desc != NULL) && (data->desc_offset != 0))
    if (load_description_fmk (&result_desc,
                              data->desc_offset,
                              f,
                              &result_error)) goto _exit;
  if (progress != NULL) next_progress_step (progress);

  if (header.num_instruments != 0)
    if (load_instruments_fmk (song,
                              data->ins_offsets, header.num_instruments,
                              f,
                              &result_error)) goto _exit;
  if (progress != NULL) next_progress_step (progress);

  if (header.num_patterns != 0)
    if (load_patterns_fmk (song, stereo,
                           data->pat_offsets, header.num_patterns,
                           f,
                           &result_error)) goto _exit;

  if (progress != NULL) next_progress_step (progress);

  result = 0;

_exit:
  if (f != NULL) fclose (f);
  if (data != NULL) free (data);
  if (result < 0)
  {
    if (result_desc != NULL) free (result_desc);
    if (state != NULL) *state = result_state;
    if (error != NULL) *error = result_error;
  }
  else
    *desc = result_desc;

  DBG_LEAVE (); //EXIT //load_song_fmk
  return result;

_err_malloc:
  result_error = "Memory allocation failed";
  goto _exit;

_err_fopen:
  result_error = "Failed to open input file";
  goto _exit;

_err_fread:
  result_error = "Failed to read input file";
  goto _exit;

_err_format:
  result_error = "Unknown file format";
  goto _exit;

_err_version:
  result_error = "Unsupported file format version";
  goto _exit;

_err_chan:
  result_error = "No channels to play";
  goto _exit;
}
