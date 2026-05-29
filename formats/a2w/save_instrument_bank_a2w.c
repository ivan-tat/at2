// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*****************************************************************************

  Bank of AdLib Tracker II instrument with FM-register macro file saver
  Supported file format version: 3
  Filename extension: .a2w

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
int8_t save_instrument_bank_a2w (const String *_fname,
                                 tFIXED_SONGDATA *song,
                                 progress_callback_t *progress, char **error)
{
  int8_t result = -1;
  char *result_error = NULL;
  FILE *f = NULL;
#pragma pack(push, 1)
  struct
  {
    header_a2w_t    main;
    header_a2w_v2_t data;
  } header;
#pragma pack(pop)
  char fname[255+1];

  DBG_ENTER ("save_instrument_bank_a2w");

  StringToStr (fname, _fname, sizeof (fname) - 1);
  if ((f = fopen (fname, "wb")) == NULL) goto _err_fopen;

  if (progress != NULL)
  {
    snprintf (progress->msg, sizeof (progress->msg), "Compressing instrument bank data...");
    progress->num_steps = 3;
    progress->step = 1;
    progress->update (progress, -1, -1);
  }

  // save header

  memcpy (header.main.id, id_a2w, sizeof (header.main.id));
  header.main.crc32 = CRC32_INITVAL;  // we'll update it in the end
  header.main.version = 3;
  memset (&header.data.block_sizes, 0, sizeof (header.data.block_sizes)); // we'll update it in the end

  if (fwrite (&header, sizeof (header), 1, f) == 0) goto _err_fwrite;

  // save data blocks

  if (save_block_0_a2w_v3 (header.main.version, f,
                           &header.main.crc32, &header.data.block_sizes[0],
                           song,
                           progress, &result_error) != 0) goto _exit;
  if (progress != NULL) next_saver_step (progress);

  if (save_macros_a2w_v1 (header.main.version, f,
                          &header.main.crc32, &header.data.block_sizes[1],
                          song,
                          progress, &result_error) != 0) goto _exit;
  if (progress != NULL) next_saver_step (progress);

  if (save_dis_fmreg_col_a2w_v2 (header.main.version, f,
                                 &header.main.crc32, &header.data.block_sizes[2],
                                 song,
                                 progress, &result_error) != 0) goto _exit;
  if (progress != NULL) next_saver_step (progress);

  // update header

  _block_sizes_crc32 (&header.main.crc32,
                      32, 16, header.data.block_sizes, lengthof (header.data.block_sizes, [0]));

  header.main.crc32 = uint32_LE (header.main.crc32);

  if (fseek (f, 0, SEEK_SET) != 0) goto _err_fwrite;
  if (fwrite (&header, sizeof (header), 1, f) == 0) goto _err_fwrite;

#if !ADT2PLAY
  _draw_screen_without_delay = true;
#endif // !ADT2PLAY
  if (progress != NULL)
  {
    progress->num_steps = 1;
    progress->step = 1;
    progress->value = 1;
    progress->old_value = BYTE_NULL;
    progress->update (progress, 1, -1);
  }

  result = 0;

_exit:
  if (f != NULL)
  {
    fclose (f);
    if (result < 0) remove (fname);
  }
  if ((result < 0) && (error != NULL)) *error = result_error;

  DBG_LEAVE (); //EXIT //save_instrument_bank_a2w
  return result;

_err_fopen:
  result_error = "Failed to open output file for writing";
  goto _exit;

_err_fwrite:
  result_error = "Failed to write output file";
  goto _exit;
}
