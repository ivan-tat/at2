// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Returns `true' on error and error description in `error'.
bool fib_file_loader (temp_instrument_t *dst, const String *fname, char **error)
{
  bool result = true; // `false' on success, `true' on error
  bool w_opened = false;
  fin_bank_t *fin_bank = NULL;
  String (*queue)[][FIB_HEADER_STR_MAX+1] = NULL; // TODO: use structure and pass it to `Menu()'
  uint16_t index, first, __UNUSED last; // `first' and `last' are for future use
  void (*old_external_proc) (void);
  uint8_t old_topic_len;
  bool old_cycle_moves;
  uint8_t xstart, ystart;
  uint8_t ysize;

  DBG_ENTER ("fib_file_loader");

  ScreenMemCopy (screen_ptr, ptr_screen_backup);
  centered_frame_vdest = screen_ptr;
  {
    char s[63+1];

    snprintf (s, sizeof (s), " ESC %c%c STOP ", charmap.bd_light_horiz, charmap.black_right_triangle); // " ESC ─► STOP "
    StrToString (dl_environment.context, s, sizeof (dl_environment.context) - 1);
  }
  {
    String_t s, t;

    s = NameOnly (fname);
    t = iCASE ((String *)&s);
    StrToString ((String *)&s, " ", sizeof (s) - 1);
    AppendString ((String *)&s, (String *)&t, sizeof (s) - 1);
    AppendString ((String *)&s, StrToString ((String *)&t, " ", sizeof (t) - 1), sizeof (s) - 1);
    centered_frame (&xstart, &ystart, 43, 3, (String *)&s,
                    dialog_background + dialog_border,
                    dialog_background + dialog_title,
                    frame_double);
  }
  ShowStr (screen_ptr, xstart + 43 - Length (dl_environment.context), ystart + 3,
           dl_environment.context,
           dialog_background + dialog_border);
  SetLength (dl_environment.context, 0);

  progress_num_steps = 1;
  progress_step = 1;
  progress_value = 100 + 100; // +100=100% loaded, +100=100% processed
  progress_old_value = UINT8_NULL;
  progress_xstart = xstart + 2;
  progress_ystart = ystart + 2;
  {
    String_t s;
    String t[63+1];

    s = iCASE (StrToString (t, "Loading data from bank file...", sizeof (t) - 1));
    ShowCStr (screen_ptr, xstart + 2, ystart + 1, (String *)&s,
              dialog_background + dialog_text,
              dialog_background + dialog_hi_text);
  }
  show_progress (0);
  w_opened = true;

  if ((fin_bank = load_fin_bank (fname, error)) == NULL) goto _exit;

  show_progress2 (100, 1);

  if ((queue = malloc (sizeof ((*queue)[0]) * (FIB_HEADER_LINES + fin_bank->capacity))) == NULL) goto _err_malloc;

  for (uint_least8_t i = 0; i < FIB_HEADER_LINES; i++)
  {
    AT2_char_t a[FIB_HEADER_STR_LEN_MAX+1];
    String_t s, t;

    UTF8nstr_to_AT2 (a, sizeof (a), fib_header_str[i]);
    t = iCASE (StrToString ((String *)&s, (char *)a, sizeof ((*queue)[0]) - 1));
    CopyString ((*queue)[i], (String *)&t, sizeof ((*queue)[0]) - 1);
  }

  fin_bank->count = 0; // clear (it was valid, but we want to do it again with user interaction)
  first = 0;
  last = 0;
  for (index = 0; index < fin_bank->capacity; index++)
  {
    char s[sizeof ((*queue)[0])];

    if ((ticklooper == 0) && (index > 0))
    {
      if (keypressed ())
      {
        fkey = GetKey ();
        if (fkey == kESC) break; // stop - that's why `index' is needed later
      }

      show_progress2 (100 + 100 * (index + 1) / fin_bank->capacity, 1);
    }

    // item string
    fib_file_loader_get_item_str (s, sizeof (s), index, fin_bank);
    StrToString ((*queue)[FIB_HEADER_LINES + index], s, sizeof ((*queue)[0]) - 1);

    if (fin_bank->items[index].available)
    {
      last = index; // update
      fin_bank->count++; // count
    }
    else if (first == index) first++; // update
  }
  // `index' is now a total number of instruments including unavailable (<= fin_bank->capacity)

  show_progress2 (100 + 100, 1);
  // delay for awhile to show progress bar
#if GO32
  Pascal_Delay (500);
#else // !GO32
  SDL_Delay (200);
#endif // !GO32
  fib_file_loader_restore (xstart, ystart);
  w_opened = false;

  if (fin_bank->count == 0) goto _err_empty;

  {
    char s[63+1];

    if (index == fin_bank->capacity)
      snprintf (s, sizeof (s), "~[~%d~/%d]~", fin_bank->count, fin_bank->capacity);
    else
      snprintf (s, sizeof (s), "[%d]~[%d/%d]~", fin_bank->count, index, fin_bank->capacity);

    StrToString (mn_environment.context, s, sizeof (mn_environment.context) - 1);
  }

  old_external_proc = mn_environment.ext_proc;
  old_topic_len = mn_setting.topic_len;
  old_cycle_moves = mn_setting.cycle_moves;
  mn_environment.ext_proc = fib_lister_external_proc;
  mn_setting.topic_len = FIB_HEADER_LINES;
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
    index = Menu (queue, 1, 1, min (1, get_bank_position (fname, fin_bank->capacity)),
                  sizeof ((*queue)[0]) - 1, ysize, FIB_HEADER_LINES + index, (String *)&s) - 1;
  }

  add_bank_position (fname, fin_bank->capacity, FIB_HEADER_LINES + index + 1);
  mn_environment.ext_proc = old_external_proc;
  mn_setting.topic_len = old_topic_len;
  mn_setting.cycle_moves = old_cycle_moves;

  load_flag_alt = UINT8_NULL; // this flag may be changed by `fin_lister_external_proc()'
  if (mn_environment.keystroke == kENTER)
  {
    const struct fin_bank_item_t *item = &fin_bank->items[index];

    dst->four_op = false;
    dst->use_macro = false;

    // instrument data
    memcpy (&dst->ins1.fm, &item->ins, sizeof (dst->ins1.fm));

    // instrument name
    if (item->iname[0] != '\0')
      StrToString (dst->ins1.name, item->iname, sizeof (dst->ins1.name) - 1);
    else if (item->dname[0] != '\0')
      StrToString (dst->ins1.name, item->dname, sizeof (dst->ins1.name) - 1);
    else
      SetLength (dst->ins1.name, 0);
    set_default_ins_name_if_needed (dst, fname);

    load_flag_alt = 1;
  }

  result = false;

_exit:
  if (w_opened) fib_file_loader_restore (xstart, ystart);
  if (fin_bank != NULL) fin_bank_free (fin_bank);
  if (queue != NULL) free (queue);

  DBG_LEAVE (); //EXIT //fib_file_loader
  return result;

_err_malloc:
  *error = "Memory allocation failed";
  goto _exit;

_err_empty:
  *error = "No instruments in file";
  goto _exit;
}
