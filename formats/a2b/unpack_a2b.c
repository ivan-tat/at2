// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// In:
//   * `version': 1..10.
//   * `unpacked_size', `progress' and `error' may be NULL.
//
// On success:
//   * Return value: 0.
//   * `unpacked_size' (if set) is set to actual unpacked data size.
//   * `error' is untouched.
// On error:
//   * Return value: -1.
//   * `unpacked_size' is untouched.
//   * `error' (if set) is set to error description.
static int8_t unpack_a2b (uint8_t version,
                          void *dst, size_t dst_limit, size_t *unpacked_size,
                          const void *src, size_t src_size,
                          progress_callback_t *progress, char **error)
{
  int8_t status = -1;
  char *result_error = NULL;
  size_t orig_size, actual_size;

  switch (version)
  {
    case 1:
    case 5:
      actual_size = SIXPACK_decompress (src, dst, src_size);
      if (actual_size > dst_limit) goto _err_fatal; // FIXME: missing checks in `SIXPACK_decompress()'
      status = 0;
      break;

    case 2:
    case 6:
      actual_size = LZW_decompress (src, dst);
      if (actual_size > dst_limit) goto _err_fatal; // FIXME: missing checks in `LZW_decompress()'
      status = 0;
      break;

    case 3:
    case 7:
      actual_size = LZSS_decompress (src, dst, src_size);
      if (actual_size > dst_limit) goto _err_fatal; // FIXME: missing checks in `LZSS_decompress()'
      status = 0;
      break;

    case 4:
    case 8:
      if (src_size > dst_limit) goto _err_overrun;
      memcpy (dst, src, src_size);
      actual_size = src_size;
      status = 0;
      break;

    case 9:
      actual_size = APACK_decompress (src, dst);
      if (actual_size > dst_limit) goto _err_fatal; // FIXME: missing checks in `APACK_decompress()'
      status = 0;
      break;

    case 10:
      orig_size = ((LZH_block_info_t *)src)->size;
      if (orig_size > dst_limit) goto _err_overrun;
      actual_size = LZH_decompress (src, dst, src_size, progress);
      if (actual_size > dst_limit) goto _err_fatal; // FIXME: missing checks in `LZH_decompress()'
      if (actual_size != orig_size) goto _err_unpack;
      status = 0;
      break;

    default: goto _err_version;
  }

_exit:
  if (status == 0)
  {
    if (unpacked_size != NULL) *unpacked_size = actual_size;
  }
  else if (error != NULL) *error = result_error;

  return status;

_err_version:
  result_error = "Unsupported file format version";
  goto _exit;

_err_unpack:
  result_error = "Failed to unpack input data";
  goto _exit;

_err_overrun:
  result_error = "Output buffer overrun";
  goto _exit;

_err_fatal:
  result_error = "Output buffer overrun: heap may be corrupted!";
  goto _exit;
}
