// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#define N_BITS 12
#define F_BITS 4
#define THRESHOLD 2
#define N (1 << N_BITS)
#define F ((1 << F_BITS) + THRESHOLD)

typedef struct
{
  const uint8_t *input_ptr;
  uint16_t input_pos;
  uint16_t input_size;
  uint8_t *output_ptr;
  uint16_t output_size;
  uint8_t last_ch;
  uint8_t dict[0x10000]; // 64KiB
} LZSSDecoderState_t;

static bool LZSS_GetChar (LZSSDecoderState_t *ds)
{
  if (ds->input_pos < ds->input_size)
  {
    ds->last_ch = ds->input_ptr[ds->input_pos++];
    return false;
  }
  else
    return true;
}

static void LZSS_PutChar (LZSSDecoderState_t *ds)
{
  ds->output_ptr[ds->output_size++] = ds->last_ch;
}

static void LZSS_decode (LZSSDecoderState_t *ds)
{
  uint16_t d = 0;
  uint32_t j = N - F;

  ds->input_pos = 0;
  ds->output_size = 0;

  for (;;)
  {
    d >>= 1;
    if ((d & 0xFF00) == 0)
    {
      if (LZSS_GetChar (ds)) break;
      d = 0xFF00 + ds->last_ch;
    }
    if ((d & 1) != 0)
    {
      if (LZSS_GetChar (ds)) break;
      ds->dict[j++] = ds->last_ch;
      j &= N - 1;
      LZSS_PutChar (ds);
    }
    else
    {
      uint32_t i;
      uint8_t a, b;

      if (LZSS_GetChar (ds)) break;
      b = ds->last_ch;
      if (LZSS_GetChar (ds)) break;
      i = ((ds->last_ch << 4) & 0xFF00) + b;
      a = (ds->last_ch & 0x0F) + THRESHOLD + 1;
      do
      {
        i &= N - 1;
        ds->last_ch = ds->dict[i++];
        ds->dict[j++] = ds->last_ch;
        j &= N - 1;
        LZSS_PutChar (ds);
      } while (--a);
    }
  }
}

uint16_t LZSS_decompress (const void *source, void *dest, uint16_t size)
{
  uint_least16_t result = 0;
  LZSSDecoderState_t *ds = NULL;

  if ((ds = malloc (sizeof (LZSSDecoderState_t))) == NULL) goto _exit;

  ds->input_ptr = source;
  //ds->input_pos
  ds->input_size = size;
  ds->output_ptr = dest;
  ds->output_size = 0;
  //ds->last_ch
  memset (ds->dict, 0, sizeof (ds->dict));
  LZSS_decode (ds);
  result = ds->output_size;

_exit:
  if (ds != NULL) free (ds);
  return result;
}
