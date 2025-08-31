// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void ins_file_loader_alt (const String *fname)
{
  void *f = NULL; // FILE
  bool f_opened = false;
  tINS_DATA buffer;
  int64_t fsize;
  int32_t size;

  bool correct_ins (const void *data)
  {
    bool result = true;

    DBG_ENTER ("ins_file_loader_alt.correct_ins");

    if (   (((tADTRACK2_INS *)data)->fm_data.WAVEFORM_modulator < 0)
        || (((tADTRACK2_INS *)data)->fm_data.WAVEFORM_modulator > 3))
      result = false;
    else if (   (((tADTRACK2_INS *)data)->fm_data.WAVEFORM_carrier < 0)
             || (((tADTRACK2_INS *)data)->fm_data.WAVEFORM_carrier > 3))
      result = false;
    else if (   (((tADTRACK2_INS *)data)->fm_data.FEEDBACK_FM < 0)
             || (((tADTRACK2_INS *)data)->fm_data.FEEDBACK_FM > 15))
      result = false;

    DBG_LEAVE (); //EXIT //ins_file_loader_alt.correct_ins
    return result;
  }

  DBG_ENTER ("ins_file_loader_alt");

  memset (&temp_instrument, 0, sizeof (temp_instrument));

  //f = fopen (fname, "rb");
  if ((f = malloc (Pascal_FileRec_size)) == NULL) goto _exit;
  Pascal_AssignFile (f, fname);
  ResetF (f);
  if (Pascal_IOResult () != 0) goto _exit;
  f_opened = true;

  fsize = Pascal_FileSize (f);
  if (fsize > sizeof (buffer)) goto _exit;

  BlockReadF (f, &buffer, fsize, &size);
  if (size != fsize) goto _exit;

  switch (force_ins)
  {
    case 0:
      if (size == 12)
        import_standard_instrument_alt (&temp_instrument, &buffer.idata);
      if ((size == 12) && !correct_ins (&buffer.idata))
        import_hsc_instrument_alt (&temp_instrument, &buffer.idata);
      else if (size > 12)
        import_sat_instrument_alt (&temp_instrument, &buffer.idata);
      break;

    case 1: import_hsc_instrument_alt (&temp_instrument, &buffer.idata); break;
    case 2: import_sat_instrument_alt (&temp_instrument, &buffer.idata); break;
    case 3: import_standard_instrument_alt (&temp_instrument, &buffer.idata); break;
    default: break;
  }

  load_flag_alt = 1;

_exit:
  //if (f) fclose (f);
  if (f)
  {
    if (f_opened) CloseF (f);
    free (f);
  }
  DBG_LEAVE (); //EXIT //ins_file_loader_alt
};
