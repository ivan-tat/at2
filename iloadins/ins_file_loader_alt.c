// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// On success: returns 0.
// On error: returns -1 and error description in `error'.
int8_t ins_file_loader_alt (temp_instrument_t *dst, const String *_fname, char **error)
{
  int8_t result = -1; // return value
  FILE *f = NULL;
  long fsize;
  tINS_DATA buffer;
  char fname[255+1];

  DBG_ENTER ("ins_file_loader_alt");

  StringToStr (fname, _fname, sizeof (fname) - 1);
  if ((f = fopen (fname, "rb")) == NULL) goto _err_fopen;

  if (fseek (f, 0, SEEK_END) != 0) goto _err_fread;
  if ((fsize = ftell (f)) < 0) goto _err_fread;
  if (fsize < (long)sizeof (buffer)) goto _err_format;
  if (fseek (f, 0, SEEK_SET) != 0) goto _err_fread;

  if (fread (&buffer, fsize, 1, f) == 0) goto _err_fread;

  dst->four_op = false;
  dst->use_macro = false;

  // instrument data
  memset (&dst->ins1.fm, 0, sizeof (dst->ins1.fm));
  switch (force_ins)
  {
    case 0:
      if (fsize == 12)
        import_standard_instrument_alt (&dst->ins1.fm, &buffer.idata);
      if (   (fsize == 12)
          && (   (buffer.idata.WAVEFORM_modulator > 3)
              || (buffer.idata.WAVEFORM_carrier > 3)
              || (buffer.idata.FEEDBACK_FM > 15)))
        import_hsc_instrument_alt (&dst->ins1.fm, &buffer.idata);
      else if (fsize > 12)
        import_sat_instrument_alt (&dst->ins1.fm, &buffer.idata);
      break;

    case 1: import_hsc_instrument_alt (&dst->ins1.fm, &buffer.idata); break;
    case 2: import_sat_instrument_alt (&dst->ins1.fm, &buffer.idata); break;
    case 3: import_standard_instrument_alt (&dst->ins1.fm, &buffer.idata); break;
    default: break;
  }

  // instrument name
  SetLength (dst->ins1.name, 0);
  set_default_ins_name_if_needed (dst, _fname);

  result = 0;

_exit:
  if (f != NULL) fclose (f);

  DBG_LEAVE (); //EXIT //ins_file_loader_alt
  return result;

_err_fopen:
  *error = "Failed to open input file";
  goto _exit;

_err_fread:
  *error = "Failed to read input file";
  goto _exit;

_err_format:
  *error = "Unknown file format";
  goto _exit;
}
