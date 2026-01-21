// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024-2026 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef PASCAL_STDIO_H
#define PASCAL_STDIO_H

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

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

FILE *custom_fopen (const char *restrict name, const char *restrict mode);
int custom_fseek (FILE *stream, long offset, int whence);
long custom_ftell (FILE *stream);
size_t custom_fread (void *ptr, size_t size, size_t n, FILE *restrict stream);
size_t custom_fwrite (void *ptr, size_t size, size_t n, FILE *restrict stream);
int custom_fflush (FILE *stream);
void custom_fclose (FILE *stream);
int custom_remove (const char *path);

int custom_printf (const char *format, ...);
int custom_vprintf (const char *format, va_list ap);
int custom_fprintf (FILE *stream, const char *format, ...);
int custom_vfprintf (FILE *stream, const char *format, va_list ap);
int custom_sprintf (char *str, const char *format, ...);
int custom_vsprintf (char *str, const char *format, va_list ap);
int custom_snprintf (char *str, size_t size, const char *format, ...);
int custom_vsnprintf (char *str, size_t size, const char *format, va_list ap);

// Aliases

#define stdout custom_stdout

#define fopen custom_fopen
#define fseek custom_fseek
#define ftell custom_ftell
#define fread custom_fread
#define fwrite custom_fwrite
#define fflush custom_fflush
#define fclose custom_fclose
#define remove custom_remove

#define printf custom_printf
#define vprintf custom_vprintf
#define fprintf custom_fprintf
#define vfprintf custom_vfprintf
#define sprintf custom_sprintf
#define vsprintf custom_vsprintf
#define snprintf custom_snprintf
#define vsnprintf custom_vsnprintf

#endif // !defined(PASCAL_STDIO_H)
