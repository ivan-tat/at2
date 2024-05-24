// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

static int16_t envelope_calc_sin (uint8_t wf, uint16_t phase,
                                  int16_t eg_out) {

  uint16_t output = 0;
  uint16_t level;
  uint16_t invert = 0;

  phase &= 0x3FF;

  switch (wf) {
  case 0: // Sine
    if (phase & 0x200)
     invert = ~invert;
    if (phase & 0x100)
      output = LOG_SIN_VAL[(phase & 0x0FF) ^ 0x0FF];
    else
      output = LOG_SIN_VAL[phase & 0x0FF];
    break;

  case 1: // Half-Sine
    if (phase & 0x200)
      output = 0x1000;
    else if (phase & 0x100)
      output = LOG_SIN_VAL[(phase & 0x0FF) ^ 0x0FF];
    else
      output = LOG_SIN_VAL[phase & 0x0FF];
    break;

  case 2: // Abs-Sine
    if (phase & 0x100)
      output = LOG_SIN_VAL[(phase & 0x0FF) ^ 0x0FF];
    else
      output = LOG_SIN_VAL[phase & 0x0FF];
    break;

  case 3: // Pulse-Sine
    if (phase & 0x100)
      output = 0x1000;
    else
      output = LOG_SIN_VAL[phase & 0x0FF];
    break;

  case 4: // Sine (EPO)
    if ((phase & 0x300) == 0x100)
      invert = ~invert;
    if (phase & 0x200)
      output = 0x1000;
    else if (phase & 0x080)
      output = LOG_SIN_VAL[((phase ^ 0x0FF) << 1) & 0x0FF];
    else
      output = LOG_SIN_VAL[(phase << 1) & 0x0FF];
    break;

  case 5: // Abs-Sine (EPO)
    if (phase & 0x200)
      output = 0x1000;
    else if (phase & 0x080)
      output = LOG_SIN_VAL[((phase ^ 0x0FF) << 1) & 0x0FF];
    else
      output = LOG_SIN_VAL[(phase << 1) & 0x0FF];
    break;

  case 6: // Square
    if (phase & 0x200)
      invert = WORD_NULL;
    output = 0;
    break;

  case 7: // Derived Square
    if (phase & 0x200) {
      invert = ~invert;
      phase = (phase & 0x1FF) ^ 0x1FF;
    }
    output = phase << 3;
    break;

  default:
    break;
  }

  level = limit_value (output + (eg_out << 3), 0, 0x1FFF);

  return (int16_t)(((EXP_VAL[(level & 0x0FF) ^ 0x0FF] | 0x400) << 1)
                   >> (level >> 8)) ^ invert;
}
