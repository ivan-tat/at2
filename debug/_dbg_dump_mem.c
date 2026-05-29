// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Destination must be of the size (len + 1) bytes to hold the result.
//
// Example:
//   #define LEN 8
//   char s[LEN+1];
//   _dbg_get_xnum(0xBAAD5EED,LEN,s);
//   // s="BAAD5EED";
static void _dbg_get_xnum (uint64_t value, uint8_t len, char *dest)
{
  char *c = &(dest[len]), i;

  *c = '\0';
  for (c--, i = len; i != 0; value >>= 4, c--, i--)
    *c = HEXDIGITS[value & 15];
}

// Destination must be of the size (max * 4 + 1) bytes to hold the result.
//
// Example:
//   #define MAX 16
//   char s[MAX*4+1];
//   _dbg_get_xline(HEXDIGITS,12,MAX,s);
//   // s="30 31 32 33|34 35 36 37|38 39 41 42|-- -- -- --|0123456789AB";
//   _dbg_get_xline(HEXDIGITS,6,MAX,s);
//   // s="30 31 32 33|34 35 -- --|-- -- -- --|-- -- -- --|012345";
static void _dbg_get_xline (const void *mem, uint8_t size, uint8_t max, char *dest)
{
  const unsigned char *p = (const unsigned char *)mem;

  for (uint8_t i = 0; i < max; i++)
  {
    if (i < size)
    {
      _dbg_get_xnum (*p, 2, dest);
      p++;
    }
    else
    {
      dest[0] = '-';
      dest[1] = '-';
    }
    dest[2] = ((i & 3) == 3) ? '|' : ' ';
    dest += 3;
  }

  p = (const unsigned char *)mem;
  for (uint8_t i = size; i != 0; i--)
  {
#if GO32
    *dest = *p < 32 ? '.' : *p;
#else // !GO32
    *dest = (*p < 32) || (*p > 127) ? '.' : *p;
#endif // !GO32
    p++;
    dest++;
  }

  *dest = '\0';
}

#define LINE_SIZE 16
void _dbg_dump_mem (const void *mem, uint32_t size, const char *padstr)
{
  const char *p = (const char *)mem;
  uint32_t o = 0;
  char s[8 + 1 + LINE_SIZE * 4 + 1];

  for (uint32_t left = size; left != 0;)
  {
    uint8_t len = left > LINE_SIZE ? LINE_SIZE : left;
    _dbg_get_xnum (o, 8, s);
    s[8] = '|';
    _dbg_get_xline (p, len, LINE_SIZE, &(s[9]));
    if (padstr)
      _dbg_printf ("%s%s\n", padstr, s);
    else
      _dbg_printf ("%s\n", s);
    p += len;
    o += len;
    left -= len;
  }
}
#undef LINE_SIZE
