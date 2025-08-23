// Fix C symbol names for Free Pascal compiler on Windows platform.
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024-2025 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef SDL__FIX_H
#define SDL__FIX_H

#if WINNT
#define SDL_FIX_BEGIN __asm__ (
#if i386||_X86_
#define SDL_FIX_NAME(x) ".equ _" #x ", _$dll$sdl$" #x "\n"
#else // !(i386||_X86_)
#define SDL_FIX_NAME(x) ".equ " #x ", _$dll$sdl$" #x "\n"
#endif // !(i386||_X86_)

#define SDL_FIX_END );
#else // !WINNT
#define SDL_FIX_BEGIN
#define SDL_FIX_NAME(x)
#define SDL_FIX_END
#endif // !WINNT

#endif // !define(SDL__FIX_H)
