// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// On success: returns `false'.
// On error: returns `true' and error description in `error'.
bool a2i_file_loader_alt (temp_instrument_t *dst, const String *_fname, bool swap_ins, char **error)
{
  #pragma pack(push, 1)
  typedef struct
  {
    char ident[7];
    uint16_t crc16;
    uint8_t ffver;
    uint8_t b0len; // or LSB of `len'
  } tOLD_HEADER;
  #pragma pack(pop)

  static const char GCC_ATTRIBUTE((nonstring)) id[7] = { "_A2ins_" };

  bool result = true; // `false' on success, `true' on error
  FILE *f = NULL;
  tOLD_HEADER header;
  uint_least16_t len;
  uint16_t crc;
  size_t unpacked_size;
  mem_stream_t stream;
  char fname[255+1];

  DBG_ENTER ("a2i_file_loader_alt");

  StringToStr (fname, _fname, sizeof (fname) - 1);
  if ((f = fopen (fname, "rb")) == NULL) goto _err_fopen;

  if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;
  if (memcmp (header.ident, id, sizeof (header.ident)) != 0) goto _err_format;

  if ((header.ffver >= 1) && (header.ffver <= 10))
  {
    if (header.ffver <= 8)
      len = header.b0len;
    else
    {
      uint8_t b;

      if (fread (&b, sizeof (b), 1, f) == 0) goto _err_fread;
      len = header.b0len + (b << 8);
    }

    if (fread (buf2, len, 1, f) == 0) goto _err_fread;

    crc = UINT16_NULL;
    crc = Update16 (&header.b0len, sizeof (header.b0len), crc); // LSB only
    crc = Update16 (buf2, len, crc);
    if (crc != header.crc16) goto _err_crc;

    switch (header.ffver)
    {
      case 1:
      case 5:
        unpacked_size = SIXPACK_decompress (buf2, buf3, len);
        break;

      case 2:
      case 6:
        unpacked_size = LZW_decompress (buf2, buf3);
        break;

      case 3:
      case 7:
        unpacked_size = LZSS_decompress (buf2, buf3, len);
        break;

      default:
      case 4:
      case 8:
        memcpy (buf3, buf2, len);
        unpacked_size = len;
        break;

      case 9:
        unpacked_size = APACK_decompress (buf2, buf3);
        break;

      case 10:
        progress_num_steps = 0;
        unpacked_size = LZH_decompress (buf2, buf3, len);
        break;
    }

    set_mem_stream (&stream, buf3, unpacked_size);

    dst->four_op = false;
    dst->use_macro = false;
    if (header.ffver <= 8)
    {
      // old instrument data
      if (read_bytes (&dst->ins1.fm.fm_data, sizeof (dst->ins1.fm.fm_data), &stream)) goto _err_eod;
      if (read_bytes (&dst->ins1.fm.panning, sizeof (dst->ins1.fm.panning), &stream)) goto _err_eod;

      if (header.ffver <= 4)
        dst->ins1.fm.panning = 0;

      if (read_bytes (&dst->ins1.fm.fine_tune, sizeof (dst->ins1.fm.fine_tune), &stream)) goto _err_eod;
      dst->ins1.fm.perc_voice = 0; // N/A
      if (read_string (dst->ins1.name, sizeof (dst->ins1.name), &stream)) goto _err_eod; // old instrument name
    }
    else
    {
      if (read_bytes (&dst->ins1.fm, sizeof (dst->ins1.fm), &stream)) goto _err_eod; // instrument data
      if (read_string (dst->ins1.name, sizeof (dst->ins1.name), &stream)) goto _err_eod; // instrument name

      if (header.ffver == 10)
        if (stream.curptr < stream.endptr) // more data present => 4op instrument
        {
          dst->four_op = true;
          if (swap_ins)
          {
            memcpy (&dst->ins2.fm, &dst->ins1.fm, sizeof (dst->ins2.fm)); // copy to 4OP 2/2
            memcpy (dst->ins2.name, dst->ins1.name, sizeof (dst->ins2.name)); // copy to 4OP 2/2
            if (read_bytes (&dst->ins1.fm, sizeof (dst->ins1.fm), &stream)) goto _err_eod; // instrument data (4OP 1/2)
            if (read_string (dst->ins1.name, sizeof (dst->ins1.name), &stream)) goto _err_eod; // instrument name (4OP 1/2)
          }
          else
          {
            if (read_bytes (&dst->ins2.fm, sizeof (dst->ins2.fm), &stream)) goto _err_eod; // instrument data (4OP 2/2)
            if (read_string (dst->ins2.name, sizeof (dst->ins2.name), &stream)) goto _err_eod; // instrument name (4OP 2/2)
          }
        }
    }
  }
  else
    goto _err_version;

  set_default_ins_name_if_needed (dst, _fname);

  result = false;

_exit:
  if (f != NULL) fclose (f);

  DBG_LEAVE (); //EXIT //a2i_file_loader_alt
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

_err_crc:
  *error = "CRC failed - file corrupted";
  goto _exit;

_err_eod:
  *error = "Unexpected end of unpacked data";
  goto _exit;
}
