// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static const unsigned char _treat_char[] = {
  'C', // CP437: 0x80, U+00C7: 'Ç' - LATIN CAPITAL LETTER C WITH CEDILLA
  'u', // CP437: 0x81, U+00FC: 'ü' - LATIN SMALL LETTER U WITH DIAERESIS
  'e', // CP437: 0x82, U+00E9: 'é' - LATIN SMALL LETTER E WITH ACUTE
  'a', // CP437: 0x83, U+00E2: 'â' - LATIN SMALL LETTER A WITH CIRCUMFLEX
  'a', // CP437: 0x84, U+00E4: 'ä' - LATIN SMALL LETTER A WITH DIAERESIS
  'a', // CP437: 0x85, U+00E0: 'à' - LATIN SMALL LETTER A WITH GRAVE
  'a', // CP437: 0x86, U+00E5: 'å' - LATIN SMALL LETTER A WITH RING ABOVE
  'c', // CP437: 0x87, U+00E7: 'ç' - LATIN SMALL LETTER C WITH CEDILLA
  'e', // CP437: 0x88, U+00EA: 'ê' - LATIN SMALL LETTER E WITH CIRCUMFLEX
  'e', // CP437: 0x89, U+00EB: 'ë' - LATIN SMALL LETTER E WITH DIAERESIS
  'e', // CP437: 0x8A, U+00E8: 'è' - LATIN SMALL LETTER E WITH GRAVE
  'i', // CP437: 0x8B, U+00EF: 'ï' - LATIN SMALL LETTER I WITH DIAERESIS
  'i', // CP437: 0x8C, U+00EE: 'î' - LATIN SMALL LETTER I WITH CIRCUMFLEX
  'i', // CP437: 0x8D, U+00EC: 'ì' - LATIN SMALL LETTER I WITH GRAVE
  'A', // CP437: 0x8E, U+00C4: 'Ä' - LATIN CAPITAL LETTER A WITH DIAERESIS
  'A', // CP437: 0x8F, U+00C5: 'Å' - LATIN CAPITAL LETTER A WITH RING ABOVE
  'E', // CP437: 0x90, U+00C9: 'É' - LATIN CAPITAL LETTER E WITH ACUTE
  '_', // CP437: 0x91, U+00E6: 'æ' - LATIN SMALL LIGATURE AE
  'A', // CP437: 0x92, U+00C6: 'Æ' - LATIN CAPITAL LIGATURE AE
  'o', // CP437: 0x93, U+00F4: 'ô' - LATIN SMALL LETTER O WITH CIRCUMFLEX
  'o', // CP437: 0x94, U+00F6: 'ö' - LATIN SMALL LETTER O WITH DIAERESIS
  'o', // CP437: 0x95, U+00F2: 'ò' - LATIN SMALL LETTER O WITH GRAVE
  'u', // CP437: 0x96, U+00FB: 'û' - LATIN SMALL LETTER U WITH CIRCUMFLEX
  'u', // CP437: 0x97, U+00F9: 'ù' - LATIN SMALL LETTER U WITH GRAVE
  'y', // CP437: 0x98, U+00FF: 'ÿ' - LATIN SMALL LETTER Y WITH DIAERESIS
  'O', // CP437: 0x99, U+00D6: 'Ö' - LATIN CAPITAL LETTER O WITH DIAERESIS
  'U', // CP437: 0x9A, U+00DC: 'Ü' - LATIN CAPITAL LETTER U WITH DIAERESIS
  '_', // CP437: 0x9B, U+00A2: '¢' - CENT SIGN
  '_', // CP437: 0x9C, U+00A3: '£' - POUND SIGN
  '_', // CP437: 0x9D, U+00A5: '¥' - YEN SIGN
  '_', // CP437: 0x9E, U+20A7: '₧' - PESETA SIGN
  '_', // CP437: 0x9F, U+0192: 'ƒ' - LATIN SMALL LETTER F WITH HOOK
  'a', // CP437: 0xA0, U+00E1: 'á' - LATIN SMALL LETTER A WITH ACUTE
  'i', // CP437: 0xA1, U+00ED: 'í' - LATIN SMALL LETTER I WITH ACUTE
  'o', // CP437: 0xA2, U+00F3: 'ó' - LATIN SMALL LETTER O WITH ACUTE
  'u', // CP437: 0xA3, U+00FA: 'ú' - LATIN SMALL LETTER U WITH ACUTE
  'n', // CP437: 0xA4, U+00F1: 'ñ' - LATIN SMALL LETTER N WITH TILDE
  'N'  // CP437: 0xA5, U+00D1: 'Ñ' - LATIN CAPITAL LETTER N WITH TILDE
};

String_t FilterStr2 (const String *str, const uint8_t *charset, char chr) {
  String_t r;
  size_t len = Length (str);
  const unsigned char *s = GetStr (str);
  unsigned char *d = r.str;

  r.len = len;

  while (len) {
    unsigned char c = *s;

    s++;

    if (!(bit_test (charset, c))) {
      if ((c >= 0x80) && (c <= 0xA5))
        c = _treat_char [c - 0x80];
      else
        c = c ? chr : ' ';
    }

    *d = c;
    d++;
    len--;
  }

  return r;
}
