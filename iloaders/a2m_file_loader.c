// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma pack(push, 1)
typedef struct
{
  char ident[10];
  int32_t crc32;
  uint8_t ffver;
  uint8_t patts;
  uint16_t b0len;
  uint16_t b1len;
  uint16_t b2len;
  uint16_t b3len;
  uint16_t b4len;
  uint16_t b5len;
  uint16_t b6len;
  uint16_t b7len;
  uint16_t b8len;
} tOLD_A2M_HEADER;

typedef struct
{
  char ident[10];
  int32_t crc32;
  uint8_t ffver;
  uint8_t patts;
  int32_t b0len;
  int32_t b1len[16];
} tA2M_HEADER;
#pragma pack(pop)

#define OLD_A2M_HEADER_SIZE 26

#if !ADT2PLAY

static void update_instruments_names (tFIXED_SONGDATA *song)
{
  char s[sizeof (song->instr_names[0])], t[sizeof (song->instr_names[0])];

  s[sizeof (song->instr_names[0]) - 1] = '\0'; // terminate string
  t[sizeof (song->instr_names[0]) - 1] = '\0'; // terminate string
  for (int i = 0; i < 255; i++)
  {
    snprintf (s, sizeof (s), " iNS_%02"PRIX8"%c %s", i + 1, /*charmap.*/'\xF7', StringToStr (t, song->instr_names[i], sizeof (t) - 1));
    StrToString (song->instr_names[i], s, sizeof (song->instr_names[0]) - 1);
  }
}

static void update_patterns_names (tFIXED_SONGDATA *song)
{
  char s[sizeof (song->pattern_names[0])], t[sizeof (song->pattern_names[0])];

  s[sizeof (song->pattern_names[0]) - 1] = '\0'; // terminate string
  t[sizeof (song->pattern_names[0]) - 1] = '\0'; // terminate string
  for (int i = 0; i < 0x80; i++)
  {
    snprintf (s, sizeof (s), " PAT_%02"PRIX8"  %c %s", i, /*charmap.*/'\xF7', StringToStr (t, song->pattern_names[i], sizeof (t) - 1));
    StrToString (song->pattern_names[i], s, sizeof (song->pattern_names[0]) - 1);
  }
}

#endif // !ADT2PLAY

// In:
//   * `progress', `state' and `error' may be NULL.
//
// On success:
//   * Return value:
//     0: `ffver' in 1..4.
//     1: `ffver' > 4.
//   * `state' and `error' are untouched.
// On error:
//   * Return value:
//     -1: open/read error.
//     -2: unknown file format.
//     -3: unsupported file format version.
//     -4: checksum mismatch.
//   * `state' (if set):
//     0: song is untouched.
//     1: song is cleared.
//     2: song is partly loaded.
//   * `error' (if set) is set to error description.
int8_t a2m_file_loader (const String *_fname, progress_callback_t *progress, uint8_t *state, char **error)
{
  tFIXED_SONGDATA *song = &songdata;
  tOLD_FIXED_SONGDATA *old_song = &old_songdata;
  int8_t result = -1;
  uint8_t result_state = 0;
  char *result_error = NULL;
  FILE *f = NULL;
  tA2M_HEADER header;
  tOLD_A2M_HEADER header2;
  int32_t crc;
  uint16_t xlen[7];
  char fname[255+1];

  static const char GCC_ATTRIBUTE((nonstring)) id[10] = { "_A2module_" };

  DBG_ENTER ("a2m_file_loader");

  StringToStr (fname, _fname, sizeof (fname) - 1);
  if ((f = fopen (fname, "rb")) == NULL) goto _err_fopen;

  snprintf (progress->msg, sizeof (progress->msg), "%s", "Decompressing module data...");
  progress->update (progress, -1, -1);

  memset (buf1, 0, sizeof (buf1));

  if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;

  if (memcmp (header.ident, id, sizeof (header.ident)) != 0) goto _err_format;

  init_old_songdata ();

  if (header.ffver == 0) goto _err_version;
  else if (header.ffver <= 4)
  {
    memset (adsr_carrier, false, sizeof (adsr_carrier));

    if (fseek (f, 0, SEEK_SET) != 0) goto _err_fread;
    if (fread (&header2, sizeof (header2), 1, f) == 0) goto _err_fread;

    if (memcmp (header2.ident, id, sizeof (header2.ident)) != 0) goto _err_format;

    if (fseek (f, OLD_A2M_HEADER_SIZE, SEEK_SET) != 0) goto _err_fread;
    if (fread (buf1, header2.b0len, 1, f) == 0) goto _err_fread;
    crc = Update32 (buf1, header2.b0len, CRC32_INITVAL);
    if (fread (buf1, header2.b1len, 1, f) == 0) goto _err_fread;
    crc = Update32 (buf1, header2.b1len, crc);
    xlen[0] = header2.b2len;
    xlen[1] = header2.b3len;
    xlen[2] = header2.b4len;
    for (int i = 0; i < 3; i++)
      if (i < (header2.patts - 1) / 16)
      {
        if (fread (buf1, xlen[i], 1, f) == 0) goto _err_fread;
        crc = Update32 (buf1, xlen[i], crc);
      }
    crc = Update32 (&header2.b0len, 2, crc);
    crc = Update32 (&header2.b1len, 2, crc);
    for (int i = 0; i < 3; i++) crc = Update32 (&xlen[i], 2, crc);
    if (crc != header2.crc32) goto _err_checksum;

    init_songdata ();
    song->patt_len = 64;
    if (adjust_tracks || song->nm_tracks < 9) song->nm_tracks = 9;
    result_state = 1;

    if (fseek (f, OLD_A2M_HEADER_SIZE, SEEK_SET) != 0) goto _err_fread;
    if (fread (buf1, header2.b0len, 1, f) == 0) goto _err_fread;

    switch (header2.ffver)
    {
      case 1: SIXPACK_decompress (buf1, old_song, header2.b0len); break;
      case 2: LZW_decompress (buf1, old_song); break;
      case 3: LZSS_decompress (buf1, old_song, header2.b0len); break;
      default:
      case 4: memcpy (old_song, buf1, header2.b0len); break;
    }

    for (int i = 0; i < 250; i++) old_song->instr_data[i].panning = 0;

    if (fread (buf1, header2.b1len, 1, f) == 0) goto _err_fread;

    switch (header2.ffver)
    {
      case 1: SIXPACK_decompress (buf1, old_hash_buffer, header2.b1len); break;
      case 2: LZW_decompress (buf1, old_hash_buffer); break;
      case 3: LZSS_decompress (buf1, old_hash_buffer, header2.b1len); break;
      default:
      case 4: memcpy (old_hash_buffer, buf1, header2.b1len); break;
    }

    import_old_a2m_patterns1 (0, 16);
    result_state = 2;

    for (int i = 0; i < 3; i++)
      if (i < (header2.patts - 1) / 16)
      {
        if (fread (buf1, xlen[i], 1, f) == 0) goto _err_fread;

        switch (header2.ffver)
        {
          case 1: SIXPACK_decompress (buf1, old_hash_buffer, xlen[i]); break;
          case 2: LZW_decompress (buf1, old_hash_buffer); break;
          case 3: LZSS_decompress (buf1, old_hash_buffer, xlen[i]); break;
          default:
          case 4: memcpy (old_hash_buffer, buf1, xlen[i]); break;
        }

        import_old_a2m_patterns1 (i + 1, 16);
      }

    replace_old_adsr (header2.patts);
    import_old_songdata (old_song);
  }
  else if (header.ffver <= 8)
  {
    if (fseek (f, 0, SEEK_SET) != 0) goto _err_fread;
    if (fread (&header2, sizeof (header2), 1, f) == 0) goto _err_fread;

    if (memcmp (header2.ident, id, sizeof (header2.ident)) != 0) goto _err_format;

    if (fread (buf1, header2.b0len, 1, f) == 0) goto _err_fread;
    crc = Update32 (buf1, header2.b0len, CRC32_INITVAL);
    if (fread (buf1, header2.b1len, 1, f) == 0) goto _err_fread;
    crc = Update32 (buf1, header2.b1len, crc);
    xlen[0] = header2.b2len;
    xlen[1] = header2.b3len;
    xlen[2] = header2.b4len;
    xlen[3] = header2.b5len;
    xlen[4] = header2.b6len;
    xlen[5] = header2.b7len;
    xlen[6] = header2.b8len;
    for (int i = 0; i < 7; i++)
      if (i < (header2.patts - 1) / 8)
      {
        if (fread (buf1, xlen[i], 1, f) == 0) goto _err_fread;
        crc = Update32 (buf1, xlen[i], crc);
      }
    crc = Update32 (&header2.b0len, 2, crc);
    crc = Update32 (&header2.b1len, 2, crc);
    for (int i = 0; i < 7; i++) crc = Update32 (&xlen[i], 2, crc);
    if (crc != header2.crc32) goto _err_checksum;

    init_songdata ();
    song->patt_len = 64;
    if (adjust_tracks || (song->nm_tracks < 18)) song->nm_tracks = 18;
    result_state = 1;

    if (fseek (f, sizeof (header2), SEEK_SET) != 0) goto _err_fread;
    if (fread (buf1, header2.b0len, 1, f) == 0) goto _err_fread;

    switch (header2.ffver)
    {
      case 5: SIXPACK_decompress (buf1, old_song, header2.b0len); break;
      case 6: LZW_decompress (buf1, old_song); break;
      case 7: LZSS_decompress (buf1, old_song, header2.b0len); break;
      default:
      case 8: memcpy (old_song, buf1, header2.b0len); break;
    }

    if (fread (buf1, header2.b1len, 1, f) == 0) goto _err_fread;

    switch (header2.ffver)
    {
      case 5: SIXPACK_decompress (buf1, hash_buffer, header2.b1len); break;
      case 6: LZW_decompress (buf1, hash_buffer); break;
      case 7: LZSS_decompress (buf1, hash_buffer, header2.b1len); break;
      default:
      case 8: memcpy (hash_buffer, buf1, header2.b1len); break;
    }

    import_old_a2m_patterns2 (0, 8);
    result_state = 2;

    for (int i = 0; i < 7; i++)
      if (i < (header2.patts-1) / 8)
      {
        if (fread (buf1, xlen[i], 1, f) == 0) goto _err_fread;

        switch (header2.ffver)
        {
          case 5: SIXPACK_decompress (buf1, hash_buffer, header2.b2len); break;
          case 6: LZW_decompress (buf1, hash_buffer); break;
          case 7: LZSS_decompress (buf1, hash_buffer, header2.b2len); break;
          default:
          case 8: memcpy (hash_buffer, buf1, header2.b2len); break;
        }

        import_old_a2m_patterns2 (i + 1, 8);
      }

    import_old_songdata (old_song);
  }
  else if (header.ffver <= 11)
  {
    if (fread (buf1, header.b0len, 1, f) == 0) goto _err_fread;
    crc = Update32 (buf1, header.b0len, CRC32_INITVAL);
    if (fread (buf1, header.b1len[0], 1, f) == 0) goto _err_fread;
    crc = Update32 (buf1, header.b1len[0], crc);
    for (int i = 1; i < 16; i++)
      if (i - 1 < (header.patts - 1) / 8)
      {
        if (fread (buf1, header.b1len[i], 1, f) == 0) goto _err_fread;
        crc = Update32 (buf1, header.b1len[i], crc);
      }
    crc = Update32 (&header.b0len, 2, crc);
    for (int i = 0; i < 16; i++) crc = Update32 (&header.b1len[i], 2, crc);
    if (crc != header.crc32) goto _err_checksum;

    init_songdata ();
    result_state = 1;

    if (fseek (f, sizeof (header), SEEK_SET) != 0) goto _err_fread;
    if (fread (buf1, header.b0len, 1, f) == 0) goto _err_fread;

    APACK_decompress (buf1, song);
    result_state = 2;

    if (fread (buf1, header.b1len[0], 1, f) == 0) goto _err_fread;

    if (header.ffver == 9) import_old_flags ();

#if !ADT2PLAY
    update_instruments_names (song);
    if (header.ffver == 11) update_patterns_names (song);
#endif // !ADT2PLAY

    APACK_decompress (buf1, &((*pattdata)[0]));

    for (int i = 1; i < 16; i++)
      if (i - 1 < (header.patts - 1) / 8)
      {
        if (fread (buf1, header.b1len[i], 1, f) == 0) goto _err_fread;

        if (i * 8 + 8 <= max_patterns)
          APACK_decompress (buf1, &((*pattdata)[i]));
        else
          limit_exceeded = true;
      }
  }
  else if (header.ffver <= 14)
  {
    if (fread (buf1, header.b0len, 1, f) == 0) goto _err_fread;
    crc = Update32 (buf1, header.b0len, CRC32_INITVAL);
    if (fread (buf1, header.b1len[0], 1, f) == 0) goto _err_fread;
    crc = Update32 (buf1, header.b1len[0], crc);
    for (int i = 1; i < 16; i++)
      if (i - 1 < (header.patts - 1) / 8)
      {
        if (fread (buf1, header.b1len[i], 1, f) == 0) goto _err_fread;
        crc = Update32 (buf1, header.b1len[i], crc);
      }
    crc = Update32 (&header.b0len, 2, crc);
    for (int i = 0; i < 16; i++) crc = Update32 (&header.b1len[i], 2, crc);
    if (crc != header.crc32) goto _err_checksum;

    init_songdata ();
    result_state = 1;

    if (fseek (f, sizeof (header), SEEK_SET) != 0) goto _err_fread;
    if (fread (buf1, header.b0len, 1, f) == 0) goto _err_fread;

    if (progress != NULL) progress->num_steps = (header.patts - 1) / 8 + 2;
    LZH_decompress (buf1, song, header.b0len, progress);
    result_state = 2;
    if (progress != NULL) progress->step++;

    if (fread (buf1, header.b1len[0], 1, f) == 0) goto _err_fread;

#if !ADT2PLAY
    update_instruments_names (song);
    update_patterns_names (song);
#endif // !ADT2PLAY

    LZH_decompress (buf1, (*pattdata)[0], header.b1len[0], progress);
    if (progress != NULL) progress->step++;

    for (int i = 1; i < 16; i++)
      if (i - 1 < (header.patts - 1) / 8)
      {
        if (fread (buf1, header.b1len[i], 1, f) == 0) goto _err_fread;

        if (i * 8 + 8 <= max_patterns)
        {
          LZH_decompress (buf1, (*pattdata)[i], header.b1len[i], progress);
          if (progress != NULL) progress->step++;
        }
        else limit_exceeded = true;
      }
  }
  else goto _err_version;

  speed = song->speed;
  tempo = song->tempo;

  {
    String_t s = NameOnly (_fname);
    CopyString (songdata_title, (String *)&s, sizeof (songdata_title) - 1);
  }

  result = header.ffver <= 4 ? 0 : 1;

_exit:
  if (f != NULL) fclose (f);

  DBG_LEAVE (); //EXIT //a2m_file_loader
  if (result < 0)
  {
    if (state != NULL) *state = result_state;
    if (error != NULL) *error = result_error;
  }
  return result;

_err_fopen:
  result_error = "Failed to open input file";
  goto _exit;

_err_fread:
  result_error = "Failed to read input file";
  goto _exit;

_err_format:
  result_error = "Unknown file format";
  result = -2;
  goto _exit;

_err_version:
  result_error = "Unsupported file format version";
  result = -3;
  goto _exit;

_err_checksum:
  result_error = "Checksum mismatch - file corrupted";
  result = -4;
  goto _exit;
}
