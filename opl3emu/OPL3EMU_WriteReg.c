// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void OPL3EMU_WriteReg (uint16_t reg, uint8_t data) {
  uint8_t reg_lo = reg & 0xFF;
  uint8_t reg_hi = (reg >> 8) & 1;

  // Misc. registers
  if ((reg_lo >= 0x01) && (reg_lo <= 0x08)) {
    if (reg_hi) {
      if ((reg_lo & 0x0F) == 4) {
        // 4OP con. sel.
        chan_update_4op (&opl3, data);
      }
    } else if ((reg_lo & 0x0F) == 8) {
      // Bit `NTS': 0 -> LSB for key is bit 10 of Fnum
      //            1 -> LSB for key is bit 9 of Fnum
      opl3.nts_bit = (data >> 6) & 1;
    }
  } else

  // AM/VIB/EGT/KSR/MULT
  if ((reg_lo >= 0x20) && (reg_lo <= 0x35)) {
    uint8_t i = CH_5BIT_MASK[reg_lo & 0x1F];

    if (i)
      update_lfo_eg_ksr_mult (&opl3.slot[18 * reg_hi + i - 1], data);
  } else

  // KSL/TL
  if ((reg_lo >= 0x40) && (reg_lo <= 0x55)) {
    uint8_t i = CH_5BIT_MASK[reg_lo & 0x1F];

    if (i)
      update_ksl_tl (&opl3.slot[18 * reg_hi + i - 1], data);
  } else

  // AR/DR
  if ((reg_lo >= 0x60) && (reg_lo <= 0x75)) {
    uint8_t i = CH_5BIT_MASK[reg_lo & 0x1F];

    if (i)
      update_ar_dr (&opl3.slot[18 * reg_hi + i - 1], data);
  } else

  // SL/RR
  if ((reg_lo >= 0x80) && (reg_lo <= 0x95)) {
    uint8_t i = CH_5BIT_MASK[reg_lo & 0x1F];

    if (i)
      update_sl_rr (&opl3.slot[18 * reg_hi + i - 1], data);
  } else

  // Fnum/block/KSR
  if ((reg_lo >= 0xA0) && (reg_lo <= 0xA8)) {
    update_fnum_block_ksr (
      &opl3.chan[9 * reg_hi + (reg_lo & 0x0F)], data, true); // MSB part
  } else

  // Fnum/block/KSR
  if ((reg_lo >= 0xB0) && (reg_lo <= 0xB8)) {
    OPL3_CHAN *chan = &opl3.chan[9 * reg_hi + (reg_lo & 0x0F)];

    // Fnum/block/KSR
    update_fnum_block_ksr (chan, data, false); // LSB part
    // Key on/off
    update_key (chan, (data & 0x20) != 0);
  } else

  // LFO/rhythm
  if (reg_lo == 0xBD) {
    if (!reg_hi) {
      // Bit 'DAM' (LFO tremolo)
      opl3.dva_bit = data >> 7;
      // Bit 'DVB' (LFO vibrato)
      opl3.dvb_bit = (data >> 6) & 1;
      // Rhythm mode flag
      chan_update_rhythm (&opl3, data);
    }
  } else

  // Slot/feedback/con.
  if ((reg_lo >= 0xC0) && (reg_lo <= 0xC8)) {
    update_fb_con (&opl3.chan[9 * reg_hi + (reg_lo & 0x0F)], data);
  } else

  // Waveform sel.
  if ((reg_lo >= 0xE0) && (reg_lo <= 0xF5)) {
    uint8_t i = CH_5BIT_MASK[reg_lo & 0x1F];

    if (i)
      opl3.slot[18 * reg_hi + i - 1].reg_wf = data & 7;
  }
}
