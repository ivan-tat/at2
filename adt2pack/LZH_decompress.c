// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Decoder state
struct LZHDecoderState_t
{
  uint16_t DIC_SIZE;
  uint16_t l_tree[2*(NC-1)+1], r_tree[2*(NC-1)+1];
  uint16_t p_table[256];
  uint8_t  p_len[NT];
  uint16_t c_table[4096];
  uint8_t  c_len[NC];
  int16_t  dec_counter;
  uint16_t bit_buf, sbit_buf, bit_count, block_size;
  uint16_t dec_ptr;
  const uint8_t *input_buffer;
  uint8_t *output_buffer;
  uint32_t input_buffer_idx, output_buffer_idx;
  uint32_t size_unpacked, input_buffer_size;
};

static void WriteDataBlock (struct LZHDecoderState_t *ds, const void *ptr, uint16_t size)
{
  memmove (&ds->output_buffer[ds->output_buffer_idx], ptr, size);
  ds->output_buffer_idx += size;
#if !ADT2PLAY
  if (!really_no_status_refresh)
    show_progress2 (ds->output_buffer_idx, 3);
#endif // !ADT2PLAY
}

static void FillBitBuffer (struct LZHDecoderState_t *ds, int16_t bits)
{
  ds->bit_buf <<= bits;
  while (bits > ds->bit_count)
  {
    bits -= ds->bit_count;
    ds->bit_buf |= ds->sbit_buf << bits;
    if (ds->input_buffer_idx <= ds->input_buffer_size)
      ds->sbit_buf = ds->input_buffer[ds->input_buffer_idx++];
    else
      ds->sbit_buf = 0;
    ds->bit_count = 8;
  }
  ds->bit_count -= bits;
  ds->bit_buf |= ds->sbit_buf >> ds->bit_count;
}

static uint16_t GetBits (struct LZHDecoderState_t *ds, int16_t bits)
{
  uint16_t result = ds->bit_buf >> (16 - bits);
  FillBitBuffer (ds, bits);
  return result;
}

static void MakeTable (struct LZHDecoderState_t *ds,
                       int16_t n_char,
                       const uint8_t *bit_len,
                       int16_t bits,
                       int16_t *table)
{
  uint16_t count[16], weight[16], start[17]; // start index 1
  int16_t j_bits = 16 - bits;
  int16_t avail = n_char;

  memset (count, 0, sizeof (count));
  memset (weight, 0, sizeof (weight));
  memset (start, 0, sizeof (start));

  for (int16_t i = 0; i < n_char; i++)
    count[bit_len[i] - 1]++;
  for (int i = 0; i < 16; i++)
    start[i + 1] = start[i] + (count[i] << (15 - i));
  for (int16_t i = 0; i < bits; i++)
  {
    start[i] >>= j_bits;
    weight[i] = 1 << (bits - i - 1);
  }
  for (int16_t i = bits; i < 16; i++)
    weight[i] = 1 << (15 - i);
  if (start[bits] >> j_bits != 0)
  {
    int16_t i = start[bits] >> j_bits;
    int16_t j = 1 << bits;

    if (i != j)
      memset (&table[i], 0, (j - i) * sizeof (table[0]));
  }
  for (int16_t chr = 0; chr < n_char; chr++)
  {
    int16_t len = bit_len[chr];
    int16_t j, next_c;

    if (len == 0) continue;
    j = start[len - 1];
    next_c = j + weight[len - 1];
    if (len <= bits)
    {
      for (int16_t i = j; i < next_c; i++)
        table[i] = chr;
    }
    else
    {
      int16_t *ptr = &table[((uint16_t) j) >> j_bits];
      int16_t mask = 1 << (15 - bits);

      for (int16_t i = len - bits; i != 0; i--)
      {
        if (*ptr == 0)
        {
          ds->r_tree[avail] = 0;
          ds->l_tree[avail] = 0;
          *ptr = avail++;
        }
        ptr = (int16_t *) ((j & mask) != 0 ? &ds->r_tree[*ptr] : &ds->l_tree[*ptr]);
        j <<= 1;
      }
      *ptr = chr;
    }
    start[len - 1] = next_c;
  }
}

static void ReadPtrLen (struct LZHDecoderState_t *ds, int16_t n_char, int16_t n_bit, int16_t i_bit)
{
  int16_t bits = GetBits (ds, n_bit);

  if (bits == 0)
  {
    int16_t chr = GetBits (ds, n_bit);

    memset (ds->p_len, 0, sizeof (ds->p_len));
    memsetw (ds->p_table, chr, sizeof (ds->p_table) / sizeof (ds->p_table[0]));
  }
  else
  {
    int16_t i = 0;

    while (i < bits)
    {
      int16_t chr = ds->bit_buf >> (16 - 3);
      if (chr == 7)
        for (uint16_t mask = 1 << (16 - 4); (mask & ds->bit_buf) != 0; mask >>= 1)
          chr++;
      FillBitBuffer (ds, chr >= 7 ? chr - 3 : 3);
      ds->p_len[i++] = chr;
      if (i == i_bit)
        for (uint8_t j = GetBits (ds, 2); j; j--)
          ds->p_len[i++] = 0;
    }

    if (i < n_char)
      memset (&ds->p_len[i], 0, (n_char - i) * sizeof (ds->p_len[0]));

    MakeTable (ds, n_char, ds->p_len, 8, (int16_t *) ds->p_table);
  }
}

static void ReadCharLen (struct LZHDecoderState_t *ds)
{
  int16_t bits = GetBits (ds, CBIT);

  if (bits == 0)
  {
    int16_t chr = GetBits (ds, CBIT);

    memset (ds->c_len, 0, sizeof (ds->c_len));
    memsetw (ds->c_table, chr, sizeof (ds->c_table) / sizeof (ds->c_table[0]));
  }
  else
  {
    int16_t i = 0;

    while (i < bits)
    {
      int16_t chr = ds->p_table[ds->bit_buf >> (16 - 8)];
      for (uint16_t mask = 1 << (16 - 9); chr >= NT; mask >>= 1)
        chr = (ds->bit_buf & mask) != 0 ? ds->r_tree[chr] : ds->l_tree[chr];
      FillBitBuffer (ds, ds->p_len[chr]);
      if (chr <= 2)
      {
        if (chr == 1)
          chr = 2 + GetBits (ds, 4);
        else if (chr == 2)
          chr = 19 + GetBits (ds, CBIT);
        for (; chr >= 0; chr--)
          ds->c_len[i++] = 0;
      }
      else
        ds->c_len[i++] = chr - 2;
    }
    while (i < NC) ds->c_len[i++] = 0;
    MakeTable (ds, NC, ds->c_len, 12, (int16_t *) ds->c_table);
  }
}

static uint16_t DecodeChar (struct LZHDecoderState_t *ds)
{
  uint16_t chr;

  if (ds->block_size == 0)
  {
    ds->block_size = GetBits (ds, 16);
    ReadPtrLen (ds, NT, TBIT, 3);
    ReadCharLen (ds);
    ReadPtrLen (ds, NP, PBIT, -1);
  }
  ds->block_size--;

  chr = ds->c_table[ds->bit_buf >> (16 - 12)];
  for (uint16_t mask = 1 << (16 - 13); chr >= NC; mask >>= 1)
    chr = (ds->bit_buf & mask) != 0 ? ds->r_tree[chr] : ds->l_tree[chr];
  FillBitBuffer (ds, ds->c_len[chr]);
  return chr;
}

static uint16_t DecodePtr (struct LZHDecoderState_t *ds)
{
  uint16_t ptr = ds->p_table[ds->bit_buf >> (16 - 8)];
  for (uint16_t mask = 1 << (16 - 9); ptr >= NP; mask >>= 1)
    ptr = (ds->bit_buf & mask) != 0 ? ds->r_tree[ptr] : ds->l_tree[ptr];
  FillBitBuffer (ds, ds->p_len[ptr]);
  if (ptr != 0)
  {
    ptr--;
    ptr = (1 << ptr) + GetBits (ds, ptr);
  }
  return ptr;
}

static void DecodeBuffer (struct LZHDecoderState_t *ds, uint16_t count, uint8_t *buffer)
{
  uint16_t j = 0;

  for (ds->dec_counter--; ds->dec_counter >= 0; ds->dec_counter--)
  {
    buffer[j++] = buffer[ds->dec_ptr++];
    ds->dec_ptr &= ds->DIC_SIZE - 1;
    if (j == count) return; // DecodeBuffer
  }
  for (;;)
  {
    uint16_t i = DecodeChar (ds);
    if (i <= 255)
    {
      buffer[j++] = i;
      if (j == count) return; //DecodeBuffer
    }
    else
    {
      ds->dec_counter = i - (256 - THRESHOLD);
      ds->dec_ptr = (j - DecodePtr (ds) - 1) & (ds->DIC_SIZE - 1);
      for (ds->dec_counter--; ds->dec_counter >= 0; ds->dec_counter--)
      {
        buffer[j++] = buffer[ds->dec_ptr++];
        ds->dec_ptr &= ds->DIC_SIZE - 1;
        if (j == count) return; //DecodeBuffer
      }
    }
  }
}

uint32_t LZH_decompress (const void *source, void *dest, uint32_t size)
{
  uint32_t result = 0;
  struct LZHDecoderState_t *ds;
  uint8_t *ptr = NULL;
  bool ultra_compression_flag;

  if ((ds = malloc (sizeof (struct LZHDecoderState_t))) == NULL) goto _exit;
  ds->DIC_SIZE = DIC_SIZE_DEF;
  ds->input_buffer = source;
  ds->input_buffer_idx = 0;
  ultra_compression_flag = ds->input_buffer[ds->input_buffer_idx++] != 0;
  ds->DIC_SIZE = ultra_compression_flag ? DIC_SIZE_MAX : DIC_SIZE_DEF;
  ds->input_buffer_size = size;
  ds->output_buffer = dest;
  ds->output_buffer_idx = 0;
  memmove (&ds->size_unpacked, &ds->input_buffer[ds->input_buffer_idx], sizeof (ds->size_unpacked));
  ds->input_buffer_idx += sizeof (ds->size_unpacked);
  size = ds->size_unpacked;
#if !ADT2PLAY
  progress_old_value = UINT8_NULL;
  progress_value = size;
#endif // !ADT2PLAY
  if ((ptr = malloc (ds->DIC_SIZE)) == NULL) goto _exit;
  ds->bit_buf = 0;
  ds->sbit_buf = 0;
  ds->bit_count = 0;
  FillBitBuffer (ds, 16);
  ds->block_size = 0;
  ds->dec_counter = 0;

  while (size > 0)
  {
    uint32_t part = size > ds->DIC_SIZE ? ds->DIC_SIZE : size;
    DecodeBuffer (ds, part, ptr);
    WriteDataBlock (ds, ptr, part);
    size -= part;
  }

  result = ds->size_unpacked;

_exit:
  if (ds != NULL) free (ds);
  if (ptr != NULL) free (ptr);
  return result;
}
