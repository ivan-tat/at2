// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void a2f_file_loader_alt (const String *fname)
{
  #pragma pack(push, 1)
  typedef struct
  {
    char ident[18];
    int32_t crc32;
    uint8_t ffver;
    uint16_t b0len;
  } tHEADER;
  #pragma pack(pop)

  static const char id[18] = { "_a2ins_w/fm-macro_" };

  void *f = NULL; // FILE
  bool f_opened = false;
  tHEADER header;
  int32_t crc, size;

  DBG_ENTER ("a2f_file_loader_alt");

  memset (&temp_instrument, 0, sizeof (temp_instrument));
  memset (&temp_instrument_macro, 0, sizeof (temp_instrument_macro));
  memset (&temp_instrument_dis_fmreg_col, 0, sizeof (temp_instrument_dis_fmreg_col));

  //f = fopen (fname, "rb");
  if ((f = malloc (Pascal_FileRec_size)) == NULL) goto _exit;
  Pascal_AssignFile (f, fname);
  ResetF (f);
  if (Pascal_IOResult () != 0) goto _exit;
  f_opened = true;

  BlockReadF (f, &header, sizeof (header), &size);
  if (   (size != sizeof (header))
      || (memcmp (header.ident, id, sizeof (header.ident)) != 0)) goto _exit;

  if ((header.ffver < 1) || (header.ffver > FFVER_A2F)) goto _exit;

  if (header.ffver == 1)
  {
    int32_t pos;

    BlockReadF (f, buf2, header.b0len, &size);
    if (size != header.b0len) goto _exit;

    crc = UINT32_NULL;
    crc = Update32 ((uint8_t *)&header.b0len, 1, crc);
    crc = Update32 (buf2, header.b0len, crc);
    if (crc != header.crc32) goto _exit;

    APACK_decompress (buf2, buf3);

    pos = 0;
    memmove (&temp_instrument, buf3, sizeof (temp_instrument));
    pos += sizeof (songdata.instr_data[0]); // instrument data
    pos += buf3[sizeof (songdata.instr_data[0])] + 1; // instrument name
    memmove (&temp_instrument_macro, &buf3[pos], sizeof (temp_instrument_macro));
    pos += sizeof (songdata.instr_macros[0]); // instrument macro
    memmove (&temp_instrument_dis_fmreg_col, &buf3[pos], sizeof (temp_instrument_dis_fmreg_col));
  }

  if (header.ffver == FFVER_A2F)
  {
    int32_t unpacked_size, pos;

    BlockReadF (f, buf2, header.b0len, &size);
    if (size != header.b0len) goto _exit;

    crc = UINT32_NULL;
    crc = Update32 ((uint8_t *)&header.b0len, 1, crc);
    crc = Update32 (buf2, header.b0len, crc);
    if (crc != header.crc32) goto _exit;

    progress_num_steps = 0;
    unpacked_size = LZH_decompress (buf2, buf3, header.b0len);

    pos = 0;
    memmove (&temp_instrument, &buf3[pos], sizeof (temp_instrument));
    pos += sizeof (temp_instrument); // instrument data
    pos += buf3[pos] + 1; // instrument name
    memmove (&temp_instrument_macro, &buf3[pos], sizeof (temp_instrument_macro));
    pos += sizeof (temp_instrument_macro); // instrument macro
    memmove (&temp_instrument_dis_fmreg_col, &buf3[pos], sizeof (temp_instrument_dis_fmreg_col));
    pos += sizeof (temp_instrument_dis_fmreg_col); // disabled FM-macro columns
    if (pos < unpacked_size) // more data present => 4op instrument
    {
      memmove (&temp_instrument2, &temp_instrument, sizeof (temp_instrument2));
      memmove (&temp_instrument_macro2, &temp_instrument_macro, sizeof (temp_instrument_macro2));
      memmove (&temp_instrument_dis_fmreg_col2, &temp_instrument_dis_fmreg_col, sizeof (temp_instrument_dis_fmreg_col2));
      memmove (&temp_instrument, &buf3[pos], sizeof (temp_instrument));
      pos += sizeof (temp_instrument); // instrument data
      pos += buf3[pos] + 1; // instrument name
      memmove (&temp_instrument_macro, &buf3[pos], sizeof (temp_instrument_macro));
      pos += sizeof (temp_instrument_macro); // instrument macro
      memmove (&temp_instrument_dis_fmreg_col, &buf3[pos], sizeof (temp_instrument_dis_fmreg_col));
    }
  }

  load_flag_alt = 1;

_exit:
  //if (f) fclose (f);
  if (f)
  {
    if (f_opened) CloseF (f);
    free (f);
  }
  DBG_LEAVE (); //EXIT //a2f_file_loader_alt
}
