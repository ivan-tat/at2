// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure envelope_calc(p_slot: P_OPL3_SLOT);

var
  rate_hi,
  rate_lo: Byte;

begin
  rate_hi := p_slot^.eg_rate SHR 2;
  rate_lo := p_slot^.eg_rate AND 3;

  // calculate increment step for output
  If (EG_SHIFT[rate_hi] > 0) then
    begin
       If ((p_slot^.p_chip^.timer AND ((1 SHL EG_SHIFT[rate_hi]) - 1)) = 0) then
         p_slot^.eg_inc := EG_VAL[EG_IDX[rate_hi],rate_lo,
                                  ((p_slot^.p_chip^.timer) SHR EG_SHIFT[rate_hi]) AND 7]
       else
         p_slot^.eg_inc := 0;
    end
  else
    p_slot^.eg_inc := EG_VAL[EG_IDX[rate_hi],rate_lo,
                             p_slot^.p_chip^.timer AND 7] SHL Abs(EG_SHIFT[rate_hi]);

  p_slot^.eg_out := p_slot^.eg_rout +
                    p_slot^.reg_tl SHL 2 +
                    p_slot^.eg_ksl SHR KSL_SHIFT[p_slot^.reg_ksl] +
                    p_slot^.p_trem^; // apply LFO tremolo

  Case p_slot^.eg_state of
    EG_OFF:
      p_slot^.eg_rout := $1ff;

    EG_ATTACK:
      If (p_slot^.eg_rout <> 0) then
        begin
          Inc(p_slot^.eg_rout,((NOT p_slot^.eg_rout) * p_slot^.eg_inc) SHR 3);
          limit_value(p_slot^.eg_rout,0,$1ff);
        end
      else
        begin
          // continue with decay if max. level is reached
          p_slot^.eg_state := EG_DECAY;
          envelope_update_rate(p_slot);
        end;

    EG_DECAY:
      If (p_slot^.eg_rout < SMALLINT(p_slot^.reg_sl) SHL 4) then
        Inc(p_slot^.eg_rout,p_slot^.eg_inc)
      else
        begin
          // sustain level was reached
          p_slot^.eg_state := EG_SUSTAIN;
          envelope_update_rate(p_slot);
        end;

    EG_SUSTAIN,
    EG_RELEASE:
      If (p_slot^.eg_state = EG_SUSTAIN) and
         (p_slot^.reg_type <> 0) then
        // sustain phase
      else
        If (p_slot^.eg_rout < $1ff) then
          Inc(p_slot^.eg_rout,p_slot^.eg_inc)
        else
          begin
            // switch off generator if min. level is reached
            p_slot^.eg_state := EG_OFF;
            p_slot^.eg_rout := $1ff;
            envelope_update_rate(p_slot);
          end;
  end;
end;
