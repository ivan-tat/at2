// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// In:
//   * `valid_crc16': original CRC-16 value from file.
//   * `data_first':
//     `false' - calculate CRC-16 value first for block sizes, then for data;
//     `true'  - calculate CRC-16 value first for data, then for block sizes.
//   * `width': bit-width of block sizes (8, 16 or 32).
//   * `width_crc': number of bits of block sizes to compute CRC-16 (8, 16 or 32).
//   * `block_sizes[capacity]': array of 8-bit, 16-bit or 32-bit block sizes.
//   * `capacity': total number of blocks.
//   * `count': number of blocks to check.
//   * `error' may be NULL.
//
// On success:
//   * Return value: 0.
//   * `error' is untouched.
// On error:
//   * Return value: -1.
//   * `error' (if set) is set to error description.
static int8_t check_crc16_a2i (uint32_t valid_crc16, bool data_first,
                               unsigned width, unsigned width_crc, const void *block_sizes, unsigned capacity, unsigned count,
                               FILE *f,
                               char **error)
{
  int8_t result = -1;
  char *result_error = NULL;
  void *data = NULL;
  union
  {
    const void *ptr;
    const uint8_t  *u8;
    const uint16_t *u16;
    const uint32_t *u32;
  } sizes = { .ptr = block_sizes };
  size_t limit;
  uint16_t crc16 = CRC16_INITVAL;

  if (   (   (width !=  8)
          && (width != 16)
          && (width != 32))
      || (   (width_crc !=  8)
          && (width_crc != 16)
          && (width_crc != 32))
      || (width < width_crc)) goto _err_args;

  switch (width)
  {
    default:
    case 8:  limit = get_max_uint8     (sizes.u8,  count); break;
    case 16: limit = get_max_uint16_LE (sizes.u16, count); break;
    case 32: limit = get_max_uint32_LE (sizes.u32, count); break;
  }
  if ((limit != 0) && ((data = malloc (limit)) == NULL)) goto _err_malloc;

  if (!data_first)
    _block_sizes_crc16 (&crc16, width, width_crc, block_sizes, capacity);

  for (unsigned i = 0; i < count; i++)
  {
    size_t size;

    switch (width)
    {
      default:
      case 8:  size =            sizes.u8 [i];  break;
      case 16: size = uint16_LE (sizes.u16[i]); break;
      case 32: size = uint16_LE (sizes.u32[i]); break;
    }

    if (size != 0)
    {
      if (fread (data, size, 1, f) == 0) goto _err_fread;

      crc16 = Update16 (data, size, crc16);
    }
  }

  if (data_first)
    _block_sizes_crc16 (&crc16, width, width_crc, block_sizes, capacity);

  if (crc16 != uint16_LE (valid_crc16)) goto _err_checksum;

  result = 0;

_exit:
  if (data != NULL) free (data);
  if ((result < 0) && (error != NULL)) *error = result_error;

  return result;

_err_args:
  result_error = "Invalid parameters";
  goto _exit;

_err_malloc:
  result_error = "Memory allocation failed";
  goto _exit;

_err_fread:
  result_error = "Failed to read input file";
  goto _exit;

_err_checksum:
  result_error = "Checksum mismatch - file corrupted";
  goto _exit;
}
