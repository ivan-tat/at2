// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// On success: returns `false'.
// On error: returns `true' and error description in `error'.
bool fin_file_loader_alt (temp_instrument_t *dst, const String *fname, char **error)
{
  bool result = true; // `false' on success, `true' on error
  void *f = NULL; // FILE
  bool f_opened = false;
  int64_t fsize;
  int32_t size;
  tFIN_DATA buffer;

  DBG_ENTER ("fin_file_loader_alt");

  //f = fopen (fname, "rb");
  if ((f = malloc (Pascal_FileRec_size)) == NULL) goto _err_malloc;
  Pascal_AssignFile (f, fname);
  ResetF (f);
  if (Pascal_IOResult () != 0) goto _err_fopen;
  f_opened = true;

  fsize = Pascal_FileSize (f);
  if (fsize < sizeof (buffer)) goto _err_format;

  BlockReadF (f, &buffer, sizeof (buffer), &size);
  if (size != sizeof (buffer)) goto _err_fread;

  dst->four_op = false;
  dst->use_macro = false;

  // instrument data
  memset (&dst->ins1.fm, 0, sizeof (dst->ins1.fm));
  import_fin_instrument_alt (&dst->ins1.fm, &buffer.idata);

  // instrument name
  {
    String_t s, t;

    StrToString ((String *)&s, buffer.iname, sizeof (buffer.iname)/* - 1*/);
    t = truncate_string ((String *)&s);
    if (t.len > 32)
    {
      StrToString ((String *)&t, buffer.dname, sizeof (buffer.dname)/* - 1*/);
      s = truncate_string ((String *)&t);
      t = Lower ((String *)&s);
    }
    s = Copy ((String *)&t, 1, 32);
    CopyString ((String *)&dst->ins1.name, (String *)&s, sizeof (dst->ins1.name) - 1);
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

  DBG_LEAVE (); //EXIT //fin_file_loader_alt
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
}
