// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure replace_old_adsr(patterns: Byte);

var
  chunk,chunk2: tCHUNK;
  temp2,temp3: Byte;
  patt_break: Byte;
  order,patt: Byte;
  patts: String;

begin
  patts := '';
  FillChar(adsr_carrier,SizeOf(adsr_carrier),0);

  order := 0; patt := BYTE_NULL;
  Repeat
    If (songdata.pattern_order[order] >= $80) then Inc(order)
    else
      begin
        patt := songdata.pattern_order[order];
        patt_break := BYTE_NULL;
        For temp2 := 0 to $3f do
          For temp3 := 1 to 9 do
            begin
              get_chunk(patt,temp2,temp3,chunk);
              chunk2 := chunk;

              If (chunk.effect_def in [ef_PositionJump,ef_PatternBreak]) then
                patt_break := temp2;

              If (chunk.effect_def in [$ff,ef_Extended]) then
                begin
                  If (chunk.effect_def = $ff) then
                    begin
                      chunk2.effect_def := 0;
                      chunk2.effect := 0;

                      If (temp2 <= patt_break) then
                        Case chunk.effect of
                          0: adsr_carrier[temp3] := TRUE;
                          1: adsr_carrier[temp3] := FALSE;
                        end;
                    end;

                  If (chunk.effect_def = ef_Extended) then
                    Case chunk.effect DIV 16 of
                      ef_ex_SetAttckRateM,
                      ef_ex_SetAttckRateC:
                        If adsr_carrier[temp3] then
                          chunk2.effect := ef_ex_SetAttckRateC*16+chunk.effect MOD 16
                        else chunk2.effect := ef_ex_SetAttckRateM*16+chunk.effect MOD 16;

                      ef_ex_SetDecayRateM,
                      ef_ex_SetDecayRateC:
                        If adsr_carrier[temp3] then
                          chunk2.effect := ef_ex_SetDecayRateC*16+chunk.effect MOD 16
                        else chunk2.effect := ef_ex_SetDecayRateM*16+chunk.effect MOD 16;

                      ef_ex_SetSustnLevelM,
                      ef_ex_SetSustnLevelC:
                        If adsr_carrier[temp3] then
                          chunk2.effect := ef_ex_SetSustnLevelC*16+chunk.effect MOD 16
                        else chunk2.effect := ef_ex_SetSustnLevelM*16+chunk.effect MOD 16;

                      ef_ex_SetRelRateM,
                      ef_ex_SetRelRateC:
                        If adsr_carrier[temp3] then
                          chunk2.effect := ef_ex_SetRelRateC*16+chunk.effect MOD 16
                        else chunk2.effect := ef_ex_SetRelRateM*16+chunk.effect MOD 16;
                    end;

                  If (Pos(CHR(songdata.pattern_order[order]),patts) = 0) then
                    If (chunk.effect_def <> chunk2.effect_def) or
                       (chunk.effect <> chunk2.effect) then
                      put_chunk(patt,temp2,temp3,chunk2);
                end;
            end;
        Inc(order);
        patts := patts+CHR(patt);
      end;
  until (patt >= patterns) or (order > $7f);
end;
