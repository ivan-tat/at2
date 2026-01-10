// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// On success: returns `false'.
// On error: returns `true' and error description in `error'.
bool cif_file_loader_alt (temp_instrument_t *dst, const String *fname, char **error)
{
  bool result = true; // `false' on success, `true' on error
  void *f = NULL; // FILE
  bool f_opened = false;
  int64_t fsize;
  int32_t size;
  tCIF_DATA buffer;

  static const char id[20] = { "<CUD-FM-Instrument>\x1A" };

  #define MIN_CIF_SIZE (sizeof (buffer.ident) +                              \
                        sizeof (buffer.idata) +                              \
                        sizeof (buffer.resrv))

  DBG_ENTER ("cif_file_loader_alt");

  //f = fopen (fname, "rb");
  if ((f = malloc (Pascal_FileRec_size)) == NULL) goto _err_malloc;
  Pascal_AssignFile (f, fname);
  ResetF (f);
  if (Pascal_IOResult () != 0) goto _err_fopen;
  f_opened = true;

  fsize = Pascal_FileSize (f);
  if (fsize < MIN_CIF_SIZE) goto _err_format;

  BlockReadF (f, &buffer, sizeof (buffer), &size);
  if (size < MIN_CIF_SIZE) goto _err_fread;
  if (memcmp (buffer.ident, id, sizeof (buffer.ident)) != 0) goto _err_format;

  dst->four_op = false;
  dst->use_macro = false;

  // instrument data
  memset (&dst->ins1.fm, 0, sizeof (dst->ins1.fm));
  import_hsc_instrument_alt (&dst->ins1.fm, &buffer.idata);
  dst->ins1.fm.fine_tune = 0;

  // instrument name
  {
    String_t s, t;

    StrToString ((String *)&s, buffer.iname, sizeof (buffer.iname)/* - 1*/);
    t = truncate_string ((String *)&s);
    CopyString ((String *)dst->ins1.name, (String *)&t, sizeof (dst->ins1.name) - 1);
  }
  set_default_ins_name_if_needed (dst, fname);

  result = false;

_exit:
  //if (f != NULL) fclose (f);
  if (f != NULL)
  {
    if (f_opened) CloseF (f);
    free (f);
  }

  DBG_LEAVE (); //EXIT //cif_file_loader_alt
  return result;

_err_malloc:
  *error = "Memory allocation failed";
  goto _exit;

_err_fopen:
  *error = "Failed to open input file";
  goto _exit;

_err_fread:
  *error = "Failed to read input file";
  goto _exit;

_err_format:
  *error = "Unknown file format";
  goto _exit;

  #undef MIN_CIF_SIZE
}
