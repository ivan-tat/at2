// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// idx: 1..MAX_TIMBRES
// On success: returns `false'.
// On error: returns `true' and error description in `error'.
bool bnk_file_loader_alt (temp_instrument_t *dst, const String *_fname, uint16_t idx, char **error)
{
  bool result = true; // `false' on success, `true' on error
  FILE *f = NULL;
  tBNK_HEADER header;
  tBNK_NAME_RECORD name_rec;
  tBNK_DATA_RECORD data_rec;
  char fname[255+1];

  DBG_ENTER ("bnk_file_loader_alt");

  StringToStr (fname, _fname, sizeof (fname) - 1);
  if ((f = fopen (fname, "rb")) == NULL) goto _err_fopen;

  if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;
  if (memcmp (header.signature, bnk_id, sizeof (header.signature)) != 0) goto _err_format;
  if ((header.fver_major != 1) || (header.fver_minor != 0)) goto _err_version;
  if (header.total_entries < header.entries_used) goto _err_header;

  if (idx > max (header.total_entries, MAX_TIMBRES)) goto _err_index;

  if (fseek (f, header.name_offset + sizeof (name_rec) * (idx - 1), SEEK_SET) != 0) goto _err_fread;
  if (fread (&name_rec, sizeof (name_rec), 1, f) == 0) goto _err_fread;

  if (fseek (f, header.data_offset + sizeof (data_rec) * name_rec.data_index, SEEK_SET) != 0) goto _err_fread;
  if (fread (&data_rec, sizeof (data_rec), 1, f) == 0) goto _err_fread;

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
  set_default_ins_name_if_needed (dst, _fname);

  result = false;

_exit:
  if (f != NULL) fclose (f);

  DBG_LEAVE (); //EXIT //bnk_file_loader_alt
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
