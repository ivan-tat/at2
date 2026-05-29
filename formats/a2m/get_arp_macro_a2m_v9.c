// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static void get_arp_macro_a2m_v9 (tARPEGGIO_TABLE *dst, arp_macro_a2m_v9_t *src)
{
  dst->length      = src->length;
  dst->speed       = src->speed;
  dst->loop_begin  = src->loop_begin;
  dst->loop_length = src->loop_length;
  dst->keyoff_pos  = src->keyoff_pos;
  memcpy (dst->data, src->data, sizeof (dst->data));
}
