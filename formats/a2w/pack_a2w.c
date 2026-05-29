// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// In:
//   * `version': 3.
//   * `progress' and `error' may be NULL.
//
// On success:
//   * Return value: 0.
//   * `packed_size' is set to actual packed data size.
//   * `error' is untouched.
// On error:
//   * Return value: -1.
//   * `packed_size' is untouched.
//   * `error' (if set) is set to error description.
static int8_t pack_a2w (uint8_t version,
                        void *dst, size_t dst_limit, size_t *packed_size,
                        const void *src, size_t src_size,
                        progress_callback_t *progress, char **error)
{
  int8_t status = -1;
  char *result_error = NULL;
  size_t actual_size;

  switch (version)
  {
    case 3:
      actual_size = LZH_compress (src, dst, src_size, progress);
      if (actual_size > dst_limit) goto _err_fatal; // FIXME: missing checks in `LZH_compress()'
      status = 0;
      break;

    default: goto _err_version;
  }

_exit:
  if (status == 0) *packed_size = actual_size;
  else if (error != NULL) *error = result_error;

  return status;

_err_version:
  result_error = "Unsupported file format version";
  goto _exit;

_err_fatal:
  result_error = "Output buffer overrun: heap may be corrupted!";
  goto _exit;
}
