// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024-2026 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pascal/stdio.h"
#include "pascal/stdlib.h"
#include "pascal/string.h"

// Stream flags

#define _SF_S_MASK (1 << 0) // Stream structure allocation
#define _SF_S_STAT (0 << 0)
#define _SF_S_HEAP (1 << 0)
#define _SF_B_MASK (1 << 1) // Buffer allocation (buffered mode)
#define _SF_B_STAT (0 << 1)
#define _SF_B_HEAP (1 << 1)
#define _SF_O_MASK (1 << 2) // Output variable allocation
#define _SF_O_STAT (0 << 2)
#define _SF_O_HEAP (1 << 2)
#define _SF_T_MASK (1 << 3) // Type of Pascal file structure
#define _SF_T_FILE (0 << 3)
#define _SF_T_TEXT (1 << 3)
#define _SF_A_MASK (1 << 4) // I/O access mode
#define _SF_A_RAW  (0 << 4)
#define _SF_A_BUF  (1 << 4)
#define _SF_M_MASK (3 << 5) // File mode bits
#define _SF_M_R    (1 << 5)
#define _SF_M_W    (1 << 6)
#define _SF_OPEN   (1 << 7) // File is opened
#define _SF_ERR    (1 << 8) // Error occurred
#define _SF_SYNC   (1 << 9) // Flush on each write (buffered mode)
#define _SF_STOP   (1 << 10) // End of buffer (memory mode)

#define _STREAM_BUF_SIZE 256

static char _stdout_buf[_STREAM_BUF_SIZE];
static FILE _stdout_file;

FILE *custom_stdout = &_stdout_file;

static void _stream_init (FILE *self, unsigned sf);
static void _stream_free (FILE *self);

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

static void _stream_free (FILE *self) {
  if ((self->output != NULL) && ((self->flags & _SF_O_MASK) == _SF_O_HEAP))
    free (self->output);

  if ((self->buf != NULL) && ((self->flags & _SF_B_MASK) == _SF_B_HEAP))
    free (self->buf);

  if ((self->flags & _SF_S_MASK) == _SF_S_HEAP)
    free (self);
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

FILE *custom_fopen (const char *restrict name, const char *restrict mode) {
  bool status = false; // `false' on error, `true' on success
  char last = '\0';
  char m_mode = '\0'; // 'r', 'w', 'a'
  bool m_plus = false; // '+'
  bool m_binary = false; // 'b'
  FILE *f = NULL;
  uint16_t err;
  String _name[255+1];

  if ((name == NULL) || (mode == NULL)) goto _err_inval;

  // parse `mode' string
  for (const char *c = mode; *c != '\0'; c++) {
    switch (*c) {
      case 'r':
      case 'w':
      case 'a':
        if (m_mode != '\0') goto _err_inval;
        m_mode = *c;
        break;

      case '+':
        if (((last != 'r') && (last != 'w') && (last != 'a')) || (last == '+') || (last == 'b')) goto _err_inval;
        m_plus = true;
        break;

      case 'b':
        if ((m_mode == '\0') || m_binary) goto _err_inval;
        m_binary = true;
        break;

      default: goto _err_inval;
    }
    last = *c;
  }

  if ((f = malloc (sizeof (*f))) == NULL) goto _exit;

  _stream_init (f, _SF_S_HEAP | _SF_O_HEAP); // we'll set other flags later

  if ((f->output = malloc (m_binary ? Pascal_FileRec_size : Pascal_TextRec_size)) == NULL) goto _exit;

  f->flags |= m_binary ? _SF_T_FILE : _SF_T_TEXT;

  StrToString (_name, name, sizeof (_name) - 1);

  if (m_binary) {
    // binary
    Pascal_AssignFile (f->output, _name);

    if ((m_mode == 'r') || (m_mode == 'a')) {
      // "rb", "r+b", "ab", "a+b"
      if (m_plus) {
        // "r+b", "a+b"
        f->flags |= _SF_M_R | _SF_M_W;
        Pascal_FileMode = Pascal_FileMode_ReadWrite;
      } else {
        // "rb", "ab"
        f->flags |= m_mode == 'r' ? _SF_M_R : _SF_M_W;
        Pascal_FileMode = m_mode == 'r' ? Pascal_FileMode_ReadOnly : Pascal_FileMode_WriteOnly;
      }
      Pascal_ResetFile (f->output, 1); // opens in `Pascal_FileMode' mode
      if ((err = Pascal_IOResult ()) != 0) goto _err_io;

      f->flags |= _SF_OPEN;

      if (m_mode == 'a') {
        // "ab", "a+b"
        // seek to EOF
        int64_t file_size = Pascal_FileSize (f->output);

        if ((err = Pascal_IOResult ()) != 0) goto _err_io;
        Pascal_Seek (f->output, file_size);
        if ((err = Pascal_IOResult ()) != 0) goto _err_io;
      }
    } else {
      // "wb", "w+b"
      Pascal_RewriteFile (f->output, 1); // creates or truncates existing in W mode only
      if ((err = Pascal_IOResult ()) != 0) goto _err_io;

      f->flags |= _SF_M_W | _SF_OPEN; // "wb"

      if (m_plus) {
        // "w+b"
        f->flags |= _SF_M_R;
        Pascal_FileMode = Pascal_FileMode_ReadWrite;
        Pascal_ResetFile (f->output, 1); // reopens in R/W mode
        if ((err = Pascal_IOResult ()) != 0) goto _err_io;
      }
    }
    status = true; // ok
  } else {
    // text
    Pascal_AssignText (f->output, _name);

    if (m_mode == 'r') {
      // "r", "r+"
      if (m_plus) goto _err_na; // "r+"
      else {
        // "r"
        f->flags |= _SF_M_R;
        Pascal_ResetText (f->output); // opens in R mode only
        if ((err = Pascal_IOResult ()) != 0) goto _err_io;
        f->flags |= _SF_OPEN;
      }
    } else if (m_mode == 'w') {
      // "w", "w+"
      if (m_plus) goto _err_na; // "w+"
      else {
        // "w"
        f->flags |= _SF_M_W;
        Pascal_RewriteText (f->output); // creates or truncates existing in W mode only
        if ((err = Pascal_IOResult ()) != 0) goto _err_io;
        f->flags |= _SF_OPEN;
      }
    } else {
      // "a", "a+"
      if (m_plus) goto _err_na; // "a+"
      else {
        // "a"
        f->flags |= _SF_M_W;

        Pascal_Append (f->output); // opens existing at EOF in W mode only
        if ((err = Pascal_IOResult ()) != 0) {
          // if not exists or wrong permissions: try to create a new file
          Pascal_RewriteText (f->output); // opens in W mode only
          if ((err = Pascal_IOResult ()) != 0) goto _err_io;
        }
        f->flags |= _SF_OPEN;
      }
    }
    status = true; // ok
  }

_exit:
  if (!status) {
    _stream_free (f);
    f = NULL;
  }

  return f;

_err_inval:
  errno = EINVAL;
  goto _exit;

_err_io:
  errno = map_InOutRes_to_errno (err);
  goto _exit;

_err_na:
  errno = ENOSYS;
  goto _exit;
}

int custom_fseek (FILE *stream, long offset, int whence) {
  int result = -1;
  uint16_t err;

  if ((stream->flags & _SF_T_MASK) == _SF_T_FILE) {
    int64_t file_pos;

    switch (whence) {
      case SEEK_SET:
        Pascal_Seek (stream->output, offset);
        if ((err = Pascal_IOResult ()) != 0) goto _err_io;
        result = 0;
        break;

      case SEEK_CUR:
        file_pos = Pascal_FilePos (stream->output);
        if ((err = Pascal_IOResult ()) != 0) goto _err_io;
        Pascal_Seek (stream->output, file_pos + offset);
        if ((err = Pascal_IOResult ()) != 0) goto _err_io;
        result = 0;
        break;

      case SEEK_END:
        file_pos = Pascal_FileSize (stream->output);
        if ((err = Pascal_IOResult ()) != 0) goto _err_io;
        Pascal_Seek (stream->output, file_pos + offset);
        if ((err = Pascal_IOResult ()) != 0) goto _err_io;
        result = 0;
        break;

      default: goto _err_na;
    }
  } else goto _err_na;

_exit:
  return result;

_err_io:
  errno = map_InOutRes_to_errno (err);
  goto _exit;

_err_na:
  errno = ENOSYS;
  goto _exit;
}

long custom_ftell (FILE *stream) {
  int64_t file_pos;

  if ((stream->flags & _SF_T_MASK) == _SF_T_FILE) {
    uint16_t err;

    file_pos = Pascal_FilePos (stream->output);
    if ((err = Pascal_IOResult ()) != 0) {
      file_pos = -1;
      errno = map_InOutRes_to_errno (err);
    }
  } else {
    file_pos = -1;
    errno = ENOSYS;
  }

  return file_pos;
}

size_t custom_fread (void *ptr, size_t size, size_t n, FILE *restrict stream) {
  size_t num_read = 0;

  if ((stream->flags & _SF_T_MASK) == _SF_T_FILE) {
    size_t bytes_to_read = size * n;

    if (bytes_to_read <= 0x7FFFFFFFL) { // __INT32_MAX__
      int32_t bytes_read = 0;
      uint16_t err;

      Pascal_BlockRead (stream->output, ptr, bytes_to_read, &bytes_read);
      if ((err = Pascal_IOResult ()) != 0) errno = map_InOutRes_to_errno (err);
      num_read = bytes_read / size;
    } else errno = EINVAL;
  } else errno = ENOSYS;

  return num_read;
}

size_t custom_fwrite (void *ptr, size_t size, size_t n, FILE *restrict stream) {
  size_t num_written = 0;

  if ((stream->flags & _SF_T_MASK) == _SF_T_FILE) {
    size_t total_bytes = size * n;

    if (total_bytes <= 0x7FFFFFFFL) { // __INT32_MAX__
      int32_t bytes_written = 0;
      uint16_t err;

      Pascal_BlockWrite (stream->output, ptr, total_bytes, &bytes_written);
      if ((err = Pascal_IOResult ()) != 0) errno = map_InOutRes_to_errno (err);
      num_written = bytes_written / size;
    } else errno = EINVAL;
  } else errno = ENOSYS;

  return num_written;
}

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

void custom_fclose (FILE *stream) {
  if ((stream->flags & _SF_OPEN) && (stream->output != NULL)) {
    if ((stream->flags & _SF_T_MASK) == _SF_T_FILE)
      Pascal_CloseFile (stream->output);
    else
      Pascal_CloseText (stream->output);

    _stream_free (stream);
  }
}

int custom_remove (const char *path) {
  void *f;
  String _path[255+1];
  uint16_t err;

  if ((f = malloc (Pascal_FileRec_size)) == NULL) return -1;

  StrToString (_path, path, sizeof (_path) - 1);
  Pascal_AssignFile (f, _path);
  Pascal_EraseFile (f);
  err = Pascal_IOResult ();
  free (f);

  if (err != 0)
    if (err == 2) { // File not found
      Pascal_RmDir (_path); // May also return 2 for directory
      err = Pascal_IOResult ();
    }

  if (err == 0)
    return 0;
  else {
    errno = map_InOutRes_to_errno (err);
    return -1;
  }
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

static void done_stdio (void);

void init_stdio (void) {
  _file_stream_init (&_stdout_file,
                     _SF_S_STAT | _SF_B_STAT | _SF_O_STAT | _SF_T_TEXT | _SF_SYNC,
                     _stdout_buf, sizeof (_stdout_buf), Pascal_Output);
  atexit (done_stdio);
}

static void done_stdio (void) {
  _stream_free (&_stdout_file);
}
