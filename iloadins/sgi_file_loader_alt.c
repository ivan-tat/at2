// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// On success: returns 0.
// On error: returns -1 and error description in `error'.
int8_t sgi_file_loader_alt (temp_instrument_t *dst, const String *_fname, char **error)
{
  int8_t result = -1; // return value
  FILE *f = NULL;
  long fsize;
  tSGI_DATA buffer;
  char fname[255+1];

  DBG_ENTER ("sgi_file_loader_alt");

  StringToStr (fname, _fname, sizeof (fname) - 1);
  if ((f = fopen (fname, "rb")) == NULL) goto _err_fopen;

  if (fseek (f, 0, SEEK_END) != 0) goto _err_fread;
  if ((fsize = ftell (f)) < 0) goto _err_fread;
  if (fsize < (long)sizeof (buffer)) goto _err_format;
  if (fseek (f, 0, SEEK_SET) != 0) goto _err_fread;

  if (fread (&buffer, sizeof (buffer), 1, f) == 0) goto _err_fread;

  dst->four_op = false;
  dst->use_macro = false;

  // instruemnt data
  memset (&dst->ins1.fm, 0, sizeof (dst->ins1.fm));
  import_sgi_instrument (&dst->ins1.fm, &buffer);

  // instrument name
  SetLength (dst->ins1.name, 0);
  set_default_ins_name_if_needed (dst, _fname);

  result = 0;

_exit:
  if (f != NULL) fclose (f);

  DBG_LEAVE (); //EXIT //sgi_file_loader_alt
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
