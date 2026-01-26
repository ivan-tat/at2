// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Returns:
//   * 0: success.
//   * -1: error occurred, error description in `error'.
//   * 1: canceled by user.
int8_t bnk_file_loader (temp_instrument_t *dst, const String *fname, char **error)
{
  int8_t result = -1; // return value
  progress_window_t progress_win;
  progress_callback_t *progress = NULL;
  bnk_bank_t *bnk_bank = NULL;
  String (*queue)[][BNK_HEADER_STR_MAX+1] = NULL; // TODO: use structure and pass it to `Menu()'
  uint16_t index, first, __UNUSED last; // `first' and `last' are for future use
  void (*old_external_proc) (void);
  uint8_t old_topic_len;
  bool old_cycle_moves;
  uint8_t ysize;

  DBG_ENTER ("bnk_file_loader");

  {
    String_t s, t;
    char u[63+1];

    s = NameOnly (fname);
    t = iCASE ((String *)&s);
    StrToString ((String *)&s, " ", sizeof (s) - 1);
    AppendString ((String *)&s, (String *)&t, sizeof (s) - 1);
    AppendString ((String *)&s, StrToString ((String *)&t, " ", sizeof (t) - 1), sizeof (s) - 1);

    snprintf (u, sizeof (u), " ESC %c%c STOP ", charmap.bd_light_horiz, charmap.black_right_triangle); // " ESC ─► STOP "
    StrToString ((String *)&t, u, sizeof (t) - 1);

    progress_window_init (&progress_win, 44, 4, (String *)&s, (String *)&t);
  }
  progress = &progress_win.callback;

  snprintf (progress->msg, sizeof (progress->msg), "%s", "Loading data from bank file...");
  progress->value = 100 + 100; // +100=100% loaded, +100=100% processed
  progress->update (progress, 0, -1);

  if ((bnk_bank = load_bnk_bank (fname, error)) == NULL) goto _exit;

  snprintf (progress->msg, sizeof (progress->msg), "%s", "Processing data from bank file...");
  progress->update (progress, 100, 1);

  if ((queue = malloc (sizeof ((*queue)[0]) * (BNK_HEADER_LINES + bnk_bank->capacity))) == NULL) goto _err_malloc;

  for (uint_least8_t i = 0; i < BNK_HEADER_LINES; i++)
  {
    AT2_char_t a[BNK_HEADER_STR_LEN_MAX+1];
    String_t s, t;

    UTF8nstr_to_AT2 (a, sizeof (a), bnk_header_str[i]);
    t = iCASE (StrToString ((String *)&s, (char *)a, sizeof ((*queue)[0]) - 1));
    CopyString ((*queue)[i], (String *)&t, sizeof ((*queue)[0]) - 1);
  }

  bnk_bank->count = 0; // clear (it was valid, but we want to do it again with user interaction)
  first = 0;
  last = 0;
  for (index = 0; index < bnk_bank->capacity; index++)
  {
    char s[sizeof ((*queue)[0])];

    if ((ticklooper == 0) && (index > 0))
    {
      if (keypressed ())
      {
        fkey = GetKey ();
        if (fkey == kESC) break; // stop - that's why `index' is needed later
      }

      progress->update (progress, 100 + 100 * (index + 1) / bnk_bank->capacity, 1);
    }

    // item string
    bnk_file_loader_get_item_str (s, sizeof (s), index, bnk_bank);
    StrToString ((*queue)[BNK_HEADER_LINES + index], s, sizeof ((*queue)[0]) - 1);

    if (bnk_bank->items[index].available)
    {
      last = index; // update
      bnk_bank->count++; // count
    }
    else if (first == index) first++; // update
  }
  // `index' is now a total number of instruments including unavailable (<= bnk_bank->capacity)

  progress->update (progress, 100 + 100, 1);
  // delay for awhile to show progress bar
#if GO32
  Pascal_Delay (500);
#else // !GO32
  SDL_Delay (200);
#endif // !GO32
  progress_window_close (&progress_win);

  if (bnk_bank->count == 0) goto _err_empty;

  {
    char s[63+1];

    if (index == bnk_bank->capacity)
      snprintf (s, sizeof (s), "~[~%d~/%d]~", bnk_bank->count, bnk_bank->capacity);
    else
      snprintf (s, sizeof (s), "[%d]~[%d/%d]~", bnk_bank->count, index, bnk_bank->capacity);

    StrToString (mn_environment.context, s, sizeof (mn_environment.context) - 1);
  }

  old_external_proc = mn_environment.ext_proc;
  old_topic_len = mn_setting.topic_len;
  old_cycle_moves = mn_setting.cycle_moves;
  mn_environment.ext_proc = bnk_lister_external_proc;
  mn_setting.topic_len = BNK_HEADER_LINES;
  mn_setting.cycle_moves = false;

  ysize = is_default_screen_mode () ? 20 : 30;

  keyboard_reset_buffer ();
  if (!_force_program_quit)
  {
    String_t s, t;

    s = NameOnly (fname);
    t = iCASE ((String *)&s);
    StrToString ((String *)&s, " ", sizeof (s) - 1);
    AppendString ((String *)&s, (String *)&t, sizeof (s) - 1);
    AppendString ((String *)&s, StrToString ((String *)&t, " ", sizeof (t) - 1), sizeof (s) - 1);
    index = Menu (queue, 1, 1, min (1, get_bank_position (fname, bnk_bank->capacity)),
                  sizeof ((*queue)[0]) - 1, ysize, BNK_HEADER_LINES + index, (String *)&s) - 1;
  }

  add_bank_position (fname, bnk_bank->capacity, BNK_HEADER_LINES + index + 1);
  mn_environment.ext_proc = old_external_proc;
  mn_setting.topic_len = old_topic_len;
  mn_setting.cycle_moves = old_cycle_moves;

  if (mn_environment.keystroke == kENTER)
  {
    const struct bnk_bank_item_t *item = &bnk_bank->items[index];

    dst->four_op = false;
    dst->use_macro = false;

    // instrument data
    memcpy (&dst->ins1.fm, &item->ins, sizeof (dst->ins1.fm));

    // instrument name
    if (item->name[0] != '\0')
      StrToString (dst->ins1.name, item->name, sizeof (dst->ins1.name) - 1);
    else
      SetLength (dst->ins1.name, 0);
    set_default_ins_name_if_needed (dst, fname);

    result = 0;
  } else
    result = 1;

_exit:
  if (progress_win.opened) progress_window_close (&progress_win);
  if (bnk_bank != NULL) bnk_bank_free (bnk_bank);
  if (queue != NULL) free (queue);

  DBG_LEAVE (); //EXIT //bnk_file_loader
  return result;

_err_malloc:
  *error = "Memory allocation failed";
  goto _exit;

_err_empty:
  *error = "No instruments in file";
  goto _exit;
}
