// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*****************************************************************************

  BoomTracker 4.0 instrument file loader
  Supported file format versions: all
  Filename extension: .cif

*****************************************************************************/

// In:
//   * `progress' and `error' may be NULL.
//
// On success:
//   * Return value: 0.
//   * `error' is untouched.
// On error:
//   * Return value: -1.
//   * `error' (if set) is set to error description.
int8_t load_instrument_cif (temp_instrument_t *dst,
                            const String *_fname,
                            UNUSED progress_callback_t *progress, char **error)
{
  int8_t result = -1;
  FILE *f = NULL;
  long fsize;
  union
  {
    header_cif_t header;
    ins_cif_t ins;
  } buffer;
  char fname[255+1];

  DBG_ENTER ("load_instrument_cif");

  StringToStr (fname, _fname, sizeof (fname) - 1);
  if ((f = fopen (fname, "rb")) == NULL) goto _err_fopen;

  if (fseek (f, 0, SEEK_END) != 0) goto _err_fread;
  if ((fsize = ftell (f)) < 0) goto _err_fread;
  if (fsize < (long)(sizeof (buffer.header) + sizeof (buffer.ins))) goto _err_format;
  if (fseek (f, 0, SEEK_SET) != 0) goto _err_fread;

  /*** header ***/

  if (fread (&buffer.header, sizeof (buffer.header), 1, f) == 0) goto _err_fread;

  if (memcmp (buffer.header.id, id_cif, sizeof (buffer.header.id)) != 0) goto _err_format;

  /*** data ***/

  if (fread (&buffer.ins, sizeof (buffer.ins), 1, f) == 0) goto _err_fread;

  get_ins_cif (dst, &buffer.ins);

  set_default_ins_name_if_needed (dst, _fname);

  result = 0;

_exit:
  if (f != NULL) fclose (f);

  DBG_LEAVE (); //EXIT //load_instrument_cif
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
