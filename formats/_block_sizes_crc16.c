// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// In:
//   * `crc16': current CRC-16 value.
//   * `width': bit-width of block sizes (8, 16 or 32).
//   * `width_crc': number of bits of block sizes to compute CRC-16 (8, 16 or 32).
//   * `block_sizes[count]': array of 8-bit, 16-bit or 32-bit block sizes.
//   * `count': number of block sizes to process.
static void _block_sizes_crc16 (uint16_t *crc16,
                                unsigned width, unsigned width_crc, const void *block_sizes, unsigned count)
{
  union
  {
    const void *ptr;
    const uint8_t *u8;
    const uint16_t *u16;
    const uint32_t *u32;
  } sizes = { .ptr = block_sizes };

  switch (width)
  {
    default:
    case 8:
      switch (width_crc)
      {
        default:
        case 8:
          *crc16 = Update16 (sizes.u8, sizeof (sizes.u8[0]) * count, *crc16);
          break;
      }
      break;

    case 16:
      switch (width_crc)
      {
        default:
        case 8:
          for (unsigned i = 0; i < count; i++)
            *crc16 = Update16 (&sizes.u16[i], sizeof (sizes.u8[0]), *crc16);
          break;
        case 16:
          *crc16 = Update16 (sizes.u16, sizeof (sizes.u16[0]) * count, *crc16);
          break;
      }
      break;

    case 32:
      switch (width_crc)
      {
        default:
        case 8:
          for (unsigned i = 0; i < count; i++)
            *crc16 = Update16 (&sizes.u32[i], sizeof (sizes.u8[0]), *crc16);
          break;
        case 16:
          for (unsigned i = 0; i < count; i++)
            *crc16 = Update16 (&sizes.u32[i], sizeof (sizes.u16[0]), *crc16);
          break;
        case 32:
          *crc16 = Update16 (sizes.u32, sizeof (sizes.u32[0]) * count, *crc16);
          break;
      }
      break;
  }
}
