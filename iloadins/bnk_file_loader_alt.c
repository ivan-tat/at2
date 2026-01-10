// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// idx: 1..MAX_TIMBRES
// On success: returns `false'.
// On error: returns `true' and error description in `error'.
bool bnk_file_loader_alt (temp_instrument_t *dst, const String *fname, uint16_t idx, char **error)
{
  bool result = true; // `false' on success, `true' on error
  void *f = NULL; // FILE
  bool f_opened = false;
  int32_t size;
  tBNK_HEADER header;
  tBNK_NAME_RECORD name_rec;
  tBNK_DATA_RECORD data_rec;

  DBG_ENTER ("bnk_file_loader_alt");

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

  if (idx > max (header.total_entries, MAX_TIMBRES)) goto _err_index;

  SeekF (f, header.name_offset + sizeof (name_rec) * (idx - 1));
  if (Pascal_IOResult () != 0) goto _err_fread;

  BlockReadF (f, &name_rec, sizeof (name_rec), &size);
  if (size != sizeof (name_rec)) goto _err_fread;

  SeekF (f, header.data_offset + sizeof (data_rec) * name_rec.data_index);
  if (Pascal_IOResult () != 0) goto _err_fread;

  BlockReadF (f, &data_rec, sizeof (data_rec), &size);
  if (size != sizeof (data_rec)) goto _err_fread;

  dst->four_op = false;
  dst->use_macro = false;

  // instrument data
  import_bnk_instrument (&dst->ins1.fm, &data_rec);

  // instrument name
  {
    String_t s, t;

    StrToString ((String *)&s, name_rec.ins_name, sizeof (s) - 1);
    t = asciiz_string ((String *)&s);
    s = CutStr ((String *)&t);
    CopyString (dst->ins1.name, (String *)&s, sizeof (dst->ins1.name) - 1);
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

  DBG_LEAVE (); //EXIT //bnk_file_loader_alt
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

_err_version:
  *error = "Unknown file format version";
  goto _exit;

_err_header:
  *error = "Bad file header";
  goto _exit;

_err_index:
  *error = "Bad index specified";
  goto _exit;
}
