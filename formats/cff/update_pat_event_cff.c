// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

typedef struct
{
  unsigned line, chan;
  uint8_t ins[CHANNELS_MAX_CFF];
  uint8_t arp_param[CHANNELS_MAX_CFF];
  unsigned pat_break;
} play_state_cff_t;

static bool update_pat_event_cff (tCHUNK *dst,
                                  play_state_cff_t *ps, pat_event_cff_t *src)
{
  bool result = false;

  switch (src->fx)
  {
    case 'C': // SET MODULATOR VOLUME
      if (dst->instr_def == INS_EMPTY)
      {
        if (!accurate_conv)
        {
          dst->instr_def = ps->ins[ps->chan];
          result = true;
        }
        else if (ps->ins[ps->chan] == INS_EMPTY)
        {
          dst->instr_def = INS_PACK (ps->chan);
          result = true;
        }
      }
      break;

    case 'G': // SET CARRIER VOLUME
      if (dst->instr_def == INS_EMPTY)
      {
        if (!accurate_conv)
        {
          dst->instr_def = ps->ins[ps->chan];
          result = true;
        }
        else if (ps->ins[ps->chan] == INS_EMPTY)
        {
          dst->instr_def = INS_PACK (ps->chan);
          result = true;
        }
      }
      break;

    case 'I': // SET INSTRUMENT
      if (src->param < INSTRUMENTS_MAX_CFF)
        if (ps->line != ps->pat_break)
        {
          ps->ins[ps->chan] = INS_PACK (src->param);
          if (accurate_conv)
          {
            dst->instr_def = ps->ins[ps->chan];
            result = true;
          }
        }
      break;

    case 'J': // ARPEGGIO
      dst->effect_def = ef_Arpeggio;
      if (src->param != 0)
      {
        dst->effect = src->param;
        ps->arp_param[ps->chan] = src->param;
      }
      else
        dst->effect = ps->arp_param[ps->chan];
      result = true;
      break;

    case 'K': // JUMP TO ORDER
      if (src->param < PATTERN_ORDER_LEN_CFF)
        ps->pat_break = ps->line + 1;
      break;

    case 'L': // JUMP TO NEXT PATTERN IN ORDER
      ps->pat_break = ps->line + 1;
      break;

    default: break;
  }

  if (   (src->note >= NOTE_PACK_CFF (0))
      && (src->note <= NOTE_PACK_CFF (NOTE_MAX_CFF))) // regular note
  {
    if (accurate_conv)
    {
      if (ps->ins[ps->chan] == INS_EMPTY)
      {
        ps->ins[ps->chan] = INS_PACK (ps->chan);
        dst->instr_def = ps->ins[ps->chan];
        result = true;
      }
    }
    else
    {
      dst->instr_def = ps->ins[ps->chan];
      result = true;
    }
  }

  return result;
}
