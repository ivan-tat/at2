// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure eg_key_on_off(p_slot: P_OPL3_SLOT; key_on: Boolean);
begin
  If key_on then
    begin
      If (p_slot^.key = 0) then
        begin
          p_slot^.eg_state := EG_ATTACK;
          envelope_update_rate(p_slot);
          If (p_slot^.eg_rate SHR 2 = $0f) then
            begin
              p_slot^.eg_state := EG_DECAY;
              envelope_update_rate(p_slot);
              p_slot^.eg_rout := 0;
            end;
          p_slot^.pg_phase := 0;
        end;

      If (p_slot^.p_chan^.ch_type <> CH_RHYTHM) then
        p_slot^.key := p_slot^.key OR 1
      else
        p_slot^.key := p_slot^.key OR 2;

      If (p_slot^.reg_ar = 0) then
        begin
          // faked decay prevents restart of envelope if AR=0
          p_slot^.eg_state := EG_DECAY;
          Inc(p_slot^.eg_rout);
        end;
    end
  else
    // key off
    If (p_slot^.key <> 0) then
      begin
        If (p_slot^.p_chan^.ch_type <> CH_RHYTHM) then
          p_slot^.key := p_slot^.key AND $0fe
        else
          p_slot^.key := p_slot^.key AND $0fd;

        If (p_slot^.key = 0) then
          begin
            p_slot^.eg_state := EG_RELEASE;
            envelope_update_rate(p_slot);
          end;
      end;
end;
