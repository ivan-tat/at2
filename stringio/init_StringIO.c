// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void init_StringIO (void) {
#if !ADT2PLAY
  uint_least32_t i;

  /*** UCS-2 case conversion ***/

  memset (UCS2_upper_case_f, 0, sizeof (UCS2_upper_case_f));
  for (i = 0; i < sizeof (UCS2_upper_case_ct) /
                  sizeof (UCS2_upper_case_ct[0]); i++)
    UCS2_upper_case_ct[i] = i;

  memset (UCS2_lower_case_f, 0, sizeof (UCS2_lower_case_f));
  for (i = 0; i < sizeof (UCS2_lower_case_ct) /
                  sizeof (UCS2_lower_case_ct[0]); i++)
    UCS2_lower_case_ct[i] = i;

  for (i = 0; UCS2_case_conv_it[i].upper; i++)
    if ((UCS2_case_conv_it[i].upper < UCS2_CP_SIZE)
    &&  (UCS2_case_conv_it[i].lower < UCS2_CP_SIZE)) {
      bit_set (UCS2_upper_case_f, UCS2_case_conv_it[i].upper);
      UCS2_upper_case_ct[UCS2_case_conv_it[i].lower] =
                         UCS2_case_conv_it[i].upper;
      bit_set (UCS2_lower_case_f, UCS2_case_conv_it[i].lower);
      UCS2_lower_case_ct[UCS2_case_conv_it[i].upper] =
                         UCS2_case_conv_it[i].lower;
    }

  /*** ASCII ***/

  memset (DEC_NUM_CHARSET, 0, sizeof (DEC_NUM_CHARSET));
  bit_set_range (DEC_NUM_CHARSET, '0', '9');

  memset (HEX_NUM_CHARSET, 0, sizeof (HEX_NUM_CHARSET));
  bit_set_range (HEX_NUM_CHARSET, '0', '9');
  bit_set_range (HEX_NUM_CHARSET, 'A', 'F');
  bit_set_range (HEX_NUM_CHARSET, 'a', 'f');

  /*** UCS-2 to `AT2' code page conversion ***/

  // Setup first 128 glyphs
  for (i = 0; i < 128; i++)
    UCS2_to_AT2_ct[i] = i;

  // Setup all other glyphs as '?'
  memset (UCS2_to_AT2_ct + 128, '?', sizeof (UCS2_to_AT2_ct) - 128);

  // Setup CP437
  for (i = 0; CP437_it[i].glyph >= 0; i++)
    if (CP437_it[i].uc < UCS2_CP_SIZE)
      UCS2_to_AT2_ct[CP437_it[i].uc] = CP437_it[i].glyph;

  /*** `AT2' code page case conversion ***/

  memset (AT2_upper_case_f, 0, sizeof (AT2_upper_case_f));
  bit_set_range (AT2_upper_case_f, 'A', 'Z');

  memset (AT2_lower_case_f, 0, sizeof (AT2_lower_case_f));
  bit_set_range (AT2_lower_case_f, 'a', 'z');

  for (i = 0; i < AT2_CP_SIZE; i++) {
    AT2_upper_case_ct[i] = i;
    AT2_lower_case_ct[i] = i;
  }

  for (i = 0; CP437_case_conv_it[i].upper; i++) {
#if AT2_CP_SIZE > 0x100
    if ((CP437_case_conv_it[i].upper < AT2_CP_SIZE)
    &&  (CP437_case_conv_it[i].lower < AT2_CP_SIZE)) {
#endif // AT2_CP_SIZE > 0x100
      bit_set (AT2_upper_case_f, CP437_case_conv_it[i].upper);
      AT2_upper_case_ct[CP437_case_conv_it[i].lower] =
                        CP437_case_conv_it[i].upper;
      bit_set (AT2_lower_case_f, CP437_case_conv_it[i].lower);
      AT2_lower_case_ct[CP437_case_conv_it[i].upper] =
                        CP437_case_conv_it[i].lower;
#if AT2_CP_SIZE > 0x100
    }
#endif // AT2_CP_SIZE > 0x100
  }
#endif // !ADT2PLAY
}
