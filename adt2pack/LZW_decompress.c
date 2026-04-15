// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#define LZW_MIN_BITS 9
#define LZW_MAX_BITS 13

typedef struct
{
  const uint8_t *input_ptr;
  uint8_t *output_ptr;
  uint16_t output_size;
  uint8_t  bits;
  uint32_t bit_pos;
  uint16_t stack[0x10000]; // 128KiB, [0] is never used and not even accessed
  uint8_t  dict[0x10000]; // 64KiB
} LZWDecoderState_t;

static const uint16_t lzw_masks[5] = { 0x01FF, 0x03FF, 0x07FF, 0x0FFF, 0x1FFF };

static uint16_t LZW_NextCode (LZWDecoderState_t *ds)
{
  uint8_t bit = ds->bit_pos & 7;
  uint32_t offs = ds->bit_pos >> 3;
  uint32_t code;

  ds->bit_pos += ds->bits;
  code =  ds->input_ptr[offs + 0]
       + (ds->input_ptr[offs + 1] << 8)
       + (ds->input_ptr[offs + 2] << 16);
  if (bit != 0) code >>= bit;

  return code & lzw_masks[ds->bits - LZW_MIN_BITS];
}

static void LZW_decode (LZWDecoderState_t *ds)
{
  uint16_t sp = 0; // stack pointer, growing up
  uint16_t index = (1 << LZW_MIN_BITS) / 2 + 2;
  uint16_t max = 1 << LZW_MIN_BITS;
  uint16_t code1 = 0, code2 = 0;
  uint8_t a = 0, b = 0;

  ds->output_size = 0;
  ds->bits = LZW_MIN_BITS;
  ds->bit_pos = 0;

  for (;;)
  {
    uint16_t code = LZW_NextCode (ds);
    uint16_t saved;

    if (code == 0x0101)
      return;

    if (code == 0x0100)
    {
      ds->bits = LZW_MIN_BITS;
      index = (1 << LZW_MIN_BITS) / 2 + 2;
      max = 1 << LZW_MIN_BITS;
      code = LZW_NextCode (ds);
      code1 = code;
      code2 = code;
      a = code;
      b = code;
      ds->output_ptr[ds->output_size++] = code;
      continue;
    }

    code1 = code;
    saved = code;
    if (code >= index)
    {
      code1 = code2;
      code = (code2 & 0xFF00) + a;
      ds->stack[++sp] = code;
    }

    while (code1 > 0x00FF)
    {
      uint16_t offs = code1 * 3;
      code = (code & 0xFF00) + ds->dict[offs + 2];
      ds->stack[++sp] = code;
      code1 = ds->dict[offs + 0] + (ds->dict[offs + 1] << 8);
    }

    a = code1;
    b = code1;
    ds->stack[++sp] = code1;

    for (; sp; sp--)
      ds->output_ptr[ds->output_size++] = ds->stack[sp];

    ds->dict[index * 3 + 0] = code2;
    ds->dict[index * 3 + 1] = code2 >> 8;
    ds->dict[index * 3 + 2] = b;
    index++;
    code2 = saved;
    if ((index >= max) && (ds->bits <= LZW_MAX_BITS))
    {
      ds->bits++;
      max <<= 1;
    }
  }
}

uint16_t LZW_decompress (const void *source, void *dest)
{
  uint_least16_t result = 0;
  LZWDecoderState_t *ds = NULL;

  if ((ds = malloc (sizeof (LZWDecoderState_t))) == NULL) goto _exit;

  ds->input_ptr = source;
  ds->output_ptr = dest;
  //ds->output_size
  //ds->bits
  //ds->bit_pos
  //ds->stack
  //ds->dict
  LZW_decode (ds);
  result = ds->output_size;

_exit:
  if (ds != NULL) free (ds);
  return result;
}
