// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// This file is included in `adt2ext3.h'

#pragma once

#ifndef ILOADERS_H
#define ILOADERS_H

int8_t a2m_file_loader (const String *_fname, progress_callback_t *progress, uint8_t *state, char **error);
int8_t a2t_file_loader (const String *_fname, progress_callback_t *progress, uint8_t *state, char **error);

#endif // !DEFINED(ILOADERS_H)
