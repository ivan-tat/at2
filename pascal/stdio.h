// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef STDIO_H
#define STDIO_H

#include "defines.h"

#include "pascal.h"
#include <stdarg.h>

typedef struct FILE_t FILE;
struct FILE_t {
  unsigned flags;
  bool (*m_write) (FILE *self, const void *ptr, size_t size);
  bool (*m_flush) (FILE *self);
  char *buf;       // buffer
  size_t buf_size; // buffer
  size_t pos;      // buffer
  size_t written;
  void *output;    // memory stream: memory pointer (direct mode)
                   // file stream: pointer to Pascal file/text structure
  size_t limit;    // memory stream: size limit (direct mode)
};

extern FILE *custom_stdout;

int custom_printf (const char *format, ...);
int custom_vprintf (const char *format, va_list ap);
int custom_fprintf (FILE *stream, const char *format, ...);
int custom_vfprintf (FILE *stream, const char *format, va_list ap);
int custom_sprintf (char *str, const char *format, ...);
int custom_vsprintf (char *str, const char *format, va_list ap);
int custom_snprintf (char *str, size_t size, const char *format, ...);
int custom_vsnprintf (char *str, size_t size, const char *format, va_list ap);

int custom_fflush (FILE *stream);

// Aliases

#define stdout custom_stdout

#define printf custom_printf
#define vprintf custom_vprintf
#define fprintf custom_fprintf
#define vfprintf custom_vfprintf
#define sprintf custom_sprintf
#define vsprintf custom_vsprintf
#define snprintf custom_snprintf
#define vsnprintf custom_vsnprintf

#define fflush custom_fflush

#endif // !defined(STDIO_H)
