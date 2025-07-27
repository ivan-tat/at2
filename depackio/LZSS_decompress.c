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

// Decoder state
struct LZSSDState_t
{
  const uint8_t *input_ptr;
  uint16_t input_pos;
  uint16_t input_size;
  uint8_t *output_ptr;
  uint16_t output_size;
  uint8_t *dict_ptr;
  uint8_t last_ch;
};

static bool LZSS_GetChar (struct LZSSDState_t *ds)
{
  if (ds->input_pos < ds->input_size)
  {
    ds->last_ch = ds->input_ptr[ds->input_pos++];
    return false;
  }
  else
    return true;
}

static void LZSS_PutChar (struct LZSSDState_t *ds)
{
  ds->output_ptr[ds->output_size++] = ds->last_ch;
}

static void LZSS_decode (struct LZSSDState_t *ds)
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
      ds->dict_ptr[j++] = ds->last_ch;
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
        ds->last_ch = ds->dict_ptr[i++];
        ds->dict_ptr[j++] = ds->last_ch;
        j &= N - 1;
        LZSS_PutChar (ds);
      } while (--a);
    }
  }
}

static struct LZSSDState_t lzss_ds;

uint16_t LZSS_decompress (const void *source, void *dest, uint16_t size)
{
  memset (work_mem, 0, WORKMEM_SIZE);
  lzss_ds.input_ptr = source;
  lzss_ds.input_size = size;
  lzss_ds.output_ptr = dest;
  lzss_ds.dict_ptr = work_mem;
  LZSS_decode (&lzss_ds);
  return lzss_ds.output_size;
}
