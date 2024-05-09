// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

String_t InputStr (const String *s, uint8_t x, uint8_t y, uint8_t ln,
                   uint8_t ln1, uint8_t attr1, uint8_t attr2) {

  String_t r, s1, s2;
  bool appn, for1st, qflg, chflag, ins;
  uint8_t cloc, xloc, xint, attr;
  uint16_t key;

#if GO32
  DBG_ENTER ("InputStr");
#endif // GO32

  r = Copy (s, 1, ln);

  if (is_environment.locate_pos > ln1)
    is_environment.locate_pos = ln1;
  if (is_environment.locate_pos > r.len + 1)
    is_environment.locate_pos = r.len;

  cloc = is_environment.locate_pos;
  xloc = is_environment.locate_pos;
  xint = x;
  qflg = false;
  ins  = is_setting.insert_mode;
  appn = !is_setting.append_enabled;

  x--;

  if (ins) {
    if (use_large_cursor)
      WideCursor ();
    else
      ThinCursor ();
  } else {
    if (use_large_cursor)
      ThinCursor ();
    else
      WideCursor ();
  }

  CopyString ((String *) &s1, (String *) &r, 255);
  if (s1.len > ln1)
    s1.len = ln1;

  {
    String_t t;

    t = ExpStrR ((String *) "", ln1, ' ');
    ShowStr (screen_ptr, xint, y, (String *) &t, attr1);

    t = FilterStr2 ((String *) &s1, is_setting.char_filter, '_');
    ShowStr (screen_ptr, xint, y, (String *) &t, attr2);
  }

  for1st = true;

  do {
    CopyString ((String *) &s2, (String *) &s1, 255);
    s1 = Copy ((String *) &r, cloc - (xloc - 1), ln1);

    attr = appn ? attr1 : attr2;

    if (appn && for1st) {
      String_t t, t1;

      t = FilterStr2 ((String *) &s1, is_setting.char_filter, '_');
      t1 = ExpStrR ((String *) &t, ln1, ' ');
      ShowStr (screen_ptr, xint, y, (String *) &t1, attr1);
      for1st = false;
    }

    if ((s1.len != s2.len)
    ||  (strncmp ((char *) s1.str, (char *) s2.str,
                  (s1.len <= s2.len) ? s1.len : s2.len))) {
      String_t t, t1;

      t = FilterStr2 ((String *) &s1, is_setting.char_filter, '_');
      t1 = ExpStrR ((String *) &t, ln1, ' ');
      ShowStr (screen_ptr, xint, y, (String *) &t1, attr1);
    }

    if (ln1 < ln) { // FIXME: (lint) this condition is the same as next
      uint8_t t[2];
      uint8_t a;

      if (r.len && (cloc > xloc)) {
        t[0] = 1;
        t[1] = '\x11'; // HINT: (AT2) custom glyph
        a = (attr & 0xF0) + 0x0F;
      } else {
        if (r.len && (cloc == xloc)) {
          t[0] = 1;
          t[1] = r.str[0];
          a = attr;
        } else {
          t[0] = 1;
          t[1] = ' ';
          a = attr1;
        }
      }
      ShowStr (screen_ptr, xint, y, (String *) &t, a);
    }

    if (ln1 < ln) { // FIXME: (lint) this condition is the same as previous
      String_t t;
      uint8_t a;

      if (cloc - xloc + ln1 < r.len) {
        t.len = 1;
        t.str[0] = '\x10'; // HINT: (AT2) custom glyph
        a = (attr & 0xF0) + 0x0F;
      } else {
        if (cloc - xloc + ln1 == r.len) {
          uint8_t t1[2];

          t1[0] = 1;
          t1[1] = r.str[r.len - 1];
          t = FilterStr2 ((String *) &t1, is_setting.char_filter, '_');
          a = attr;
        } else {
          t.len = 1;
          t.str[0] = ' ';
          a = attr1;
        }
      }
      ShowStr (screen_ptr, xint + ln1 - 1, y, (String *) &t, a);
    }

    GotoXY (x + xloc, y);

    if (keypressed ()) {
      key = getkey ();

      qflg = qflg || LookUpKey (key, is_setting.terminate_keys,
                                sizeof (is_setting.terminate_keys)
                                / sizeof (is_setting.terminate_keys[0]));

      chflag = false;

      if (!qflg) {
        switch (key) {
        case kTAB:
          appn = true;
          break;

        case kCHplus:
        case kNPplus:
          chflag = true;
          {
            uint_least8_t base;

            if (memcmp (is_setting.character_set,
                        DEC_NUM_CHARSET, sizeof (CharSet_t)) == 0)
              base = 10;
            else if (memcmp (is_setting.character_set,
                             HEX_NUM_CHARSET, sizeof (CharSet_t)) == 0)
              base = 16;
            else
              base = 0;

            if (base) {
              int64_t v = Str2num ((String *) &r, base) + 1;

              if (v <= is_environment.max_num) {
                String_t t = Num2str (v, base);

                if (t.len <= ln1) {
                  CopyString ((String *) &r, (String *) &t, 255);
                  chflag = false;
                }
              }
            }
          }
          break;

        case kCHmins:
        case kNPmins:
          {
            uint_least8_t base;

            if (memcmp (is_setting.character_set,
                        DEC_NUM_CHARSET, sizeof (CharSet_t)) == 0)
              base = 10;
            else if (memcmp (is_setting.character_set,
                             HEX_NUM_CHARSET, sizeof (CharSet_t)) == 0)
              base = 16;
            else
              base = 0;

            if (base) {
              int64_t v = Str2num ((String *) &r, base) - 1;

              if ((v >= 0) && (v >= is_environment.min_num)) {
                String_t t = Num2str (v, base);

                if (t.len <= ln1) {
                  CopyString ((String *) &r, (String *) &t, 255);
                  chflag = false;
                }
              }
            }
          }
          break;

        case kCtrlY:
          appn = true;
          r.len = 0;
          cloc = 1;
          xloc = 1;
          break;

        case kCtrlT:
          appn = true;

          while ((cloc <= r.len)
          &&     bit_test (is_setting.word_characters, r.str[cloc - 1]))
            Delete ((String *) &r, cloc, 1);

          while ((cloc <= r.len)
          &&     !bit_test (is_setting.word_characters, r.str[cloc - 1]))
            Delete ((String *) &r, cloc, 1);
          break;

        case kCtrlK:
          appn = true;
          Delete ((String *) &r, cloc, r.len);
          break;

        case kCtBkSp:
          appn = true;

          while ((cloc > 1)
          &&     bit_test (is_setting.word_characters, r.str[cloc - 1 - 1])) {
            cloc--;
            Delete ((String *) &r, cloc, 1);
            if (xloc > 1)
              xloc--;
          }

          while ((cloc > 1)
          &&     !bit_test (is_setting.word_characters, r.str[cloc - 1])) {
            cloc--;
            Delete ((String *) &r, cloc, 1);
            if (xloc > 1)
              xloc--;
          }
          break;

        case kBkSPC:
          appn = true;
          if (cloc > 1) {
            cloc--;
            Delete ((String *) &r, cloc, 1);
            if (xloc > 1)
              xloc--;
          }
          break;

        case kDELETE:
          appn = true;
          if (cloc <= r.len)
            Delete ((String *) &r, cloc, 1);
          break;

        case kCtLEFT:
          appn = true;

          while ((cloc > 1)
          &&     bit_test (is_setting.word_characters, r.str[cloc - 1])) {
            cloc--;
            if (xloc > 1)
              xloc--;
          }

          while ((cloc > 1)
          &&     !bit_test (is_setting.word_characters, r.str[cloc - 1])) {
            cloc--;
            if (xloc > 1)
              xloc--;
          }
          break;

        case kCtRGHT:
          appn = true;

          while ((cloc < r.len)
          &&     bit_test (is_setting.word_characters, r.str[cloc - 1])) {
            cloc++;
            if (xloc < ln1)
              xloc++;
          }

          while ((cloc < r.len)
          &&     !bit_test (is_setting.word_characters, r.str[cloc - 1])) {
            cloc++;
            if (xloc < ln1)
              xloc++;
          }
          break;

        case kLEFT:
          appn = true;
          if (cloc > 1)
            cloc--;
          if (xloc > 1)
            xloc--;
          break;

        case kRIGHT:
          appn = true;
          if ((cloc < r.len)
          ||  ((cloc == r.len) && ((r.len < ln) || (r.len < ln1))))
            cloc++;
          if ((xloc <= r.len) && (xloc < ln1))
            xloc++;
          break;

        case kINSERT:
          if (is_setting.replace_enabled) {
            ins = !ins;
            if (ins) {
              if (use_large_cursor)
                WideCursor ();
              else
                ThinCursor ();
            } else {
              if (use_large_cursor)
                ThinCursor ();
              else
                WideCursor ();
            }
          }
          break;

        case kHOME:
          appn = true;
          cloc = 1;
          xloc = 1;
          break;

        case kEND:
          appn = true;
          cloc = r.len + (((r.len < ln) || (r.len < ln1)) ? 1 : 0);
          xloc = (cloc < ln1) ? cloc : ln1;
          break;

        default:
          chflag = true;
          break;
        }
      }

      if (chflag && bit_test (is_setting.character_set, key & 0xFF)) {
        String_t t;

        if (!appn) {
          r.len = 0;
          cloc = 1;
          xloc = 1;
        }
        appn = true;

        t = CutStrR ((String *) &r, cloc);
        if (ins && (t.len < ln)) {
          if (t.len < ln) // FIXME: (lint) this condition is always true
            InsertChr (key & 0xFF, (String *) &r, 255, cloc);
          else
            r.str[cloc - 1] = key & 0xFF;

          t = FilterStr2 ((String *) &r, is_setting.valid_chars, '_');
          CopyString ((String *) &r, (String *) &t, 255);
          if (cloc < ln)
            cloc++;
          if ((xloc < ln) && (xloc < ln1))
            xloc++;
        } else if ((r.len < ln) || !ins) {
          if ((cloc > r.len) && (r.len < ln))
            r.len++;
          r.str[cloc - 1] = key & 0xFF;
          t = FilterStr2 ((String *) &r, is_setting.valid_chars, '_');
          CopyString ((String *) &r, (String *) &t, 255);
          if (cloc < ln)
            cloc++;
          if ((xloc < ln) && (xloc < ln1))
            xloc++;
        }
      }
    }

    CopyString (is_environment.cur_str, (String *) &r, 255);
    if (is_environment.ext_proc)
      is_environment.ext_proc ();
#if GO32
    //draw_screen ();
    keyboard_reset_buffer_alt ();
#else // !GO32
    draw_screen ();
    //keyboard_reset_buffer ();
#endif // !GO32
  } while (!qflg);

  is_environment.locate_pos = cloc ? cloc : 1;
  is_environment.keystroke = key;
  is_environment.insert_mode = ins;

  return r;
}
