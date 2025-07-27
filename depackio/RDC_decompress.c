// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

uint16_t RDC_decompress (const void *source, void *dest, uint16_t size)
{
  const uint8_t *input = source;
  const uint8_t *input_end = (uint8_t *) source + size;
  uint8_t *output = dest;
  uint16_t ctrl_bits = 0, ctrl_mask = 0;

  while (input < input_end)
  {
    uint16_t command, count, offs;

    ctrl_mask >>= 1;
    if (ctrl_mask == 0)
    {
      ctrl_bits = *((uint16_t *) input);
      input += 2;
      ctrl_mask = 0x8000;
    }

    if ((ctrl_bits & ctrl_mask) == 0)
    {
      *output++ = *input++;
      continue;
    };

    command = (*input >> 4) & 0x0F;
    count = *input++ & 0x0F;

    switch (command)
    {
      case 0: // short RLE
        count += 3;
        memset (output, *input++, count);
        output += count;
        break;

      case 1: // long RLE
        count += (*input++ << 4) + 19;
        memset (output, *input++, count);
        output += count;
        break;

      case 2: // long pattern
        offs = count + 3 + (*input++ << 4);
        count = *input++ + 16;
        memmove (output, output - offs, count);
        output += count;
        break;

      default: // short pattern
        offs = count + 3 + (*input++ << 4);
        memmove (output, output - offs, command);
        output += command;
        break;
    }
  }

  return (uint16_t) (output - (uint8_t *) dest);
}
