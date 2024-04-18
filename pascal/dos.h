// DJGPP support
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef PASCAL_DOS_H
#define PASCAL_DOS_H

#include "pascal.h"

int custom_enable (void);
int custom_disable (void);

// Aliases

#define enable custom_enable
#define disable custom_disable

#define delay Pascal_Delay

#endif // !defined(PASCAL_DOS_H)
