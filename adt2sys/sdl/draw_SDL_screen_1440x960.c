// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#define FONT_WIDTH  8
#define FONT_HEIGHT 16
#define IN_TEXT_WIDTH  180 // 1440p
#define IN_TEXT_HEIGHT 60 // 960p
//#define IN_WIDTH  (FONT_WIDTH * IN_TEXT_WIDTH)
//#define IN_HEIGHT (FONT_HEIGHT * IN_TEXT_HEIGHT)
//#define OUT_WIDTH  1440 // 180 characters
//#define OUT_HEIGHT 960 // 60 characters

static void draw_SDL_screen_1440x960 (void)
{
  const uint8_t *font = font8x16; // input font
  const uint8_t *s = screen_ptr; // input screen
  uint8_t *p = _FrameBuffer; // output buffer
  //uint8_t *p_end; // output buffer end
  //int32_t skip; // output buffer start offset
  uint8_t y = 1; // input text screen Y position (1-based)
  uint8_t cur_pos_x = virtual_cur_pos & 0xFF;
  uint8_t cur_pos_y = virtual_cur_pos >> 8;
  uint8_t cur_shape_start = virtual_cur_shape >> 8;
  uint8_t cur_shape_end = virtual_cur_shape & 0xFF;

  if (_cursor_blink_pending_frames >= _cursor_blink_factor)
  {
    _cursor_blink_pending_frames = 0;
    cursor_sync = !cursor_sync;
  }

  for (uint8_t r = IN_TEXT_HEIGHT; r; r--) // input text screen rows left
  {
    const uint8_t *s0 = s; // save input text screen start address
    uint8_t cy = 0; // in character's Y position (0-based)

    for (uint8_t fr = FONT_HEIGHT; fr; fr--) // character's rows left
    {
      uint8_t x = 1; // input text screen X position (1-based)

      for (uint8_t c = IN_TEXT_WIDTH; c; c--) // input text screen columns left
      {
        if (   cursor_sync
            && (x == cur_pos_x) && (y == cur_pos_y)
            && (cy >= cur_shape_start) && (cy <= cur_shape_end))
        {
          uint8_t attr = s[1] & 0x0F; // screen attribute

          for (uint8_t fc = FONT_WIDTH; fc; fc--) // character's columns left
          {
            *p = attr;
            p++; // next output buffer pixel
          }
        }
        else
        {
          uint8_t mask = font[s[0] * FONT_HEIGHT + cy];
          uint8_t bit = 0x80;
          uint8_t attr[2] = { s[1] & 0x0F, s[1] >> 4 }; // screen attributes

          for (uint8_t fc = FONT_WIDTH; fc; fc--) // character's columns left
          {
            *p = (mask & bit) != 0 ? attr[0] : attr[1];
            bit >>= 1;
            p++; // next output buffer pixel
          }
        }

        s += 2; // next input text screen character
        x++; // next input text screen X position
      }

      s = s0; // same input text screen row
      cy++; // next in character Y position
    }

    s += IN_TEXT_WIDTH * 2; // next input text screen row
    y++; // next input text screen Y position
  }
}

#undef FONT_WIDTH
#undef FONT_HEIGHT
#undef IN_TEXT_WIDTH
#undef IN_TEXT_HEIGHT
//#undef IN_WIDTH
//#undef IN_HEIGHT
//#undef OUT_WIDTH
//#undef OUT_HEIGHT
