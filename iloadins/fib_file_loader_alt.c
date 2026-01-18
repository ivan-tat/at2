// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// idx: 1..MAX_TIMBRES
// On success: returns 0.
// On error: returns -1 and error description in `error'.
int8_t fib_file_loader_alt (temp_instrument_t *dst, const String *_fname, uint16_t idx, char **error)
{
  int8_t result = -1; // return value
  FILE *f = NULL;
  tFIB_HEADER header;
  tFIN_DATA data;
  char ident[4];
  char fname[255+1];

  DBG_ENTER ("fib_file_loader_alt");

  StringToStr (fname, _fname, sizeof (fname) - 1);
  if ((f = fopen (fname, "rb")) == NULL) goto _err_fopen;

  if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;
  if (memcmp (header.ident, fib_id, sizeof (header.ident)) != 0) goto _err_format;

  if (fseek (f, sizeof (header) + sizeof (tFIN_DATA) * header.nmins, SEEK_SET) != 0) goto _err_fread;
  if (fread (&ident, sizeof (ident), 1, f) == 0) goto _err_fread;
  if (memcmp (ident, fib_id, sizeof (ident)) != 0) goto _err_format;

  if (fseek (f, sizeof (header) + sizeof (tFIN_DATA) * (idx - 1), SEEK_SET) != 0) goto _err_fread;
  if (fread (&data, sizeof (data), 1, f) == 0) goto _err_fread;

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
  set_default_ins_name_if_needed (dst, _fname);

  result = 0;

_exit:
  if (f != NULL) fclose (f);

  DBG_LEAVE (); //EXIT //fib_file_loader_alt
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
