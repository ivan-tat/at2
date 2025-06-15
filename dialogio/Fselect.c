// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void read_masks (const String *masks)
{
  String_t s, t;
  size_t ml = Length (masks);
  size_t sl;
  size_t i = 1;

  _fs.count = 0;
  do
  {
    s = Upper (ReadChunk ((String *)&t, masks, i));
    sl = s.len;
    i += sl + 1;
    if ((sl != 0) && (_fs.count < 20))
    {
      CopyString ((String *)&_fs.masks[_fs.count], (String *)&s, 255);
      _fs.count++;
    }
  } while ((i < ml) && (sl != 0) && (_fs.count < 20));
}

static void add_trailing_path_separator (String *s, uint8_t max)
{
  uint8_t len = Length (s);

  if ((len < max) && (GetStr (s)[len - 1] != PATHSEP))
  {
    GetStr (s)[len] = PATHSEP;
    SetLength (s, len + 1);
  }
}

static String *remove_lastname (String *s)
{
  size_t len = Length (s);

  while ((len != 0) && (GetStr (s)[len - 1] != PATHSEP))
  {
    len--;
    SetLength (s, len);
  }

  return s;
}

String_t Fselect (const String *mask)
{
  String_t result;
  bool respawn_dialog;

  int32_t  item_idx; // user selected menu entry (1-based)
  int32_t  list_start; // start index of directory contents (1-based)
  int32_t  lastp = 0; // last index (1-based)
  String_t searchdir;
  String_t lastname;
  String_t curdir;

  DBG_ENTER ("Fselect");

  result.len = 0;
  do
  {
    respawn_dialog = false;

    read_masks (mask);

    Pascal_GetDir (0, (String *)&curdir);
    if (Pascal_IOResult () != 0)
      CopyString ((String *)&curdir, fs_environment.last_dir, 255);

    if (Length (fs_environment.last_dir) != 0)
    {
      Pascal_ChDir (fs_environment.last_dir);
      if (Pascal_IOResult () != 0)
      {
        Pascal_ChDir ((String *)&curdir);
        if (Pascal_IOResult () != 0) { /*nothing*/ }
        StrToString (fs_environment.last_file, "FNAME:EXT", FILENAME_SIZE);
      }
    }

    Pascal_GetDir (0, (String *)&searchdir);
    if (Pascal_IOResult () != 0)
      CopyString ((String *)&searchdir, (String *)&curdir, 255);
    add_trailing_path_separator ((String *)&searchdir, 255);
    mn_setting.cycle_moves = false;
    lastname.len = 0;

    mn_environment.descr_len = DESCDAT_LEN;
    mn_environment.descr = &_fs.descr;
    mn_environment.winshade = false;
    ScreenMemCopy (screen_ptr, ptr_scr_backup);

    do
    {
      {
        String_t t;
        CopyString (_path[UpCase (searchdir.str[0]) - 'A'], path_filter ((String *)&t, (String *)&searchdir), PATH_SIZE);
      }
      make_stream ((String *)&searchdir, mask, &_fs.fstream);

#if GO32

      for (int32_t i = 0; i < _fs.fstream.count; i++)
      {
        String_t s, t;

        if (CompareStrings(_fs.fstream.stuff[i].name, updir_str) != 0)
        {
          String_t name; // at most 8 characters, see below
          String_t ext; // at most 3 characters, see below

          s = FilterStr2 (_fs.fstream.stuff[i].name, _valid_characters, '_');
          name = BaseNameOnly ((String *)&s);
          ext = ExtOnly ((String *)&s);
          CopyString ((String *)&s, (String *)"\x01" " ", MENUDAT_LEN);
          t = ExpStrR ((String *)&name, 8, ' ');
          AppendString ((String *)&s, (String *)&t, MENUDAT_LEN);
          AppendString ((String *)&s, (String *)"\x01" " ", MENUDAT_LEN);
          t = ExpStrR ((String *)&ext, 3, ' ');
          AppendString ((String *)&s, (String *)&t, MENUDAT_LEN);
          AppendString ((String *)&s, (String *)"\x01" " ", MENUDAT_LEN);
          CopyString (_fs.menudat[i], (String *)&s, MENUDAT_LEN);

          if ((_fs.fstream.stuff[i].attr & Pascal_Directory) != 0)
          {
            s = iCASE (_fs.menudat[i]);
            CopyString (_fs.menudat[i], (String *)&s, MENUDAT_LEN);
          }
        }
        else
        {
          CopyString ((String *)&s, (String *)"\x01" " ", MENUDAT_LEN);
          t = ExpStrR (TWO_DOTS_STR, mn_environment.descr_len - 1, ' ');
          AppendString ((String *)&s, (String *)&t, MENUDAT_LEN);
          CopyString (_fs.menudat[i], (String *)&s, MENUDAT_LEN);
          CopyString (_fs.fstream.stuff[i].name, TWO_DOTS_STR, FILENAME_SIZE);
        }
      }

      for (int32_t i = 0; i < _fs.fstream.count; i++)
      {
        if ((_fs.fstream.stuff[i].attr & Pascal_VolumeId) != 0)
        {
          const char *p;

          if (CompareStrings (_fs.fstream.stuff[i].name, (String *)"\x03" "~\xFF~") == 0) // TODO: implement new flag for this
            p = "";
          else
            p = "[~DRiVE~]"; // TODO: i18n

          StrToString (_fs.descr[i], p, DESCDAT_LEN);
        }
        else if ((_fs.fstream.stuff[i].attr & Pascal_Directory) != 0)
        {
          String_t s;
          const String  *p;

          if (CompareStrings( _fs.fstream.stuff[i].name, TWO_DOTS_STR) == 0)
            p = (String *)"\x08" "[UP-DiR]"; // TODO: i18n
          else
            p = (String *)"\x05" "[DiR]"; // TODO: i18n

          s = ExpStrL (p, mn_environment.descr_len - 1, ' ');
          CopyString (_fs.descr[i], (String *)&s, DESCDAT_LEN);
        }
        else
        {
          String_t s;
          String numstr[13+1];
          s = ExpStrL (number_with_commas (numstr, _fs.fstream.stuff[i].size, 10),
                       mn_environment.descr_len - 1, ' ');
          CopyString (_fs.descr[i], (String *)&s, DESCDAT_LEN);
        }
      }

#else // !GO32

      for (int32_t i = 0; i < _fs.fstream.count; i++)
      {
        if ((_fs.fstream.stuff[i].attr & Pascal_Directory) != 0)
        {
          if (CompareStrings (_fs.fstream.stuff[i].name, updir_str) == 0)
          {
            String_t s, t;

            CopyString ((String *)&s, (String *)"\x01" " ", MENUDAT_LEN);
            t = ExpStrR (TWO_DOTS_STR, MENUDAT_LEN - 1, ' ');
            AppendString ((String *)&s, (String *)&t, MENUDAT_LEN);
            AppendString ((String *)&s, (String *)"\x01" " ", MENUDAT_LEN);
            CopyString (_fs.menudat[i], (String *)&s, MENUDAT_LEN);

            s = ExpStrL ((String *)&"\x08" "[UP-DiR]", mn_environment.descr_len - 1, ' '); // TODO: i18n
            CopyString (_fs.descr[i], (String *)&s, DESCDAT_LEN);

            CopyString (_fs.fstream.stuff[i].name, TWO_DOTS_STR, FILENAME_SIZE);
          }
          else
          {
            String_t s, t, fname;

            s = FilterStr2 (_fs.fstream.stuff[i].name, _valid_characters_fname, '_');
            t = DietStr ((String *)&s, MENUDAT_LEN - 1 + (mn_environment.descr_len - 1 - 10));
            fname = iCASE_filename ((String *)&t);
            if (fname.len < MENUDAT_LEN - 1)
            {
              CopyString ((String *)&s, (String *)"\x01" " ", MENUDAT_LEN);
              t = ExpStrR ((String *)&fname, MENUDAT_LEN - 1, ' ');
              AppendString ((String *)&s, (String *)&t, MENUDAT_LEN);
              AppendString ((String *)&s, (String *)"\x01" " ", MENUDAT_LEN);
              CopyString (_fs.menudat[i], (String *)&s, MENUDAT_LEN);

              t.len = 0;
            }
            else
            {
              s = Copy ((String *)&fname, 1, MENUDAT_LEN - 1);
              t = ExpStrR ((String *)&s, MENUDAT_LEN - 1, ' ');
              CopyString ((String *)&s, (String *)"\x01" " ", MENUDAT_LEN);
              AppendString ((String *)&s, (String *)&t, MENUDAT_LEN);
              CopyString (_fs.menudat[i], (String *)&s, MENUDAT_LEN);

              t = Copy ((String *)&fname, MENUDAT_LEN, fname.len - (MENUDAT_LEN - 2));
            }

            s = ExpStrR ((String *)&t, mn_environment.descr_len - 1 - 10, ' ');
            t = ExpStrL ((String *)&"\x05" "[DiR]", 10, ' '); // TODO: i18n
            AppendString ((String *)&s, (String *)&t, DESCDAT_LEN);
            CopyString (_fs.descr[i], (String *)&s, DESCDAT_LEN);
          }
        }
        else
        {
          String_t s, t;

          s = FilterStr2 (_fs.fstream.stuff[i].name, _valid_characters_fname, '_');
          t = BaseNameOnly ((String *)&s);
          s = DietStr ((String *)&t, MENUDAT_LEN - 2);
          t = ExpStrR ((String *)&s, MENUDAT_LEN - 2, ' ');
          CopyString ((String *)&s, (String *)"\x01" " ", MENUDAT_LEN);
          AppendString ((String *)&s, (String *)&t, MENUDAT_LEN);
          AppendString ((String *)&s, (String *)"\x01" " ", MENUDAT_LEN);
          CopyString (_fs.menudat[i], (String *)&s, MENUDAT_LEN);
        }
      }

      for (int32_t i = 0; i < _fs.fstream.count; i++)
      {
        String_t s;

        if ((_fs.fstream.stuff[i].attr & Pascal_VolumeId) != 0)
        {
          if (CompareStrings (_fs.fstream.stuff[i].name, (String *)& "\x03" "~\xFF~") == 0) // TODO: implement new flag for this
            CopyString (_fs.descr[i], (String *)"\x00" "", DESCDAT_LEN);
          else
          {
            CopyString ((String *)&s, (String *)"\x02" "[~", DESCDAT_LEN);
            AppendString ((String *)&s, (String *)&_fs.fstream.stuff[i].info, DESCDAT_LEN);
            AppendString ((String *)&s, (String *)"\x02" "~]", DESCDAT_LEN);
            CopyString ((String *)_fs.descr[i], (String *)&s, DESCDAT_LEN);
          }
        }
        else if ((_fs.fstream.stuff[i].attr & Pascal_Directory) == 0)
        {
          String_t t, u;
          String numstr[13+1];

          CopyString (_fs.descr[i], number_with_commas (numstr, _fs.fstream.stuff[i].size, 10), DESCDAT_LEN);
          s = ExtOnly (_fs.fstream.stuff[i].name);
          t = Copy((String *)&s, 1, 3);
          s = ExpStrR ((String *)&t, 3, ' ');
          AppendString ((String *)&s, (String *)"\x01" " ", DESCDAT_LEN);
          t = DietStr (_fs.descr[i], mn_environment.descr_len - 1 - 4);
          u = ExpStrL ((String *)&t, mn_environment.descr_len - 1 - 4, ' ');
          AppendString ((String *)&s, (String *)&u, DESCDAT_LEN);
          CopyString (_fs.descr[i], (String *)&s, DESCDAT_LEN);
        }
      }

#endif // !GO32

      for (int32_t i = 0; i < _fs.fstream.count; i++)
      {
        if (   (PosChr ('~', _fs.fstream.stuff[i].name) != 0)
            && (CompareStrings (_fs.fstream.stuff[i].name, (String *)"\x03" "~\xFF~") != 0)) // TODO: implement new flag for this
        {
          ssize_t j = PosChr ('~', _fs.menudat[i]);

          while (j != 0)
          {
            GetStr (_fs.menudat[i])[j - 1] = PATHSEP;
            j = PosChr ('~', _fs.menudat[i]);
          }
        }
      }

      list_start = _fs.fstream.drive_count + DRIVE_DIVIDER;
      while (   (list_start <= _fs.fstream.count)
             && (lastname.len != 0)
             && (CompareStrings ((String *)&lastname, _fs.fstream.stuff[list_start - 1].name) != 0))
        list_start++;
      if (list_start > _fs.fstream.count)
        list_start = 1;

      {
        String_t s;
        int32_t i;

        for (i = 0; i < _fs.fstream.count; i++)
        {
          s = Lower_filename (_fs.fstream.stuff[i].name);

          if (   (CompareStrings ((String *)&s, fs_environment.last_file) == 0)
              && ((_fs.fstream.stuff[i].attr & Pascal_VolumeId) == 0))
          {
            lastp = i + 1;
            break;
          }
        }

        s = Lower_filename (_fs.fstream.stuff[i].name);
        if (CompareStrings ((String *)&s, fs_environment.last_file) != 0)
          lastp = 0;
        if ((lastp == 0) || (lastp > MAX_FILES))
          lastp = list_start;
      }

      mn_setting.reverse_use = true;
      {
        char s[127+1];

        snprintf (s, 127+1, " ~%u FILES FOUND~ ", _fs.fstream.match_count); // TODO: i18n
        StrToString (mn_environment.context, s, 255);
      }
      mn_setting.terminate_keys[2] = kBkSPC;
#if linux || unix
      mn_setting.terminate_keys[3] = kSlash;
#else // !(linux || unix)
      mn_setting.terminate_keys[3] = kSlashR;
#endif // !(linux || unix)
      mn_setting.terminate_keys[4] = kF1;
      old_fselect_external_proc = mn_environment.ext_proc;
      mn_environment.ext_proc = new_fselect_external_proc;

      {
        uint16_t i = 1;

        while (   (i < _fs.fstream.count)
               && (Pos ((String *)"\x08" "[UP-DiR]", _fs.descr[i - 1]) == 0)) // TODO: i18n
          i++;
        mn_setting.homing_pos = i < _fs.fstream.count ? i : (_fs.fstream.drive_count + DRIVE_DIVIDER);
      }

#if linux || unix
      _fs.fstream.count--;
#endif // linux || unix

      {
        String_t s, t;

#if GO32
        s = DietStr (path_filter ((String *)&t, (String *)&searchdir), 28);
#else // !GO32
        t = FilterStr2 (path_filter ((String *)&s, (String *)&searchdir), _valid_characters_fname, '_');
        s = DietStr ((String *)&t, 38);
#endif // !GO32

        t = iCASE ((String *)&s);
        CopyString ((String *)&s, (String *)"\x01" " ", 255);
        AppendString ((String *)&s, (String *)&t, 255);
        AppendString ((String *)&s, (String *)"\x01" " ", 255);

        item_idx = Menu (&_fs.menudat, 1, 1, lastp, MENUDAT_LEN,
#if GO32
                         max (work_MaxLn - 7, 30),
#else // !GO32
                         max (work_MaxLn - 5, 30),
#endif // !GO32
                         _fs.fstream.count, (String *)&s);
      }

      mn_environment.ext_proc = old_fselect_external_proc;
      mn_setting.reverse_use = false;
      SetLength (mn_environment.context, 0);
      mn_setting.terminate_keys[2] = 0;
      mn_setting.terminate_keys[3] = 0;
      mn_setting.terminate_keys[4] = 0;

      if (   (mn_environment.keystroke == kENTER)
          && ((_fs.fstream.stuff[item_idx - 1].attr & Pascal_Directory) != 0))
      {
        String_t s;

        StrToString (fs_environment.last_file, "FNAME:EXT", FILENAME_SIZE);
        mn_environment.keystroke = ATEK_UNKNOWN;
        if (CompareStrings (_fs.fstream.stuff[item_idx - 1].name, TWO_DOTS_STR) == 0)
        {
          if (searchdir.len != 0)
            searchdir.len--;
          lastname = NameOnly ((String *)&searchdir);
          remove_lastname ((String *)&searchdir);
          s = Lower_filename ((String *)&lastname);
          CopyString (fs_environment.last_file, (String *)&s, FILENAME_SIZE);
        }
        else
        {
          AppendString ((String *)&searchdir, _fs.fstream.stuff[item_idx - 1].name, 255);
          add_trailing_path_separator ((String *)&searchdir, 255);
          lastname.len = 0;
          SetLength (fs_environment.last_file, 0);
        }

        s = Copy ((String *)&searchdir, 1, searchdir.len - 1);
        Pascal_ChDir ((String *)&s);
        if (Pascal_IOResult () != 0) { /*nothing*/ }
      }
      else if (   (mn_environment.keystroke == kENTER)
               && ((_fs.fstream.stuff[item_idx - 1].attr & Pascal_VolumeId) != 0))
      {
        StrToString (fs_environment.last_file, "FNAME:EXT", FILENAME_SIZE);
        mn_environment.keystroke = ATEK_UNKNOWN;
        Pascal_ChDir (_path[UpCase (GetStr (_fs.fstream.stuff[item_idx - 1].name)[0]) - 'A']);
        if (Pascal_IOResult () != 0)
          CopyString ((String *)&searchdir, _path[UpCase (GetStr (_fs.fstream.stuff[item_idx - 1].name)[0]) - 'A'], 255);
        else
        {
          Pascal_GetDir (0, (String *)&searchdir);
          if (Pascal_IOResult () != 0)
            CopyString ((String *)&searchdir, (String *)&curdir, 255);
        }
        add_trailing_path_separator ((String *)&searchdir, 255);
        lastname.len = 0;
        SetLength (fs_environment.last_file, 0);
      }
      else if (mn_environment.keystroke == kBkSPC)
      {
        String_t s;

        if (shift_pressed ())
        {
          String_t t;

          if (Length (home_dir_path) != 0)
            CopyString ((String *)&t, home_dir_path, 255);
          else
          {
            s = Pascal_ParamStr (0);
            t = PathOnly ((String *)&s);
          }

          add_trailing_path_separator ((String *)&t, 255);

          s = Copy ((String *)&t, 1, t.len - 1);
          Pascal_ChDir ((String *)&s);
          if (Pascal_IOResult () == 0)
          {
            CopyString ((String *)&searchdir, (String *)&t, 255);
            CopyString ((String *)&lastname, TWO_DOTS_STR, 255);
          }
          else
          {
            Pascal_ChDir ((String *)&searchdir);
            if (Pascal_IOResult () != 0) { /*nothing*/ }
          }
        }
        else
        {
#if linux || unix
          // remove last character (PATHSEP)
          s = Copy ((String *)&searchdir, 1, searchdir.len - 1);
#else // !(linux || unix)
          // remove leading `<disk>:' and remove last character (PATHSEP)
          s = Copy ((String *)&searchdir, 3, searchdir.len - 3);
#endif // !(linux || unix)
          if (PosChr (PATHSEP, (String *)&s) != 0)
          {
            searchdir.len--;
            lastname = NameOnly ((String *)&searchdir);
            remove_lastname ((String *)&searchdir);
            s = Lower_filename ((String *)&lastname);
            CopyString (fs_environment.last_file, (String *)&s, FILENAME_SIZE);
            s = Copy ((String *)&searchdir, 1, searchdir.len - 1);
            Pascal_ChDir ((String *)&s);
            if (Pascal_IOResult () != 0) { /*nothing*/ }
          }
        }
      }
#if linux || unix
      else if (mn_environment.keystroke == kSlash)
#else // !(linux || unix)
      else if (mn_environment.keystroke == kSlashR)
#endif // !(linux || unix)
      {
#if linux || unix
        searchdir.len = 1; // truncate to `<PATHSEP>'
#else // !(linux || unix)
        searchdir.len = 3; // truncate to `<disk>:<PATHSEP>'
#endif // !(linux || unix)
        lastname.len = 0;
        SetLength (fs_environment.last_file, 0);
        Pascal_ChDir ((String *)&searchdir);
        if (Pascal_IOResult () != 0) { /*nothing*/ }
      }
      else
      {
        String_t s;

        s = Lower_filename (_fs.fstream.stuff[item_idx - 1].name);
        CopyString (fs_environment.last_file, (String *)&s, FILENAME_SIZE);
      }

    } while (   (mn_environment.keystroke != kENTER)
             && (mn_environment.keystroke != kESC)
             && (mn_environment.keystroke != kF1));

    mn_environment.descr_len = 0;
    mn_environment.descr = NULL;
    mn_environment.winshade = true;
    mn_setting.frame_enabled = true;
    mn_setting.shadow_enabled = true;
    mn_setting.homing_pos = 0;

    move_to_screen_data = ptr_scr_backup;
    move_to_screen_area[0] = mn_environment.xpos;
    move_to_screen_area[1] = mn_environment.ypos;
    move_to_screen_area[2] = mn_environment.xpos + mn_environment.xsize + 2 + 1;
    move_to_screen_area[3] = mn_environment.ypos + mn_environment.ysize + 1;
    move2screen ();

    if (mn_environment.keystroke == kF1)
    {
      HELP ((String *)&"\x0C" "file_browser");
      respawn_dialog = true;
    }
  } while (respawn_dialog);

  CopyString ((String *)&result, (String *)&searchdir, 255);
  AppendString ((String *)&result, _fs.fstream.stuff[item_idx - 1].name, 255);
  CopyString (fs_environment.last_dir, _path[UpCase (searchdir.str[0]) - 'A'], DIR_SIZE);
  Pascal_ChDir ((String *)&curdir);
  if (Pascal_IOResult () != 0) { /*nothing*/ }
  if (mn_environment.keystroke == kESC)
    result.len = 0;

  DBG_LEAVE (); //EXIT //Fselect
  return result;
}
