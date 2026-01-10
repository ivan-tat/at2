// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// On success: `return->count' is 0 (undefined)
// On error: returns NULL and error description in `error'.
static fib_t *load_fib (const String *fname, char **error)
{
  bool status = true; // `false' on success, `true' on error
  fib_t *fib = NULL;
  void *f = NULL; // FILE
  bool f_opened = false;
  int32_t size, ins_size;
  tFIB_HEADER header;
  char ident[4];
  uint_least16_t capacity;

  DBG_ENTER ("load_fib");

  //f = fopen (fname, "rb");
  if ((f = malloc (Pascal_FileRec_size)) == NULL) goto _err_malloc;
  Pascal_AssignFile (f, fname);
  ResetF (f);
  if (Pascal_IOResult () != 0) goto _err_fopen;
  f_opened = true;

  BlockReadF (f, &header, sizeof (header), &size);
  if (size != sizeof (header)) goto _err_fread;

  if (memcmp (header.ident, fib_id, sizeof (header.ident)) != 0) goto _err_format;
  if (header.nmins == 0) goto _err_empty;

  SeekF (f, sizeof (header) + sizeof (tFIN_DATA) * header.nmins);
  if (Pascal_IOResult () != 0) goto _err_fread;

  BlockReadF (f, &ident, sizeof (ident), &size);
  if (size != sizeof (ident)) goto _err_fread;

  if (memcmp (ident, fib_id, sizeof (ident)) != 0) goto _err_format;

  SeekF (f, sizeof (header));
  if (Pascal_IOResult () != 0) goto _err_fread;

  capacity = header.nmins <= MAX_TIMBRES ? header.nmins : MAX_TIMBRES;

  if ((fib = fib_new (capacity, error)) == NULL) goto _exit;

  ins_size = sizeof (tFIN_DATA) * capacity;
  BlockReadF (f, fib->instruments, ins_size, &size);
  if (size != ins_size) goto _err_fread;

  // we do not update `fib->count' because it's expensive

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
  if (status && (fib != NULL))
  {
    fib_free (fib);
    fib = NULL;
  }

  DBG_LEAVE (); //EXIT //load_fib
  return fib;

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

_err_empty:
  *error = "No instruments in file";
  goto _exit;
}
