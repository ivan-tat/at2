// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void make_iCASE_fullname (String *dest, size_t maxlen, const String *filename, const String *ext)
{
  String_t s, t;

  s = PathOnly (filename);
  s = iCASE_filename ((String *)&s);
  t = BaseNameOnly (filename);
  t = Lower_filename ((String *)&t);
  AppendString ((String *)&s, (String *)&t, sizeof (s) - 1);
  AppendString ((String *)&s, StrToString ((String *)&t, ".", sizeof (t) - 1), sizeof (s) - 1);
  AppendString ((String *)&s, ext, sizeof (s) - 1);
  CopyString (dest, (String *)&s, maxlen);
}

static void make_fullname (String *dest, size_t maxlen, const String *path, const String *src)
{
  if ((path == NULL) || (Length (path) == 0))
    CopyString (dest, src, maxlen);
  else
  {
    String_t s, t;

    s = iCASE_filename (path);
    t = NameOnly (src);
    AppendString ((String *)&s, (String *)&t, sizeof (s) - 1);
    CopyString (dest, (String *)&s, maxlen);
  }
}

static void make_alt_ins_fullname (String *dest, size_t maxlen, const String *path, uint8_t index, const String *ext)
{
  String_t s;
  char t[255+1], u[15+1], v[255+1];

  s = iCASE_filename (path);
  snprintf (v, sizeof (v),
    "%sinstr%03"PRIu8".%s",
    StringToStr (t, (String *)&s, sizeof (t) - 1),
    index,
    StringToStr (u, ext, sizeof (u) - 1)
  );
  StrToString (dest, v, maxlen);
}

typedef struct
{
  String_t instdata_source;
  String_t songdata_source;
} source_filenames_t;

static void backup_source_filenames (source_filenames_t *backup)
{
  CopyString ((String *)&backup->instdata_source, instdata_source, sizeof (backup->instdata_source) - 1);
  CopyString ((String *)&backup->songdata_source, songdata_source, sizeof (backup->songdata_source) - 1);
}

static void restore_source_filenames (const source_filenames_t *backup)
{
  CopyString (instdata_source, (String *)&backup->instdata_source, sizeof (instdata_source) - 1);
  CopyString (songdata_source, (String *)&backup->songdata_source, sizeof (songdata_source) - 1);
}

static void iCASE_source_filenames (const String *ext)
{
  if (Length (instdata_source) != 0)
    make_iCASE_fullname (instdata_source, sizeof (instdata_source) - 1, instdata_source, ext);
  if (Length (songdata_source) != 0)
    make_iCASE_fullname (songdata_source, sizeof (songdata_source) - 1, songdata_source, ext);
}

static String *get_source_filename (ftype_t type, size_t *max)
{
  String *result = NULL;
  size_t result_max = 0;

  switch (type)
  {
    case FTYPE_INSTRUMENT:
    case FTYPE_INSTRUMENT_BANK:
      result = instdata_source;
      result_max = sizeof (instdata_source);
      break;

    case FTYPE_PATTERN:
    case FTYPE_SONG:
      result = songdata_source;
      result_max = sizeof (songdata_source);
      break;

    default: break;
  }

  if (max != NULL) *max = result_max;
  return result;
}

void FILE_save (const String *ext)
{
  tFIXED_SONGDATA *song = &songdata;
  source_filenames_t backup; // TODO: use `malloc()'
  String_t ext_lower;
  String_t output_fname;
  int8_t overwrite;
  const struct file_saver_t *saver = NULL;

  DBG_ENTER ("FILE_save");

  ext_lower = Lower (ext);

  // find saver
  {
    char s[15+1];

    StringToStr (s, (String *)&ext_lower, sizeof (s) - 1);
    for (uint8_t i = 0; file_savers[i].ext != NULL; i++)
      if (strcmp (s, file_savers[i].ext) == 0)
      {
        saver = &file_savers[i];
        break;
      }
  }
  if (saver == NULL) goto _exit;

  backup_source_filenames (&backup);
  iCASE_source_filenames (ext);

  do
  {
    String_t title;

    if (!quick_cmd || (saver->type != FTYPE_SONG) || (Length (get_source_filename (FTYPE_SONG, NULL)) == 0))
    {
      bool quit_flag;

      do
      {
        is_setting.append_enabled    = true;
        bit_clear_range (is_setting.character_set, 0, 31);
        bit_set_range (is_setting.character_set, 32, 255);
        dl_setting.center_text       = false;
        dl_setting.terminate_keys[2] = kTAB;
        is_setting.terminate_keys[2] = kTAB;
        is_environment.locate_pos    = 1;
        {
          char s[127+1], t[127+1];
          String_t u;

          u = iCASE (StrToString ((String *)&u, "File selector", sizeof (u) - 1));
          StringToStr (t, (String *)&u, sizeof (t) - 1);
          snprintf (s, sizeof (s), " TAB %c%c %s ", charmap.bd_light_horiz, charmap.black_right_triangle, t);
          StrToString (dl_environment.context, s, sizeof (dl_environment.context) - 1);
        }

        if (   (saver->type == FTYPE_INSTRUMENT)
            || (saver->type == FTYPE_INSTRUMENT_BANK))
        {
          if (saver->alt_name && alt_ins_name)
            make_alt_ins_fullname (dl_environment.input_str, sizeof (dl_environment.input_str) - 1, saver->path,
                                   current_inst, (String *)&ext_lower);
          else
            make_fullname (dl_environment.input_str, sizeof (dl_environment.input_str) - 1, saver->path,
                           get_source_filename (saver->type, NULL));
        }
        else if (   (saver->type == FTYPE_PATTERN)
                 || (saver->type == FTYPE_SONG))
          make_fullname (dl_environment.input_str, sizeof (dl_environment.input_str) - 1, saver->path,
                         get_source_filename (saver->type, NULL));

        {
          String_t a, b, c;
          char s[63+1], t[15+1], u[127+1];

          a = iCASE (StrToString ((String *)&a, "extension is set to", sizeof (a) - 1));
          StringToStr (s, (String *)&a, sizeof (s) - 1);
          a = iCASE_filename (ext);
          StringToStr (t, (String *)&a, sizeof (t) - 1);
          snprintf (u, sizeof (u), "{PATH}[FILENAME] %s \"%s\"$", s, t);
          StrToString ((String *)&a, u, sizeof (a) - 1);

          snprintf (s, sizeof (s),
                    "%%string_input%%255$50$%"PRIX8"$%"PRIX8"$",
                    dialog_input_bckg + dialog_input,
                    dialog_def_bckg + dialog_def
          );
          StrToString ((String *)&b, s, sizeof (b) - 1);

          c = iCASE (StrToString ((String *)&s, " Save file ", sizeof (s) - 1));

          Dialog ((String *)&a, (String *)&b, (String *)&c, 0);
        }

        dl_setting.terminate_keys[2] = 0;
        is_setting.terminate_keys[2] = 0;
        dl_setting.center_text       = true;
        SetLength (dl_environment.context, 0);

        {
          String_t s = BaseNameOnly (dl_environment.input_str);
          if (   (dl_environment.keystroke == kESC)
              || ((dl_environment.keystroke != kTAB) && (s.len == 0))) goto _cancel;
        }

        if (dl_environment.keystroke == kENTER)
        {
          String *dst;
          size_t max;

          make_iCASE_fullname ((String *)&output_fname, sizeof (output_fname) - 1, dl_environment.input_str, (String *)&ext_lower);

          dst = get_source_filename (saver->type, &max);
          CopyString (dst, (String *)&output_fname, max - 1);
        }

        quit_flag = true;
        if (dl_environment.keystroke == kTAB)
        {
          uint8_t mpos =  (saver->type == FTYPE_INSTRUMENT)
                       || (saver->type == FTYPE_INSTRUMENT_BANK) ? 3 : 2;

          CopyString (fs_environment.last_file, last_file[mpos], sizeof (fs_environment.last_file) - 1);
          CopyString (fs_environment.last_dir,  last_dir[mpos],  sizeof (fs_environment.last_dir) - 1);

          {
            String_t a;
            char s[15+1], t[15+1];
            snprintf (s, sizeof (s), "*.%s$", StringToStr (t, ext, sizeof (t) - 1));
            output_fname = Fselect (StrToString ((String *)&a, s, sizeof (a) - 1));
          }

          CopyString (last_file[mpos], fs_environment.last_file, sizeof (last_file[0]) - 1);
          CopyString (last_dir[mpos],  fs_environment.last_dir,  sizeof (last_dir[0]) - 1);

          if (mn_environment.keystroke == kESC)
            quit_flag = false;
          else
          {
            String *dst;
            size_t max;

            make_iCASE_fullname ((String *)&output_fname, sizeof (output_fname) - 1, (String *)&output_fname, (String *)&ext_lower);

            dst = get_source_filename (saver->type, &max);
            CopyString (dst, (String *)&output_fname, max - 1);
          }
        }
      } while (!quit_flag);

      if (dl_environment.keystroke == kESC) goto _cancel;
    }

    CopyString ((String *)&output_fname, get_source_filename (saver->type, NULL), sizeof (output_fname) - 1);

    {
      String_t s;
      StrToString ((String *)&title, " ", sizeof (title) - 1);
      StrToString ((String *)&s, saver->name, sizeof (s) - 1);
      s = iCASE ((String *)&s);
      AppendString ((String *)&title, (String *)&s, sizeof (title) - 1);
      StrToString ((String *)&s, " ", sizeof (s) - 1);
      AppendString ((String *)&title, (String *)&s, sizeof (title) - 1);
      overwrite = ask_file_overwrite_rename ((String *)&output_fname, (String *)&title);
    }

    if (overwrite == 0)
    {
      progress_window_t progress_win;
      progress_callback_t *progress;
      char *error = NULL;
      int8_t saver_result = -1;

      HideCursor ();
      progress_window_init (&progress_win, 44, 4, (String *)&title, NULL);
      progress = &progress_win.callback;

      if (saver->type == FTYPE_INSTRUMENT)
      {
        saver_result = saver->proc.instrument (
          (String *)&output_fname,
          song, current_inst > 0 ? current_inst - 1 : 0,
          progress, &error
        );
      }
      else if (saver->type == FTYPE_INSTRUMENT_BANK)
      {
        saver_result = saver->proc.instrument_bank (
          (String *)&output_fname,
          song,
          progress, &error
        );
      }
      else if (saver->type == FTYPE_PATTERN)
      {
        saver_result = saver->proc.pattern (
          (String *)&output_fname,
          song, pattern2use != UINT8_NULL ? pattern2use : pattern_patt,
          progress, &error
        );
      }
      else if (saver->type == FTYPE_SONG)
      {
        song->bpm_data.rows_per_beat = mark_line;
        song->bpm_data.tempo_finetune = IRQ_freq_shift;
        saver_result = saver->proc.song (
          (String *)&output_fname,
          song,
          progress, &error
        );
        if (saver_result >= 0)
        {
          String_t s = NameOnly ((String *)&output_fname);
          CopyString (songdata_title, (String *)&s, sizeof (songdata_title) - 1);
          songdata_crc = Update32 (song, sizeof (*song), /*CRC32_INITVAL*/ 0);
          songdata_crc_ord = Update32 (song->pattern_order, sizeof (song->pattern_order), /*CRC32_INITVAL*/ 0);
          module_archived = true;
        }
      }

      // delay for awhile to show progress bar at 100%
#if GO32
      delay (500);
#else // !GO32
      SDL_Delay (500);
#endif // !GO32
      progress_window_close (&progress_win);
      progress = NULL;

      if (saver_result < 0)
      {
        String_t a, b;
        char s[255+1];

        snprintf (s, sizeof (s), "%s$Saving stopped$", error);
        a = iCASE (StrToString ((String *)&a, s, sizeof (a) - 1));
        b = iCASE (StrToString ((String *)&b, "~O~Kay$", sizeof (b) - 1));
        Dialog ((String *)&a, (String *)&b, (String *)&title, 1);
        break;
      }
    }
  } while (overwrite > 0);

_exit:
  DBG_LEAVE(); //EXIT //FILE_save
  return;

_cancel:
  restore_source_filenames (&backup);
  goto _exit;
}
