// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// This file is included in `adt2ext3.h'

#pragma once

#ifndef ILOADERS_H
#define ILOADERS_H

/*
  Song description (use `free()' to free):

  struct song_desc_s {
    size_t count; // number of lines (0..inf)
    fast_str_t lines[.count];
  };
*/

int8_t a2m_file_loader (const String *_fname, progress_callback_t *progress, uint8_t *state, char **error);
int8_t a2t_file_loader (const String *_fname, progress_callback_t *progress, uint8_t *state, char **error);
#if !ADT2PLAY
int8_t a2p_file_loader (const String *_fname, progress_callback_t *progress, uint8_t *state, char **error);
#endif // !ADT2PLAY
int8_t amd_file_loader (const String *_fname, progress_callback_t *progress, uint8_t *state, char **error);
int8_t cff_file_loader (const String *_fname, progress_callback_t *progress, uint8_t *state, char **error);
int8_t dfm_file_loader (const String *_fname, progress_callback_t *progress, uint8_t *state, char **error);
int8_t fmk_file_loader (const String *_fname, void **desc, progress_callback_t *progress, uint8_t *state, char **error);
int8_t hsc_file_loader (const String *_fname, progress_callback_t *progress, uint8_t *state, char **error);
int8_t mtk_file_loader (const String *_fname, progress_callback_t *progress, uint8_t *state, char **error);
int8_t rad_file_loader (const String *_fname, void **desc, progress_callback_t *progress, uint8_t *state, char **error);
int8_t s3m_file_loader (const String *_fname, progress_callback_t *progress, uint8_t *state, char **error);
int8_t sat_file_loader (const String *_fname, progress_callback_t *progress, uint8_t *state, char **error);

#endif // !DEFINED(ILOADERS_H)
