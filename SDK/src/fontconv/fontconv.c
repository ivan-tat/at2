// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#define _GNU_SOURCE
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if linux||__gnu_linux__||__linux||__linux__||unix||__unix||__unix__
# if _POSIX_C_SOURCE >= 200809L
#  define HAS_getline 1
# else
#  define HAS_getline 0
# endif
#endif

#define NL "\n"

#define PROGRAM "fontconv"
#define VERSION "0.1"
#define DESCRIPTION                                                          \
"Converter between PBM file and raw binary font file"
#define COPYRIGHT                                                            \
"Copyright (C) 2024 Ivan Tatarinov <ivan-tat" "@" "ya" "." "ru>"
#define HOMEPAGE                                                             \
"https://github.com/ivan-tat/at2/"
#define LICENSE                                                              \
"This program is free software: you can redistribute it and/or modify" NL    \
"it under the terms of the GNU General Public License as published by" NL    \
"the Free Software Foundation, either version 3 of the License, or" NL       \
"(at your option) any later version."

#define HELP_HINT                                                            \
"Use \"-h\" to get help."

#define FONT_WIDTH 8
#define FONT_HEIGHT_MIN 8
#define FONT_HEIGHT_MAX 16

//////////////////////////////////////////////////////////////////////////////

#define STD_ERR_LEN 128

char std_err[STD_ERR_LEN];

char *get_std_err (int err) {
  const char *s = strerror (err);

  if (!s)
    s = "<undefined>";

  snprintf (std_err, STD_ERR_LEN, "code %d: %s", err, s);
  return std_err;
}

//////////////////////////////////////////////////////////////////////////////

#define PROG_ERR_LEN 1024

const char *prog_err = NULL;

char prog_errf[PROG_ERR_LEN]; // run-time formatted error message

#define prog_set_err(s) prog_err = s

void prog_set_errf (const char *format, ...) {
  va_list ap;

  va_start (ap, format);
  vsnprintf (prog_errf, PROG_ERR_LEN, format, ap);
  va_end (ap);
  prog_err = prog_errf;
}

//////////////////////////////////////////////////////////////////////////////

// Return 0 on success, or error code otherwise
int read_pipe (char **ptr, size_t *size, size_t delta, FILE *stream) {
  int err = 0;
  size_t err_off;
  size_t err_size;
  char *_ptr = NULL;
  size_t _size = 0, capacity = 0;

  clearerr (stream);
  errno = 0;

  do {
    char *p;
    size_t nread;

    // Grow buffer
    capacity += delta;
    if (_ptr) {
      p = realloc (_ptr, capacity);
    } else
      p = malloc (capacity);
    if (!p) {
      err_size = capacity;
      goto _err_malloc;
    }
    _ptr = p;

    // Try to append data to buffer
    nread = fread (_ptr + _size, 1, capacity - _size, stream);
    _size += nread;
    if (ferror (stream)) {
      err_off = _size;
      goto _err_fread;
    }
  } while (!feof (stream));

  // Shrink buffer
  if (_size < capacity) {
    char *p;

    p = realloc (_ptr, _size);
    if (!p) {
      err_size = _size;
      goto _err_malloc;
    }
    _ptr = p;
  }

_exit:
  if (!err) {
    *ptr = _ptr;
    *size = _size;
  } else
    if (_ptr)
      free (_ptr);

  return err;

_err_malloc:
  prog_set_errf ("Memory allocation failed (%u bytes requested, %s)",
    err_size, get_std_err (errno));
  err = 1;
  goto _exit;

_err_fread:
  prog_set_errf ("Failed to read input (offset 0x%X, %s)",
    err_off, get_std_err (errno));
  err = 1;
  goto _exit;
}

//////////////////////////////////////////////////////////////////////////////

enum syntax_t {
  SYN_C = 0,
  SYN_PASCAL = 1
};

struct syntax_name_t {
  const char *name;
  enum syntax_t id;
} syntax_names [] = {
  { "c", SYN_C },
  { "pascal", SYN_PASCAL },
  { NULL }
};

// Returns 0 on success, -1 on error
int syntax_get_id (enum syntax_t *id, const char *name) {
  struct syntax_name_t *t = syntax_names;

  while (t->name) {
    if (strcmp (t->name, name) == 0) {
      *id = t->id;
      return 0;
    }
    t++;
  }

  return -1;
}

const char *syntax_get_name (enum syntax_t id) {
  struct syntax_name_t *t = syntax_names;

  while (t->name) {
    if (t->id == id)
      return t->name;
    t++;
  }

  return "unknown";
}

//////////////////////////////////////////////////////////////////////////////

// "filename" may be NULL
// Returns 0 on success, or error code otherwise
int dump_memory_to_file (const char *ptr, size_t size, enum syntax_t syntax,
                         const char *filename) {

  int err = 0;
  FILE *f = NULL;
  const char *src = ptr;
  char line[96];
  unsigned char b, d;

  if (filename) {
    f = fopen (filename, "w");
    if (!f)
      goto _err_fopen;
  } else
    f = stdout;

  switch (syntax) {
  case SYN_C:
    sprintf (line, "\t" "0x  ,0x  ,0x  ,0x  ,0x  ,0x  ,0x  ,0x  ,"
                        "0x  ,0x  ,0x  ,0x  ,0x  ,0x  ,0x  ,0x  ," NL);
    b = 3;
    d = 5;
    break;
  case SYN_PASCAL:
    sprintf (line, "\t" "$  ,$  ,$  ,$  ,$  ,$  ,$  ,$  ,"
                        "$  ,$  ,$  ,$  ,$  ,$  ,$  ,$  ," NL);
    b = 2;
    d = 4;
    break;
  default:
    goto _err_syntax;
  }

  while (size) {
    char *s = line;
    unsigned char n = (size >= 16) ? 16 : size, count;

    for (count = n; count; count--) {
      unsigned char x = *src;

      src++;
      s[b + 0] = ((x >> 4) <= 9) ? (x >> 4) + '0' : (x >> 4) + 'A' - 10;
      s[b + 1] = ((x & 15) <= 9) ? (x & 15) + '0' : (x & 15) + 'A' - 10;
      s += d;
    }

    if (size <= 16)
      sprintf (s, NL); // Remove last comma

    if (fprintf (f, "%s", line) < 0)
      goto _err_fwrite;

    size -= n;
  }

_exit:
  if (f && (f != stdout))
    fclose (f);

  return err;

_err_syntax:
  prog_set_errf ("Bad syntax specified \"%d\"",
    syntax);
  err = 1;
  goto _exit;

_err_fopen:
  prog_set_errf ("Failed to open input file \"%s\" (%s)",
    filename, get_std_err (errno));
  err = 1;
  goto _exit;

_err_fwrite:
  if (filename)
    prog_set_errf ("Failed to write output file \"%s\" (%s)",
      filename, get_std_err (errno));
  else
    prog_set_errf ("Failed to write output (%s)",
      get_std_err (errno));
  err = 1;
  goto _exit;
}

//////////////////////////////////////////////////////////////////////////////

struct font_t {
  unsigned char height;
  unsigned short count;
  size_t size;
  char *data;
};

#define FONT_ERR_LEN 1024

const char *font_err = NULL;

char font_errf[FONT_ERR_LEN]; // run-time formatted error message

#define font_set_err(s) font_err = s

void font_set_errf (const char *format, ...) {
  va_list ap;

  va_start (ap, format);
  vsnprintf (font_errf, FONT_ERR_LEN, format, ap);
  va_end (ap);
  font_err = font_errf;
}

struct font_t *font_new (void) {
  struct font_t *self = malloc (sizeof (struct font_t));

  if (self) {
    self->height = 0;
    self->count = 0;
    self->size = 0;
    self->data = NULL;
  } else
    font_set_errf ("Memory allocation failed (%u bytes requested, %s)",
      sizeof (struct font_t), get_std_err (errno));

  return self;
}

void font_free (struct font_t *self) {
  if (self->data)
    free (self->data);
  free (self);
}

// "count" may be 0
// "filename" may be NULL
// Returns 0 on success, or error code otherwise
int font_import_from_file (const char *filename, unsigned char height,
                           unsigned short count, struct font_t **font) {

  int err = 0;
  size_t err_size;
  FILE *f = NULL;
  struct font_t *_font = NULL;
  size_t _count; // must be large enough to hold actual characters count

  _font = font_new ();
  if (!_font)
    goto _err_font;

  _font->height = height;

  if (filename) {
    f = fopen (filename, "r");
    if (!f)
      goto _err_fopen;

    if (fseek (f, 0, SEEK_END))
      goto _err_fseek;

    _font->size = ftell (f);

    _count = _font->size / height;
    if (!_count)
      goto _err_empty;

    // Limit count
    if (count && (_count > count))
      _count = count;

    _font->count = _count;
    _font->size = _font->count * height;

    _font->data = malloc (_font->size);
    if (!_font->data) {
      err_size = _font->size;
      goto _err_malloc;
    }

    if (fseek (f, 0, SEEK_SET))
      goto _err_fseek;

    if (!fread (_font->data, _font->size, 1, f))
      goto _err_fread;
  } else {
    size_t nread;

    err = read_pipe (&_font->data, &nread, 4 * 1024, stdin);
    if (err)
      goto _err_prog;

    _count = nread / height;
    if (!_count)
      goto _err_empty;

    // Limit count
    if (count && (_count > count))
      _count = count;

    _font->count = _count;
    _font->size = _font->count * height;

    // Shrink buffer (adjust)
    if (_font->size < nread) {
      char *p;

      p = realloc (_font->data, _font->size);
      if (!p) {
        err_size = _font->size;
        goto _err_malloc;
      }
      _font->data = p;
    }
  }

_exit:
  if (f)
    fclose (f);
  if (!err)
    *font = _font;
  else
    if (_font)
      font_free (_font);

  return err;

_err_prog:
  font_set_errf ("%s", prog_err);
  err = 1;
  goto _exit;

_err_font:
  err = 1;
  goto _exit;

_err_fopen:
  font_set_errf ("Failed to open input file \"%s\" (%s)",
    filename, get_std_err (errno));
  err = 1;
  goto _exit;

_err_fseek:
  font_set_errf ("Failed to seek in input file \"%s\" (%s)",
    filename, get_std_err (errno));
  err = 1;
  goto _exit;

_err_fread:
  font_set_errf ("Failed to read input file \"%s\" (%s)",
    filename, get_std_err (errno));
  err = 1;
  goto _exit;

_err_malloc:
  font_set_errf ("Memory allocation failed (%u bytes requested, %s)",
    err_size, get_std_err (errno));
  err = 1;
  goto _exit;

_err_empty:
  if (filename)
    font_set_errf ("No characters in input font file \"%s\"",
      filename);
  else
    font_set_err ("No characters in input font");
  err = 1;
  goto _exit;
}

// "filename" may be NULL
// Returns 0 on success, or error code otherwise
int font_export_to_file (const struct font_t *self, const char *filename) {

  int err = 0;
  FILE *f = NULL;

  if (filename) {
    f = fopen (filename, "w");
    if (!f)
      goto _err_fopen;
  } else
    f = stdout;

  if (!fwrite (self->data, self->size, 1, f))
    goto _err_fwrite;

_exit:
  if (f && (f != stdout))
    fclose (f);

  return err;

_err_fopen:
  font_set_errf ("Failed to open output file \"%s\" (%s)",
    filename, get_std_err (errno));
  err = 1;
  goto _exit;

_err_fwrite:
  if (filename)
    font_set_errf ("Failed to write output file \"%s\" (%s)",
      filename, get_std_err (errno));
  else
    font_set_errf ("Failed to write output (%s)",
      get_std_err (errno));
  err = 1;
  goto _exit;
}

//////////////////////////////////////////////////////////////////////////////

#if !HAS_getline

struct text_t {
  char *data;
  size_t size;
  unsigned line_num;
  char *line_start;
  char *line_end;
  size_t line_len;
  char *next_line;
  char *end;
  bool eot;
};

void text_set (const char *data, size_t size, struct text_t *self) {
  self->data = (char *) data;
  self->size = size;
  self->line_num = 0;
  self->line_start = NULL;
  self->line_end = NULL;
  self->line_len = 0;
  self->next_line = NULL;
  self->end = (char *) data + size;
  self->eot = false;
}

char *skip_nl (const char *s, size_t n) {
  if (n) {
    if (*s == '\n')
      return (char *)s + 1;
    else if (*s == '\r') {
      if ((n > 1) && (s[1] == '\n'))
        return (char *)s + 2;
      else
        return (char *)s + 1;
    }
  }

  return (char *)s;
}

char *text_get_line (struct text_t *self) {
  const char *s;
  size_t n;

  if (self->eot) {
    if (self->line_start != self->end) {
      self->line_num++;
      self->line_start = self->end;
      self->line_len = 0;
      self->next_line = NULL;
    }
    return NULL;
  }

  if (self->line_start) {
    self->line_num++;
    self->line_start = self->next_line;
  } else {
    self->line_num = 1;
    self->line_start = self->data;
  }

  s = self->line_start;
  n = self->end - s;
  while (n) {
    if ((*s == '\n') || (*s == '\r')) {
      self->line_end = (char*) s;
      self->line_len = s - self->line_start;
      self->next_line = skip_nl (s, n);
      self->eot = (self->next_line == self->end);
      return self->line_start;
    }
    s++;
    n--;
  }

  self->line_end = (char*) s;
  self->line_len = s - self->line_start;
  self->next_line = NULL;
  self->eot = true;
  return self->line_start;
}

#endif // !HAS_getline

//////////////////////////////////////////////////////////////////////////////

enum pbmf_t {
  PBMF_TEXT = 0,
  PBMF_BINARY = 1
};

struct pbm_t {
  unsigned width, height;
  size_t line_size, size;
  char *data;
};

#define PBM_ERR_LEN 1024

const char *pbm_err = NULL;

char pbm_errf[PBM_ERR_LEN]; // run-time formatted error message

#define pbm_set_err(s) pbm_err = s

void pbm_set_errf (const char *format, ...) {
  va_list ap;

  va_start (ap, format);
  vsnprintf (pbm_errf, PBM_ERR_LEN, format, ap);
  va_end (ap);
  pbm_err = pbm_errf;
}

struct pbm_t *pbm_new (void) {
  struct pbm_t *self;

  self = malloc (sizeof (struct pbm_t));
  if (self) {
    self->width = 0;
    self->height = 0;
    self->line_size = 0;
    self->size = 0;
    self->data = NULL;
  } else {
    pbm_set_errf ("Memory allocation failed (%u bytes requested, %s)",
      sizeof (struct pbm_t), get_std_err (errno));
  }

  return self;
}

void pbm_free (struct pbm_t *self) {
  if (self->data)
    free (self->data);
  free (self);
}

#if !HAS_getline

// Returns 0 on success, or error code otherwise
int pbm_import_from_memory (const char *ptr, size_t size,
                            struct pbm_t **pbm) {

  int err = 0;
  size_t err_size, err_off;
  struct pbm_t *_pbm = NULL;
  struct text_t t;
  char *s, *endptr;
  enum pbmf_t format;
  long a;
  size_t _size;

  if (size < 8)
    goto _err_not_pbm;

  _pbm = pbm_new ();
  if (!_pbm)
    goto _err_pbm;

  text_set ((char *) ptr, size, &t);

  // Read format description
  s = text_get_line (&t);

  if (s && (t.line_len == 2) && (*s == 'P')) {
    if (s[1] == '1')
      format = PBMF_TEXT;
    else if (s[1] == '4')
      format = PBMF_BINARY;
    else
      goto _err_not_pbm;
  } else
    goto _err_not_pbm;

  // Skip comment lines
  do {
    s = text_get_line (&t);
    if ((!s) || (!t.line_len)) {
      err_off = t.line_end - t.data;
      goto _err_end;
    }
  } while (*s == '#');

  if (t.eot) {
    err_off = s - t.data;
    goto _err_end;
  }

  errno = 0;

  // Read width
  if (!isdigit (*s)) {
    err_off = s - t.data;
    goto _err_format;
  }
  a = strtol (s, &endptr, 10);
  if ((!errno) && endptr && (isblank (*endptr))
  &&  (a > 0) && (a <= INT_MAX)) {
    _pbm->width = a;
  } else {
    if (endptr)
      err_off = endptr - t.data;
    else
      err_off = s - t.data;
    goto _err_format;
  }

  // Read height
  s = endptr;
  a = strtol (s, &endptr, 10);
  if ((!errno) && (endptr == t.line_end)
  &&  (a > 0) && (a <= INT_MAX)) {
    _pbm->height = a;
  } else {
    if (endptr)
      err_off = endptr - t.data;
    else
      err_off = s - t.data;
    goto _err_format;
  }

  _pbm->line_size = (_pbm->width + 7) >> 3;
  _pbm->size = _pbm->line_size * _pbm->height;

  // Get data pointer
  s = t.next_line;
  if (s == t.end) {
    err_off = s - t.data;
    goto _err_end;
  }

  _size = size - (s - ptr);
  if ((format == PBMF_BINARY) && (_size < _pbm->size)) {
    err_off = size;
    goto _err_end;
  }

  _pbm->data = malloc (_pbm->size);
  if (!_pbm->data) {
    err_size = _pbm->size;
    goto _err_malloc;
  }

  if (format == PBMF_BINARY)
    memcpy (_pbm->data, s, _pbm->size);
  else {
    size_t count = 0;
    unsigned width = _pbm->width;
    unsigned char mask = 1 << 7, byte = 0;
    bool stop = false;

    text_set (s, _size, &t);

    // Read data
    while ((!t.eot) && (!stop)) {
      s = text_get_line (&t);

      while ((s < t.line_end) && (!stop)) {
        if ((*s == '0') || (*s == '1')) {
          // Form a byte
          if (*s == '1')
            byte |= mask;
          s++;
          width--;
          mask >>= 1;
          if ((!mask) || (!width)) {
            // Store byte
            _pbm->data[count] = byte;
            count++;
            if (!width)
              width = _pbm->width;
            mask = 1 << 7;
            byte = 0;
            stop = (count == _pbm->size);
          }
        } else if (*s == '#') {
          // Skip comment to EOL
          s = t.line_end;
        } else if (isblank (*s)) {
          // Skip blank
          s++;
        } else {
          err_off = s - t.data;
          goto _err_format;
        }
      }
    }
  }

_exit:
  if (!err)
    *pbm = _pbm;
  else
    if (_pbm)
      pbm_free (_pbm);

  return err;

_err_pbm:
  err = 1;
  goto _exit;

_err_not_pbm:
  pbm_set_err ("Input is not a PBM");
  err = 1;
  goto _exit;

_err_format:
  pbm_set_errf ("Bad input data format (offset 0x%X)",
    err_off);
  err = 1;
  goto _exit;

_err_malloc:
  pbm_set_errf ("Memory allocation failed (%u bytes requested, %s)",
    err_size, get_std_err (errno));
  err = 1;
  goto _exit;

_err_end:
  pbm_set_errf ("Unexpected end of input data (offset 0x%X)",
    err_off);
  err = 1;
  goto _exit;
}

// Returns 0 on success, or error code otherwise
int pbm_import_from_file (const char *filename, struct pbm_t **pbm) {

  int err = 0;
  size_t err_size;
  FILE *f = NULL;
  char *ptr = NULL;
  size_t size;

  if (!filename)
    goto _err_filename;

  f = fopen (filename, "r");
  if (!f)
    goto _err_fopen;

  if (fseek (f, 0, SEEK_END))
    goto _err_fseek;

  size = ftell (f);

  if (fseek (f, 0, SEEK_SET))
    goto _err_fseek;

  ptr = malloc (size);
  if (!ptr) {
    err_size = size;
    goto _err_malloc;
  }

  if (!fread (ptr, size, 1, f))
    goto _err_fread;

  err = pbm_import_from_memory (ptr, size, pbm);
  if (err)
    goto _exit;

  free (ptr);
  ptr = NULL;

_exit:
  if (f)
    fclose (f);
  if (err) {
    if (ptr)
      free (ptr);
  }

  return err;

_err_filename:
  pbm_set_errf ("No %s specified", "input filename");
  err = 1;
  goto _exit;

_err_fopen:
  pbm_set_errf ("Failed to open input file \"%s\" (%s)",
    filename, get_std_err (errno));
  err = 1;
  goto _exit;

_err_fseek:
  pbm_set_errf ("Failed to seek in input file \"%s\" (%s)",
    filename, get_std_err (errno));
  err = 1;
  goto _exit;

_err_fread:
  pbm_set_errf ("Failed to read input file \"%s\" (%s)",
    filename, get_std_err (errno));
  err = 1;
  goto _exit;

_err_malloc:
  pbm_set_errf ("Memory allocation failed (%u bytes requested, %s)",
    err_size, get_std_err (errno));
  err = 1;
  goto _exit;
}

#else // HAS_getline

// Replace new line delimiter with '\0' (if possible) and return string length
// (without trailing new line delimiter)
unsigned strip_nl (char *s, size_t max) {
  char *p = s;
  const char *end = s + max;

  while (p < end) {
    if ((*p == '\n') || (*p == '\r')) {
      *p = '\0';
      break;
    }
    p++;
  }

  return p - s;
}

// "filename" may be NULL
// Returns 0 on success, or error code otherwise
int pbm_import_from_file (const char *filename, struct pbm_t **pbm) {

  int err = 0;
  size_t err_off = 0;
  unsigned err_line = 1;
  size_t err_size;
  struct pbm_t *_pbm = NULL;
  FILE *f = NULL;
  size_t size = 0, len;
  ssize_t nread;
  char *line = NULL, *s, *end, *endptr;
  enum pbmf_t format;
  long a;

  _pbm = pbm_new ();
  if (!_pbm)
    goto _err_pbm;

  if (filename) {
    f = fopen (filename, "r");
    if (!f)
      goto _err_fopen;
  } else
    f = stdin;

  errno = 0;

  // Read format description line
  nread = getline (&line, &size, f);
  if (nread == -1)
    goto _err_fread;
  else if ((!nread) || (feof (f)))
    goto _err_end;

  // Parse it
  len = strip_nl (line, nread);
  s = line;
  if ((len == 2) && (*s == 'P')) {
    if (s[1] == '1')
      format = PBMF_TEXT;
    else if (s[1] == '4')
      format = PBMF_BINARY;
    else
      goto _err_not_pbm;
  } else
    goto _err_not_pbm;

  // Skip comment lines
  do {
    // Step forward (add parsed line)
    err_off += nread;
    err_line++;

    // Read line
    nread = getline (&line, &size, f);
    if (nread == -1)
      goto _err_fread;
    else if ((!nread) || (feof (f)))
      goto _err_end;
  } while (*line == '#');

  // Parse dimensions line
  len = strip_nl (line, nread);
  if (!len)
    goto _err_end;

  // Parse width
  end = line + len;
  s = line;
  if (!isdigit (*s))
    goto _err_format;
  a = strtol (s, &endptr, 10);
  if ((!errno) && endptr && (isblank (*endptr))
  &&  (a > 0) && (a <= INT_MAX)) {
    _pbm->width = a;
  } else {
    if (endptr)
      err_off += endptr - line;
    goto _err_format;
  }

  // Parse height
  s = endptr;
  a = strtol (s, &endptr, 10);
  if ((!errno) && (endptr == end)
  &&  (a > 0) && (a <= INT_MAX)) {
    _pbm->height = a;
  } else {
    if (endptr)
      err_off += endptr - line;
    else
      err_off += s - line;
    goto _err_format;
  }

  // Step forward (add parsed line)
  err_off += nread;
  err_line++;

  _pbm->line_size = (_pbm->width + 7) >> 3;
  _pbm->size = _pbm->line_size * _pbm->height;

  _pbm->data = malloc (_pbm->size);
  if (!_pbm->data) {
    err_size = _pbm->size;
    goto _err_malloc;
  }

  // Read bitmap
  if (format == PBMF_BINARY) {
    if (!fread (_pbm->data, _pbm->size, 1, f))
      goto _err_fread;
  } else {
    size_t count = 0;
    unsigned width = _pbm->width;
    unsigned char mask = 1 << 7, byte = 0;
    bool stop = false;

    do {
      // Read line
      nread = getline (&line, &size, f);
      if (nread == -1)
        goto _err_fread;
      else if (!nread)
        goto _err_end;

      // Parse it
      s = line;
      len = strip_nl (line, nread);
      end = line + len;
      while ((s < end) && (!stop)) {
        if ((*s == '0') || (*s == '1')) {
          // Form a byte
          if (*s == '1')
            byte |= mask;
          s++;
          width--;
          mask >>= 1;
          if ((!mask) || (!width)) {
            // Store byte
            _pbm->data[count] = byte;
            count++;
            if (!width)
              width = _pbm->width;
            mask = 1 << 7;
            byte = 0;
            stop = (count == _pbm->size);
          }
        } else if (*s == '#') {
          // Skip comment to EOL
          s = end;
        } else if (isblank (*s)) {
          // Skip blank
          s++;
        } else {
          err_off += s - line;
          goto _err_format;
        }
      }
      if (s == end) {
        // Step forward (add parsed line)
        err_off += nread;
        err_line++;
      }
    } while ((!feof (f)) && (!stop));

    if (!stop)
      goto _err_end;
  }

_exit:
  if (f && (f != stdin))
    fclose (f);
  if (line)
    free (line);
  if (!err)
    *pbm = _pbm;
  else
    if (_pbm)
      pbm_free (_pbm);

  return err;

_err_pbm:
  err = 1;
  goto _exit;

_err_fopen:
  pbm_set_errf ("Failed to open input file \"%s\" (%s)",
    filename, get_std_err (errno));
  err = 1;
  goto _exit;

_err_fread:
  if (filename)
    pbm_set_errf ("Failed to read input file \"%s\" (offset 0x%X, line %u, %s)",
      filename, err_off, err_line, get_std_err (errno));
  else
    pbm_set_errf ("Failed to read input (offset 0x%X, line %u, %s)",
      err_off, err_line, get_std_err (errno));
  err = 1;
  goto _exit;

_err_malloc:
  pbm_set_errf ("Memory allocation failed (%u bytes requested, %s)",
    err_size, get_std_err (errno));
  err = 1;
  goto _exit;

_err_not_pbm:
  if (filename)
    pbm_set_errf ("Input file \"%s\" is not a PBM",
      filename);
  else
    pbm_set_err ("Input is not a PBM");
  err = 1;
  goto _exit;

_err_end:
  if (filename)
    pbm_set_errf ("Unexpected end of input file \"%s\" (offset 0x%X, line %u)",
      filename, err_off, err_line);
  else
    pbm_set_errf ("Unexpected end of input (offset 0x%X, line %u)",
      err_off, err_line);
  err = 1;
  goto _exit;

_err_format:
  if (filename)
    pbm_set_errf ("Bad input file \"%s\" format (offset 0x%X, line %u)",
      filename, err_off, err_line);
  else
    pbm_set_errf ("Bad input format (offset 0x%X, line %u)",
      err_off, err_line);
  err = 1;
  goto _exit;
}

#endif // HAS_getline

// "wrap" may be 0
// "filename" may be NULL
// Returns 0 on success, or error code otherwise
int pbm_export_to_file (const struct pbm_t *self, enum pbmf_t mode,
                        unsigned wrap, const char *filename) {

  int err = 0;
  size_t err_size;
  FILE *f = NULL;
  char *buf = NULL;

  if (filename) {
    f = fopen (filename, "w");
    if (!f)
      goto _err_fopen;
  } else
    f = stdout;

  if (mode == PBMF_TEXT) {
    if (fprintf (f, "P1\n%u %u\n", self->width, self->height) < 0)
      goto _err_fwrite;
  } else {
    if (fprintf (f, "P4\n%u %u\n", self->width, self->height) < 0)
      goto _err_fwrite;
  }

  if (mode == PBMF_TEXT) {
    size_t count = self->width * self->height;
    size_t buf_len = wrap ? wrap : self->width, i = 0;
    unsigned y;

    buf = malloc (buf_len + 1); // '\n' at the end
    if (!buf) {
      err_size = buf_len + 1;
      goto _err_malloc;
    }

    buf[buf_len] = '\n';

    for (y = 0; y < self->height; y++) {
      const char *src = self->data + self->line_size * y;
      unsigned char byte = 0;
      unsigned x;

      for (x = 0; x < self->width; x++) {
        if (!(x & 7)) {
          // Read 8 pixels
          byte = *src;
          src++;
        }

        // Store 1 pixel
        buf[i] = (byte & 0x80) ? '1' : '0';
        byte <<= 1;
        count--;

        i++;
        if (i == buf_len) {
          // Flush buffer
          size_t len = buf_len + (count ? 1 : 0); // Check end
          if (!fwrite (buf, len, 1, f))
            goto _err_fwrite;
          i = 0;
        }
      }
    }

    if (i) {
      // Flush buffer
      if (!fwrite (buf, i, 1, f))
        goto _err_fwrite;
    }
  } else {
    if (!fwrite (self->data, self->size, 1, f))
      goto _err_fwrite;
  }

_exit:
  if (f && (f != stdout))
    fclose (f);
  if (buf)
    free (buf);

  return err;

_err_fopen:
  pbm_set_errf ("Failed to open output file \"%s\" (%s)",
    filename, get_std_err (errno));
  err = 1;
  goto _exit;

_err_fwrite:
  if (filename)
    pbm_set_errf ("Failed to write output file \"%s\" (%s)",
      filename, get_std_err (errno));
  else
    pbm_set_errf ("Failed to write output (%s)",
      get_std_err (errno));
  err = 1;
  goto _exit;

_err_malloc:
  pbm_set_errf ("Memory allocation failed (%u bytes requested, %s)",
    err_size, get_std_err (errno));
  err = 1;
  goto _exit;
}

//////////////////////////////////////////////////////////////////////////////

// "columns" may be 0
// Returns 0 on success, or error code otherwise
int convert_font_to_pbm (const struct font_t *font, unsigned short columns,
                         struct pbm_t **pbm) {

  int err = 0;
  size_t err_size;
  struct pbm_t *_pbm = NULL;
  const char *src = font->data;
  char *dst;
  unsigned n, x = 0, y = 0;

  _pbm = pbm_new ();
  if (!_pbm)
    goto _err_pbm;

  if (columns && (font->count > columns)) {
    _pbm->width = columns * FONT_WIDTH;
    _pbm->height = ((font->count + columns - 1) / columns) * font->height;
  } else {
    _pbm->width = font->count * FONT_WIDTH;
    _pbm->height = font->height;
  }

  _pbm->line_size = (_pbm->width + 7) >> 3;
  _pbm->size = _pbm->line_size * _pbm->height;

  _pbm->data = malloc (_pbm->size);
  if (!_pbm->data) {
    err_size = _pbm->size;
    goto _err_malloc;
  }
  memset (_pbm->data, 0, _pbm->size);

  dst = _pbm->data;
  for (n = font->count; n; n--) {
    unsigned i;

    for (i = font->height; i; i--) {
      *dst = *src;
      src++;
      dst += _pbm->line_size;
    }
    x += FONT_WIDTH;
    if (x >= _pbm->width) {
      x = 0;
      y += font->height;
    }
    dst = _pbm->data + y * _pbm->line_size + (x >> 3);
  }

_exit:
  if (!err)
    *pbm = _pbm;
  else
    if (_pbm)
      pbm_free (_pbm);

  return err;

_err_pbm:
  err = 1;
  goto _exit;

_err_malloc:
  prog_set_errf ("Memory allocation failed (%u bytes requested, %s)",
    err_size, get_std_err (errno));
  err = 1;
  goto _exit;
}

//////////////////////////////////////////////////////////////////////////////

// "count" may be 0
// Returns 0 on success, or error code otherwise
int convert_pbm_to_font (const struct pbm_t *pbm, unsigned char height,
                         unsigned short count, struct font_t **font) {

  int err = 0;
  size_t err_size;
  struct font_t *_font = NULL;
  const char *src;
  char *dst;
  unsigned x, y, n;

  if ((pbm->width < FONT_WIDTH) || (pbm->height < height))
    goto _err_size;

  _font = font_new ();
  if (!_font)
    goto _err_font;

  _font->height = height;
  _font->count = (pbm->width / FONT_WIDTH) * (pbm->height / _font->height);

  // Limit count
  if (count && (_font->count > count))
    _font->count = count;

  _font->size = _font->count * _font->height;
  _font->data = malloc (_font->size);
  if (!_font->data) {
    err_size = _font->size;
    goto _err_malloc;
  }

  src = pbm->data;
  dst = _font->data;
  x = 0;
  y = 0;
  for (n = _font->count; n; n--) {
    unsigned i;

    for (i = _font->height; i; i--) {
      *dst = *src;
      src += pbm->line_size;
      dst++;
    }
    x += FONT_WIDTH;
    if (x >= pbm->width) {
      x = 0;
      y += _font->height;
    }
    src = pbm->data + y * pbm->line_size + (x >> 3);
  }

_exit:
  if (!err)
    *font = _font;
  else
    if (_font)
      font_free (_font);

  return err;

_err_size:
  prog_set_err ("Image's dimensions are less than character's dimensions");
  err = 1;
  goto _exit;

_err_font:
  prog_set_errf ("%s", font_err);
  err = 1;
  goto _exit;

_err_malloc:
  prog_set_errf ("Memory allocation failed (%u bytes requested, %s)",
    err_size, get_std_err (errno));
  err = 1;
  goto _exit;
}

//////////////////////////////////////////////////////////////////////////////

enum action_t {
  ACT_NONE = 0,
  ACT_PBM_TO_INC,
  ACT_PBM_TO_PBM,
  ACT_PBM_TO_RAW,
  ACT_RAW_TO_INC,
  ACT_RAW_TO_PBM
};

bool opt_help = false;
enum action_t opt_action = ACT_NONE;
unsigned short opt_columns = 32;
unsigned char opt_height = 16;
unsigned short opt_count = 0;
enum pbmf_t opt_mode = PBMF_TEXT;
enum syntax_t opt_syntax = SYN_C;
unsigned opt_wrap = 0;
const char *opt_input = NULL;
const char *opt_output = NULL;

void show_usage (void) {
  printf (
    PROGRAM " version " VERSION
    " (compiled on " __DATE__ " at " __TIME__")" NL
    DESCRIPTION NL
    COPYRIGHT NL
    "Home page: <" HOMEPAGE ">" NL
    LICENSE NL
    NL
    "Usage:" NL
    "  " PROGRAM " [option ...] [--] [INPUT]" NL
    NL
    "Options (default value):" NL
    "  -h         Show this help and quit" NL
    "  -pbmtoinc  Convert PBM file into source file" NL
    "  -pbmtopbm  Convert PBM file into PBM file" NL
    "  -pbmtoraw  Convert PBM file into raw binary font file" NL
    "  -rawtoinc  Convert raw binary font file into source file" NL
    "  -rawtopbm  Convert raw binary font file into PBM file" NL
  );
  printf (
    "  -c N       Number of characters on a line in output bitmap:" NL
    "             N=0..%hu; 0=all (%hu)" NL,
    (unsigned short) USHRT_MAX,
    (unsigned short) opt_columns
  );
  printf (
    "  -f N       Height of a character in pixels: N=%hhu..%hhu (%hhu)" NL,
    (unsigned char) FONT_HEIGHT_MIN,
    (unsigned char) FONT_HEIGHT_MAX,
    (unsigned char) opt_height
  );
  printf (
    "  -n N       Limit the number of characters to proceed:" NL
    "             N=0..%hu; 0=all (%hu)" NL,
    (unsigned short) USHRT_MAX,
    (unsigned short) opt_count
  );
  printf (
    "  -m N       Output bitmap mode: N=0, 1; 0=text, 1=binary (%hhu)" NL,
    (unsigned char) opt_mode
  );
  printf (
    "  -w N       Wrap output lines of bitmap after each N pixels in text mode." NL
    "             N=0, 8..%u; 0=use actual bitmap width (%u)" NL,
    (unsigned) INT_MAX,
    (unsigned) opt_wrap
  );
  printf (
    "  -s SYNTAX  Use syntax for output source file: c, pascal (%s)" NL,
    (char *) syntax_get_name (opt_syntax)
  );
  printf (
    "  -o OUTPUT  Output filename. If not specified - standard output used." NL
    "  --         Stop parsing options and treat remaining arguments as filenames" NL
    NL
    "  INPUT      Input filename. If not specified - standard input is used." NL
    NL
  );
  printf (
    "Notice: the width of a font is %hhu pixels." NL,
    (unsigned char) FONT_WIDTH
  );
}

void message (const char *format, ...) {
  va_list ap;

  va_start (ap, format);
  fprintf (stderr, PROGRAM ": ");
  vfprintf (stderr, format, ap);
  fprintf (stderr, "\n");
  va_end (ap);
}

void error (const char *format, ...) {
  va_list ap;

  va_start (ap, format);
  fprintf (stderr, PROGRAM ": ERROR: ");
  vfprintf (stderr, format, ap);
  fprintf (stderr, "\n");
  va_end (ap);
}

// Returns 0 on success, -1 on error
int parse_arg_long (long *a, const char *arg) {
  long _a;
  char *endptr;

  _a = strtol (arg, &endptr, 10);
  if (errno || !endptr)
    return -1;

  *a = _a;
  return 0;
}

// Returns 0 on success, -1 on error, 1 if no arguments
int parse_args (int argc, char **argv) {
  char *err_argv;
  int err_argi;
  bool opt;
  int i, f;

  if (argc == 1)
    return 1;

  opt = true;
  i = 1;
  f = 0;
  while (i < argc) {
    bool is_opt;

    if (opt) {
      is_opt = true;

      if (strcmp (argv[i], "--") == 0)
        opt = false;
      else if (argv[i][0] == '-') {
        if (strcmp (&argv[i][1], "h") == 0) {
          opt_help = true;
        } else if (strcmp (&argv[i][1], "pbmtoinc") == 0) {
          if (opt_action != ACT_NONE) {
            err_argv = argv[i];
            err_argi = i;
            goto _err_action;
          }
          opt_action = ACT_PBM_TO_INC;
        } else if (strcmp (&argv[i][1], "pbmtopbm") == 0) {
          if (opt_action != ACT_NONE) {
            err_argv = argv[i];
            err_argi = i;
            goto _err_action;
          }
          opt_action = ACT_PBM_TO_PBM;
        } else if (strcmp (&argv[i][1], "pbmtoraw") == 0) {
          if (opt_action != ACT_NONE) {
            err_argv = argv[i];
            err_argi = i;
            goto _err_action;
          }
          opt_action = ACT_PBM_TO_RAW;
        } else if (strcmp (&argv[i][1], "rawtoinc") == 0) {
          if (opt_action != ACT_NONE) {
            err_argv = argv[i];
            err_argi = i;
            goto _err_action;
          }
          opt_action = ACT_RAW_TO_INC;
        } else if (strcmp (&argv[i][1], "rawtopbm") == 0) {
          if (opt_action != ACT_NONE) {
            err_argv = argv[i];
            err_argi = i;
            goto _err_action;
          }
          opt_action = ACT_RAW_TO_PBM;
        } else if (strcmp (&argv[i][1], "c") == 0) {
          long a;

          if (i + 1 == argc) {
            err_argv = argv[i];
            err_argi = i;
            goto _err_no_arg;
          }

          if (parse_arg_long (&a, argv[i + 1])
          ||  (a < 0) || (a > USHRT_MAX)) {
            err_argv = argv[i];
            err_argi = i;
            goto _err_bad_arg;
          }

          opt_columns = a;
          i++;

        } else if (strcmp (&argv[i][1], "f") == 0) {
          long a;

          if (i + 1 == argc) {
            err_argv = argv[i];
            err_argi = i;
            goto _err_no_arg;
          }

          if (parse_arg_long (&a, argv[i + 1])
          ||  (a < FONT_HEIGHT_MIN) || (a > FONT_HEIGHT_MAX)) {
            err_argv = argv[i];
            err_argi = i;
            goto _err_bad_arg;
          }

          opt_height = a;
          i++;

        } else if (strcmp (&argv[i][1], "n") == 0) {
          long a;

          if (i + 1 == argc) {
            err_argv = argv[i];
            err_argi = i;
            goto _err_no_arg;
          }

          if (parse_arg_long (&a, argv[i + 1])
          ||  (a < 0) || (a > USHRT_MAX)) {
            err_argv = argv[i];
            err_argi = i;
            goto _err_bad_arg;
          }

          opt_count = a;
          i++;

        } else if (strcmp (&argv[i][1], "m") == 0) {
          long a;

          if (i + 1 == argc) {
            err_argv = argv[i];
            err_argi = i;
            goto _err_no_arg;
          }

          if (parse_arg_long (&a, argv[i + 1])
          ||  ((a != PBMF_TEXT) && (a != PBMF_BINARY))) {
            err_argv = argv[i];
            err_argi = i;
            goto _err_bad_arg;
          }

          opt_mode = a;
          i++;

        } else if (strcmp (&argv[i][1], "w") == 0) {
          long a;

          if (i + 1 == argc) {
            err_argv = argv[i];
            err_argi = i;
            goto _err_no_arg;
          }

          if (parse_arg_long (&a, argv[i + 1])
          ||  (a < 0) || ((a >= 1) && (a <= 7)) || (a > INT_MAX)) {
            err_argv = argv[i];
            err_argi = i;
            goto _err_bad_arg;
          }

          opt_wrap = a;
          i++;

        } else if (strcmp (&argv[i][1], "s") == 0) {
          enum syntax_t a;

          if (i + 1 == argc) {
            err_argv = argv[i];
            err_argi = i;
            goto _err_no_arg;
          }

          if (syntax_get_id (&a, argv[i + 1])) {
            err_argv = argv[i];
            err_argi = i;
            goto _err_bad_arg;
          }

          opt_syntax = a;
          i++;

        } else if (strcmp (&argv[i][1], "o") == 0) {
          if (i + 1 == argc) {
            err_argv = argv[i];
            err_argi = i;
            goto _err_no_arg;
          }

          if (opt_output) {
            err_argv = argv[i + 1];
            err_argi = i;
            goto _err_output;
          }

          opt_output = argv[i + 1];
          i++;

        } else {
          err_argv = argv[i];
          err_argi = i;
          goto _err_opt;
        }
      } else
        is_opt = false;
    } else
      is_opt = false;

    if (!is_opt) {
      switch (f) {
      case 0:
        opt_input = argv[i];
        break;
      default:
        err_argv = argv[i];
        err_argi = i;
        goto _err_input;
      }
      f++;
    }

    i++;
  }

  return 0;

_err_action:
  prog_set_errf ("Extra conversion type \"%s\" specified (argument %u).",
    err_argv, err_argi);
  return -1;

_err_no_arg:
  prog_set_errf ("Missing parameter for \"%s\" (argument %u).",
    err_argv, err_argi);
  return -1;

_err_bad_arg:
  prog_set_errf ("Bad parameter value for \"%s\" (argument %u).",
    err_argv, err_argi);
  return -1;

_err_opt:
  prog_set_errf ("Unknown option \"%s\" (argument %u)",
    err_argv, err_argi);
  return -1;

_err_input:
  prog_set_errf ("Extra input file \"%s\" given (argument %u)",
    err_argv, err_argi);
  return -1;

_err_output:
  prog_set_errf ("Extra output file \"%s\" given (argument %u)",
    err_argv, err_argi);
  return -1;
}

//////////////////////////////////////////////////////////////////////////////

// Returns 0 on success, or error code otherwise
int convert_pbm_to_inc () {
  int err;
  struct pbm_t *pbm = NULL;
  struct font_t *font = NULL;

  err = pbm_import_from_file (opt_input, &pbm);
  if (err)
    prog_set_errf ("%s", pbm_err);
  else {
    err = convert_pbm_to_font (pbm, opt_height, opt_count, &font);
    if (err)
      prog_set_errf ("%s", font_err);
    else {
      pbm_free (pbm);
      pbm = NULL;

      err = dump_memory_to_file (font->data, font->size, opt_syntax,
                                 opt_output);
    }
  }

  if (pbm)
    pbm_free (pbm);
  if (font)
    font_free (font);

  return err;
}

//////////////////////////////////////////////////////////////////////////////

// Returns 0 on success, or error code otherwise
int convert_pbm_to_pbm () {
  int err;
  struct pbm_t *pbm_in = NULL;
  struct font_t *font = NULL;
  struct pbm_t *pbm_out = NULL;

  err = pbm_import_from_file (opt_input, &pbm_in);
  if (err)
    prog_set_errf ("%s", pbm_err);
  else {
    err = convert_pbm_to_font (pbm_in, opt_height, opt_count, &font);
    if (err)
      prog_set_errf ("%s", font_err);
    else {
      pbm_free (pbm_in);
      pbm_in = NULL;

      err = convert_font_to_pbm (font, opt_columns, &pbm_out);
      if (!err) {
        font_free (font);
        font = NULL;

        err = pbm_export_to_file (pbm_out, opt_mode, opt_wrap, opt_output);
        if (err)
          prog_set_errf ("%s", pbm_err);
      }
    }
  }

  if (pbm_in)
    pbm_free (pbm_in);
  if (font)
    font_free (font);
  if (pbm_out)
    pbm_free (pbm_out);

  return err;
}

//////////////////////////////////////////////////////////////////////////////

// Returns 0 on success, or error code otherwise
int convert_pbm_to_raw () {
  int err;
  struct pbm_t *pbm = NULL;
  struct font_t *font = NULL;

  err = pbm_import_from_file (opt_input, &pbm);
  if (err)
    prog_set_errf ("%s", pbm_err);
  else {
    err = convert_pbm_to_font (pbm, opt_height, opt_count, &font);
    if (!err) {
      pbm_free (pbm);
      pbm = NULL;

      err = font_export_to_file (font, opt_output);
      if (err)
        prog_set_errf ("%s", font_err);
    }
  }

  if (pbm)
    pbm_free (pbm);
  if (font)
    font_free (font);

  return err;
}

//////////////////////////////////////////////////////////////////////////////

// Returns 0 on success, or error code otherwise
int convert_raw_to_inc () {
  int err;
  struct font_t *font = NULL;

  err = font_import_from_file (opt_input, opt_height, opt_count, &font);
  if (err)
    prog_set_errf ("%s", font_err);
  else
    err = dump_memory_to_file (font->data, font->size, opt_syntax,
                               opt_output);

  if (font)
    font_free (font);

  return err;
}

//////////////////////////////////////////////////////////////////////////////

// Returns 0 on success, or error code otherwise
int convert_raw_to_pbm () {
  int err;
  struct font_t *font = NULL;
  struct pbm_t *pbm = NULL;

  err = font_import_from_file (opt_input, opt_height, opt_count, &font);
  if (err)
    prog_set_errf ("%s", font_err);
  else {
    err = convert_font_to_pbm (font, opt_columns, &pbm);
    if (!err) {
      font_free (font);
      font = NULL;

      err = pbm_export_to_file (pbm, opt_mode, opt_wrap, opt_output);
      if (err)
        prog_set_errf ("%s", pbm_err);
    }
  }

  if (font)
    font_free (font);
  if (pbm)
    pbm_free (pbm);

  return err;
}

//////////////////////////////////////////////////////////////////////////////

// Returns: 0=success, or error code otherwise
int main (int argc, char **argv) {
  int err = parse_args (argc, argv);

  switch (err) {
  case 0:
    break;
  case 1:
    message ("No parameters. " HELP_HINT);
    err = 0;
    goto _exit;
  default:
    err = -1;
    goto _exit;
  }

  if (opt_help)
    show_usage ();
  else
    switch (opt_action) {
    case ACT_NONE:
      prog_set_errf ("No %s specified", "conversion type");
      err = -1;
      break;
    case ACT_PBM_TO_INC: err = convert_pbm_to_inc (); break;
    case ACT_PBM_TO_PBM: err = convert_pbm_to_pbm (); break;
    case ACT_PBM_TO_RAW: err = convert_pbm_to_raw (); break;
    case ACT_RAW_TO_INC: err = convert_raw_to_inc (); break;
    case ACT_RAW_TO_PBM: err = convert_raw_to_pbm (); break;
    default:
      prog_set_err ("Conversion is not implemented");
      err = -1;
      break;
    }

_exit:
  if (err)
    error ("%s", prog_err);

  return err;
}
