// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

/*** CFF decoder ***/

// CFF decoder state

// flags
#define CFFD_FL_INFIN (1 << 0) // infinite input (reads zeroes outside input buffer)

#pragma pack(push, 1)
typedef struct { uint8_t len, str[255]; } cffd_string_t;
#pragma pack(pop)

typedef struct
{
  unsigned flags;
  const uint8_t *input;
  uint8_t *output;
  size_t input_size;
  size_t input_idx;
  size_t output_limit;
  size_t output_idx;  // actual output size
  cffd_string_t the_string;
  uint8_t code_length;
  int32_t bits_buffer;
  uint16_t bits_left;
  uint16_t heap_length;
  uint16_t dictionary_length;
  uint8_t heap[0x10000];
  uint16_t dictionary[0x8000];  // offset in `heap'
  char *error;
} cffds_t;

typedef bool cffd_status_t;
#define CFFD_SUCCESS false
#define CFFD_ERROR true

static cffd_status_t cffd_get_code (cffds_t *ds, int32_t *code)
{
  cffd_status_t status = CFFD_ERROR;
  int32_t result_code;

  while (ds->bits_left < ds->code_length)
  {
    if (ds->input_idx < ds->input_size)
      ds->bits_buffer |= ds->input[ds->input_idx++] << ds->bits_left;
    else if ((ds->flags & CFFD_FL_INFIN) == 0) goto _err_eod;
    ds->bits_left += 8;
  }

  result_code = ds->bits_buffer & ((1 << ds->code_length) - 1);
  ds->bits_buffer >>= ds->code_length;
  ds->bits_left -= ds->code_length;
  status = CFFD_SUCCESS;

_exit:
  if (status == CFFD_SUCCESS) *code = result_code;
  return status;

_err_eod:
  ds->error = "Unexpected end of packed data";
  goto _exit;
}

static void cffd_translate_code (cffds_t *ds, int32_t code, cffd_string_t *str)
{
  const cffd_string_t *translated_string;
#pragma pack(push, 1)
  struct { uint8_t len, str[1]; } translated_char;
#pragma pack(pop)

  if (code >= 0x104)
    translated_string = (const cffd_string_t *)&ds->heap[ds->dictionary[code - 0x104]];
  else
  {
    translated_char.len = 1;
    translated_char.str[0] = (code - 4) & 0xFF;
    translated_string = (const cffd_string_t *)&translated_char;
  }

  memcpy (str, translated_string, translated_string->len + 1);
}

static cffd_status_t cffd_startup (cffds_t *ds)
{
  cffd_status_t status = CFFD_ERROR;
  int32_t old_code;

  if (cffd_get_code (ds, &old_code) != CFFD_SUCCESS) goto _exit;
  cffd_translate_code (ds, old_code, &ds->the_string);

  for (int i = 0; i < ds->the_string.len; i++)
  {
    if (ds->output_idx >= ds->output_limit) goto _err_overrun;
    ds->output[ds->output_idx++] = ds->the_string.str[i];
  }

  status = CFFD_SUCCESS;

_exit:
  return status;

_err_overrun:
  ds->error = "Output buffer overrun";
  goto _exit;
}

static void cffd_cleanup (cffds_t *ds)
{
  ds->code_length = 9;
  ds->bits_buffer = 0;
  ds->bits_left = 0;
  ds->heap_length = 0;
  ds->dictionary_length = 0;
}

static cffd_status_t cffd_expand_dictionary (cffds_t *ds, const cffd_string_t *str)
{
  cffd_status_t status = CFFD_ERROR;

  if (str->len < 0xF0)
  {
    size_t size = str->len + 1;
    if ((size_t)ds->heap_length + size >= sizeof (ds->heap) / sizeof (ds->heap[0])) goto _err_heap;
    if ((size_t)ds->dictionary_length + 1 >= sizeof (ds->dictionary) / sizeof (ds->dictionary[0])) goto _err_dict;
    memcpy (&ds->heap[ds->heap_length], str, size);
    ds->dictionary[ds->dictionary_length++] = ds->heap_length;
    ds->heap_length += size;
  }
  status = CFFD_SUCCESS;

_exit:
  return status;

_err_heap:
  ds->error = "Decoder's heap overrun";
  goto _exit;

_err_dict:
  ds->error = "Decoder's dictionary overrun";
  goto _exit;
}

// In:
//   * `error' may be NULL.
//
// On success:
//   * Return value is 0.
//   * `out_size' is set.
//   * `error' is untouched.
// On error:
//   * Return value is -1.
//   * `out_size' is set.
//   * `error' (if set) is set to error description.
static int unpack_cff (void *output, size_t output_limit, size_t *out_size,
                       const void *input, size_t input_size, bool infinite,
                       char **error)
{
  int result = -1;
  size_t result_out_size = 0;
  char *result_error = NULL;
  cffds_t *ds = NULL;

  DBG_ENTER ("unpack_cff");

  if ((ds = malloc (sizeof (*ds))) == NULL) goto _err_malloc;

  ds->flags = infinite ? CFFD_FL_INFIN : 0;
  ds->input = input;
  ds->output = output;
  ds->input_size = input_size;
  ds->input_idx = 0;
  ds->output_limit = output_limit;
  ds->output_idx = 0;
  cffd_cleanup (ds);
  if (cffd_startup (ds) != CFFD_SUCCESS) goto _err_decoder;

  for (;;)
  {
    int32_t new_code;

    if (cffd_get_code (ds, &new_code) != CFFD_SUCCESS) goto _err_decoder;
    if (new_code == 0)  // 0x00: end of data
      break;
    else if (new_code == 1) // 0x01: end of block
    {
      cffd_cleanup (ds);
      if (cffd_startup (ds) != CFFD_SUCCESS) goto _err_decoder;
      continue;
    }
    else if (new_code == 2) // 0x02: expand code length
    {
      ds->code_length++;
      continue;
    }
    else if (new_code == 3) // 0x03: RLE
    {
      uint8_t old_code_length = ds->code_length;
      uint8_t repeat_length;
      int32_t repeat_counter;
      int32_t code;

      ds->code_length = 2;
      if (cffd_get_code (ds, &code) != CFFD_SUCCESS) goto _err_decoder;
      repeat_length = code + 1;
      if (cffd_get_code (ds, &code) != CFFD_SUCCESS) goto _err_decoder;
      ds->code_length = 4 << code;
      if (cffd_get_code (ds, &repeat_counter) != CFFD_SUCCESS) goto _err_decoder;

      for (uint_least16_t i = 0; i < repeat_counter * repeat_length; i++)
      {
        if (ds->output_idx >= ds->output_limit) goto _err_overrun;
        ds->output[ds->output_idx] = ds->output_idx >= repeat_length ? ds->output[ds->output_idx - repeat_length] : 0;
        ds->output_idx++;
      }

      ds->code_length = old_code_length;
      if (cffd_startup (ds) != CFFD_SUCCESS) goto _err_decoder;
      continue;
    }
    else if (new_code >= 0x104 + ds->dictionary_length)
      ds->the_string.str[ds->the_string.len++] = ds->the_string.str[0];
    else
    {
      cffd_string_t s;

      cffd_translate_code (ds, new_code, &s);
      ds->the_string.str[ds->the_string.len++] = s.str[0];
    }

    if (cffd_expand_dictionary (ds, &ds->the_string) != CFFD_SUCCESS) goto _err_decoder;
    cffd_translate_code (ds, new_code, &ds->the_string);

    for (int i = 0; i < ds->the_string.len; i++)
    {
      if (ds->output_idx >= ds->output_limit) goto _err_overrun;
      ds->output[ds->output_idx++] = ds->the_string.str[i];
    }
  }

  result = 0;
  result_out_size = ds->output_idx;

_exit:
  if (ds != NULL) free (ds);
  *out_size = result_out_size;
  if (result < 0)
    if (error != NULL) *error = result_error;

  DBG_LEAVE (); //EXIT //unpack_cff
  return result;

_err_malloc:
  result_error = "Memory allocation failed";
  goto _exit;

_err_overrun:
  result_error = "Output buffer overrun";
  goto _exit;

_err_decoder:
  result_error = ds->error;
  goto _exit;
}
