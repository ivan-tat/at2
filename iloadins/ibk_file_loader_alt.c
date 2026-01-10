// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// idx: 1..SBI_BANK_CAPACITY
// On success: returns `false'.
// On error: returns `true' and error description in `error'.
bool ibk_file_loader_alt (temp_instrument_t *dst, const String *fname, uint16_t idx, char **error)
{
  bool result = true; // `false' on success, `true' on error
  void *f = NULL; // FILE
  bool f_opened = false;
  int64_t fsize;
  int32_t size;
  char header[4];
  #pragma pack(push, 1);
  struct
  {
    tFM_INST_DATA idata;
    uint8_t dummy[5];
  } data_rec;
  #pragma pack(pop);
  char name_rec[8+1];

  DBG_ENTER ("ibk_file_loader_alt");

  //f = fopen (fname, "rb");
  if ((f = malloc (Pascal_FileRec_size)) == NULL) goto _err_malloc;
  Pascal_AssignFile (f, fname);
  ResetF (f);
  if (Pascal_IOResult () != 0) goto _err_fopen;
  f_opened = true;

  fsize = Pascal_FileSize (f);
  if (fsize < sizeof (header)) goto _err_format;

  BlockReadF (f, &header, sizeof (header), &size);
  if (size != sizeof (header)) goto _err_fread;

  if (memcmp (header, ibk_id, sizeof (header)) != 0) goto _err_format;

  SeekF (f, sizeof (header) + sizeof (data_rec) * (idx - 1));
  if (Pascal_IOResult () != 0) goto _err_fread;

  BlockReadF (f, &data_rec, sizeof (data_rec), &size);
  if (size != sizeof (data_rec)) goto _err_fread;

  SeekF (f, sizeof (header) + sizeof (data_rec) * SBI_BANK_CAPACITY + sizeof (name_rec) * (idx - 1) );
  if (Pascal_IOResult () != 0) goto _err_fread;

  BlockReadF (f, name_rec, sizeof (name_rec), &size);
  if (size != sizeof (name_rec)) goto _err_fread;

  dst->four_op = false;
  dst->use_macro = false;

  // instrument data
  memset (&dst->ins1.fm, 0, sizeof (dst->ins1.fm));
  import_sbi_instrument_alt (&dst->ins1.fm, &data_rec);

  // instrument name
  {
    String_t s, t;

    StrToString ((String *)&s, name_rec, sizeof (s) - 1);
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

  DBG_LEAVE (); //EXIT //ibk_file_loader_alt
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
}
