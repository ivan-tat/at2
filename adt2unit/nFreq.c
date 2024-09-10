// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static const uint16_t Fnum[12] = {
   0x157,
   0x16B,
   0x181,
   0x198,
   0x1B0,
   0x1CA,
   0x1E5,
   0x202,
   0x220,
   0x241,
   0x263,
   0x287
};

uint16_t nFreq (uint8_t note) {
  return (note >= 12*8) ? (          7 << 10) + FreqEnd
                        : ((note / 12) << 10) + Fnum[note % 12];
}
