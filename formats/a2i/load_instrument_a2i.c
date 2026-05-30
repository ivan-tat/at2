// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*****************************************************************************

  AdLib Tracker II instrument file loader
  Supported file format versions: 1..10
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
int8_t load_instrument_a2i (temp_instrument_t *dst,
                            const String *_fname, bool swap_ins,
                            progress_callback_t *progress, char **error)
{
  int8_t result = -1;
  char *result_error = NULL;
  FILE *f = NULL;
  header_a2i_t main_header;
  char fname[255+1];

  DBG_ENTER ("load_instrument_a2i");

  StringToStr (fname, _fname, sizeof (fname) - 1);
  if ((f = fopen (fname, "rb")) == NULL) goto _err_fopen;

  if (fread (&main_header, sizeof (main_header), 1, f) == 0) goto _err_fread;

  if (memcmp (main_header.id, id_a2i, sizeof (main_header.id)) != 0) goto _err_format;
  if ((main_header.version < 1) || (main_header.version > 10)) goto _err_version;

  if (main_header.version <= 8)
  {
    header_a2i_v1_t header;

    if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;

    if (check_crc16_a2i (main_header.crc16, false,
                         8, 8, &header.block_size, 1, 1,
                         f,
                         &result_error) != 0) goto _exit;

    if (fseek (f, sizeof (main_header) + sizeof (header), SEEK_SET) != 0) goto _err_fread;

    if (main_header.version <= 4)
    {
      if (load_block_0_a2i_v1 (main_header.version, dst,
                               header.block_size,
                               f,
                               progress, &result_error) != 0) goto _exit;
    }
    else  // main_header.version == 5..8
      if (load_block_0_a2i_v5 (main_header.version, dst,
                               header.block_size,
                               f,
                               progress, &result_error) != 0) goto _exit;
  }
  else // main_header.version == 9..10
  {
    header_a2i_v9_t header;

    if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;

    if (check_crc16_a2i (main_header.crc16, false,
                         16, 8, &header.block_size, 1, 1,
                         f,
                         &result_error) != 0) goto _exit;

    if (fseek (f, sizeof (main_header) + sizeof (header), SEEK_SET) != 0) goto _err_fread;

    if (main_header.version == 9)
    {
      if (load_block_0_a2i_v9 (main_header.version, dst,
                               uint16_LE (header.block_size),
                               f,
                               progress, &result_error) != 0) goto _exit;
    }
    else  // main_header.version == 10
      if (load_block_0_a2i_v10 (main_header.version, dst,
                                uint16_LE (header.block_size),
                                f, swap_ins,
                                progress, &result_error) != 0) goto _exit;
  }

  set_default_ins_name_if_needed (dst, _fname);

  result = 0;

_exit:
  if (f != NULL) fclose (f);
  if ((result < 0) && (error != NULL)) *error = result_error;

  DBG_LEAVE (); //EXIT //load_instrument_a2i
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
