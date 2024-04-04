// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#ifndef VGA_H
#define VGA_H

#define VGA_ATTR_WRITE_PORT     0x3C0 // Attribute Address/Data Register (write)
#define VGA_ATTR_READ_PORT      0x3C1 // Attribute Data Register (read)
#define VGA_STATUS_0_PORT       0x3C2 // Input Status #0 Register
#define VGA_STATUS_1_PORT       0x3DA // Input Status #1 Register
#define VGA_MISC_WRITE_PORT     0x3C2 // Miscellaneous Output Register (write)
#define VGA_MISC_READ_PORT      0x3CC // Miscellaneous Output Register (read)
#define VGA_ENABLE_PORT         0x3C3 // Video Subsystem Enable Register
#define VGA_SEQ_ADDR_PORT       0x3C4 // Sequencer Address Register
#define VGA_SEQ_DATA_PORT       0x3C5 // Sequencer Data Register
#define VGA_DAC_ADDR_READ_PORT  0x3C7 // DAC Address Read Mode Register
#define VGA_DAC_ADDR_WRITE_PORT 0x3C8 // DAC Address Write Mode Register
#define VGA_DAC_DATA_PORT       0x3C9 // DAC Data Register
#define VGA_DAC_STATE_PORT      0x3C7 // DAC State Register
#define VGA_GRAPH_ADDR_PORT     0x3CE // Graphics Controller Address Register
#define VGA_GRAPH_DATA_PORT     0x3CF // Graphics Controller Data Register
#define VGA_CRTC_ADDR_PORT      0x3D4 // CRT Controller Address Register
#define VGA_CRTC_DATA_PORT      0x3D5 // CRT Controller Data Register

#endif // !defined(VGA_H)
