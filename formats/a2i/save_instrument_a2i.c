// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*****************************************************************************

  AdLib Tracker II instrument file saver
  Supported file format version: 10
  Filename extension: .a2i

*****************************************************************************/

// In:
//   * `progress' and `error' may be NULL.
//
// On success:
//   * Return value: 0.
//   * `error' is untouched.
// On error:
//   * Return value: -1.
//   * `error' (if set) is set to error description.
int8_t save_instrument_a2i (const String *_fname,
                            tFIXED_SONGDATA *song, unsigned idx,
                            progress_callback_t *progress, char **error)
{
  int8_t result = -1;
  char *result_error = NULL;
  FILE *f = NULL;
#pragma pack(push, 1)
  struct
  {
    header_a2i_t    main;
    header_a2i_v9_t data;
  } header;
#pragma pack(pop)
  char fname[255+1];

  DBG_ENTER ("save_instrument_a2i");

  if (progress != NULL) progress->num_steps = 0;

  StringToStr (fname, _fname, sizeof (fname) - 1);
  if ((f = fopen (fname, "wb")) == NULL) goto _err_fopen;

  // save header

  memcpy (header.main.id, id_a2i, sizeof (header.main.id));
  header.main.crc16 = CRC16_INITVAL;  // we'll update it in the end
  header.main.version = 10;
  memset (&header.data, 0, sizeof (header.data)); // we'll update it in the end

  if (fwrite (&header, sizeof (header), 1, f) == 0) goto _err_fwrite;

  // save data block

  if (save_block_0_a2i_v10 (header.main.version, f,
                            &header.main.crc16, &header.data.block_size,
                            song, idx,
                            progress, &result_error) != 0) goto _exit;

  // update header

  header.main.crc16 = uint16_LE (header.main.crc16);

  if (fseek (f, 0, SEEK_SET) != 0) goto _err_fwrite;
  if (fwrite (&header, sizeof (header), 1, f) == 0) goto _err_fwrite;

  result = 0;

_exit:
  if (f != NULL)
  {
    fclose (f);
    if (result < 0) remove (fname);
  }
  if ((result < 0) && (error != NULL)) *error = result_error;

  DBG_LEAVE (); //EXIT //save_instrument_a2i
  return result;

_err_fopen:
  result_error = "Failed to open output file for writing";
  goto _exit;

_err_fwrite:
  result_error = "Failed to write output file";
  goto _exit;
}
