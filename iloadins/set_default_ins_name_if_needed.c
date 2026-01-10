// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*static*/ void set_default_ins_name_if_needed (temp_instrument_t *dst, const String *fname)
{
  if (!dst->four_op ?    (Length (dst->ins1.name) == 0)
                    :    (Length (dst->ins1.name) == 0)
                      || (Length (dst->ins2.name) == 0))
  {
    String_t s, t;

    s = NameOnly (fname);
    t = Lower ((String *)&s);
    s = truncate_string ((String *)&t);

    if (!dst->four_op)
    {
      CopyString ((String *)&dst->ins1.name, (String *)&s, sizeof (dst->ins1.name) - 1);
    }
    else
    {
      if (Length (dst->ins1.name) == 0)
      {
        CopyString ((String *)&dst->ins1.name, (String *)&s, sizeof (dst->ins1.name) - 1);
        AppendString ((String *)&dst->ins1.name, (String *)"\x0A" " [4OP 1/2]", sizeof (dst->ins1.name) - 1);
      }
      if (Length (dst->ins2.name) == 0)
      {
        CopyString ((String *)&dst->ins2.name, (String *)&s, sizeof (dst->ins2.name) - 1);
        AppendString ((String *)&dst->ins2.name, (String *)"\x0A" " [4OP 2/2]", sizeof (dst->ins2.name) - 1);
      }
    }
  }
}
