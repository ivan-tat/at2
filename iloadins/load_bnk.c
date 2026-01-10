// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Returns NULL on error and error description in `error'.
static bnk_t *load_bnk (const String *fname, char **error)
{
  bool status = true; // `false' on success, `true' on error
  bnk_t *bnk = NULL;
  void *f = NULL; // FILE
  bool f_opened = false;
  int32_t size, names_size;
  tBNK_HEADER header;
  uint16_t capacity;

  DBG_ENTER ("load_bnk");

  //f = fopen (fname, "rb");
  if ((f = malloc (Pascal_FileRec_size)) == NULL) goto _err_malloc;
  Pascal_AssignFile (f, fname);
  ResetF (f);
  if (Pascal_IOResult () != 0) goto _err_fopen;
  f_opened = true;

  BlockReadF (f, &header, sizeof (header), &size);
  if (size != sizeof (header)) goto _err_fread;

  if (memcmp (header.signature, bnk_id, sizeof (header.signature)) != 0) goto _err_format;
  if ((header.fver_major != 1) || (header.fver_minor != 0)) goto _err_version;
  if (header.total_entries < header.entries_used) goto _err_header;
  if (header.total_entries == 0) goto _err_empty;

  capacity = header.total_entries <= MAX_TIMBRES ? header.total_entries : MAX_TIMBRES;

  if ((bnk = bnk_new (capacity, error)) == NULL) goto _exit;

  SeekF (f, header.name_offset);
  if (Pascal_IOResult () != 0) goto _err_fread;

  names_size = sizeof (tBNK_NAME_RECORD) * capacity;

  BlockReadF (f, bnk->names, names_size, &size);
  if (size != names_size) goto _err_fread;

  bnk->count = 0;
  for (uint16_t i = 0; i < capacity; i++)
  {
    SeekF (f, header.data_offset + sizeof (tBNK_DATA_RECORD) * bnk->names[i].data_index);
    if (Pascal_IOResult () != 0) goto _err_fread;

    BlockReadF (f, &bnk->data[i], sizeof (tBNK_DATA_RECORD), &size);
    if (size != sizeof (tBNK_DATA_RECORD)) goto _err_fread;

    if (bnk->names[i].usage_flag) bnk->count++;
  }

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
  if (status && (bnk != NULL))
  {
    bnk_free (bnk);
    bnk = NULL;
  }

  DBG_LEAVE (); //EXIT //load_bnk
  return bnk;

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

_err_version:
  *error = "Unknown file format version";
  goto _exit;

_err_header:
  *error = "Bad file header";
  goto _exit;

_err_empty:
  *error = "No instruments in file";
  goto _exit;
}
