// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Returns NULL on error and error description in `error'.
static bnk_t *load_bnk (const String *_fname, char **error)
{
  bool status = true; // `false' on success, `true' on error
  bnk_t *bnk = NULL;
  FILE *f = NULL;
  size_t names_size;
  tBNK_HEADER header;
  uint16_t capacity;
  char fname[255+1];

  DBG_ENTER ("load_bnk");

  StringToStr (fname, _fname, sizeof (fname) - 1);
  if ((f = fopen (fname, "rb")) == NULL) goto _err_fopen;

  if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;
  if (memcmp (header.signature, bnk_id, sizeof (header.signature)) != 0) goto _err_format;
  if ((header.fver_major != 1) || (header.fver_minor != 0)) goto _err_version;
  if (header.total_entries < header.entries_used) goto _err_header;
  if (header.total_entries == 0) goto _err_empty;

  capacity = header.total_entries <= MAX_TIMBRES ? header.total_entries : MAX_TIMBRES;
  if ((bnk = bnk_new (capacity, error)) == NULL) goto _exit;

  if (fseek (f, header.name_offset, SEEK_SET) != 0) goto _err_fread;
  names_size = sizeof (tBNK_NAME_RECORD) * capacity;
  if (fread (bnk->names, names_size, 1, f) == 0) goto _err_fread;

  bnk->count = 0;
  for (uint16_t i = 0; i < capacity; i++)
  {
    if (fseek (f, header.data_offset + sizeof (tBNK_DATA_RECORD) * bnk->names[i].data_index, SEEK_SET) != 0) goto _err_fread;
    if (fread (&bnk->data[i], sizeof (tBNK_DATA_RECORD), 1, f) == 0) goto _err_fread;

    if (bnk->names[i].usage_flag) bnk->count++;
  }

  status = false;

_exit:
  if (f != NULL) fclose (f);
  if (status && (bnk != NULL))
  {
    bnk_free (bnk);
    bnk = NULL;
  }

  DBG_LEAVE (); //EXIT //load_bnk
  return bnk;

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
