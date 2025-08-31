// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void fin_file_loader_alt (const String *fname)
{
  void *f = NULL; // FILE
  bool f_opened = false;
  tFIN_DATA buffer;
  int32_t size;

  DBG_ENTER ("fin_file_loader_alt");

  memset (&temp_instrument, 0, sizeof (temp_instrument));

  //f = fopen (fname, "rb");
  if ((f = malloc (Pascal_FileRec_size)) == NULL) goto _exit;
  Pascal_AssignFile (f, fname);
  ResetF (f);
  if (Pascal_IOResult () != 0) goto _exit;
  f_opened = true;

  BlockReadF (f, &buffer, sizeof (buffer), &size);
  if (size != sizeof (buffer)) goto _exit;

  import_fin_instrument_alt (&temp_instrument, &buffer.idata);

  load_flag_alt = 1;

_exit:
  //if (f) fclose (f);
  if (f)
  {
    if (f_opened) CloseF (f);
    free (f);
  }
  DBG_LEAVE (); //EXIT //fin_file_loader_alt
}
