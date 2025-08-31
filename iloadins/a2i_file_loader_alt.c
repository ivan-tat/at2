// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void a2i_file_loader_alt (const String *fname)
{
  #pragma pack(push, 1)
  typedef struct
  {
    char ident[7];
    uint16_t crc16;
    uint8_t ffver;
    uint8_t b0len;
  } tOLD_HEADER;
  typedef struct
  {
    char ident[7];
    uint16_t crc16;
    uint8_t ffver;
    uint16_t b0len;
  } tHEADER;
  #pragma pack(pop)

  static const char id[7] = { "_A2ins_" };

  void *f = NULL; // FILE
  bool f_opened = false;
  tOLD_HEADER header;
  tHEADER header2;
  int32_t size;
  uint16_t crc;

  DBG_ENTER ("a2i_file_loader_alt");

  memset (&temp_instrument, 0, sizeof (temp_instrument));

  //f = fopen (fname, "rb");
  if ((f = malloc (Pascal_FileRec_size)) == NULL) goto _exit;
  Pascal_AssignFile (f, fname);
  ResetF (f);
  if (Pascal_IOResult () != 0) goto _exit;
  f_opened = true;

  BlockReadF (f, &header, sizeof (header), &size);
  if (   (size != sizeof (header))
      || (memcmp (header.ident, id, sizeof (header.ident)) != 0)) goto _exit;

  if ((header.ffver < 1) && (header.ffver > FFVER_A2I)) goto _exit;

  if ((header.ffver >= 1) && (header.ffver <= 4))
  {
    BlockReadF (f, buf2, header.b0len, &size);
    if (size != header.b0len) goto _exit;

    crc = UINT16_NULL;
    crc = Update16 (&header.b0len, 1, crc);
    crc = Update16 (buf2, header.b0len, crc);
    if (crc != header.crc16) goto _exit;

    switch (header.ffver)
    {
      case 4:
        memmove (buf3, buf2, header.b0len);
        break;
      case 3:
        LZSS_decompress (buf2, buf3, header.b0len);
        break;
      case 2:
        LZW_decompress (buf2, buf3);
        break;
      case 1:
        SIXPACK_decompress (buf2, buf3, header.b0len);
        break;
      default: // never reached
        break;
    }

    memmove (&temp_instrument, buf3, sizeof (temp_instrument));
    temp_instrument.panning = 0;
  }

  if ((header.ffver >= 5) && (header.ffver <= 8))
  {
    BlockReadF (f, buf2, header.b0len, &size);
    if (size != header.b0len) goto _exit;

    crc = UINT16_NULL;
    crc = Update16 (&header.b0len, 1, crc);
    crc = Update16 (buf2, header.b0len, crc);
    if (crc != header.crc16) goto _exit;

    switch (header.ffver)
    {
      case 8:
        memmove (buf3, buf2, header.b0len);
        break;
      case 7:
        LZSS_decompress (buf2, buf3, header.b0len);
        break;
      case 6:
        LZW_decompress (buf2, buf3);
        break;
      case 5:
        SIXPACK_decompress (buf2, buf3, header.b0len);
        break;
      default: // never reached
        break;
    }

    memmove (&temp_instrument, buf3, sizeof (temp_instrument));
  }

  if (header.ffver == 9)
  {
    ResetF (f);
    BlockReadF (f, &header2, sizeof (header2), &size);
    if (   (size != sizeof (header2))
        || (memcmp (header2.ident, id, sizeof (header2.ident)) != 0)) goto _exit;

    BlockReadF (f, buf2, header2.b0len, &size);
    if (size != header2.b0len) goto _exit;

    crc = UINT16_NULL;
    crc = Update16 ((uint8_t *)&header2.b0len, 1, crc);
    crc = Update16 (buf2, header2.b0len, crc);
    if (crc != header2.crc16) goto _exit;

    APACK_decompress (buf2, buf3);

    memmove (&temp_instrument, buf3, sizeof (temp_instrument));
  }

  if (header.ffver == FFVER_A2I)
  {
    int32_t unpacked_size, pos;

    ResetF (f);
    BlockReadF (f, &header2, sizeof (header2), &size);
    if (   (size != sizeof (header2))
        || (memcmp (header2.ident, id, sizeof (header2.ident)) != 0)) goto _exit;

    BlockReadF (f, buf2, header2.b0len, &size);
    if (size != header2.b0len) goto _exit;

    crc = UINT16_NULL;
    crc = Update16 ((uint8_t *)&header2.b0len, 1, crc);
    crc = Update16 (buf2, header2.b0len, crc);

    if (crc != header2.crc16) goto _exit;

    progress_num_steps = 0;
    unpacked_size = LZH_decompress (buf2, buf3, header2.b0len);

    pos = 0;
    memmove (&temp_instrument, &buf3[pos], sizeof (temp_instrument));
    pos += sizeof (temp_instrument); // instrument data
    pos += buf3[pos] + 1; // instrument name
    if (pos < unpacked_size) // more data present => 4op instrument
    {
      memmove (&temp_instrument2, &temp_instrument, sizeof (temp_instrument2));
      memmove (&temp_instrument, &buf3[pos], sizeof (temp_instrument));
      pos += sizeof (temp_instrument); // instrument data
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
  DBG_LEAVE (); //EXIT //a2i_file_loader_alt
}
