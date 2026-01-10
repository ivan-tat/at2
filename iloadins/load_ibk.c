// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Returns NULL on error and error description in `error'.
static ibk_t *load_ibk (const String *fname, char **error)
{
  bool status = true; // `false' on success, `true' on error
  ibk_t *ibk = NULL;
  void *f = NULL;
  bool f_opened = false;
  char header[4];
  int32_t size;

  DBG_ENTER ("load_ibk");

  //f = fopen (fname, "rb");
  if ((f = malloc (Pascal_FileRec_size)) == NULL) goto _err_malloc;
  Pascal_AssignFile (f, fname);
  ResetF (f);
  if (Pascal_IOResult () != 0) goto _err_fopen;
  f_opened = true;

  BlockReadF (f, &header, sizeof (header), &size);
  if (size != sizeof (header)) goto _err_fread;

  if (memcmp (header, ibk_id, sizeof (header)) != 0) goto _err_format;

  if ((ibk = ibk_new (error)) == NULL) goto _exit;

  BlockReadF (f, ibk, sizeof (*ibk), &size);
  if (size != sizeof (*ibk)) goto _err_fread;

  //fclose (f);
  CloseF (f);
  f_opened = false;
  free (f);
  f = NULL;

  status = false;

_exit:
  //if (f != NULL) fclose (f);
  if (f != NULL)
  {
    if (f_opened) CloseF (f);
    free (f);
  }
  if (status && (ibk != NULL))
  {
    ibk_free (ibk);
    ibk = NULL;
  }

  DBG_LEAVE (); //EXIT //load_ibk
  return ibk;

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
