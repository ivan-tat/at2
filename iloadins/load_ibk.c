// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Returns NULL on error and error description in `error'.
static ibk_t *load_ibk (const String *_fname, char **error)
{
  bool status = true; // `false' on success, `true' on error
  ibk_t *ibk = NULL;
  FILE *f = NULL;
  char header[4];
  char fname[255+1];

  DBG_ENTER ("load_ibk");

  StringToStr (fname, _fname, sizeof (fname) - 1);
  if ((f = fopen (fname, "rb")) == NULL) goto _err_fopen;

  if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;
  if (memcmp (header, ibk_id, sizeof (header)) != 0) goto _err_format;

  if ((ibk = ibk_new (error)) == NULL) goto _exit;

  if (fread (ibk, sizeof (*ibk), 1, f) == 0) goto _err_fread;

  status = false;

_exit:
  if (f != NULL) fclose (f);
  if (status && (ibk != NULL))
  {
    ibk_free (ibk);
    ibk = NULL;
  }

  DBG_LEAVE (); //EXIT //load_ibk
  return ibk;

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
