// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*****************************************************************************

  AdLib Tracker II instrument with FM-register macro file saver
  Supported file format version: 2
  Filename extension: .a2f

*****************************************************************************/

// In:
//   * `idx': 0..INSTRUMENTS_MAX-1.
//   * `progress' and `error' may be NULL.
//
// On success:
//   * Return value: 0.
//   * `error' is untouched.
// On error:
//   * Return value: -1.
//   * `error' (if set) is set to error description.
int8_t save_instrument_a2f (const String *_fname,
                            tFIXED_SONGDATA *song, unsigned idx,
                            progress_callback_t *progress, char **error)
{
  int8_t result = -1;
  char *result_error = NULL;
  FILE *f = NULL;
#pragma pack(push, 1)
  struct
  {
    header_a2f_t    main;
    header_a2f_v1_t data;
  } header;
#pragma pack(pop)
  char fname[255+1];

  DBG_ENTER ("save_instrument_a2f");

  if (progress != NULL) progress->num_steps = 0;

  StringToStr (fname, _fname, sizeof (fname) - 1);
  if ((f = fopen (fname, "wb")) == NULL) goto _err_fopen;

  // save header

  memcpy (header.main.id, id_a2f, sizeof (header.main.id));
  header.main.crc32 = CRC32_INITVAL;  // we'll update it in the end
  header.main.version = 2;
  memset (&header.data, 0, sizeof (header.data)); // we'll update it in the end

  if (fwrite (&header, sizeof (header), 1, f) == 0) goto _err_fwrite;

  // save data block

  if (save_block_0_a2f_v2 (header.main.version, f,
                           &header.main.crc32, &header.data.block_size,
                           song, idx,
                           progress, &result_error) != 0) goto _exit;

  // update header

  header.main.crc32 = uint32_LE (header.main.crc32);

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

  DBG_LEAVE (); //EXIT //save_instrument_a2f
  return result;

_err_fopen:
  result_error = "Failed to open output file for writing";
  goto _exit;

_err_fwrite:
  result_error = "Failed to write output file";
  goto _exit;
}
