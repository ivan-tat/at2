// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void ibk_file_loader_alt (uint16_t instr)
{
  void *f = NULL; // FILE
  bool f_opened = false;
  char header[4];
  int32_t size;
  #pragma pack(push, 1);
  struct
  {
    tFM_INST_DATA idata;
    uint8_t dummy[5];
  } instrument_data;
  #pragma pack(pop);

  const static char id[4] = { "IBK\x1A" };

  DBG_ENTER ("ibk_file_loader_alt");

  //f = fopen (fname, "rb");
  if ((f = malloc (Pascal_FileRec_size)) == NULL) goto _exit;
  Pascal_AssignFile (f, instdata_source);
  ResetF (f);
  if (Pascal_IOResult () != 0) goto _exit;
  f_opened = true;

  BlockReadF (f, &header, sizeof (header), &size);
  if (   (size != sizeof (header))
      || (memcmp (header, id, sizeof (header)) != 0)) goto _exit;

  SeekF (f, 4 + (instr - 1) * sizeof (instrument_data));
  if (Pascal_IOResult () != 0) goto _exit;

  BlockReadF (f, &instrument_data, sizeof (instrument_data), &size);
  if (size != sizeof (instrument_data)) goto _exit;

  memset (&temp_instrument, 0, sizeof (temp_instrument));
  import_sbi_instrument_alt (&temp_instrument, &instrument_data);

  load_flag_alt = 1;

_exit:
  //if (f) fclose (f);
  if (f)
  {
    if (f_opened) CloseF (f);
    free (f);
  }
  DBG_LEAVE (); //EXIT //ibk_file_loader_alt
}
