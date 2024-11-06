// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#include "platform.h"

#if !ADT2PLAY

const char PATHSEP = DIR_SEP;
const String WILDCARD_ASTERISK[4] = {
#if linux||unix
  "\x01" "*"
#else // !(linux||unix)
  "\x03" "*.*"
#endif // !(linux||unix)
};

#endif // !ADT2PLAY
