// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// idx: 1..SBI_BANK_CAPACITY
// On success: returns 0.
// On error: returns -1 and error description in `error'.
int8_t ibk_file_loader_alt (temp_instrument_t *dst, const String *_fname, uint16_t idx, char **error)
{
  int8_t result = -1; // return value
  FILE *f = NULL;
  long fsize;
  char header[4];
  #pragma pack(push, 1)
  struct
  {
    tFM_INST_DATA idata;
    uint8_t dummy[5];
  } data_rec;
  #pragma pack(pop)
  char name_rec[8+1];
  char fname[255+1];

  DBG_ENTER ("ibk_file_loader_alt");

  StringToStr (fname, _fname, sizeof (fname) - 1);
  if ((f = fopen (fname, "rb")) == NULL) goto _err_fopen;

  if (fseek (f, 0, SEEK_END) != 0) goto _err_fread;
  if ((fsize = ftell (f)) < 0) goto _err_fread;
  if (fsize < (long)sizeof (header)) goto _err_format;
  if (fseek (f, 0, SEEK_SET) != 0) goto _err_fread;

  if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;
  if (memcmp (header, ibk_id, sizeof (header)) != 0) goto _err_format;

  if (fseek (f, sizeof (header) + sizeof (data_rec) * (idx - 1), SEEK_SET) != 0) goto _err_fread;
  if (fread (&data_rec, sizeof (data_rec), 1, f) == 0) goto _err_fread;

  if (fseek (f, sizeof (header) + sizeof (data_rec) * SBI_BANK_CAPACITY
                                + sizeof (name_rec) * (idx - 1), SEEK_SET) != 0) goto _err_fread;
  if (fread (name_rec, sizeof (name_rec), 1, f) == 0) goto _err_fread;

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
  set_default_ins_name_if_needed (dst, _fname);

  result = 0;

_exit:
  if (f != NULL) fclose (f);

  DBG_LEAVE (); //EXIT //ibk_file_loader_alt
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
