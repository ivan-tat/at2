// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#define MAXFREQ       2000
#define MINCOPY       3
#define MAXCOPY       255
#define COPYRANGES    6
#define TERMINATE     256
#define FIRSTCODE     257
#define ROOT          1
#define CODESPERRANGE (MAXCOPY - MINCOPY + 1)
#define MAXCHAR       (FIRSTCODE + COPYRANGES * CODESPERRANGE - 1)
#define SUCCMAX       (MAXCHAR + 1)
#define TWICEMAX      (2 * MAXCHAR + 1)
#define MAXBUF        (WORKMEM_SIZE - 1) // 64KiB
#define MAXDISTANCE   21389
#define MAXSIZE       (MAXDISTANCE + MAXCOPY)

// Decoder state
struct SixpackDState_t
{
  const uint8_t *input_ptr;
  //uint16_t input_size; // unused
  uint8_t *output_ptr;
  uint16_t output_size;
  uint8_t *dict_ptr;
  uint16_t leftC[MAXCHAR + 1], rghtC[MAXCHAR + 1];
  uint16_t dad[TWICEMAX + 1], frq[TWICEMAX + 1];
  uint16_t ibitCount, ibitBuffer, ibufCount, obufCount;
};

static const uint16_t BitValue[14] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192 };
static const uint16_t CopyBits[COPYRANGES] = { 4, 6, 8, 10, 12, 14 };
static const uint16_t CopyMin[COPYRANGES] = { 0, 16, 80, 336, 1360, 5456 };

static void Sixpack_InitTree (struct SixpackDState_t *ds)
{
  for (uint16_t i = 2; i <= TWICEMAX; i++)
  {
    ds->dad[i] = i / 2;
    ds->frq[i] = 1;
  }

  for (uint16_t i = 1; i <= MAXCHAR; i++)
  {
    ds->leftC[i] = 2 * i;
    ds->rghtC[i] = 2 * i + 1;
  }
}

static void Sixpack_UpdateFreq (struct SixpackDState_t *ds, uint16_t a, uint16_t b)
{
  do
  {
    uint16_t code = ds->dad[a];

    ds->frq[code] = ds->frq[a] + ds->frq[b];
    a = code;
    code = ds->dad[a];
    if (a != ROOT) b = ds->leftC[code] == a ? ds->rghtC[code] : ds->leftC[code];
  } while (a != ROOT);

  if (ds->frq[ROOT] == MAXFREQ)
    for (uint16_t i = 1; i <= TWICEMAX; i++) ds->frq[i] >>= 1;
}

static void Sixpack_UpdateModel (struct SixpackDState_t *ds, uint16_t code)
{
  uint16_t a = code + SUCCMAX;

  ds->frq[a]++;

  if (ds->dad[a] != ROOT)
  {
    uint16_t code1 = ds->dad[a];

    Sixpack_UpdateFreq (ds, a, ds->leftC[code1] == a ? ds->rghtC[code1] : ds->leftC[code1]);

    do
    {
      uint16_t code2 = ds->dad[code1];
      uint16_t b = ds->leftC[code2] == code1 ? ds->rghtC[code2] : ds->leftC[code2];

      if (ds->frq[a] > ds->frq[b])
      {
        uint16_t c;

        if (ds->leftC[code2] == code1)
          ds->rghtC[code2] = a;
        else
          ds->leftC[code2] = a;

        if (ds->leftC[code1] == a)
        {
          ds->leftC[code1] = b;
          c = ds->rghtC[code1];
        }
        else
        {
          ds->rghtC[code1] = b;
          c = ds->leftC[code1];
        }

        ds->dad[b] = code1;
        ds->dad[a] = code2;
        Sixpack_UpdateFreq (ds, b, c);
        a = b;
      }

      a = ds->dad[a];
      code1 = ds->dad[a];
    } while (code1 != ROOT);
  }
}

static uint16_t Sixpack_InputCode (struct SixpackDState_t *ds, uint16_t bits)
{
  uint16_t code = 0;

  for (uint16_t i = 0; i < bits; i++)
  {
    if (ds->ibitCount == 0)
    {
      if (ds->ibufCount == MAXBUF) ds->ibufCount = 0;
      ds->ibitBuffer = ((uint16_t *)ds->input_ptr)[ds->ibufCount++];
      ds->ibitCount = 15;
    }
    else
      ds->ibitCount--;

    if (ds->ibitBuffer > 0x7FFF) code |= BitValue[i];
    ds->ibitBuffer <<= 1;
  }

  return code;
}

static uint16_t Sixpack_Uncompress (struct SixpackDState_t *ds)
{
  uint16_t a = 1;

  do
  {
    if (ds->ibitCount == 0)
    {
      if (ds->ibufCount == MAXBUF) ds->ibufCount = 0;
      ds->ibitBuffer = ((uint16_t *)ds->input_ptr)[ds->ibufCount++];
      ds->ibitCount = 15;
    }
    else
      ds->ibitCount--;

    a = ds->ibitBuffer <= 0x7FFF ? ds->leftC[a] : ds->rghtC[a];
    ds->ibitBuffer <<= 1;
  } while (a <= MAXCHAR);

  a -= SUCCMAX;
  Sixpack_UpdateModel (ds, a);
  return a;
}

static void Sixpack_Decode (struct SixpackDState_t *ds)
{
  uint16_t count = 0;
  uint16_t c;

  ds->ibitCount = 0;
  ds->ibitBuffer = 0;
  ds->ibufCount = 0;
  ds->obufCount = 0;
  Sixpack_InitTree (ds);
  c = Sixpack_Uncompress (ds);

  while (c != TERMINATE)
  {
    if (c < 256)
    {
      ds->output_ptr[ds->obufCount++] = c;
      if (ds->obufCount == MAXBUF)
      {
        ds->output_size = MAXBUF;
        ds->obufCount = 0;
      }

      ds->dict_ptr[count++] = c;
      if (count == MAXSIZE) count = 0;
    }
    else
    {
      uint16_t j, k, t;
      uint16_t dist, len, index;

      t = c - FIRSTCODE;
      index = t / CODESPERRANGE;
      len = t + MINCOPY - index * CODESPERRANGE;
      dist = Sixpack_InputCode (ds, CopyBits[index]) + len + CopyMin[index];

      j = count;
      k = count - dist;
      if (count < dist) k += MAXSIZE;

      for (uint16_t i = 0; i < len; i++)
      {
        ds->output_ptr[ds->obufCount++] = ds->dict_ptr[k];
        if (ds->obufCount == MAXBUF)
        {
          ds->output_size = MAXBUF;
          ds->obufCount = 0;
        }

        ds->dict_ptr[j++] = ds->dict_ptr[k++];
        if (j == MAXSIZE) j = 0;
        if (k == MAXSIZE) k = 0;
      }

      count += len;
      if (count >= MAXSIZE) count -= MAXSIZE;
    }

    c = Sixpack_Uncompress (ds);
  }

  ds->output_size = ds->obufCount;
}

static struct SixpackDState_t sixpack_ds;

uint16_t SIXPACK_decompress (const void *source, void *dest, __UNUSED uint16_t size)
{
  sixpack_ds.input_ptr = source;
  //sixpack_ds.input_size = size;
  sixpack_ds.output_ptr = dest;
  sixpack_ds.dict_ptr = work_mem;
  Sixpack_Decode (&sixpack_ds);
  return sixpack_ds.output_size;
}
