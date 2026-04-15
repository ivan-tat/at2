// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void done_adt2play (void)
{
#if GO32
  uint8_t cur_video_mode;
  uint8_t cur_video_page;

  // restore original video mode if needed
  VBIOS_get_video_mode (&cur_video_mode, &cur_video_page);
  if (orig_video_mode != cur_video_mode) VBIOS_set_video_mode (orig_video_mode);
  VBIOS_get_video_mode (&cur_video_mode, &cur_video_page);
  if (orig_video_page != cur_video_page) VBIOS_set_active_video_page (orig_video_page);
#endif // !GO32

  if (exiting && (exit_status != 0))
  {
    printf (
#if GO32
      "\xDC\xDC\xDC\xDC\xDC\xDC\xDC\xDC\xDC\xDC\xDC\xDC\xDC\xDC\xDC\xDC"
      "\xDC\xDC\xDC\xDC\xDC\xDC\xDC\xDC\xDC\xDC\xDC\xDC\xDC\xDC\xDC\xDC" NL
      "\xDB ABNORMAL PROGRAM TERMiNATiON \xDB" NL
      "\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF"
      "\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF\xDF" NL
#else // !GO32
      "▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄" NL
      "█ ABNORMAL PROGRAM TERMiNATiON █" NL
      "▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀" NL
#endif // !GO32
      "PROGRAM VERSION: " VERSION NL
    );
#if USE_FPC
    printf ("ERROR #%"PRIu16" at 0x%"PRIXPTR "(exit status %"PRIu16")" NL,
            *Pascal_ErrorCode_ptr, *Pascal_ErrorAddr_ptr, exit_status);
#else // !USE_FPC
    printf ("ERROR #%"PRIu16 NL, exit_status);
#endif // !USE_FPC
    _dbg_dump_callstack ();
    printf (
      "Please send this information with brief description what you were doing" NL
      "when you encountered this error to following email address:" NL
      "" NL
      "subz3ro.altair&gmail.com" NL
      "" NL
      "Thanks and sorry for your inconvenience! :-)" NL
    );
#if USE_FPC
    *Pascal_ErrorAddr_ptr = NULL;
#endif // USE_FPC
  }
}
