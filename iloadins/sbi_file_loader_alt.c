// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// On success: returns 0.
// On error: returns -1 and error description in `error'.
int8_t sbi_file_loader_alt (temp_instrument_t *dst, const String *_fname, char **error)
{
  int8_t result = -1; // return value
  FILE *f = NULL;
  long fsize;
  tSBI_DATA buffer;
  char fname[255+1];

  static const char GCC_ATTRIBUTE((nonstring)) id[4] = { "SBI\x1A" };

  DBG_ENTER ("sbi_file_loader_alt");

  StringToStr (fname, _fname, sizeof (fname) - 1);
  if ((f = fopen (fname, "rb")) == NULL) goto _err_fopen;

  if (fseek (f, 0, SEEK_END) != 0) goto _err_fread;
  if ((fsize = ftell (f)) < 0) goto _err_fread;
  if (fsize < (long)sizeof (buffer)) goto _err_format;
  if (fseek (f, 0, SEEK_SET) != 0) goto _err_fread;

  if (fread (&buffer, sizeof (buffer), 1, f) == 0) goto _err_fread;
  if (memcmp (buffer.ident, id, sizeof (buffer.ident)) != 0) goto _err_format;

  dst->four_op = false;
  dst->use_macro = false;

  // instrument data
  memset (&dst->ins1.fm, 0, sizeof (dst->ins1.fm));
  import_sbi_instrument_alt (&dst->ins1.fm, &buffer.idata);

  // instrument name
  {
    String_t s, t;

    s.len = sizeof (buffer.iname);
    memcpy (s.str, buffer.iname, sizeof (buffer.iname));
    t = truncate_string ((String *)&s);
    CopyString ((String *)&dst->ins1.name, (String *)&t, sizeof (dst->ins1.name) - 1);
  }
  set_default_ins_name_if_needed (dst, _fname);

  result = 0;

_exit:
  if (f != NULL) fclose (f);

  DBG_LEAVE (); //EXIT //sbi_file_loader_alt
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
