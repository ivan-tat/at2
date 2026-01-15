// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// On success: `return->count' is 0 (undefined)
// On error: returns NULL and error description in `error'.
static fib_t *load_fib (const String *_fname, char **error)
{
  bool status = true; // `false' on success, `true' on error
  fib_t *fib = NULL;
  FILE *f = NULL;
  tFIB_HEADER header;
  char ident[4];
  uint_least16_t capacity;
  size_t ins_size;
  char fname[255+1];

  DBG_ENTER ("load_fib");

  StringToStr (fname, _fname, sizeof (fname) - 1);
  if ((f = fopen (fname, "rb")) == NULL) goto _err_fopen;

  if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;
  if (memcmp (header.ident, fib_id, sizeof (header.ident)) != 0) goto _err_format;
  if (header.nmins == 0) goto _err_empty;

  if (fseek (f, sizeof (header) + sizeof (tFIN_DATA) * header.nmins, SEEK_SET) != 0) goto _err_fread;
  if (fread (&ident, sizeof (ident), 1, f) == 0) goto _err_fread;
  if (memcmp (ident, fib_id, sizeof (ident)) != 0) goto _err_format;
  if (fseek (f, sizeof (header), SEEK_SET) != 0) goto _err_fread;

  capacity = header.nmins <= MAX_TIMBRES ? header.nmins : MAX_TIMBRES;
  if ((fib = fib_new (capacity, error)) == NULL) goto _exit;

  ins_size = sizeof (tFIN_DATA) * capacity;
  if (fread (fib->instruments, ins_size, 1, f) == 0) goto _err_fread;

  // we do not update `fib->count' because it's expensive

  status = false;

_exit:
  if (f != NULL) fclose (f);
  if (status && (fib != NULL))
  {
    fib_free (fib);
    fib = NULL;
  }

  DBG_LEAVE (); //EXIT //load_fib
  return fib;

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
