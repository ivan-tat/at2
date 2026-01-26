// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// In: `progress' and `error' may be NULL.
// On success: returns 0.
// On error: returns -1 and error description in `error' (if set).
int8_t a2f_file_loader_alt (temp_instrument_t *dst, const String *_fname, bool swap_ins,
                            progress_callback_t *progress, char **error)
{
  #pragma pack(push, 1)
  typedef struct
  {
    char ident[18];
    uint32_t crc32;
    uint8_t ffver;
    uint16_t len;
  } tHEADER;
  #pragma pack(pop)

  static const char GCC_ATTRIBUTE((nonstring)) id[18] = { "_a2ins_w/fm-macro_" };

  int8_t result = -1; // return value
  char *result_error = NULL;
  FILE *f = NULL;
  uint32_t crc;
  tHEADER header;
  size_t unpacked_size;
  mem_stream_t stream;
  char fname[255+1];

  DBG_ENTER ("a2f_file_loader_alt");

  StringToStr (fname, _fname, sizeof (fname) - 1);
  if ((f = fopen (fname, "rb")) == NULL) goto _err_fopen;

  if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;
  if (memcmp (header.ident, id, sizeof (header.ident)) != 0) goto _err_format;

  if ((header.ffver == 1) || (header.ffver == 2))
  {
    if (fread (buf2, header.len, 1, f) == 0) goto _err_fread;

    crc = Update32 (&header.len, 1, CRC32_INITVAL); // LSB only
    crc = Update32 (buf2, header.len, crc);
    if (crc != header.crc32) goto _err_checksum;

    if (header.ffver == 1)
      unpacked_size = APACK_decompress (buf2, buf3);
    else
    {
      if (progress != NULL) progress->num_steps = 0;
      unpacked_size = LZH_decompress (buf2, buf3, header.len, progress);
    }

    set_mem_stream (&stream, buf3, unpacked_size);

    dst->four_op = false;
    dst->use_macro = true;
    if (read_bytes (&dst->ins1.fm, sizeof (dst->ins1.fm), &stream)) goto _err_eod; // instrument data
    if (read_string (dst->ins1.name, sizeof (dst->ins1.name), &stream)) goto _err_eod; // instrument name
    if (read_bytes (&dst->ins1.macro, sizeof (dst->ins1.macro), &stream)) goto _err_eod; // FM-macro data
    if (read_bytes (&dst->ins1.dis_fmreg_col, sizeof (dst->ins1.dis_fmreg_col), &stream)) goto _err_eod; // disabled FM-macro columns

    if (header.ffver == 2)
      if (stream.curptr < stream.endptr) // more data present => 4op instrument
      {
        dst->four_op = true;
        if (swap_ins)
        {
          memcpy (&dst->ins2, &dst->ins1, sizeof (dst->ins2)); // copy to 4OP 2/2
          if (read_bytes (&dst->ins1.fm, sizeof (dst->ins1.fm), &stream)) goto _err_eod; // instrument data (4OP 1/2)
          if (read_string (dst->ins1.name, sizeof (dst->ins1.name), &stream)) goto _err_eod; // instrument name (4OP 1/2)
          if (read_bytes (&dst->ins1.macro, sizeof (dst->ins1.macro), &stream)) goto _err_eod; // FM-macro data (4OP 1/2)
          if (read_bytes (&dst->ins1.dis_fmreg_col, sizeof (dst->ins1.dis_fmreg_col), &stream)) goto _err_eod; // disabled FM-macro columns (4OP 1/2)
        }
        else
        {
          if (read_bytes (&dst->ins2.fm, sizeof (dst->ins2.fm), &stream)) goto _err_eod; // instrument data (4OP 2/2)
          if (read_string (dst->ins2.name, sizeof (dst->ins2.name), &stream)) goto _err_eod; // instrument name (4OP 2/2)
          if (read_bytes (&dst->ins2.macro, sizeof (dst->ins2.macro), &stream)) goto _err_eod; // FM-macro data (4OP 2/2)
          if (read_bytes (&dst->ins2.dis_fmreg_col, sizeof (dst->ins2.dis_fmreg_col), &stream)) goto _err_eod; // disabled FM-macro columns (4OP 2/2)
        }
      }
  }
  else
    goto _err_version;

  set_default_ins_name_if_needed (dst, _fname);

  result = 0;

_exit:
  if (f != NULL) fclose (f);
  if ((result < 0) && (error != NULL)) *error = result_error;

  DBG_LEAVE (); //EXIT //a2f_file_loader_alt
  return result;

_err_fopen:
  result_error = "Failed to open input file";
  goto _exit;

_err_fread:
  result_error = "Failed to read input file";
  goto _exit;

_err_format:
  result_error = "Unknown file format";
  goto _exit;

_err_version:
  result_error = "Unknown file format version";
  goto _exit;

_err_checksum:
  result_error = "Checksum mismatch - file corrupted";
  goto _exit;

_err_eod:
  result_error = "Unexpected end of unpacked data";
  goto _exit;
}
