// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Decoder state
struct aPDState_t
{
  const uint8_t *source;
  uint8_t *destination;
  uint8_t tag;
  uint8_t bitcount;
};

static unsigned aP_getbit (struct aPDState_t *ud)
{
  uint8_t bit;

  // check if tag is empty
  if (!ud->bitcount--)
  {
    // load next tag
    ud->tag = *ud->source++;
    ud->bitcount = 7;
  }

  // shift bit out of tag
  bit = (ud->tag >> 7) & 1;
  ud->tag <<= 1;

  return bit;
}

static uint32_t aP_getgamma (struct aPDState_t *ud)
{
  uint32_t result = 1;

  // input gamma2-encoded bits
  do
  {
    result = (result << 1) + aP_getbit (ud);
  } while (aP_getbit (ud));

  return result;
}

uint32_t APACK_decompress (const void *source, void *dest)
{
  struct aPDState_t ud;
  uint32_t r0 = 0;

  ud.source = source;
  ud.destination = dest;
  ud.bitcount = 0;

  // first byte verbatim
  *ud.destination++ = *ud.source++;

  // main decompression loop
  for (;;)
  {
    uint32_t offs, len;

    if (aP_getbit (&ud))
    {
      if (aP_getbit (&ud))
      {
        if (aP_getbit (&ud))
        {
          offs = 0;
          len = 1;

          for (int i = 4; i; i--)
            offs = (offs << 1) + aP_getbit (&ud);

          if (offs == 0)
          {
            *ud.destination++ = 0;
            continue;
          }
        }
        else
        {
          offs = *ud.source >> 1;
          if (offs == 0) break;
          len = 2 + (*ud.source++ & 1);

          r0 = offs;
        }
      }
      else
      {
        len = aP_getgamma (&ud) - 2;

        if (len == 0)
        {
          len = aP_getgamma (&ud);

          offs = r0;
        }
        else
        {
          offs = ((len - 1) << 8) + *ud.source++;
          len = aP_getgamma (&ud);
          if ((offs < 128) || (offs >= 32000)) len += 2;
          else if (offs >= 1280) len++;

          r0 = offs;
        }
      }

      for (; len; len--)
      {
        *ud.destination = *(ud.destination - offs);
        ud.destination++;
      }
    }
    else
      *ud.destination++ = *ud.source++;
  }

  return (uint32_t) (ud.destination - (uint8_t *) dest);
}
