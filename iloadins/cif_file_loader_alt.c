// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void cif_file_loader_alt (const String *fname)
{
  void *f = NULL; // FILE
  bool f_opened = false;
  tCIF_DATA buffer;
  int32_t size;

  static const char id[20] = { "<CUD-FM-Instrument>\x1A" };

  #define MIN_CIF_SIZE (sizeof (buffer.ident) +                              \
                        sizeof (buffer.idata) +                              \
                        sizeof (buffer.resrv))

  DBG_ENTER ("cif_file_loader_alt");

  memset (&temp_instrument, 0, sizeof (temp_instrument));

  //f = fopen (fname, "rb");
  if ((f = malloc (Pascal_FileRec_size)) == NULL) goto _exit;
  Pascal_AssignFile (f, fname);
  ResetF (f);
  if (Pascal_IOResult () != 0) goto _exit;
  f_opened = true;

  BlockReadF (f, &buffer, sizeof (buffer), &size);
  if (   (size < MIN_CIF_SIZE)
      || (memcmp (buffer.ident, id, sizeof (buffer.ident)) != 0)) goto _exit;

  import_hsc_instrument_alt (&temp_instrument, &buffer.idata);
  temp_instrument.fine_tune = 0;

  load_flag_alt = 1;

_exit:
  //if (f) fclose (f);
  if (f)
  {
    if (f_opened) CloseF (f);
    free (f);
  }
  DBG_LEAVE (); //EXIT //cif_file_loader_alt
}
