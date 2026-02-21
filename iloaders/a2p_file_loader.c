// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#define A2P_ID_SIZE 11

static const char GCC_ATTRIBUTE((nonstring)) a2p_id[A2P_ID_SIZE] = { "_A2pattern_" };

#pragma pack(push, 1)
typedef struct
{
  char id[A2P_ID_SIZE]; // ID string
  uint32_t crc32; // 32-bit cyclic redundancy check value
  uint8_t ffver;  // file format version
} tA2P_HEADER;

typedef struct  // file format version 1..8
{
  uint16_t blen;  // length of data block (pattern data + name)
} tA2P_HEADER_1;

typedef struct  // file format version 9..11
{
  uint32_t blen;  // length of data block (pattern data + name)
} tA2P_HEADER_9;
#pragma pack(pop)

// In: `dst_size' may be NULL.
static int unpack_a2p_block (void *dst, uint32_t *dst_size,
                             const void *src, uint32_t src_size,
                             uint8_t version, progress_callback_t *progress)
{
  int status = 0;
  uint32_t _dst_size = 0, unpacked_size;

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
      unpacked_size = APACK_decompress (src, dst);
      if (dst_size != NULL) *dst_size = unpacked_size;
      break;
    case 11:
      _dst_size = ((LZH_block_info_t *)src)->size;
      unpacked_size = LZH_decompress (src, dst, src_size, progress);
      if (dst_size != NULL) *dst_size = unpacked_size;
      if (unpacked_size != _dst_size) status = -1;
      break;
    default:
      status = -1;
      break;
  }
  return status;
}

// In: `name' may be NULL.
static void set_pattern_name (String *dst, const String *name, const String *def_name)
{
  String_t s = Copy (dst, 1, 11);
  AppendString ((String *)&s, name != NULL && Length (name) != 0 ? name : def_name, 42);
  CopyString (dst, (String *)&s, 42);
}

static void next_a2p_step (progress_callback_t *progress)
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
//   * Returns 0.
//   * `state' and `error' are untouched.
// On error:
//   * Return value:
//     -1: open/read error.
//     -2: unknown file format.
//     -3: unsupported file format version.
//     -4: format is supported.
//   * `state' (if set):
//     0: pattern(s) is(are) untouched.
//     1: pattern(s) is(are) cleared.
//     2: pattern(s) is(are) partly loaded.
//   * `error' (if set) is set to error description.
int8_t a2p_file_loader (const String *_fname, progress_callback_t *progress, uint8_t *state, char **error)
{
  tFIXED_SONGDATA *song = &songdata;
  uint8_t _pattern = pattern2use != UINT8_NULL ? pattern2use : pattern_patt;
  FILE *f = NULL;
  int8_t result = -1;
  uint8_t result_state = 0;
  char *result_error = NULL;
  uint32_t crc = CRC32_INITVAL;
  tA2P_HEADER header;
  String_t pat_name;
  char fname[255+1];
  String_t onlyname;

  DBG_ENTER ("a2p_file_loader");

  StringToStr (fname, _fname, sizeof (fname) - 1);
  {
    String_t s = NameOnly (_fname);
    onlyname = Lower ((String *)&s);
  }

  if ((f = fopen (fname, "rb")) == NULL) goto _err_fopen;
  if (fread (&header, sizeof (header), 1, f) == 0) goto _err_fread;

  result = -2;

  if (memcmp (header.id, a2p_id, sizeof (header.id)) != 0) goto _err_format;

  result = -3;
  header.crc32 = uint32_LE (header.crc32);

  if ((header.ffver < 1) || (header.ffver > 11)) goto _err_version;

  result = -4;
  memset (buf1, 0, sizeof (buf1));
  init_old_songdata ();

  if (header.ffver <= 4)
  {
    tA2P_HEADER_1 header1;

    if (fread (&header1, sizeof (header1), 1, f) == 0) goto _err_fread;

    if (progress != NULL) progress->num_steps = 2;

    // checksum
    if (fread (buf1, uint16_LE (header1.blen), 1, f) == 0) goto _err_fread;
    crc = Update32 (buf1, uint16_LE (header1.blen), crc);
    crc = Update32 (&header1.blen, sizeof (header1.blen), crc);
    if (crc != header.crc32) goto _err_checksum;
    next_a2p_step (progress);

    // pattern(s) (9x64)
    if (unpack_a2p_block (old_hash_buffer, NULL, buf1, uint16_LE (header1.blen), header.ffver, progress) != 0) goto _err_unpack;
    memset (adsr_carrier, false, sizeof (adsr_carrier));
    if ((pattern2use != UINT8_NULL) && (_patts_marked () != 0))
    {
      for (int pat = 0; pat < max_patterns; pat++)
        if (GetStr (song->pattern_names[pat])[0] != ' ')  // pattern is marked
        {
          for (uint8_t line = 0; line < 64; line++)
            for (uint8_t chan = 0; chan < 9; chan++)
              import_old_a2m_event1 (pat, line, chan + 1, old_hash_buffer[0][line][chan], false);

          set_pattern_name (song->pattern_names[pat], NULL, (String *)&onlyname);
          result_state = 2;
        }
    }
    else
    {
      for (uint8_t line = 0; line < 64; line++)
        for (uint8_t chan = 0; chan < 9; chan++)
          import_old_a2m_event1 (_pattern, line, chan + 1, old_hash_buffer[0][line][chan], false);

      set_pattern_name (song->pattern_names[_pattern], NULL, (String *)&onlyname);
    }
    next_a2p_step (progress);
  }
  else if (header.ffver <= 8)
  {
    tA2P_HEADER_1 header1;

    if (fread (&header1, sizeof (header1), 1, f) == 0) goto _err_fread;

    if (progress != NULL) progress->num_steps = 2;

    // checksum
    if (fread (buf1, uint16_LE (header1.blen), 1, f) == 0) goto _err_fread;
    crc = Update32 (buf1, uint16_LE (header1.blen), crc);
    crc = Update32 (&header1.blen, sizeof (header1.blen), crc);
    if (crc != header.crc32) goto _err_checksum;
    next_a2p_step (progress);

    // pattern(s) (18x64)
    if (unpack_a2p_block (hash_buffer[0], NULL, buf1, uint16_LE (header1.blen), header.ffver, progress) != 0) goto _err_unpack;
    if ((pattern2use != UINT8_NULL) && (_patts_marked () != 0))
    {
      for (int pat = 0; pat < max_patterns; pat++)
        if (GetStr (song->pattern_names[pat])[0] != ' ')  // pattern is marked
        {
          for (uint8_t line = 0; line < 64; line++)
            for (uint8_t chan = 0; chan < 18; chan++)
              import_old_a2m_event2 (pat, line, chan + 1, hash_buffer[0][chan][line]);

          set_pattern_name (song->pattern_names[pat], NULL, (String *)&onlyname);
          result_state = 2;
        }
    }
    else
    {
      for (uint8_t line = 0; line < 64; line++)
        for (uint8_t chan = 0; chan < 18; chan++)
          import_old_a2m_event2 (_pattern, line, chan + 1, hash_buffer[0][chan][line]);

      set_pattern_name (song->pattern_names[_pattern], NULL, (String *)&onlyname);
    }
    next_a2p_step (progress);
  }
  else if (header.ffver == 9)
  {
    tA2P_HEADER_9 header9;

    if (fread (&header9, sizeof (header9), 1, f) == 0) goto _err_fread;

    if (progress != NULL) progress->num_steps = 2;

    // checksum
    if (fread (buf1, uint32_LE (header9.blen), 1, f) == 0) goto _err_fread;
    crc = Update32 (buf1, uint32_LE (header9.blen), crc);
    crc = Update32 (&header9.blen, /*sizeof (header9.blen)*/ 2, crc);  // it's not a bug - it's a feature
    if (crc != header.crc32) goto _err_checksum;
    next_a2p_step (progress);

    // pattern(s) (20x256)
    if ((pattern2use != UINT8_NULL) && (_patts_marked () != 0))
    {
      for (int pat = 0; pat < max_patterns; pat++)
      {
        if (GetStr (song->pattern_names[pat])[0] != ' ')  // pattern is marked
        {
          if (unpack_a2p_block ((*pattdata)[pat / 8][pat % 8], NULL, buf1, uint32_LE (header9.blen), header.ffver, progress) != 0) goto _err_unpack;

          set_pattern_name (song->pattern_names[pat], NULL, (String *)&onlyname);
          result_state = 2;
        }
      }
    }
    else
    {
      if (unpack_a2p_block ((*pattdata)[_pattern / 8][_pattern % 8], NULL, buf1, uint32_LE (header9.blen), header.ffver, progress) != 0) goto _err_unpack;

      set_pattern_name (song->pattern_names[_pattern], NULL, (String *)&onlyname);
    }
    next_a2p_step (progress);
  }
  else if (header.ffver == 10)
  {
    tA2P_HEADER_9 header9;

    if (fread (&header9, sizeof (header9), 1, f) == 0) goto _err_fread;

    if (progress != NULL) progress->num_steps = 2;

    // checksum
    if (fread (buf1, uint32_LE (header9.blen), 1, f) == 0) goto _err_fread;
    crc = Update32 (buf1, uint32_LE (header9.blen), crc);
    crc = Update32 (&header9.blen, /*sizeof (header9.blen)*/ 2, crc);  // it's not a bug - it's a feature
    if (crc != header.crc32) goto _err_checksum;
    next_a2p_step (progress);

    // pattern(s) (20x256)
    if ((pattern2use != UINT8_NULL) && (_patts_marked () != 0))
    {
      for (int pat = 0; pat < max_patterns; pat++)
        if (GetStr (song->pattern_names[pat])[0] != ' ')  // pattern is marked
        {
          uint32_t unpacked_size = 0;
          mem_stream_t stream;

          if (unpack_a2p_block (buf2, &unpacked_size, buf1, uint32_LE (header9.blen), header.ffver, progress) != 0) goto _err_unpack;

          set_mem_stream (&stream, buf2, unpacked_size);
          if (read_bytes ((*pattdata)[pat / 8][pat % 8], PATTERN_SIZE, &stream)) goto _err_eod;
          result_state = 2;
          if (read_string ((String *)&pat_name, sizeof (pat_name) - 1, &stream)) goto _err_eod;
          set_pattern_name (song->pattern_names[pat], (String *)&pat_name, (String *)&onlyname);
        }
    }
    else
    {
      uint32_t unpacked_size = 0;
      mem_stream_t stream;

      if (unpack_a2p_block (buf2, &unpacked_size, buf1, uint32_LE (header9.blen), header.ffver, progress) != 0) goto _err_unpack;

      set_mem_stream (&stream, buf2, unpacked_size);
      if (read_bytes ((*pattdata)[_pattern / 8][_pattern % 8], PATTERN_SIZE, &stream)) goto _err_eod;
      result_state = 2;
      if (read_string ((String *)&pat_name, sizeof (pat_name) - 1, &stream)) goto _err_eod;
      set_pattern_name (song->pattern_names[_pattern], (String *)&pat_name, (String *)&onlyname);
    }
    next_a2p_step (progress);
  }
  else // header.ffver == 11
  {
    tA2P_HEADER_9 header9;

    if (fread (&header9, sizeof (header9), 1, f) == 0) goto _err_fread;

    if (progress != NULL) progress->num_steps = 2;

    // checksum
    if (fread (buf1, uint32_LE (header9.blen), 1, f) == 0) goto _err_fread;
    crc = Update32 (buf1, uint32_LE (header9.blen), crc);
    crc = Update32 (&header9.blen, /*sizeof (header9.blen)*/ 2, crc);  // it's not a bug - it's a feature
    if (crc != header.crc32) goto _err_checksum;
    next_a2p_step (progress);

    // pattern(s) (20x256)
    if ((pattern2use != UINT8_NULL) && (_patts_marked () != 0))
    {
      for (int pat = 0; pat < max_patterns; pat++)
        if (GetStr (song->pattern_names[pat])[0] != ' ')  // pattern is marked
        {
          uint32_t unpacked_size = 0;
          mem_stream_t stream;

          if (unpack_a2p_block (buf2, &unpacked_size, buf1, uint32_LE (header9.blen), header.ffver, progress) != 0) goto _err_unpack;

          set_mem_stream (&stream, buf2, unpacked_size);
          if (read_bytes ((*pattdata)[pat / 8][pat % 8], PATTERN_SIZE, &stream)) goto _err_eod;
          result_state = 2;
          if (read_string ((String *)&pat_name, sizeof (pat_name) - 1, &stream)) goto _err_eod;
          set_pattern_name (song->pattern_names[pat], (String *)&pat_name, (String *)&onlyname);
        }
    }
    else
    {
      uint32_t unpacked_size = 0;
      mem_stream_t stream;

      if (unpack_a2p_block (buf2, &unpacked_size, buf1, uint32_LE (header9.blen), header.ffver, progress) != 0) goto _err_unpack;

      set_mem_stream (&stream, buf2, unpacked_size);
      if (read_bytes ((*pattdata)[_pattern / 8][_pattern % 8], PATTERN_SIZE, &stream)) goto _err_eod;
      result_state = 2;
      if (read_string ((String *)&pat_name, sizeof (pat_name) - 1, &stream)) goto _err_eod;
      set_pattern_name (song->pattern_names[_pattern], (String *)&pat_name, (String *)&onlyname);
    }
    next_a2p_step (progress);
  }

  result = 0;

_exit:
  if (f != NULL) fclose (f);
  if (result < 0)
  {
    if (state != NULL) *state = result_state;
    if (error != NULL) *error = result_error;
  }

  DBG_LEAVE (); //EXIT //a2p_file_loader
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
