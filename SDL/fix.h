// Fix C symbol names for Free Pascal compiler on Windows platform.
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef SDL_FIX_H
#define SDL_FIX_H

#if WINNT
#define SDL_FIX_BEGIN __asm__ (
#define SDL_FIX_NAME(x) ".equ _" #x ", _$dll$sdl$" #x "\n\t"
#define SDL_FIX_END );
#else // !WINNT
#define SDL_FIX_BEGIN
#define SDL_FIX_NAME(x)
#define SDL_FIX_END
#endif // !WINNT

#endif // !define(SDL_FIX_H)
