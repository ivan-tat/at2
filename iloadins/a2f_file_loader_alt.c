// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*****************************************************************************

  AdLib Tracker II instrument with FM-register macro file loader
  Supported versions: 1..2
  Filename extension: .a2f

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
int8_t a2f_file_loader_alt (temp_instrument_t *dst,
                            const String *_fname, bool swap_ins,
                            progress_callback_t *progress, char **error)
{
  int8_t result = -1;
  char *result_error = NULL;
  FILE *f = NULL;
  header_a2f_t main_header;
  header_a2f_v1_t header;
  char fname[255+1];

  DBG_ENTER ("a2f_file_loader_alt");

  StringToStr (fname, _fname, sizeof (fname) - 1);
  if ((f = fopen (fname, "rb")) == NULL) goto _err_fopen;

  if (fread (&main_header, sizeof (main_header), 1, f) == 0) goto _err_fread;

  if (memcmp (main_header.id, id_a2f, sizeof (main_header.id)) != 0) goto _err_format;
  if ((main_header.version < 1) || (main_header.version > 2)) goto _err_version;

  if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;

  if (check_crc32_a2f (main_header.crc32, false,
                       16, 8, &header.block_size, 1, 1,
                       f,
                       &result_error) != 0) goto _exit;

  if (fseek (f, sizeof (main_header) + sizeof (header), SEEK_SET) != 0) goto _err_fread;

  if (progress != NULL) progress->num_steps = 0;

  if (main_header.version == 1)
  {
    if (load_block_0_a2f_v1 (main_header.version, dst,
                             uint16_LE (header.block_size),
                             f,
                             progress, &result_error) != 0) goto _exit;
  }
  else  // main_header.version == 2
    if (load_block_0_a2f_v2 (main_header.version, dst,
                             uint16_LE (header.block_size),
                             f, swap_ins,
                             progress, &result_error) != 0) goto _exit;

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

}
