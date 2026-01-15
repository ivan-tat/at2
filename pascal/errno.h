// DJGPP support
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024-2026 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef PASCAL_ERRNO_H
#define PASCAL_ERRNO_H

extern int custom_errno;

enum custom_errno_enum
{
  EDOM = 1,     // Numerical argument out of domain
  ERANGE,       // Numerical result out of range
  E2BIG,        // Argument list too long
  EACCES,       // Permission denied
  EAGAIN,       // Resource temporarily unavailable
  EBADF,        // Bad file descriptor
  EBUSY,        // Device or resource busy
  ECHILD,       // No child processes
  EDEADLK,      // Resource deadlock avoided
  EEXIST,       // File exists
  EFAULT,       // Bad address
  EFBIG,        // File too large
  EINTR,        // Interrupted system call
  EINVAL,       // Invalid argument
  EIO,          // Input/output error
  EISDIR,       // Is a directory
  EMFILE,       // Too many open files
  EMLINK,       // Too many links
  ENAMETOOLONG, // File name too long
  ENFILE,       // Too many open files in system
  ENODEV,       // No such device
  ENOENT,       // No such file or directory
  ENOEXEC,      // Exec format error
  ENOLCK,       // No locks available
  ENOMEM,       // Cannot allocate memory
  ENOSPC,       // No space left on device
  ENOSYS,       // Function not implemented
  ENOTDIR,      // Not a directory
  ENOTEMPTY,    // Directory not empty
  ENOTTY,       // Inappropriate ioctl for device
  ENXIO,        // No such device or address
  EPERM,        // Operation not permitted
  EPIPE,        // Broken pipe
  EROFS,        // Read-only file system
  ESPIPE,       // Illegal seek
  ESRCH,        // No such process
  EXDEV,        // Invalid cross-device link
  ENMFILE,      // No more files
  ELOOP,        // Too many levels of symbolic links
  EOVERFLOW,    // Value too large for defined data type
  EILSEQ        // Invalid or incomplete multibyte or wide character
};

// Aliases

#ifdef errno
#undef errno
#endif // defined(errno)
#define errno custom_errno

#endif // !defined(PASCAL_ERRNO_H)
