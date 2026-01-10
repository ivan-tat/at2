// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// idx: 1..MAX_TIMBRES
// On success: returns `false'.
// On error: returns `true' and error description in `error'.
bool fib_file_loader_alt (temp_instrument_t *dst, const String *fname, uint16_t idx, char **error)
{
  bool result = true; // `false' on success, `true' on error
  void *f = NULL; // FILE
  bool f_opened = false;
  int32_t size;
  tFIB_HEADER header;
  tFIN_DATA data;
  char ident[4];

  DBG_ENTER ("fib_file_loader_alt");

  //f = fopen (fname, "rb");
  if ((f = malloc (Pascal_FileRec_size)) == NULL) goto _err_malloc;
  Pascal_AssignFile (f, fname);
  ResetF (f);
  if (Pascal_IOResult () != 0) goto _err_fopen;
  f_opened = true;

  BlockReadF (f, &header, sizeof (header), &size);
  if (size != sizeof (header)) goto _err_fread;

  if (memcmp (header.ident, fib_id, sizeof (header.ident)) != 0) goto _err_format;

  SeekF (f, sizeof (header) + sizeof (tFIN_DATA) * header.nmins);
  if (Pascal_IOResult () != 0) goto _err_fread;

  BlockReadF (f, &ident, sizeof (ident), &size);
  if (size != sizeof (ident)) goto _err_fread;

  if (memcmp (ident, fib_id, sizeof (ident)) != 0) goto _err_format;

  SeekF (f, sizeof (header) + sizeof (tFIN_DATA) * (idx - 1));
  if (Pascal_IOResult () != 0) goto _err_fread;

  BlockReadF (f, &data, sizeof (data), &size);
  if (size != sizeof (data)) goto _err_fread;

  dst->four_op = false;
  dst->use_macro = false;

  // instrument data
  import_standard_instrument_alt (&dst->ins1.fm, &data.idata);
  dst->ins1.fm.perc_voice = (data.perc_voice >= 6) && (data.perc_voice <= 10) ? data.perc_voice - 6 + 1 : 0;


  // instrument name
  {
    union
    {
      char dname[sizeof (data.dname)+1]; // DOS file name (small)
      char iname[sizeof (data.iname)+1]; // instrument name (large)
    } s;

    strncpy (s.iname, data.iname, sizeof (data.iname));
    if (s.iname[0] != '\0')
    {
      // use instrument name
      s.iname[sizeof (s.iname) - 1] = '\0'; // terminate string
    }
    else
    {
      // use DOS file name
      strncpy (s.dname, data.dname, sizeof (data.dname));
      s.dname[sizeof (s.dname) - 1] = '\0'; // terminate string
    }
    StrToString (dst->ins1.name, (char *)&s, sizeof (dst->ins1.name) - 1);
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

  DBG_LEAVE (); //EXIT //fib_file_loader_alt
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
