// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Sampling_Frequency = 14.31818 MHz / 288 ~= 49716 Hz
// Octave = 0..7
// Fnum = 1..1023
// Real_Frequency = Fnum * Sampling_Frequency / pow(2,20-Octave)
// or:
// Fnum = Real_Frequency * pow(2,20-Octave) / Sampling_Frequency

static const uint16_t Fnum[12] = {
   0x157, // C (FreqStart+1)
   0x16B, // C#
   0x181, // D
   0x198, // D#
   0x1B0, // E
   0x1CA, // F
   0x1E5, // F#
   0x202, // G
   0x220, // G#
   0x241, // A (~440 Hz at chip's sampling frequency ~49716 Hz)
   0x263, // A#
   0x287  // B
// 0x2AE  // C (Fnum[12] = Fnum[0]*2 = FreqEnd)
};

// note: 0..12*8 (everything above will be: octave=7, freq.=FreqEnd)
uint16_t nFreq (uint8_t note) {
  return (note >= 12*8) ? (          7 << 10) + FreqEnd
                        : ((note / 12) << 10) + Fnum[note % 12];
}
