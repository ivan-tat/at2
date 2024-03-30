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

#define NL "\n"

#define PROGRAM "bintoinc"
#define VERSION "0.1"
#define DESCRIPTION                                                          \
"Binary data to source code file converter"
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

struct data_t {
  size_t size;
  char *data;
};

#define DATA_ERR_LEN 1024

const char *data_err = NULL;

char data_errf[DATA_ERR_LEN]; // run-time formatted error message

#define data_set_err(s) data_err = s

void data_set_errf (const char *format, ...) {
  va_list ap;

  va_start (ap, format);
  vsnprintf (data_errf, DATA_ERR_LEN, format, ap);
  va_end (ap);
  data_err = data_errf;
}

struct data_t *data_new (void) {
  struct data_t *self = malloc (sizeof (struct data_t));

  if (self) {
    self->size = 0;
    self->data = NULL;
  } else
    data_set_errf ("Memory allocation failed (%u bytes requested, %s)",
      sizeof (struct data_t), get_std_err (errno));

  return self;
}

void data_free (struct data_t *self) {
  if (self->data)
    free (self->data);
  free (self);
}

// "count" may be 0
// "filename" may be NULL
// Returns 0 on success, or error code otherwise
int data_import_from_file (const char *filename, struct data_t **data) {

  int err = 0;
  size_t err_size;
  FILE *f = NULL;
  struct data_t *_data = NULL;

  _data = data_new ();
  if (!_data)
    goto _err_font;

  if (filename) {
    f = fopen (filename, "r");
    if (!f)
      goto _err_fopen;

    if (fseek (f, 0, SEEK_END))
      goto _err_fseek;

    _data->size = ftell (f);

    _data->data = malloc (_data->size);
    if (!_data->data) {
      err_size = _data->size;
      goto _err_malloc;
    }

    if (fseek (f, 0, SEEK_SET))
      goto _err_fseek;

    if (!fread (_data->data, _data->size, 1, f))
      goto _err_fread;
  } else {
    err = read_pipe (&_data->data, &_data->size, 16 * 1024, stdin);
    if (err)
      goto _err_prog;
  }

_exit:
  if (f)
    fclose (f);
  if (!err)
    *data = _data;
  else
    if (_data)
      data_free (_data);

  return err;

_err_prog:
  data_set_errf ("%s", prog_err);
  err = 1;
  goto _exit;

_err_font:
  err = 1;
  goto _exit;

_err_fopen:
  data_set_errf ("Failed to open input file \"%s\" (%s)",
    filename, get_std_err (errno));
  err = 1;
  goto _exit;

_err_fseek:
  data_set_errf ("Failed to seek in input file \"%s\" (%s)",
    filename, get_std_err (errno));
  err = 1;
  goto _exit;

_err_fread:
  data_set_errf ("Failed to read input file \"%s\" (%s)",
    filename, get_std_err (errno));
  err = 1;
  goto _exit;

_err_malloc:
  data_set_errf ("Memory allocation failed (%u bytes requested, %s)",
    err_size, get_std_err (errno));
  err = 1;
  goto _exit;
}

//////////////////////////////////////////////////////////////////////////////

bool opt_help = false;
enum syntax_t opt_syntax = SYN_C;
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
int convert_data_to_inc () {
  int err;
  struct data_t *data = NULL;

  err = data_import_from_file (opt_input, &data);
  if (err)
    prog_set_errf ("%s", data_err);
  else
    err = dump_memory_to_file (data->data, data->size, opt_syntax,
                               opt_output);

  if (data)
    data_free (data);

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
    err = convert_data_to_inc ();

_exit:
  if (err)
    error ("%s", prog_err);

  return err;
}
