// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

int run_adt2play (int argc, char **argv)
{
  int files = argc - 1;

  VBIOS_get_video_mode (&orig_video_mode, &orig_video_page);

  atexit (done_adt2play);

  for (int i = 1; i <= argc; i++)
  {
    const char *arg = argv[i];

    if (arg[0] == '/')
    {
      if (strcmp (&arg[1], "gfx") == 0) gfx_mode = true;
      else if (strcmp (&arg[1], "jukebox") == 0) jukebox = true;
      else if (strcmp (&arg[1], "latency") == 0) opl3out = opl2out;
      else
      {
        print_error ("Unknown parameter `%s' (arg %d)", arg, i);
        return 1;
      }
      files--;
    }
  }

  if (files == 0)
  {
    String_t s;
    char t[255+1], u[255+1];

    _list_title ();

    s = BaseNameOnly (StrToString ((String *)&s, argv[0], sizeof (t) - 1));
    StringToStr (t, (String *)&s, sizeof (t) - 1);
    snprintf (u, sizeof (u), "Syntax: %s files|wildcards [files|wildcards{...}] [options]", t);
    CWriteLn (StrToString ((String *)&s, u, sizeof (s) - 1), 0x07, 0x00);
    CWriteLn (StrToString ((String *)&s, "", sizeof (s) - 1), 0x07, 0x00);
    CWriteLn (StrToString ((String *)&s, "Command-line options:", sizeof (s) - 1), 0x07, 0x00);
    CWriteLn (StrToString ((String *)&s, "  /jukebox    play modules w/ no repeat", sizeof (s) - 1), 0x07, 0x00);
    CWriteLn (StrToString ((String *)&s, "  /gfx        graphical interface", sizeof (s) - 1), 0x07, 0x00);
    CWriteLn (StrToString ((String *)&s, "  /latency    compatibility mode for OPL3 latency", sizeof (s) - 1), 0x07, 0x00);
    return 1; // TODO: return 0
  }

  opl3port = 0x388;
  if (!detect_OPL3 ())
  {
    print_error ("No AdLib detected." NL);
    return 1;
  }

  if (!iVGA ())
  {
    print_error ("No VGA detected.");
    return 1;
  }

#if GO32
  {
    int32_t paras;

    __dpmi_allocate_dos_memory (0xFFFF, &paras);
    if (paras < 120 * 1024 / 16)
    {
      print_error ("Insufficient DOS memory.");
      return 1;
    }
  }
#endif // GO32

  return 0;
}
