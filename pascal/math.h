// DJGPP support
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef PASCAL_MATH_H
#define PASCAL_MATH_H

#include "pascal.h"

#define truncf(x) Pascal_Trunc_Single (x)
#define trunc(x) Pascal_Trunc_Double (x)

#endif // !defined(PASCAL_MATH_H)
