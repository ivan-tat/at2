// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static bool is_empty_event_cff (const pat_event_cff_t *src)
{
  return (src->note == NOTE_EMPTY_CFF) && (src->fx == 0) && (src->param == 0);
}
