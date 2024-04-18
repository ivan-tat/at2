// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pascal/stdio.h"
#include "pascal/stdlib.h"
#include "pascal/string.h"

// Stream flags

#define _SF_S_MASK (1 << 0) // Stream structure allocation
#define _SF_S_STAT (0 << 0)
#define _SF_S_HEAP (1 << 0)
#define _SF_O_MASK (1 << 1) // Output variable allocation (buffered mode)
#define _SF_O_STAT (0 << 1)
#define _SF_O_HEAP (1 << 1)
#define _SF_T_MASK (1 << 2) // Type of Pascal file structure (file mode)
#define _SF_T_FILE (0 << 2)
#define _SF_T_TEXT (1 << 2)
#define _SF_A_MASK (1 << 3) // Access type
#define _SF_A_RAW  (0 << 3)
#define _SF_A_BUF  (1 << 3)
#define _SF_SYNC   (1 << 4) // Flush on each write (buffered mode)
#define _SF_STOP   (1 << 5) // End of buffer (memory mode)
#define _SF_ERR    (1 << 6)

#define _STREAM_BUF_SIZE 256

static char _stdout_buf[_STREAM_BUF_SIZE];
static FILE _stdout_file;

FILE *custom_stdout = &_stdout_file;

static void _stream_init (FILE *self, unsigned sf);

static bool _stream_vprintf (FILE *self, const char *format, va_list ap);

static void _mem_stream_init (FILE *self, unsigned sf, void *ptr,
                              size_t limit);
static bool _mem_stream_write (FILE *self, const void *ptr, size_t size);

static void _file_stream_init (FILE *self, unsigned sf, void *buf,
                               size_t size, void *output);
static bool _file_stream_write (FILE *self, const void *ptr, size_t size);
static bool _file_stream_flush (FILE *self);

//=************************************************************************=//

static void _stream_init (FILE *self, unsigned sf) {
  memset (self, 0, sizeof (FILE));
  self->flags = sf;
}

// Format flags

#define _F_LEFTALIGN (1 << 0)
#define _F_PUTSIGN   (1 << 1)
#define _F_PUTSPACE  (1 << 2)
#define _F_ZEROPAD   (1 << 3)
#define _F_UPPERCASE (1 << 4)
#define _F_NEGATE    (1 << 7)

// -2^63   =  -9 223 372 036 854 775 808 (20 characters)
// +2^64-1 = +18 446 744 073 709 551 615 (21 characters)
#define MAX_LEN 21

static bool _stream_write_udecimal (FILE *self,
  const long long unsigned value, const unsigned f, const unsigned w) {

  long long unsigned x = value;
  uint_least8_t count = (w > MAX_LEN) ? MAX_LEN : w;
  char num[MAX_LEN], s[MAX_LEN];
  char *p = num + MAX_LEN;
  uint_least8_t digits = 0;
  int i = 0, len;

  do {
    p--;
    *p = '0' + (x % 10);
    x /= 10;
    digits++;
  } while (x);

  // Put blanks first

  if (!(f & (_F_LEFTALIGN | _F_ZEROPAD))) {
    len = count - digits;

    if (f & (_F_NEGATE | _F_PUTSIGN | _F_PUTSPACE))
      len--;

    for (; len > 0; len--)
      s[i++] = ' ';
  }

  // Put sign

  if (f & _F_NEGATE)
    s[i++] = '-';
  else if (f & _F_PUTSIGN)
    s[i++] = '+';
  else if (f & _F_PUTSPACE)
    s[i++] = ' ';

  // Pad with zeroes (if not left aligned)

  if ((f & (_F_LEFTALIGN | _F_ZEROPAD)) == _F_ZEROPAD)
    for (len = count - i - digits; len > 0; len--)
      s[i++] = '0';

  // Copy number

  for (len = digits; len; len--) {
    s[i++] = *p;
    p++;
  }

  // Pad with blanks

  if (f & _F_LEFTALIGN)
    for (len = count - i; len > 0; len--)
      s[i++] = ' ';

  return self->m_write (self, s, i);
}

#undef MAX_LEN

static bool _stream_write_decimal (FILE *self,
  const long long value, unsigned f, const unsigned w) {

  long long unsigned x;

  if (value < 0) {
    x = -value;
    f |= _F_NEGATE;
  } else
    x = value;

  return _stream_write_udecimal (self, x, f, w);
}

#define MAX_LEN 16

static bool _stream_write_hexadecimal (FILE *self,
  const long long unsigned value, const unsigned f, const unsigned w) {

  long long unsigned x = value;
  uint_least8_t count = w <= MAX_LEN ? w : MAX_LEN;
  char num[MAX_LEN], s[MAX_LEN];
  char *p = num + MAX_LEN;
  uint_least8_t digits = 0;
  int i = 0, len;

  if (f & _F_UPPERCASE) {
    do {
      uint_least8_t d = x & 15;

      p--;
      *p = (d <= 9) ? '0' + d : 'A' + d - 10;
      digits++;
      x >>= 4;
    } while (x);
  } else {
    do {
      uint_least8_t d = x & 15;

      p--;
      *p = (d <= 9) ? '0' + d : 'a' + d - 10;
      digits++;
      x >>= 4;
    } while (x);
  }

  // Put blanks first

  if (!(f & (_F_LEFTALIGN | _F_ZEROPAD)))
    for (len = count - digits; len > 0; len--)
      s[i++] = ' ';

  // Pad with zeroes (if not left aligned)

  if ((f & (_F_LEFTALIGN | _F_ZEROPAD)) == _F_ZEROPAD)
    for (len = count - i - digits; len > 0; len--)
      s[i++] = '0';

  // Copy number

  for (len = digits; len; len--) {
    s[i++] = *p;
    p++;
  }

  // Pad with blanks

  if (f & _F_LEFTALIGN)
    for (len = count - i; len > 0; len--)
      s[i++] = ' ';

  return self->m_write (self, s, i);
}

#undef MAX_LEN

/*
  Flag characters after %:
    supported: 0, -, space, +
    not:       #, ', I

  Length modifiers:
    supported: h, hh, l, ll, L, z, t
    not:       q, j, Z

  Conversion specifiers:
    supported: d, i, u, x, X, c, s, p, %
      * c: length modifiers ignored
    not:       o, e, E, f, F, g, G, a, A, C, S, n, m

  Multibyte forms are not supported.
*/

static bool _stream_vprintf (FILE *self, const char *format, va_list ap) {
  int i = 0, j = 0;
  bool ok = true;
  bool end = false;

  while (ok && !end) {
    unsigned f; // Flags
    unsigned w; // Width
    enum {
      _M_none = 0,
      _M_h, _M_hh, _M_l, _M_ll, _M_z, _M_t
    } m; // Modifiers
    char c;
    bool readnext;

    switch (c = format[i]) {
    case '%':
      if (i - j > 0) {
        ok = self->m_write (self, format + j, i - j);
        if (!ok)
          break;
      }
      i++;
      f = 0;
      w = 0;
      m = _M_none;

      do {
        readnext = false;
        switch (c = format[i]) {
        case '%':
          ok = self->m_write (self, "%", 1);
          break;

        case '-':
          f |= _F_LEFTALIGN;
          readnext = true;
          break;

        case '+':
          f |= _F_PUTSIGN;
          readnext = true;
          break;

        case ' ':
          f |= _F_PUTSPACE;
          readnext = true;
          break;

        case '0':
          if (!w) {
            f |= _F_ZEROPAD;
            readnext = true;
            break;
          }
          __FALLTHROUGH
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
          w = w * 10 + c - '0';
          readnext = true;
          break;

        case 'h':
          m = (m == _M_h) ? _M_hh : _M_h;
          readnext = true;
          break;

        case 'l':
          m = (m == _M_l) ? _M_ll : _M_l;
          readnext = true;
          break;

        case 'L':
          m = _M_ll;
          readnext = true;
          break;

        case 'z':
          m = _M_z;
          readnext = true;
          break;

        case 't':
          m = _M_t;
          readnext = true;
          break;

        case 'c': {
            uint8_t x = va_arg (ap, int);

            ok = self->m_write (self, &x, 1);
          }
          break;

        case 'd':
        case 'i': {
            long long x;

            switch (m) {
            case _M_none:
            default:
              x = va_arg (ap, int);
              break;
            case _M_hh:
              x = (signed char) va_arg (ap, int);
              break;
            case _M_h:
              x = (short) va_arg (ap, int);
              break;
            case _M_l:
              x = va_arg (ap, long);
              break;
            case _M_ll:
              x = va_arg (ap, long long);
              break;
            case _M_z:
              x = va_arg (ap, ssize_t);
              break;
            case _M_t:
              x = va_arg (ap, ptrdiff_t);
              break;
            }

            ok = _stream_write_decimal (self, x, f, w);
          }
          break;

        case 'u':
        case 'x':
        case 'X': {
            long long unsigned x;

            switch (m) {
            case _M_none:
            default:
              x = va_arg (ap, unsigned);
              break;
            case _M_hh:
              x = (unsigned char) va_arg (ap, unsigned);
              break;
            case _M_h:
              x = (short unsigned) va_arg (ap, unsigned);
              break;
            case _M_l:
              x = va_arg (ap, long unsigned);
              break;
            case _M_ll:
              x = va_arg (ap, long long unsigned);
              break;
            case _M_z:
              x = va_arg (ap, size_t);
              break;
            case _M_t:
#if __PTRDIFF_WIDTH__ == 32
                x = (__UINT32_TYPE__) va_arg (ap, ptrdiff_t);
#elif __PTRDIFF_WIDTH__ == 64
                x = (__UINT64_TYPE__) va_arg (ap, ptrdiff_t);
#else // __PTRDIFF_WIDTH__ == ?
#error Unsupported __PTRDIFF_WIDTH__ value
#endif // __PTRDIFF_WIDTH__ == ?
              break;
            }

            if (c == 'X')
              f |= _F_UPPERCASE;

            if (c == 'u')
              ok = _stream_write_udecimal (self, x, f, w);
            else
              ok = _stream_write_hexadecimal (self, x, f, w);
          }
          break;

        case 'p':
#if __INTPTR_WIDTH__ == 32
          ok = _stream_write_hexadecimal (
            self, (__UINT32_TYPE__) va_arg (ap, void *), f, w);
#elif __INTPTR_WIDTH__ == 64
          ok = _stream_write_hexadecimal (
            self, (__UINT64_TYPE__) va_arg (ap, void *), f, w);
#else // __INTPTR_WIDTH__ == ?
#error Unsupported __INTPTR_WIDTH__ value
#endif // __INTPTR_WIDTH__ == ?
          break;

        case 's': {
            const char *s = va_arg (ap, char *);

            ok = self->m_write (self, s, strlen (s));
          }
          break;

        case 0:
          end = true;
          break;

        default:
          break;
        }
        i++;
        j = i;
      } while (ok && readnext);
      break;

    case 0:
      if (i - j)
        ok = self->m_write (self, format + j, i - j);
      end = true;
      break;

    default:
      i++;
    }
  }

  if (ok && (self->flags & _SF_SYNC)) {
    if (self->m_flush)
      ok = self->m_flush (self);
    else {
      ok = false; // m_flush is not set
      self->flags |= _SF_ERR;
    }
  }

  return ok;
}

//=************************************************************************=//

static void _mem_stream_init (FILE *self, unsigned sf, void *ptr,
                              size_t limit) {

  _stream_init (self, (sf & ~_SF_A_MASK) | _SF_A_RAW);
  self->m_write = _mem_stream_write;
  self->output = ptr;
  self->limit = limit;
}

static bool _mem_stream_write (FILE *self, const void *ptr, size_t size) {
  bool ok;

  if ((!(self->flags & (_SF_STOP | _SF_ERR))) && self->output) {
    if (size) {
      char *output = (char *) self->output + self->pos;

      if (self->limit) {
        size_t left = self->limit - self->written;

        if (size > left)
          size = left;
      }

      memcpy (output, ptr, size);
      self->pos += size;
      self->written += size;

      if (self->limit)
        if (self->written == self->limit)
          self->flags |= _SF_STOP;

      ok = true;
    } else
      ok = true; // no data
  } else
    ok = false; // cannot write to stopped stream or when error occurred

  return ok;
}

//=************************************************************************=//

static void _file_stream_init (FILE *self, unsigned sf, void *buf,
                               size_t size, void *output) {

  if (buf && (size > 1)) {
    sf = (sf & ~_SF_A_MASK) | _SF_A_BUF;
    size--; // -1 for terminating zero
  } else {
    sf = (sf & ~_SF_A_MASK) | _SF_A_RAW;
    buf = NULL;
    size = 0;
  }
  _stream_init (self, sf);
  self->m_write = _file_stream_write;
  self->m_flush = _file_stream_flush;
  self->buf = buf;
  self->buf_size = size;
  self->output = output;
}

static bool _file_stream_write (FILE *self, const void *ptr, size_t size) {
  bool ok;

  if (!(self->flags & (_SF_STOP | _SF_ERR))) {
    if (size) {
      if ((self->flags & _SF_A_MASK) == _SF_A_BUF) {
        const char *input = ptr;

        ok = true;

        do {
          if (self->pos < self->buf_size) {
            size_t n = self->buf_size - self->pos;

            if (n > size)
              n = size;
            memcpy (&self->buf[self->pos], input, n);
            input += n;
            self->pos += n;
            size -= n;
          }

          if (self->pos == self->buf_size) {
            if (self->m_flush)
              ok = self->m_flush (self);
            else {
              ok = false; // m_flush is not set
              self->flags |= _SF_ERR;
            }
          }

        } while (((self->flags & (_SF_STOP | _SF_ERR)) == 0) && size);
      } else {
        if ((self->flags & _SF_T_MASK) == _SF_T_TEXT) {
          const char *p = ptr;
          size_t n = size;
          String buf[255+1];

          if (n >= 255) {
            // Write using blocks of 255 bytes
            SetLength (buf, 255);
            do {
              memcpy (GetStr (buf), p, 255);
              p += 255;
              n -= 255;

              Pascal_Write_String (self->output, buf);
              ok = !Pascal_InOutRes;
              if (ok)
                self->written += 255;
            } while (ok && (n >= 255));
          } else
            ok = true; // continue

          if (ok && n) {
            // Write tail
            SetLength (buf, n);
            memcpy (GetStr (buf), p, n);

            Pascal_Write_String (self->output, buf);
            ok = !Pascal_InOutRes;
            if (ok)
              self->written += n;
          }
        } else
          ok = false; // TODO: not implemented

        if (!ok)
          self->flags |= _SF_ERR;
      }
    } else
      ok = true; // no data
  } else
    ok = false; // cannot write to stopped stream or when error occurred

  return ok;
}

static bool _file_stream_flush (FILE *self) {
  bool ok;

  if ((self->flags & _SF_A_MASK) == _SF_A_BUF) {
    if (self->pos) {
      if (!(self->flags & (_SF_STOP | _SF_ERR))) {
        if ((self->flags & _SF_T_MASK) == _SF_T_TEXT) {
          self->buf[self->pos] = '\0';
          Pascal_Write_PChar (self->output, self->buf);
          ok = !Pascal_InOutRes;
          if (ok && (self->flags & _SF_SYNC)) {
            Pascal_Flush (self->output);
            ok = !Pascal_InOutRes;
          }
        } else
          ok = false; // TODO: not implemented

        if (ok)
          self->written += self->pos;
        else
          self->flags |= _SF_ERR;
      } else
        ok = false; // cannot write to stopped stream or when error occurred

      self->pos = 0; // drop buffer
    } else
      ok = true; // no data to write
  } else
    ok = false; // cannot flush in direct mode

  return ok;
}

//=************************************************************************=//

int custom_printf (const char *format, ...) {
  va_list ap;

  va_start (ap, format);
  _stream_vprintf (custom_stdout, format, ap);
  va_end (ap);

  return custom_stdout->written;
}

int custom_vprintf (const char *format, va_list ap) {
  _stream_vprintf (custom_stdout, format, ap);

  return custom_stdout->written;
}

//=************************************************************************=//

int custom_fprintf (FILE *stream, const char *format, ...) {
  va_list ap;
  int written;

  va_start (ap, format);
  written = custom_vfprintf (stream, format, ap);
  va_end (ap);

  return written;
}

int custom_vfprintf (FILE *stream, const char *format, va_list ap) {
  FILE f;
  char buf[_STREAM_BUF_SIZE];
  void *buf_ptr;
  size_t buf_size;

  if (((stream->flags & _SF_A_MASK) == _SF_A_BUF)
  &&  stream->buf && (stream->buf_size > 1)) {
    buf_ptr = stream->buf;
    buf_size = stream->buf_size;
  } else {
    buf_ptr = buf;
    buf_size = sizeof (buf);
  }

  _file_stream_init (&f,
                     _SF_S_STAT | _SF_O_STAT
                     | (stream->flags & (_SF_T_MASK | _SF_SYNC)),
                     buf_ptr, buf_size, stream->output);
  _stream_vprintf (&f, format, ap);

  return f.written;
}

//=************************************************************************=//

int custom_sprintf (char *str, const char *format, ...) {
  va_list ap;
  int written;

  va_start (ap, format);
  written = custom_vsprintf (str, format, ap);
  va_end (ap);

  return written;
}

int custom_vsprintf (char *str, const char *format, va_list ap) {
  FILE f;

  _mem_stream_init (&f, _SF_S_STAT, str, 0);
  _stream_vprintf (&f, format, ap);
  str[f.written] = 0;

  return f.written;
}

//=************************************************************************=//

int custom_snprintf (char *str, size_t size, const char *format, ...) {
  va_list ap;
  int written;

  va_start (ap, format);
  written = custom_vsnprintf (str, size, format, ap);
  va_end (ap);

  return written;
}

int custom_vsnprintf (char *str, size_t size, const char *format,
                      va_list ap) {

  FILE f;

  _mem_stream_init (&f, _SF_S_STAT, str, size);
  _stream_vprintf (&f, format, ap);
  str[f.written] = 0;

  return f.written;
}

//=************************************************************************=//

int custom_fflush (FILE *stream) {
  bool ok;

  if (stream->m_flush)
    ok = stream->m_flush (stream);
  else {
    ok = false; // m_flush is not set
    stream->flags |= _SF_ERR;
  }

  return ok ? 0 : -1;
}

//=************************************************************************=//

static void done_stdio (void);

void init_stdio (void) {
  _file_stream_init (&_stdout_file,
                     _SF_S_STAT | _SF_O_STAT | _SF_T_TEXT | _SF_SYNC,
                     _stdout_buf, sizeof (_stdout_buf), Pascal_Output);
  atexit (done_stdio);
}

static void done_stdio (void) {
}
