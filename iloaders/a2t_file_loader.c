// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#define A2T_ID_SIZE 15

static const char GCC_ATTRIBUTE((nonstring)) a2t_id[A2T_ID_SIZE] = { "_A2tiny_module_" };

#pragma pack(push, 1)
typedef struct  // common for all file format versions
{
  char id[A2T_ID_SIZE]; // ID string
  uint32_t crc32; // 32-bit cyclic redundancy check value
  uint8_t ffver;  // file format version
  uint8_t patts;  // number of patterns
  uint8_t tempo;  // initial tempo
  uint8_t speed;  // initial speed
} tA2T_HEADER;

typedef struct  // file format version 11..14
{
  uint8_t cflag;      // flags
  uint16_t patln;     // pattern length
  uint8_t nmtrk;      // number of tracks
  uint16_t mcspd;     // macro speed-up factor
  uint8_t is4op;      // 4-op track extension flags
  uint8_t locks[20];  // initial lock flags for 20 tracks
  uint32_t blen[21];  // length of data blocks
                      // 0=instrument specs
                      // 1=instrument macro-defs
                      // 2=arpeggio/vibrato macro-defs
                      // 3=disabled fm-register columns
                      // 4=pattern order
                      // 5..20=patterns (x8)
} tA2T_HEADER_11;
#pragma pack(pop)

static bool is_a2t_packed (uint8_t version)
{
  return (version >= 1) && (version != 4) && (version != 8) && (version <= 14);
}

static int unpack_a2t_block (void *dst, const void *src, uint32_t src_size,
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

static void next_a2t_step (progress_callback_t *progress)
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
int8_t a2t_file_loader (const String *_fname, progress_callback_t *progress, uint8_t *state, char **error)
{
  tFIXED_SONGDATA *song = &songdata;
  tOLD_FIXED_SONGDATA *old_song = &old_songdata;
  int8_t result = -1;
  uint8_t result_state = 0;
  char *result_error = NULL;
  FILE *f = NULL;
  tA2T_HEADER header;
  uint32_t crc = CRC32_INITVAL;
  int pat_blocks;
  char fname[255+1];

  DBG_ENTER ("a2t_file_loader");

  StringToStr (fname, _fname, sizeof (fname) - 1);
  if ((f = fopen (fname, "rb")) == NULL) goto _err_fopen;

  result = -2;

  if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;

  if (memcmp (header.id, a2t_id, sizeof (header.id)) != 0) goto _err_format;

  result = -3;
  header.crc32 = uint32_LE (header.crc32);

  if ((header.ffver < 1) || (header.ffver > 14)) goto _err_version;

  result = -4;

  if (progress != NULL)
  {
    snprintf (progress->msg, sizeof (progress->msg), "%s",
      is_a2t_packed (header.ffver) ? "Decompressing tiny module data..."
                                   : "Reading tiny module data...");
    progress->update (progress, -1, -1);
  }

  memset (buf1, 0, sizeof (buf1));
  init_old_songdata ();

  if (header.ffver <= 4)
  {
#pragma pack(push, 1)
    struct  // file format version 1..4
    {
      uint16_t blen[6]; // length of data blocks
                        // 0=instruments, 1=patterns order, 2..5=patterns (x16)
    } header1;
#pragma pack(pop)

    memset (adsr_carrier, false, sizeof (adsr_carrier));

    if (fread (&header1, sizeof (header1), 1, f) == 0) goto _err_fread;

    pat_blocks = ((header.patts > 64 ? 64 : header.patts) + 15) / 16; // 0..4
    if (progress != NULL) progress->num_steps = 3 + pat_blocks;

    // checksum
    for (int i = 0; i < 2 + pat_blocks; i++)
      if (header1.blen[i] != 0)
      {
        if (fread (buf1, uint16_LE (header1.blen[i]), 1, f) == 0) goto _err_fread;
        crc = Update32 (buf1, uint16_LE (header1.blen[i]), crc);
      }
    crc = Update32 (header1.blen, sizeof (header1.blen), crc);
    if (crc != header.crc32) goto _err_checksum;
    next_a2t_step (progress);

    init_songdata ();
    song->patt_len = 64;
    if (adjust_tracks || song->nm_tracks < 9) song->nm_tracks = 9;
    result_state = 1;

    if (fseek (f, sizeof (header) + sizeof (header1), SEEK_SET) != 0) goto _err_fread;

    // instruments
    if (header1.blen[0] != 0)
    {
      if (fread (buf1, uint16_LE (header1.blen[0]), 1, f) == 0) goto _err_fread;
      if (unpack_a2t_block (old_song->instr_data, buf1, uint16_LE (header1.blen[0]), header.ffver, progress) != 0) goto _err_unpack;
      for (int i = 0; i < 250; i++) old_song->instr_data[i].panning = 0;
    }
    next_a2t_step (progress);

    // patterns order
    if (header1.blen[1] != 0)
    {
      if (fread (buf1, uint16_LE (header1.blen[1]), 1, f) == 0) goto _err_fread;
      if (unpack_a2t_block (old_song->pattern_order, buf1, uint16_LE (header1.blen[1]), header.ffver, progress) != 0) goto _err_unpack;
    }
    next_a2t_step (progress);

    // patterns (x16)
    for (int i = 0; i < pat_blocks; i++)
    {
      if (header1.blen[2 + i] != 0)
      {
        if (fread (buf1, uint16_LE (header1.blen[2 + i]), 1, f) == 0) goto _err_fread;
        memset (old_hash_buffer, 0, sizeof (old_hash_buffer));
        if (unpack_a2t_block (old_hash_buffer, buf1, uint16_LE (header1.blen[2 + i]), header.ffver, progress) != 0) goto _err_unpack;
        import_old_a2m_patterns1 (i, 16);
        result_state = 2;
      }
      next_a2t_step (progress);
    }

    old_song->tempo = header.tempo;
    old_song->speed = header.speed;
    replace_old_adsr (header.patts);
    import_old_songdata (old_song);
  }
  else if (header.ffver <= 8)
  {
#pragma pack(push, 1)
    struct  // file format version 5-8
    {
      uint8_t cflag;      // flags
      uint16_t blen[10];  // length of data blocks
                          // 0=instruments, 1=patterns order, 2..9=patterns (x8)
    } header5;
#pragma pack(pop)

    if (fread (&header5, sizeof (header5), 1, f) == 0) goto _err_fread;

    pat_blocks = ((header.patts > 64 ? 64 : header.patts) + 7) / 8; // 0..8
    if (progress != NULL) progress->num_steps = 3 + pat_blocks;

    // checksum
    for (int i = 0; i < 2 + pat_blocks; i++)
      if (header5.blen[i] != 0)
      {
        if (fread (buf1, uint16_LE (header5.blen[i]), 1, f) == 0) goto _err_fread;
        crc = Update32 (buf1, uint16_LE (header5.blen[i]), crc);
      }
    crc = Update32 (header5.blen, sizeof (header5.blen), crc);
    if (crc != header.crc32) goto _err_checksum;
    next_a2t_step (progress);

    init_songdata ();
    song->patt_len = 64;
    if (adjust_tracks || song->nm_tracks < 18) song->nm_tracks = 18;
    result_state = 1;

    if (fseek (f, sizeof (header) + sizeof (header5), SEEK_SET) != 0) goto _err_fread;

    // instruments
    if (header5.blen[0] != 0)
    {
      if (fread (buf1, uint16_LE (header5.blen[0]), 1, f) == 0) goto _err_fread;
      if (unpack_a2t_block (old_song->instr_data, buf1, uint16_LE (header5.blen[0]), header.ffver, progress) != 0) goto _err_unpack;
    }
    next_a2t_step (progress);

    // patterns order
    if (header5.blen[1] != 0)
    {
      if (fread (buf1, uint16_LE (header5.blen[1]), 1, f) == 0) goto _err_fread;
      if (unpack_a2t_block (old_song->pattern_order, buf1, uint16_LE (header5.blen[1]), header.ffver, progress) != 0) goto _err_unpack;
    }
    next_a2t_step (progress);

    // patterns (x8)
    for (int i = 0; i < pat_blocks; i++)
    {
      if (header5.blen[2 + i] != 0)
      {
        if (fread (buf1, uint16_LE (header5.blen[2 + i]), 1, f) == 0) goto _err_fread;
        memset (hash_buffer, 0, sizeof (hash_buffer));
        if (unpack_a2t_block (hash_buffer, buf1, uint16_LE (header5.blen[2 + i]), header.ffver, progress) != 0) goto _err_unpack;
        import_old_a2m_patterns2 (i, 8);
        result_state = 2;
      }
      next_a2t_step (progress);
    }

    old_song->tempo = header.tempo;
    old_song->speed = header.speed;
    old_song->common_flag = header5.cflag;
    import_old_songdata (old_song);
  }
  else if (header.ffver == 9)
  {
#pragma pack(push, 1)
    struct  // file format version 9
    {
      uint8_t cflag;      // flags
      uint16_t patln;     // pattern length
      uint8_t nmtrk;      // number of tracks
      uint16_t mcspd;     // macro speed-up factor
      uint32_t blen[20];  // length of data blocks
                          // 0=instrument specs
                          // 1=instrument macro-defs
                          // 2=arpeggio/vibrato macro-defs
                          // 3=pattern order
                          // 4..19=patterns (x8)
    } header9;
#pragma pack(pop)

    if (fread (&header9, sizeof (header9), 1, f) == 0) goto _err_fread;

    pat_blocks = ((header.patts > 128 ? 128 : header.patts) + 7) / 8; // 0..16
    if (progress != NULL) progress->num_steps = 5 + pat_blocks;

    // checksum
    for (int i = 0; i < 4 + pat_blocks; i++)
      if (header9.blen[i] != 0)
      {
        if (fread (buf1, uint32_LE (header9.blen[i]), 1, f) == 0) goto _err_fread;
        crc = Update32 (buf1, uint32_LE (header9.blen[i]), crc);
      }
    for (unsigned i = 0; i < sizeof (header9.blen) / sizeof (header9.blen[0]); i++)
      crc = Update32 (&header9.blen[i], /*sizeof (header9.blen[0])*/ 2, crc); // it's not a bug - it's a feature
    if (crc != header.crc32) goto _err_checksum;
    next_a2t_step (progress);

    init_songdata ();
    result_state = 1;

    if (fseek (f, sizeof (header) + sizeof (header9), SEEK_SET) != 0) goto _err_fread;

    // instrument specs
    if (header9.blen[0] != 0)
    {
      if (fread (buf1, uint32_LE (header9.blen[0]), 1, f) == 0) goto _err_fread;
      if (unpack_a2t_block (song->instr_data, buf1, uint32_LE (header9.blen[0]), header.ffver, progress) != 0) goto _err_unpack;
      result_state = 2;
    }
    next_a2t_step (progress);

    // instrument macro-defs
    if (header9.blen[1] != 0)
    {
      if (fread (buf1, uint32_LE (header9.blen[1]), 1, f) == 0) goto _err_fread;
      if (unpack_a2t_block (song->instr_macros, buf1, uint32_LE (header9.blen[1]), header.ffver, progress) != 0) goto _err_unpack;
      result_state = 2;
    }
    next_a2t_step (progress);

    // arpeggio/vibrato macro-defs
    if (header9.blen[2] != 0)
    {
      if (fread (buf1, uint32_LE (header9.blen[2]), 1, f) == 0) goto _err_fread;
      if (unpack_a2t_block (song->macro_table, buf1, uint32_LE (header9.blen[2]), header.ffver, progress) != 0) goto _err_unpack;
      result_state = 2;
    }
    next_a2t_step (progress);

    // pattern order
    if (header9.blen[3] != 0)
    {
      if (fread (buf1, uint32_LE (header9.blen[3]), 1, f) == 0) goto _err_fread;
      if (unpack_a2t_block (song->pattern_order, buf1, uint32_LE (header9.blen[3]), header.ffver, progress) != 0) goto _err_unpack;
      result_state = 2;
    }
    next_a2t_step (progress);

    // patterns (x8)
    for (int i = 0; i < pat_blocks; i++)
    {
      if (header9.blen[4 + i] != 0)
      {
        if (fread (buf1, uint32_LE (header9.blen[4 + i]), 1, f) == 0) goto _err_fread;

        if ((i + 1) * 8 <= max_patterns)
        {
          if (unpack_a2t_block ((*pattdata)[i], buf1, uint32_LE (header9.blen[4 + i]), header.ffver, progress) != 0) goto _err_unpack;
          result_state = 2;
        }
        else limit_exceeded = true;
      }
      next_a2t_step (progress);
    }

    song->tempo = header.tempo;
    song->speed = header.speed;
    song->common_flag = header9.cflag;
    song->patt_len = uint16_LE (header9.patln);
    song->nm_tracks = header9.nmtrk;
    song->macro_speedup = uint16_LE (header9.mcspd);
    import_old_flags ();
  }
  else if (header.ffver == 10)
  {
#pragma pack(push, 1)
    struct  // file format version 10
    {
      uint8_t cflag;      // flags
      uint16_t patln;     // pattern length
      uint8_t nmtrk;      // number of tracks
      uint16_t mcspd;     // macro speed-up factor
      uint8_t is4op;      // 4-op track extension flags
      uint8_t locks[20];  // initial lock flags for 20 tracks
      uint32_t blen[20];  // length of data blocks
                          // 0=instrument specs
                          // 1=instrument macro-defs
                          // 2=arpeggio/vibrato macro-defs
                          // 3=pattern order
                          // 4..19=patterns (x8)
    } header10;
#pragma pack(pop)

    if (fread (&header10, sizeof (header10), 1, f) == 0) goto _err_fread;

    pat_blocks = ((header.patts > 128 ? 128 : header.patts) + 7) / 8; // 0..16
    if (progress != NULL) progress->num_steps = 5 + pat_blocks;

    // checksum
    for (int i = 0; i < 4 + pat_blocks; i++)
      if (header10.blen[i] != 0)
      {
        if (fread (buf1, uint32_LE (header10.blen[i]), 1, f) == 0) goto _err_fread;
        crc = Update32 (buf1, uint32_LE (header10.blen[i]), crc);
      }
    for (unsigned i = 0; i < sizeof (header10.blen) / sizeof (header10.blen[0]); i++)
      crc = Update32 (&header10.blen[i], /*sizeof (header10.blen[0])*/ 2, crc); // it's not a bug - it's a feature
    if (crc != header.crc32) goto _err_checksum;
    next_a2t_step (progress);

    init_songdata ();
    result_state = 1;

    if (fseek (f, sizeof (header) + sizeof (header10), SEEK_SET) != 0) goto _err_fread;

    // instrument specs
    if (header10.blen[0] != 0)
    {
      if (fread (buf1, uint32_LE (header10.blen[0]), 1, f) == 0) goto _err_fread;
      if (unpack_a2t_block (song->instr_data, buf1, uint32_LE (header10.blen[0]), header.ffver, progress) != 0) goto _err_unpack;
      result_state = 2;
    }
    next_a2t_step (progress);

    // instrument macro-defs
    if (header10.blen[1] != 0)
    {
      if (fread (buf1, uint32_LE (header10.blen[1]), 1, f) == 0) goto _err_fread;
      if (unpack_a2t_block (song->instr_macros, buf1, uint32_LE (header10.blen[1]), header.ffver, progress) != 0) goto _err_unpack;
      result_state = 2;
    }
    next_a2t_step (progress);

    // arpeggio/vibrato macro-defs
    if (header10.blen[2] != 0)
    {
      if (fread (buf1, uint32_LE (header10.blen[2]), 1, f) == 0) goto _err_fread;
      if (unpack_a2t_block (song->macro_table, buf1, uint32_LE (header10.blen[2]), header.ffver, progress) != 0) goto _err_unpack;
      result_state = 2;
    }
    next_a2t_step (progress);

    // pattern order
    if (header10.blen[3] != 0)
    {
      if (fread (buf1, uint32_LE (header10.blen[3]), 1, f) == 0) goto _err_fread;
      if (unpack_a2t_block (song->pattern_order, buf1, uint32_LE (header10.blen[3]), header.ffver, progress) != 0) goto _err_unpack;
      result_state = 2;
    }
    next_a2t_step (progress);

    // patterns (x8)
    for (int i = 0; i < pat_blocks; i++)
    {
      if (header10.blen[4 + i] != 0)
      {
        if (fread (buf1, uint32_LE (header10.blen[4 + i]), 1, f) == 0) goto _err_fread;

        if ((i + 1) * 8 <= max_patterns)
        {
          if (unpack_a2t_block ((*pattdata)[i], buf1, uint32_LE (header10.blen[4 + i]), header.ffver, progress) != 0) goto _err_unpack;
          result_state = 2;
        }
        else limit_exceeded = true;
      }
      next_a2t_step (progress);
    }

    song->tempo = header.tempo;
    song->speed = header.speed;
    song->common_flag = header10.cflag;
    song->patt_len = uint16_LE (header10.patln);
    song->nm_tracks = header10.nmtrk;
    song->macro_speedup = uint16_LE (header10.mcspd);
    song->flag_4op = header10.is4op;
    memcpy (song->lock_flags, header10.locks, sizeof (song->lock_flags));
  }
  else if (header.ffver == 11)
  {
    tA2T_HEADER_11 header11;

    if (fread (&header11, sizeof (header11), 1, f) == 0) goto _err_fread;

    pat_blocks = ((header.patts > 128 ? 128 : header.patts) + 7) / 8; // 0..16
    if (progress != NULL) progress->num_steps = 6 + pat_blocks;

    // checksum
    for (int i = 0; i < 5 + pat_blocks; i++)
      if (header11.blen[i] != 0)
      {
        if (fread (buf1, uint32_LE (header11.blen[i]), 1, f) == 0) goto _err_fread;
        crc = Update32 (buf1, uint32_LE (header11.blen[i]), crc);
      }
    for (unsigned i = 0; i < sizeof (header11.blen) / sizeof (header11.blen[0]); i++)
      crc = Update32 (&header11.blen[i], /*sizeof (header11.blen[0])*/ 2, crc); // it's not a bug - it's a feature
    if (crc != header.crc32) goto _err_checksum;
    next_a2t_step (progress);

    init_songdata ();
    result_state = 1;

    if (fseek (f, sizeof (header) + sizeof (header11), SEEK_SET) != 0) goto _err_fread;

    // instrument specs
    if (header11.blen[0] != 0)
    {
      if (fread (buf1, uint32_LE (header11.blen[0]), 1, f) == 0) goto _err_fread;
      if (unpack_a2t_block (song->instr_data, buf1, uint32_LE (header11.blen[0]), header.ffver, progress) != 0) goto _err_unpack;
      result_state = 2;
    }
    next_a2t_step (progress);

    // instrument macro-defs
    if (header11.blen[1] != 0)
    {
      if (fread (buf1, uint32_LE (header11.blen[1]), 1, f) == 0) goto _err_fread;
      if (unpack_a2t_block (song->instr_macros, buf1, uint32_LE (header11.blen[1]), header.ffver, progress) != 0) goto _err_unpack;
      result_state = 2;
    }
    next_a2t_step (progress);

    // arpeggio/vibrato macro-defs
    if (header11.blen[2] != 0)
    {
      if (fread (buf1, uint32_LE (header11.blen[2]), 1, f) == 0) goto _err_fread;
      if (unpack_a2t_block (song->macro_table, buf1, uint32_LE (header11.blen[2]), header.ffver, progress) != 0) goto _err_unpack;
      result_state = 2;
    }
    next_a2t_step (progress);

    // disabled fm-register columns
    if (header11.blen[3] != 0)
    {
      if (fread (buf1, uint32_LE (header11.blen[3]), 1, f) == 0) goto _err_fread;
      if (unpack_a2t_block (song->dis_fmreg_col, buf1, uint32_LE (header11.blen[3]), header.ffver, progress) != 0) goto _err_unpack;
      result_state = 2;
    }
    next_a2t_step (progress);

    // pattern order
    if (header11.blen[4])
    {
      if (fread (buf1, uint32_LE (header11.blen[4]), 1, f) == 0) goto _err_fread;
      if (unpack_a2t_block (song->pattern_order, buf1, uint32_LE (header11.blen[4]), header.ffver, progress) != 0) goto _err_unpack;
      result_state = 2;
    }
    next_a2t_step (progress);

    // patterns (x8)
    for (int i = 0; i < pat_blocks; i++)
    {
      if (header11.blen[5 + i] != 0)
      {
        if (fread (buf1, uint32_LE (header11.blen[5 + i]), 1, f) == 0) goto _err_fread;

        if ((i + 1) * 8 <= max_patterns)
        {
          if (unpack_a2t_block ((*pattdata)[i], buf1, uint32_LE (header11.blen[5 + i]), header.ffver, progress) != 0) goto _err_unpack;
          result_state = 2;
        }
        else limit_exceeded = true;
      }
      next_a2t_step (progress);
    }

    song->tempo = header.tempo;
    song->speed = header.speed;
    song->common_flag = header11.cflag;
    song->patt_len = uint16_LE (header11.patln);
    song->nm_tracks = header11.nmtrk;
    song->macro_speedup = uint16_LE (header11.mcspd);
    song->flag_4op = header11.is4op;
    memcpy (song->lock_flags, header11.locks, sizeof (song->lock_flags));
  }
  else // header.ffver <= 14
  {
    tA2T_HEADER_11 header11;
    size_t unpacked_size;
    mem_stream_t stream;

    pat_blocks = ((header.patts > 128 ? 128 : header.patts) + 7) / 8; // 0..16
    if (progress != NULL) progress->num_steps = 6 + pat_blocks;

    // checksum
    for (int i = 0; i < 5 + pat_blocks; i++)
      if (header11.blen[i] != 0)
      {
        if (fread (buf1, uint32_LE (header11.blen[i]), 1, f) == 0) goto _err_fread;
        crc = Update32 (buf1, uint32_LE (header11.blen[i]), crc);
      }
    for (unsigned i = 0; i < sizeof (header11.blen) / sizeof (header11.blen[0]); i++)
      crc = Update32 (&header11.blen[i], /*sizeof (header11.blen[0])*/ 2, crc); // it's not a bug - it's a feature
    if (crc != header.crc32) goto _err_checksum;
    next_a2t_step (progress);

    init_songdata ();
    result_state = 1;

    if (fseek (f, sizeof (header) + sizeof (header11), SEEK_SET) != 0) goto _err_fread;

    // instrument specs
    if (header11.blen[0] != 0)
    {
      if (fread (buf1, uint32_LE (header11.blen[0]), 1, f) == 0) goto _err_fread;
      unpacked_size = ((LZH_block_info_t *)buf1)->size;
      if (unpack_a2t_block (buf2, buf1, uint32_LE (header11.blen[0]), header.ffver, progress) != 0) goto _err_unpack;
      set_mem_stream (&stream, buf2, unpacked_size);

      if (header.ffver == 14)
      {
        // rows per beat & tempo finetune
        if (read_bytes (&song->bpm_data, sizeof (song->bpm_data), &stream)) goto _err_eod;
        result_state = 2;
      }

      // 4OP instrument flags
      if (read_bytes (&song->ins_4op_flags, sizeof (song->ins_4op_flags), &stream)) goto _err_eod;
      result_state = 2;

      // reserved data
      if (read_bytes (song->reserved_data, sizeof (song->reserved_data), &stream)) goto _err_eod;

      // instrument specs
      unpacked_size = (uint8_t *)stream.endptr - (uint8_t *)stream.curptr;
      if (unpacked_size != 0)
      {
        if (unpacked_size > sizeof (song->instr_data)) unpacked_size = sizeof (song->instr_data);
        if (read_bytes (song->instr_data, unpacked_size, &stream)) goto _err_eod;
      }
    }
    next_a2t_step (progress);

    // instrument macro-defs
    if (header11.blen[1] != 0)
    {
      if (fread (buf1, uint32_LE (header11.blen[1]), 1, f) == 0) goto _err_fread;
      if (unpack_a2t_block (song->instr_macros, buf1, uint32_LE (header11.blen[1]), header.ffver, progress) != 0) goto _err_unpack;
      result_state = 2;
    }
    next_a2t_step (progress);

    if (header11.blen[2] != 0)
    {
      if (fread (buf1, uint32_LE (header11.blen[2]), 1, f) == 0) goto _err_fread;
      if (unpack_a2t_block (song->macro_table, buf1, uint32_LE (header11.blen[2]), header.ffver, progress) != 0) goto _err_unpack;
      result_state = 2;
    }
    next_a2t_step (progress);

    if (header11.blen[3] != 0)
    {
      if (fread (buf1, uint32_LE (header11.blen[3]), 1, f) == 0) goto _err_fread;
      if (unpack_a2t_block (song->dis_fmreg_col, buf1, uint32_LE (header11.blen[3]), header.ffver, progress) != 0) goto _err_unpack;
      result_state = 2;
    }
    next_a2t_step (progress);

    if (header11.blen[4] != 0)
    {
      if (fread (buf1, uint32_LE (header11.blen[4]), 1, f) == 0) goto _err_fread;
      if (unpack_a2t_block (song->pattern_order, buf1, uint32_LE (header11.blen[4]), header.ffver, progress) != 0) goto _err_unpack;
      result_state = 2;
    }
    next_a2t_step (progress);

    for (int i = 0; i < pat_blocks; i++)
    {
      if (header11.blen[5 + i] != 0)
      {
        if (fread (buf1, uint32_LE (header11.blen[5 + i]), 1, f) == 0) goto _err_fread;

        if ((i + 1) * 8 <= max_patterns)
        {
          if (unpack_a2t_block ((*pattdata)[i], buf1, uint32_LE (header11.blen[5 + i]), header.ffver, progress) != 0) goto _err_unpack;
          result_state = 2;
        }
        else limit_exceeded = true;
      }
      next_a2t_step (progress);
    }

    song->tempo = header.tempo;
    song->speed = header.speed;
    song->common_flag = header11.cflag;
    song->patt_len = uint16_LE (header11.patln);
    song->nm_tracks = header11.nmtrk;
    song->macro_speedup = uint16_LE (header11.mcspd);
    song->flag_4op = header11.is4op;
    memcpy (song->lock_flags, header11.locks, sizeof (song->lock_flags));
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

  DBG_LEAVE (); //EXIT //a2t_file_loader
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

_err_eod:
  result_error = "Unexpected end of unpacked data";
  goto _exit;
}
