// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure OPL3EMU_WriteReg(reg: Word; data: Byte);

var
  reg_hi,
  reg_lo: Byte;

begin
  reg := reg AND $1ff;
  reg_hi := (reg SHR 8) AND 1;
  reg_lo := reg AND $0ff;

  Case reg_lo of
    // misc. registers
    $01..$08:
      If (reg_hi <> 0) then
        begin
          If (reg_lo AND $0f = 4) then
            // 4OP con. sel.
            chan_update_4op(@opl3,data);
        end
      else If (reg_lo AND $0f = 8) then
             // bit 'NTS'
             // 0 -> LSB for key is bit 10 of Fnum
             // 1 -> LSB for key is bit 9 of Fnum
             opl3.nts_bit := (data SHR 6) AND 1;

    // AM/VIB/EGT/KSR/MULT
    $20..$35:
      If (CH_5BIT_MASK[reg_lo AND $1f] <> 0) then
        update_lfo_eg_ksr_mult(@opl3.slot[18 * reg_hi + PRED(CH_5BIT_MASK[reg_lo AND $1f])],
                               data);

    // KSL/TL
    $40..$55:
      If (CH_5BIT_MASK[reg_lo AND $1f] <> 0) then
        update_ksl_tl(@opl3.slot[18 * reg_hi + PRED(CH_5BIT_MASK[reg_lo AND $1f])],
                      data);

    // AR/DR
    $60..$75:
      If (CH_5BIT_MASK[reg_lo AND $1f] <> 0) then
        update_ar_dr(@opl3.slot[18 * reg_hi + PRED(CH_5BIT_MASK[reg_lo AND $1f])],
                     data);

    // SL/RR
    $80..$95:
      If (CH_5BIT_MASK[reg_lo AND $1f] <> 0) then
        update_sl_rr(@opl3.slot[18 * reg_hi + PRED(CH_5BIT_MASK[reg_lo AND $1f])],
                     data);

    // Fnum/block/KSR
    $0a0..$a8:
      update_fnum_block_ksr(@opl3.chan[9 * reg_hi + (reg_lo AND $0f)],
                            data,TRUE); // MSB part

    // Fnum/block/KSR
    $0b0..$0b8:
      begin
        // Fnum/block/KSR
        update_fnum_block_ksr(@opl3.chan[9 * reg_hi + (reg_lo AND $0f)],
                              data,FALSE); // LSB part
        // key on/off
        update_key(@opl3.chan[9 * reg_hi + (reg_lo AND $0f)],data AND $20 <> 0);
      end;

    // LFO/rhythm
    $0bd:
      If (reg_hi = 0) then
        begin
          // bit 'DAM' (LFO tremolo)
          opl3.dva_bit := data SHR 7;
          // bit 'DVB' (LFO vibrato)
          opl3.dvb_bit := (data SHR 6) AND 1;
          // rhythm mode flag
          chan_update_rhythm(@opl3,data);
        end;

    // slot/feedback/con.
    $0c0..$0c8:
      update_fb_con(@opl3.chan[9 * reg_hi + (reg_lo AND $0f)],
                    data);

    // waveform sel.
    $0e0..$0f5:
      If (CH_5BIT_MASK[reg_lo AND $1f] <> 0) then
        opl3.slot[18 * reg_hi + PRED(CH_5BIT_MASK[reg_lo AND $1f])].reg_wf := data AND 7;
  end;
end;
