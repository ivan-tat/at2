// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// In: `count' = 1..INSTRUMENTS_MAX_A2M_V1
static void get_ins_names_a2m_v1 (tFIXED_SONGDATA *dst, ins_name_a2m_v1_t *src, unsigned count)
{
  char prefix[8];
  bool cut;

  // check prefix " st-###:" and " st-###: ", where ###=1..count (zero padded decimal)

  prefix[0] = ' ';
  prefix[1] = 's';
  prefix[2] = 't';
  prefix[3] = '-';
  prefix[7] = ':';

  cut = true;
  for (unsigned i = 0; i < count; i++)
  {
    if (src[i].length < sizeof (prefix))
    {
      cut = false;
      break;
    }

    prefix[4] = '0' + ((i + 1) / 100) % 10;
    prefix[5] = '0' + ((i + 1) / 10) % 10;
    prefix[6] = '0' + (i + 1) % 10;

    if (   (memcmp (&src[i].data, prefix, sizeof (prefix)) != 0)
        || (   (src[i].length >= sizeof (prefix) + 1)
            && (src[i].data[sizeof (prefix)] != ' ')))
    {
      cut = false;
      break;
    }
  }

  if (!cut)
  {
    // check prefix " iNS_##:" and " iNS_##: ", where ##=1..count (zero padded hexadecimal)

    prefix[1] = 'i';
    prefix[2] = 'N';
    prefix[3] = 'S';
    prefix[4] = '_';

    cut = true;
    for (unsigned i = 0; i < count; i++)
    {
      if (src[i].length < sizeof (prefix))
      {
        cut = false;
        break;
      }

      prefix[5] = HEXDIGITS[((i + 1) / 16) % 16];
      prefix[6] = HEXDIGITS[(i + 1) % 16];

      if (   (memcmp (&src[i].data, prefix, sizeof (prefix)) != 0)
          || (   (src[i].length >= sizeof (prefix) + 1)
              && (src[i].data[sizeof (prefix)] != ' ')))
      {
        cut = false;
        break;
      }
    }
  }

  for (unsigned i = 0; i < count; i++)
  {
    if (cut)
    {
      String_t s = Copy ((String *)&src[i], 1 + sizeof (prefix) + 1, sizeof (src[0]) - sizeof (prefix) - 1);  // skip prefix
      AppendString (dst->instr_names[i], (String *)&s, sizeof (dst->instr_names[0]) - 1);
    }
    else
      AppendString (dst->instr_names[i], (String *)&src[i], sizeof (dst->instr_names[0]) - 1);
  }

/*
  for (unsigned i = count; i < INSTRUMENTS_MAX; i++)
    SetLength (song->instr_names[i], 0);
*/
}
