// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

const VGACustomTextMode_t VGACustomTextModes[53] = {
  { // 0
    .name = "Text 36x14, 9x14",
    .font_height = 14,
    .columns = 36,
    .rows = 14,
    .regen_size = 1024, // = 1008 actual + 16 padding, 32 pages
    .Seq = { 0x08, 0x03, 0x00, 0x02 },
    .Misc = 0x63,
    .CRTC = {
      0x28, 0x23, 0x24, 0x8A, 0x26, 0xC0, 0xB7, 0x1F,
      0x00, 0xCD, 0x0B, 0x0C, 0x00, 0x00, 0x00, 0x00,
      0x94, 0x86, 0x87, 0x12, 0x1F, 0x8E, 0xB1, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x0C, 0x00, 0x0F, 0x08
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 1
    .name = "Text 40x14, 8x14",
    .font_height = 14,
    .columns = 40,
    .rows = 14,
    .regen_size = 1280, // = 1120 actual + 160 padding, 25 pages
    .Seq = { 0x09, 0x03, 0x00, 0x02 },
    .Misc = 0x63,
    .CRTC = {
      0x2D, 0x27, 0x28, 0x90, 0x2B, 0xA0, 0xB7, 0x1F,
      0x00, 0xCD, 0x0B, 0x0C, 0x00, 0x00, 0x00, 0x00,
      0x94, 0x86, 0x87, 0x14, 0x1F, 0x8E, 0xB1, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x0C, 0x00, 0x0F, 0x00
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 2
    .name = "Text 40x14, 9x14",
    .font_height = 14,
    .columns = 40,
    .rows = 14,
    .regen_size = 1280, // = 1120 actual + 160 padding, 25 pages
    .Seq = { 0x08, 0x03, 0x00, 0x02 },
    .Misc = 0x67,
    .CRTC = {
      0x2D, 0x27, 0x28, 0x90, 0x2B, 0xA0, 0xB7, 0x1F,
      0x00, 0xCD, 0x0B, 0x0C, 0x00, 0x00, 0x00, 0x00,
      0x94, 0x86, 0x87, 0x14, 0x1F, 0x8E, 0xB1, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x0C, 0x00, 0x0F, 0x08
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 3
    .name = "Text 46x14, 8x14",
    .font_height = 14,
    .columns = 46,
    .rows = 14,
    .regen_size = 1536, // = 1288 actual + 248 padding, 21 pages
    .Seq = { 0x09, 0x03, 0x00, 0x02 },
    .Misc = 0x67,
    .CRTC = {
      0x34, 0x2D, 0x2E, 0x97, 0x32, 0x96, 0xB7, 0x1F,
      0x00, 0xCD, 0x0B, 0x0C, 0x00, 0x00, 0x00, 0x00,
      0x94, 0x86, 0x87, 0x17, 0x1F, 0x8E, 0xB1, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x0C, 0x00, 0x0F, 0x00
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 4
    .name = "Text 36x15, 9x16",
    .font_height = 16,
    .columns = 36,
    .rows = 15,
    .regen_size = 1280, // = 1080 actual + 200 padding, 25 pages
    .Seq = { 0x08, 0x03, 0x00, 0x02 },
    .Misc = 0xE3,
    .CRTC = {
      0x28, 0x23, 0x24, 0x8A, 0x26, 0xC0, 0x0B, 0x3E,
      0x00, 0xCF, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x00,
      0xEA, 0xAC, 0xDF, 0x12, 0x1F, 0xE7, 0x06, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x0C, 0x00, 0x0F, 0x08
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 5
    .name = "Text 40x15, 8x16",
    .font_height = 16,
    .columns = 40,
    .rows = 15,
    .regen_size = 1280, // = 1200 actual + 80 padding, 25 pages
    .Seq = { 0x09, 0x03, 0x00, 0x02 },
    .Misc = 0xE3,
    .CRTC = {
      0x2D, 0x27, 0x28, 0x90, 0x2B, 0xA0, 0x0B, 0x3E,
      0x00, 0xCF, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x00,
      0xEA, 0xAC, 0xDF, 0x14, 0x1F, 0xE7, 0x06, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x0C, 0x00, 0x0F, 0x00
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 6
    .name = "Text 40x15, 9x16",
    .font_height = 16,
    .columns = 40,
    .rows = 15,
    .regen_size = 1280, // = 1200 actual + 80 padding, 25 pages
    .Seq = { 0x08, 0x03, 0x00, 0x02 },
    .Misc = 0xE7,
    .CRTC = {
      0x2D, 0x27, 0x28, 0x90, 0x2B, 0xA0, 0x0B, 0x3E,
      0x00, 0xCF, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x00,
      0xEA, 0xAC, 0xDF, 0x14, 0x1F, 0xE7, 0x06, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x0C, 0x00, 0x0F, 0x08
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 7
    .name = "Text 46x15, 8x16",
    .font_height = 16,
    .columns = 46,
    .rows = 15,
    .regen_size = 1536, // = 1380 actual + 156 padding, 21 pages
    .Seq = { 0x09, 0x03, 0x00, 0x02 },
    .Misc = 0xE7,
    .CRTC = {
      0x34, 0x2D, 0x2E, 0x97, 0x32, 0x96, 0x0B, 0x3E,
      0x00, 0xCF, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x00,
      0xEA, 0xAC, 0xDF, 0x17, 0x1F, 0xE7, 0x06, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x0C, 0x00, 0x0F, 0x00
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 8
    .name = "Text 36x17, 9x14",
    .font_height = 14,
    .columns = 36,
    .rows = 17,
    .regen_size = 1280, // = 1224 actual + 56 padding, 25 pages
    .Seq = { 0x08, 0x03, 0x00, 0x02 },
    .Misc = 0xE3,
    .CRTC = {
      0x28, 0x23, 0x24, 0x8A, 0x26, 0xC0, 0x07, 0x3E,
      0x00, 0xCD, 0x0B, 0x0C, 0x00, 0x00, 0x00, 0x00,
      0xE6, 0xA8, 0xDB, 0x12, 0x1F, 0xE3, 0x02, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x0C, 0x00, 0x0F, 0x08
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 9
    .name = "Text 40x17, 8x14",
    .font_height = 14,
    .columns = 40,
    .rows = 17,
    .regen_size = 1536, // = 1360 actual + 176 padding, 21 pages
    .Seq = { 0x09, 0x03, 0x00, 0x02 },
    .Misc = 0xE3,
    .CRTC = {
      0x2D, 0x27, 0x28, 0x90, 0x2B, 0xA0, 0x07, 0x3E,
      0x00, 0xCD, 0x0B, 0x0C, 0x00, 0x00, 0x00, 0x00,
      0xE6, 0xA8, 0xDB, 0x14, 0x1F, 0xE3, 0x02, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x0C, 0x00, 0x0F, 0x00
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 10
    .name = "Text 40x17, 9x14",
    .font_height = 14,
    .columns = 40,
    .rows = 17,
    .regen_size = 1536, // = 1360 actual + 176 padding, 21 pages
    .Seq = { 0x08, 0x03, 0x00, 0x02 },
    .Misc = 0xE7,
    .CRTC = {
      0x2D, 0x27, 0x28, 0x90, 0x2B, 0xA0, 0x07, 0x3E,
      0x00, 0xCD, 0x0B, 0x0C, 0x00, 0x00, 0x00, 0x00,
      0xE6, 0xA8, 0xDB, 0x14, 0x1F, 0xE3, 0x02, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x0C, 0x00, 0x0F, 0x08
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 11
    .name = "Text 46x17, 9x14",
    .font_height = 14,
    .columns = 46,
    .rows = 17,
    .regen_size = 1792, // = 1564 actual + 228 padding, 18 pages
    .Seq = { 0x09, 0x03, 0x00, 0x02 },
    .Misc = 0xE7,
    .CRTC = {
      0x34, 0x2D, 0x2E, 0x97, 0x32, 0x96, 0x07, 0x3E,
      0x00, 0xCD, 0x0B, 0x0C, 0x00, 0x00, 0x00, 0x00,
      0xE6, 0xA8, 0xDB, 0x17, 0x1F, 0xE3, 0x02, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x0C, 0x00, 0x0F, 0x00
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 12
    .name = "Text 36x22, 9x16",
    .font_height = 16,
    .columns = 36,
    .rows = 22,
    .regen_size = 1792, // = 1584 actual + 208 padding, 18 pages
    .Seq = { 0x08, 0x03, 0x00, 0x02 },
    .Misc = 0xA3,
    .CRTC = {
      0x28, 0x23, 0x24, 0x8A, 0x26, 0xC0, 0xC1, 0x1F,
      0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x00,
      0x85, 0xA5, 0x5F, 0x12, 0x1F, 0x65, 0xBB, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x0C, 0x00, 0x0F, 0x08
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 13
    .name = "Text 40x22, 8x16",
    .font_height = 16,
    .columns = 40,
    .rows = 22,
    .regen_size = 1792, // = 1760 actual + 32 padding, 18 pages
    .Seq = { 0x09, 0x03, 0x00, 0x02 },
    .Misc = 0xA3,
    .CRTC = {
      0x2D, 0x27, 0x28, 0x90, 0x2B, 0xA0, 0xC1, 0x1F,
      0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x00,
      0x85, 0xA5, 0x5F, 0x14, 0x1F, 0x65, 0xBB, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x0C, 0x00, 0x0F, 0x00
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 14
    .name = "Text 40x22, 9x16",
    .font_height = 16,
    .columns = 40,
    .rows = 22,
    .regen_size = 1792, // = 1760 actual + 32 padding, 18 pages
    .Seq = { 0x08, 0x03, 0x00, 0x02 },
    .Misc = 0xA7,
    .CRTC = {
      0x2D, 0x27, 0x28, 0x90, 0x2B, 0xA0, 0xC1, 0x1F,
      0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x00,
      0x85, 0xA5, 0x5F, 0x14, 0x1F, 0x65, 0xBB, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x0C, 0x00, 0x0F, 0x08
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 15
    .name = "Text 46x22, 8x16",
    .font_height = 16,
    .columns = 46,
    .rows = 22,
    .regen_size = 2048, // = 2024 actual + 24 padding, 16 pages
    .Seq = { 0x09, 0x03, 0x00, 0x02 },
    .Misc = 0xA7,
    .CRTC = {
      0x34, 0x2D, 0x2E, 0x97, 0x32, 0x96, 0xC1, 0x1F,
      0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x00,
      0x85, 0xA5, 0x5F, 0x17, 0x1F, 0x65, 0xBB, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x0C, 0x00, 0x0F, 0x00
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 16
    .name = "Text 70x22, 9x16",
    .font_height = 16,
    .columns = 70,
    .rows = 22,
    .regen_size = 3328, // = 3080 actual + 248 padding, 9 pages
    .Seq = { 0x00, 0x03, 0x00, 0x02 },
    .Misc = 0xA3,
    .CRTC = {
      0x53, 0x45, 0x46, 0x96, 0x4B, 0x15, 0xC1, 0x1F,
      0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x00,
      0x85, 0xA5, 0x5F, 0x23, 0x1F, 0x65, 0xBB, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x0C, 0x00, 0x0F, 0x08
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 17
    .name = "Text 80x22, 8x16",
    .font_height = 16,
    .columns = 80,
    .rows = 22,
    .regen_size = 3584, // = 3520 actual + 64 padding, 9 pages
    .Seq = { 0x01, 0x03, 0x00, 0x02 },
    .Misc = 0xA3,
    .CRTC = {
      0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81, 0xC1, 0x1F,
      0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x00,
      0x85, 0xA5, 0x5F, 0x28, 0x1F, 0x65, 0xBB, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x0C, 0x00, 0x0F, 0x00
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 18
    .name = "Text 80x22, 9x16",
    .font_height = 16,
    .columns = 80,
    .rows = 22,
    .regen_size = 3584, // = 3520 actual + 64 padding, 9 pages
    .Seq = { 0x00, 0x03, 0x00, 0x02 },
    .Misc = 0xA7,
    .CRTC = {
      0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81, 0xC1, 0x1F,
      0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x00,
      0x85, 0xA5, 0x5F, 0x28, 0x1F, 0x65, 0xBB, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x0C, 0x00, 0x0F, 0x08
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 19
    .name = "Text 90x22, 8x16",
    .font_height = 16,
    .columns = 90,
    .rows = 22,
    .regen_size = 4096, // = 3960 actual + 136 padding, 8 pages
    .Seq = { 0x01, 0x03, 0x00, 0x02 },
    .Misc = 0xA7,
    .CRTC = {
      0x6B, 0x59, 0x5A, 0x8E, 0x5F, 0x8A, 0xC1, 0x1F,
      0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x00,
      0x85, 0xA5, 0x5F, 0x2D, 0x1F, 0x65, 0xBB, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x0C, 0x00, 0x0F, 0x00
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 20
    .name = "Text 36x25, 9x16",
    .font_height = 16,
    .columns = 36,
    .rows = 25,
    .regen_size = 2048, // = 1800 actual + 248 padding, 16 pages
    .Seq = { 0x08, 0x03, 0x00, 0x02 },
    .Misc = 0x63,
    .CRTC = {
      0x28, 0x23, 0x24, 0x8A, 0x26, 0xC0, 0xBF, 0x1F,
      0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x00,
      0x9C, 0x8E, 0x8F, 0x12, 0x1F, 0x96, 0xB9, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x0C, 0x00, 0x0F, 0x08
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 21
    .name = "Text 40x25, 8x16",
    .font_height = 16,
    .columns = 40,
    .rows = 25,
    .regen_size = 2048, // = 2000 actual + 48 padding, 16 pages
    .Seq = { 0x09, 0x03, 0x00, 0x02 },
    .Misc = 0x63,
    .CRTC = {
      0x2D, 0x27, 0x28, 0x90, 0x2B, 0xA0, 0xBF, 0x1F,
      0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x00,
      0x9C, 0x8E, 0x8F, 0x14, 0x1F, 0x96, 0xB9, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x0C, 0x00, 0x0F, 0x00
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 22
    .name = "Text 40x25, 9x16",
    .font_height = 16,
    .columns = 40,
    .rows = 25,
    .regen_size = 2048, // = 2000 actual + 48 padding, 16 pages
    .Seq = { 0x08, 0x03, 0x00, 0x02 },
    .Misc = 0x67,
    .CRTC = {
      0x2D, 0x27, 0x28, 0x90, 0x2B, 0xA0, 0xBF, 0x1F,
      0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x00,
      0x9C, 0x8E, 0x8F, 0x14, 0x1F, 0x96, 0xB9, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x0C, 0x00, 0x0F, 0x08
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 23
    .name = "Text 46x25, 8x16",
    .font_height = 16,
    .columns = 46,
    .rows = 25,
    .regen_size = 2560, // = 2300 actual + 260 padding, 12 pages
    .Seq = { 0x09, 0x03, 0x00, 0x02 },
    .Misc = 0x67,
    .CRTC = {
      0x34, 0x2D, 0x2E, 0x97, 0x32, 0x96, 0xBF, 0x1F,
      0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x00,
      0x9C, 0x8E, 0x8F, 0x17, 0x1F, 0x96, 0xB9, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x0C, 0x00, 0x0F, 0x00
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 24
    .name = "Text 70x25, 9x16",
    .font_height = 16,
    .columns = 70,
    .rows = 25,
    .regen_size = 3584, // = 3500 actual + 84 padding, 9 pages
    .Seq = { 0x00, 0x03, 0x00, 0x02 },
    .Misc = 0x63,
    .CRTC = {
      0x53, 0x45, 0x46, 0x96, 0x4B, 0x15, 0xBF, 0x1F,
      0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x00,
      0x9C, 0x8E, 0x8F, 0x23, 0x1F, 0x96, 0xB9, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x0C, 0x00, 0x0F, 0x08
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 25
    .name = "Text 80x25, 8x16",
    .font_height = 16,
    .columns = 80,
    .rows = 25,
    .regen_size = 4096, // = 4000 actual + 96 padding, 8 pages
    .Seq = { 0x01, 0x03, 0x00, 0x02 },
    .Misc = 0x63,
    .CRTC = {
      0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81, 0xBF, 0x1F,
      0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x00,
      0x9C, 0x8E, 0x8F, 0x28, 0x1F, 0x96, 0xB9, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x0C, 0x00, 0x0F, 0x00
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 26
    .name = "Text 80x25, 9x16 (standard)",
    .font_height = 16,
    .columns = 80,
    .rows = 25,
    .regen_size = 4096, // = 4000 actual + 96 padding, 8 pages
    .Seq = { 0x00, 0x03, 0x00, 0x02 },
    .Misc = 0x67,
    .CRTC = {
      0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81, 0xBF, 0x1F,
      0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x00,
      0x9C, 0x8E, 0x8F, 0x28, 0x1F, 0x96, 0xB9, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x0C, 0x00, 0x0F, 0x08
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 27
    .name = "Text 90x25, 8x16",
    .font_height = 16,
    .columns = 90,
    .rows = 25,
    .regen_size = 4608, // = 4500 actual + 108 padding, 7 pages
    .Seq = { 0x01, 0x03, 0x00, 0x02 },
    .Misc = 0x67,
    .CRTC = {
      0x6B, 0x59, 0x5A, 0x8E, 0x5F, 0x8A, 0xBF, 0x1F,
      0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x00,
      0x9C, 0x8E, 0x8F, 0x2D, 0x1F, 0x96, 0xB9, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x08, 0x00, 0x0F, 0x00
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 28
    .name = "Text 46x29, 8x16",
    .font_height = 14,
    .columns = 46,
    .rows = 29,
    .regen_size = 2816, // = 2668 actual + 148 padding, 11 pages
    .Seq = { 0x09, 0x03, 0x00, 0x02 },
    .Misc = 0x67,
    .CRTC = {
      0x34, 0x2D, 0x2E, 0x97, 0x32, 0x96, 0xC1, 0x1F,
      0x00, 0x4D, 0x0B, 0x0C, 0x00, 0x00, 0x00, 0x00,
      0x9F, 0x91, 0x95, 0x17, 0x1F, 0x9B, 0xB9, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x0C, 0x00, 0x0F, 0x00
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 29
    .name = "Text 70x29, 9x14",
    .font_height = 14,
    .columns = 70,
    .rows = 29,
    .regen_size = 4096, // = 4060 actual + 36 padding, 8 pages
    .Seq = { 0x00, 0x03, 0x00, 0x02 },
    .Misc = 0x63,
    .CRTC = {
      0x53, 0x45, 0x46, 0x96, 0x4B, 0x15, 0xC1, 0x1F,
      0x00, 0x4D, 0x0B, 0x0C, 0x00, 0x00, 0x00, 0x00,
      0x9F, 0x91, 0x95, 0x23, 0x1F, 0x9B, 0xB9, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x08, 0x00, 0x0F, 0x08
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 30
    .name = "Text 80x29, 8x14",
    .font_height = 14,
    .columns = 80,
    .rows = 29,
    .regen_size = 4864, // = 4640 actual + 224 padding, 6 pages
    .Seq = { 0x01, 0x03, 0x00, 0x02 },
    .Misc = 0x63,
    .CRTC = {
      0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81, 0xC1, 0x1F,
      0x00, 0x4D, 0x0B, 0x0C, 0x00, 0x00, 0x00, 0x00,
      0x9F, 0x91, 0x95, 0x28, 0x1F, 0x9B, 0xB9, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x08, 0x00, 0x0F, 0x00
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 31
    .name = "Text 80x29, 9x14",
    .font_height = 14,
    .columns = 80,
    .rows = 29,
    .regen_size = 4864, // = 4640 actual + 224 padding, 6 pages
    .Seq = { 0x00, 0x03, 0x00, 0x02 },
    .Misc = 0x67,
    .CRTC = {
      0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81, 0xC1, 0x1F,
      0x00, 0x4D, 0x0B, 0x0C, 0x00, 0x00, 0x00, 0x00,
      0x9F, 0x91, 0x95, 0x28, 0x1F, 0x9B, 0xB9, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x08, 0x00, 0x0F, 0x08
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 32
    .name = "Text 90x29, 8x14",
    .font_height = 14,
    .columns = 90,
    .rows = 29,
    .regen_size = 5376, // = 5220 actual + 156 padding, 6 pages
    .Seq = { 0x01, 0x03, 0x00, 0x02 },
    .Misc = 0x67,
    .CRTC = {
      0x6B, 0x59, 0x5A, 0x8E, 0x5F, 0x8A, 0xC1, 0x1F,
      0x00, 0x4D, 0x0B, 0x0C, 0x00, 0x00, 0x00, 0x00,
      0x9F, 0x91, 0x95, 0x2D, 0x1F, 0x9B, 0xB9, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x08, 0x00, 0x0F, 0x00
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 33
    .name = "Text 70x30, 9x16",
    .font_height = 16,
    .columns = 70,
    .rows = 30,
    .regen_size = 4352, // = 4200 actual + 152 padding, 7 pages
    .Seq = { 0x00, 0x03, 0x00, 0x02 },
    .Misc = 0xE3,
    .CRTC = {
      0x53, 0x45, 0x46, 0x96, 0x4B, 0x15, 0x0B, 0x3E,
      0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x00,
      0xEA, 0xAC, 0xDF, 0x23, 0x1F, 0xE7, 0x06, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x08, 0x00, 0x0F, 0x08
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 34
    .name = "Text 80x30, 8x16",
    .font_height = 16,
    .columns = 80,
    .rows = 30,
    .regen_size = 4864, // = 4800 actual + 64 padding, 6 pages
    .Seq = { 0x01, 0x03, 0x00, 0x02 },
    .Misc = 0xE3,
    .CRTC = {
      0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81, 0x0B, 0x3E,
      0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x00,
      0xEA, 0xAC, 0xDF, 0x28, 0x1F, 0xE7, 0x06, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x08, 0x00, 0x0F, 0x00
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 35
    .name = "Text 80x30, 9x16",
    .font_height = 16,
    .columns = 80,
    .rows = 30,
    .regen_size = 4864, // = 4800 actual + 64 padding, 6 pages
    .Seq = { 0x00, 0x03, 0x00, 0x02 },
    .Misc = 0xE7,
    .CRTC = {
      0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81, 0x0B, 0x3E,
      0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x00,
      0xEA, 0xAC, 0xDF, 0x28, 0x1F, 0xE7, 0x06, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x08, 0x00, 0x0F, 0x08
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 36
    .name = "Text 90x30, 8x16",
    .font_height = 16,
    .columns = 90,
    .rows = 30,
    .regen_size = 5632, // = 5400 actual + 232 padding, 5 pages
    .Seq = { 0x01, 0x03, 0x00, 0x02 },
    .Misc = 0xE7,
    .CRTC = {
      0x6B, 0x59, 0x5A, 0x8E, 0x5F, 0x8A, 0x0B, 0x3E,
      0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x00,
      0xEA, 0xAC, 0xDF, 0x2D, 0x1F, 0xE7, 0x06, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x08, 0x00, 0x0F, 0x00
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 37
    .name = "Text 70x34, 9x14",
    .font_height = 14,
    .columns = 70,
    .rows = 34,
    .regen_size = 4864, // = 4760 actual + 104 padding, 6 pages
    .Seq = { 0x00, 0x03, 0x00, 0x02 },
    .Misc = 0xE3,
    .CRTC = {
      0x53, 0x45, 0x46, 0x96, 0x4B, 0x15, 0x07, 0x3E,
      0x00, 0x4D, 0x0B, 0x0C, 0x00, 0x00, 0x00, 0x00,
      0xE6, 0xA8, 0xDB, 0x23, 0x1F, 0xE3, 0x02, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x08, 0x00, 0x0F, 0x08
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 38
    .name = "Text 80x34, 8x14",
    .font_height = 14,
    .columns = 80,
    .rows = 34,
    .regen_size = 5632, // = 5440 actual + 192 padding, 5 pages
    .Seq = { 0x01, 0x03, 0x00, 0x02 },
    .Misc = 0xE3,
    .CRTC = {
      0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81, 0x07, 0x3E,
      0x00, 0x4D, 0x0B, 0x0C, 0x00, 0x00, 0x00, 0x00,
      0xE6, 0xA8, 0xDB, 0x28, 0x1F, 0xE3, 0x02, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x08, 0x00, 0x0F, 0x00
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 39
    .name = "Text 80x34, 9x14",
    .font_height = 14,
    .columns = 80,
    .rows = 34,
    .regen_size = 5632, // = 5440 actual + 192 padding, 5 pages
    .Seq = { 0x00, 0x03, 0x00, 0x02 },
    .Misc = 0xE7,
    .CRTC = {
      0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81, 0x07, 0x3E,
      0x00, 0x4D, 0x0B, 0x0C, 0x00, 0x00, 0x00, 0x00,
      0xE6, 0xA8, 0xDB, 0x28, 0x1F, 0xE3, 0x02, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x08, 0x00, 0x0F, 0x08
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 40
    .name = "Text 90x34, 8x14",
    .font_height = 14,
    .columns = 90,
    .rows = 34,
    .regen_size = 6144, // = 6120 actual + 24 padding, 5 pages
    .Seq = { 0x01, 0x03, 0x00, 0x02 },
    .Misc = 0xE7,
    .CRTC = {
      0x6B, 0x59, 0x5A, 0x8E, 0x5F, 0x8A, 0x07, 0x3E,
      0x00, 0x4D, 0x0B, 0x0C, 0x00, 0x00, 0x00, 0x00,
      0xE6, 0xA8, 0xDB, 0x2D, 0x1F, 0xE3, 0x02, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x08, 0x00, 0x0F, 0x00
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 41
    .name = "Text 70x44, 9x8",
    .font_height = 8,
    .columns = 70,
    .rows = 44,
    .regen_size = 6400, // = 6160 actual + 240 padding, 5 pages
    .Seq = { 0x00, 0x03, 0x00, 0x02 },
    .Misc = 0xA3,
    .CRTC = {
      0x53, 0x45, 0x46, 0x96, 0x4B, 0x15, 0xC1, 0x1F,
      0x00, 0x47, 0x06, 0x07, 0x00, 0x00, 0x00, 0x00,
      0x85, 0x87, 0x5F, 0x23, 0x0F, 0x65, 0xBB, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x0C, 0x00, 0x0F, 0x08
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 42
    .name = "Text 80x44, 8x8",
    .font_height = 8,
    .columns = 80,
    .rows = 44,
    .regen_size = 7168, // = 7040 actual + 128 padding, 4 pages
    .Seq = { 0x01, 0x03, 0x00, 0x02 },
    .Misc = 0xA3,
    .CRTC = {
      0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81, 0xC1, 0x1F,
      0x00, 0x47, 0x06, 0x07, 0x00, 0x00, 0x00, 0x00,
      0x85, 0x87, 0x5F, 0x28, 0x0F, 0x65, 0xBB, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x0C, 0x00, 0x0F, 0x00
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 43
    .name = "Text 80x44, 9x8",
    .font_height = 8,
    .columns = 80,
    .rows = 44,
    .regen_size = 7168, // = 7040 actual + 128 padding, 4 pages
    .Seq = { 0x00, 0x03, 0x00, 0x02 },
    .Misc = 0xA7,
    .CRTC = {
      0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81, 0xC1, 0x1F,
      0x00, 0x47, 0x06, 0x07, 0x00, 0x00, 0x00, 0x00,
      0x85, 0x87, 0x5F, 0x28, 0x0F, 0x65, 0xBB, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x0C, 0x00, 0x0F, 0x08
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 44
    .name = "Text 90x44, 8x8",
    .font_height = 8,
    .columns = 90,
    .rows = 44,
    .regen_size = 7936, // = 7920 actual + 16 padding, 4 pages
    .Seq = { 0x01, 0x03, 0x00, 0x02 },
    .Misc = 0xA7,
    .CRTC = {
      0x6B, 0x59, 0x5A, 0x8E, 0x5F, 0x8A, 0xC1, 0x1F,
      0x00, 0x47, 0x06, 0x07, 0x00, 0x00, 0x00, 0x00,
      0x85, 0x87, 0x5F, 0x2D, 0x0F, 0x65, 0xBB, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x0C, 0x00, 0x0F, 0x00
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 45
    .name = "Text 70x50, 9x8",
    .font_height = 8,
    .columns = 70,
    .rows = 50,
    .regen_size = 7168, // = 7000 actual + 168 padding, 4 pages
    .Seq = { 0x00, 0x03, 0x00, 0x02 },
    .Misc = 0x63,
    .CRTC = {
      0x53, 0x45, 0x46, 0x96, 0x4B, 0x15, 0xBF, 0x1F,
      0x00, 0x47, 0x06, 0x07, 0x00, 0x00, 0x00, 0x00,
      0x9C, 0x8E, 0x8F, 0x23, 0x1F, 0x96, 0xB9, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x0C, 0x00, 0x0F, 0x08
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 46
    .name = "Text 80x50, 8x8",
    .font_height = 8,
    .columns = 80,
    .rows = 50,
    .regen_size = 8192, // = 8000 actual + 192 padding, 4 pages
    .Seq = { 0x01, 0x03, 0x00, 0x02 },
    .Misc = 0x63,
    .CRTC = {
      0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81, 0xBF, 0x1F,
      0x00, 0x47, 0x06, 0x07, 0x00, 0x00, 0x00, 0x00,
      0x9C, 0x8E, 0x8F, 0x28, 0x1F, 0x96, 0xB9, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x0C, 0x00, 0x0F, 0x00
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 47
    .name = "Text 80x50, 9x8 (standard)",
    .font_height = 8,
    .columns = 80,
    .rows = 50,
    .regen_size = 8192, // = 8000 actual + 192 padding, 4 pages
    .Seq = { 0x00, 0x03, 0x00, 0x02 },
    .Misc = 0x67,
    .CRTC = {
      0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81, 0xBF, 0x1F,
      0x00, 0x47, 0x06, 0x07, 0x00, 0x00, 0x00, 0x00,
      0x9C, 0x8E, 0x8F, 0x28, 0x1F, 0x96, 0xB9, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x0C, 0x00, 0x0F, 0x08
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 48
    .name = "Text 90x50, 8x8",
    .font_height = 8,
    .columns = 90,
    .rows = 50,
    .regen_size = 9216, // = 9000 actual + 216 padding, 3 pages
    .Seq = { 0x01, 0x03, 0x00, 0x02 },
    .Misc = 0x67,
    .CRTC = {
      0x6B, 0x59, 0x5A, 0x8E, 0x5F, 0x8A, 0xBF, 0x1F,
      0x00, 0x47, 0x06, 0x07, 0x00, 0x00, 0x00, 0x00,
      0x9C, 0x8E, 0x8F, 0x2D, 0x1F, 0x96, 0xB9, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x08, 0x00, 0x0F, 0x00
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 49
    .name = "Text 70x60, 9x8",
    .font_height = 8,
    .columns = 70,
    .rows = 60,
    .regen_size = 8448, // = 8400 actual + 48 padding, 3 pages
    .Seq = { 0x00, 0x03, 0x00, 0x02 },
    .Misc = 0xE3,
    .CRTC = {
      0x53, 0x45, 0x46, 0x96, 0x4B, 0x15, 0x0B, 0x3E,
      0x00, 0x47, 0x06, 0x07, 0x00, 0x00, 0x00, 0x00,
      0xEA, 0xAC, 0xDF, 0x23, 0x1F, 0xE7, 0x06, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x08, 0x00, 0x0F, 0x08
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 50
    .name = "Text 80x60, 8x8",
    .font_height = 8,
    .columns = 80,
    .rows = 60,
    .regen_size = 9728, // = 9600 actual + 128 padding, 3 pages
    .Seq = { 0x01, 0x03, 0x00, 0x02 },
    .Misc = 0xE3,
    .CRTC = {
      0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81, 0x0B, 0x3E,
      0x00, 0x47, 0x06, 0x07, 0x00, 0x00, 0x00, 0x00,
      0xEA, 0xAC, 0xDF, 0x28, 0x1F, 0xE7, 0x06, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x08, 0x00, 0x0F, 0x00
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 51
    .name = "Text 80x60, 9x8",
    .font_height = 8,
    .columns = 80,
    .rows = 60,
    .regen_size = 9728, // = 9600 actual + 128 padding, 3 pages
    .Seq = { 0x00, 0x03, 0x00, 0x02 },
    .Misc = 0xE7,
    .CRTC = {
      0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81, 0x0B, 0x3E,
      0x00, 0x47, 0x06, 0x07, 0x00, 0x00, 0x00, 0x00,
      0xEA, 0xAC, 0xDF, 0x28, 0x1F, 0xE7, 0x06, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x08, 0x00, 0x0F, 0x08
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }, { // 52
    .name = "Text 90x60, 8x8",
    .font_height = 8,
    .columns = 90,
    .rows = 60,
    .regen_size = 10880, // = 10800 actual + 80 padding, 3 pages
    .Seq = { 0x01, 0x03, 0x00, 0x02 },
    .Misc = 0xE7,
    .CRTC = {
      0x6B, 0x59, 0x5A, 0x8E, 0x5F, 0x8A, 0x0B, 0x3E,
      0x00, 0x47, 0x06, 0x07, 0x00, 0x00, 0x00, 0x00,
      0xEA, 0xAC, 0xDF, 0x2D, 0x1F, 0xE7, 0x06, 0xA3,
      0xFF
    },
    .Attr = {
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
      0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
      0x08, 0x00, 0x0F, 0x00
    },
    .Graph = {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
      0xFF
    }
  }
};