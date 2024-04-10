// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

int _VESA_CheckStatus (uint16_t status) {
  if ((status >> 8) == 0x4F) {
    VESA_CallStatus = VESACS_OK;
    VESA_Error = status & 0xFF;
    return VESA_Error ? -1 : 0;
  } else {
    VESA_CallStatus = VESACS_NS;
    VESA_Error = 1; // Function call failed
    return -1;
  }
}

