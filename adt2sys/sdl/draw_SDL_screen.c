// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#define FONT_WIDTH  8
#define FONT_HEIGHT 16

static void draw_SDL_screen (uint8_t columns, uint8_t rows, uint32_t scr_res)
{
  const uint8_t *font = font8x16;
  const uint8_t *s = screen_ptr;
  uint8_t *p     = _FrameBuffer;
  uint8_t *p_end = &_FrameBuffer[scr_res];
  uint32_t skip = virtual_screen__first_row;
  uint8_t cur_pos_x = virtual_cur_pos & 0xFF;
  uint8_t cur_pos_y = virtual_cur_pos >> 8;
  uint8_t cur_shape_start = virtual_cur_shape >> 8;
  uint8_t cur_shape_end = virtual_cur_shape & 0xFF;

  if (_cursor_blink_pending_frames >= _cursor_blink_factor)
  {
    _cursor_blink_pending_frames = 0;
    cursor_sync = !cursor_sync;
  }

  for (uint8_t r = 1; r <= rows; r++)
  {
    for (uint8_t fr = 0; fr < FONT_HEIGHT; fr++)
    {
      for (uint8_t c = 1; c <= columns; c++)
      {
        uint8_t mask = font[s[0] * FONT_HEIGHT + fr];
        uint8_t bit = 0x80;
        uint8_t attr[2] = { s[1] & 0x0F, s[1] >> 4 };

        for (uint8_t fc = FONT_WIDTH; fc; fc--)
        {
          if (skip != 0)
            skip--;
          else if (p < p_end)
          {
            if ( cursor_sync
                && (c == cur_pos_x) && (r == cur_pos_y)
                && (fr >= cur_shape_start) && (fr <= cur_shape_end))
              p[0] = attr[0];
            else
              p[0] = (mask & bit) != 0 ? attr[0] : attr[1];

            p++;
          }

          bit >>= 1;
        }
        s += 2;
      }
      s -= columns * 2;
    }
    s += columns * 2;
  }
}

#undef FONT_WIDTH
#undef FONT_HEIGHT
