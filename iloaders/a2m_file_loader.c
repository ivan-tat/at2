// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#define A2M_ID_SIZE 10

static const char GCC_ATTRIBUTE((nonstring)) a2m_id[A2M_ID_SIZE] = { "_A2module_" };

#pragma pack(push, 1)
typedef struct
{
  char id[A2M_ID_SIZE]; // ID string
  uint32_t crc32; // 32-bit cyclic redundancy check value
  uint8_t ffver;  // file format version
  uint8_t patts;  // number of patterns
} tA2M_HEADER;

typedef struct  // file format version 9..14
{
  uint32_t blen[17];  // length of data blocks: 0=songdata, 1..16=patterns (x8)
} tA2M_HEADER_9;
#pragma pack(pop)

static bool is_a2m_packed (uint8_t version)
{
  return (version >= 1) && (version != 4) && (version != 8) && (version <= 14);
}

static int unpack_a2m_block (void *dst, const void *src, uint32_t src_size,
                             uint8_t version, progress_callback_t *progress)
{
  int status = 0;
  uint32_t dst_size, unpacked_size;

  switch (version)
  {
    case 1:
    case 5:
      SIXPACK_decompress (src, dst, src_size);
      break;
    case 2:
    case 6:
      LZW_decompress (src, dst);
      break;
    case 3:
    case 7:
      LZSS_decompress (src, dst, src_size);
      break;
    case 4:
    case 8:
      memcpy (dst, src, src_size);
      break;
    case 9:
    case 10:
    case 11:
      APACK_decompress (src, dst);
      break;
    case 12:
    case 13:
    case 14:
      dst_size = ((LZH_block_info_t *)src)->size;
      unpacked_size = LZH_decompress (src, dst, src_size, progress);
      if (unpacked_size != dst_size) status = -1;
      break;
    default:
      status = -1;
      break;
  }
  return status;
}

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

static void next_a2m_step (progress_callback_t *progress)
{
  if (progress != NULL)
  {
    progress->step++;
    progress->value = 1;
    progress->update (progress, 1, -1);
  }
}

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
//     -4: format is supported.
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
  uint32_t crc = CRC32_INITVAL;
  int pat_blocks;
  char fname[255+1];

  DBG_ENTER ("a2m_file_loader");

  StringToStr (fname, _fname, sizeof (fname) - 1);
  if ((f = fopen (fname, "rb")) == NULL) goto _err_fopen;

  result = -2;

  if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;

  if (memcmp (header.id, a2m_id, sizeof (header.id)) != 0) goto _err_format;

  result = -3;

  if ((header.ffver < 1) || (header.ffver > 14)) goto _err_version;

  result = -4;

  if (progress != NULL)
  {
    snprintf (progress->msg, sizeof (progress->msg), "%s",
      is_a2m_packed (header.ffver) ? "Decompressing module data..."
                                   : "Reading module data...");
    progress->update (progress, -1, -1);
  }

  memset (buf1, 0, sizeof (buf1));
  init_old_songdata ();

  if (header.ffver <= 4)
  {
#pragma pack(push, 1)
    struct  // file format version 1..4
    {
      uint16_t blen[5]; // length of data blocks: 0=songdata, 1..4=patterns (x16)
    } header1;
#pragma pack(pop)

    memset (adsr_carrier, false, sizeof (adsr_carrier));

    if (fread (&header1, sizeof (header1), 1, f) == 0) goto _err_fread;

    pat_blocks = ((header.patts > 64 ? 64 : header.patts) + 15) / 16; // 0..4
    if (progress != NULL) progress->num_steps = 2 + pat_blocks;

    // checksum
    for (int i = 0; i < 1 + pat_blocks; i++)
      if (header1.blen[i] != 0)
      {
        if (fread (buf1, header1.blen[i], 1, f) == 0) goto _err_fread;
        crc = Update32 (buf1, header1.blen[i], crc);
      }
    crc = Update32 (header1.blen, sizeof (header1.blen), crc);
    if (crc != header.crc32) goto _err_checksum;
    next_a2m_step (progress);

    init_songdata ();
    song->patt_len = 64;
    if (adjust_tracks || song->nm_tracks < 9) song->nm_tracks = 9;
    result_state = 1;

    if (fseek (f, sizeof (header) + sizeof (header1), SEEK_SET) != 0) goto _err_fread;

    // songdata
    if (header1.blen[0] != 0)
    {
      if (fread (buf1, header1.blen[0], 1, f) == 0) goto _err_fread;
      if (unpack_a2m_block (old_song, buf1, header1.blen[0], header.ffver, progress) != 0) goto _err_unpack;
      for (int i = 0; i < 250; i++) old_song->instr_data[i].panning = 0;
    }
    next_a2m_step (progress);

    // patterns (x16)
    for (int i = 0; i < pat_blocks; i++)
    {
      if (header1.blen[1 + i] != 0)
      {
        if (fread (buf1, header1.blen[1 + i], 1, f) == 0) goto _err_fread;
        memset (old_hash_buffer, 0, sizeof (old_hash_buffer));
        if (unpack_a2m_block (old_hash_buffer, buf1, header1.blen[1 + i], header.ffver, progress) != 0) goto _err_unpack;
        import_old_a2m_patterns1 (i, 16);
        result_state = 2;
      }
      next_a2m_step (progress);
    }

    replace_old_adsr (header.patts);
    import_old_songdata (old_song);
  }
  else if (header.ffver <= 8)
  {
#pragma pack(push, 1)
    struct  // file format version 5..8
    {
      uint16_t blen[9]; // length of data blocks: 0=songdata, 1..8=patterns (x8)
    } header5;
#pragma pack(pop)

    if (fread (&header5, sizeof (header5), 1, f) == 0) goto _err_fread;

    pat_blocks = ((header.patts > 64 ? 64 : header.patts) + 7) / 8; // 0..8
    if (progress != NULL) progress->num_steps = 2 + pat_blocks;

    // checksum
    for (int i = 0; i < 1 + pat_blocks; i++)
      if (header5.blen[i] != 0)
      {
        if (fread (buf1, header5.blen[i], 1, f) == 0) goto _err_fread;
        crc = Update32 (buf1, header5.blen[i], crc);
      }
    crc = Update32 (header5.blen, sizeof (header5.blen), crc);
    if (crc != header.crc32) goto _err_checksum;
    next_a2m_step (progress);

    init_songdata ();
    song->patt_len = 64;
    if (adjust_tracks || (song->nm_tracks < 18)) song->nm_tracks = 18;
    result_state = 1;

    if (fseek (f, sizeof (header) + sizeof (header5), SEEK_SET) != 0) goto _err_fread;

    // songdata
    if (header5.blen[0] != 0)
    {
      if (fread (buf1, header5.blen[0], 1, f) == 0) goto _err_fread;
      if (unpack_a2m_block (old_song, buf1, header5.blen[0], header.ffver, progress) != 0) goto _err_unpack;
    }
    next_a2m_step (progress);

    // patterns (x8)
    for (int i = 0; i < pat_blocks; i++)
    {
      if (header5.blen[1 + i] != 0)
      {
        if (fread (buf1, header5.blen[1 + i], 1, f) == 0) goto _err_fread;
        if (unpack_a2m_block (hash_buffer, buf1, header5.blen[1 + i], header.ffver, progress) != 0) goto _err_unpack;
        import_old_a2m_patterns2 (i, 8);
        result_state = 2;
      }
      next_a2m_step (progress);
    }

    import_old_songdata (old_song);
  }
  else // header.ffver <= 14
  {
    tA2M_HEADER_9 header9;

    if (fread (&header9, sizeof (header9), 1, f) == 0) goto _err_fread;

    pat_blocks = ((header.patts > 128 ? 128 : header.patts) + 7) / 8; // 0..16
    if (progress != NULL) progress->num_steps = 2 + pat_blocks;

    // checksum
    for (int i = 0; i < 1 + pat_blocks; i++)
      if (header9.blen[i] != 0)
      {
        if (fread (buf1, header9.blen[i], 1, f) == 0) goto _err_fread;
        crc = Update32 (buf1, header9.blen[i], crc);
      }
    for (unsigned i = 0; i < sizeof (header9.blen) / sizeof (header9.blen[0]); i++)
      crc = Update32 (&header9.blen[i], /*sizeof (header9.blen[0])*/ 2, crc); // it's not a bug - it's a feature
    if (crc != header.crc32) goto _err_checksum;
    next_a2m_step (progress);

    init_songdata ();
    result_state = 1;

    if (fseek (f, sizeof (header) + sizeof (header9), SEEK_SET) != 0) goto _err_fread;

    // songdata
    if (header9.blen[0] != 0)
    {
      if (fread (buf1, header9.blen[0], 1, f) == 0) goto _err_fread;
      if (unpack_a2m_block (song, buf1, header9.blen[0], header.ffver, progress) != 0) goto _err_unpack;
      result_state = 2;
    }
    next_a2m_step (progress);

    // patterns (x8)
    for (int i = 0; i < pat_blocks; i++)
    {
      if (header9.blen[1 + i] != 0)
      {
        if (fread (buf1, header9.blen[1 + i], 1, f) == 0) goto _err_fread;

        if ((i + 1) * 8 <= max_patterns)
        {
          if (unpack_a2m_block ((*pattdata)[i], buf1, header9.blen[1 + i], header.ffver, progress) != 0) goto _err_unpack;
          result_state = 2;
        }
        else limit_exceeded = true;
      }
      next_a2m_step (progress);
    }

    if (header.ffver == 9) import_old_flags ();
#if !ADT2PLAY
    update_instruments_names (song);
    if (header.ffver >= 11) update_patterns_names (song);
#endif // !ADT2PLAY
  }

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
  goto _exit;

_err_version:
  result_error = "Unsupported file format version";
  goto _exit;

_err_checksum:
  result_error = "Checksum mismatch - file corrupted";
  goto _exit;

_err_unpack:
  result_error = "Failed to unpack input data";
  goto _exit;
}
