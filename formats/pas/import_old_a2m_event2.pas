// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure import_old_a2m_event2(patt,line,chan: Byte; old_chunk: tOLD_CHUNK);

const
  ef_ManualFSlide = 22;

var
  chunk: tCHUNK;

begin
  FillChar(chunk,SizeOf(chunk),0);
  chunk.note := old_chunk.note;
  chunk.instr_def := old_chunk.instr_def;

  If (old_chunk.effect_def <> ef_ManualFSlide) then
    begin
      chunk.effect_def := old_chunk.effect_def;
      chunk.effect := old_chunk.effect;
    end
  else If (old_chunk.effect DIV 16 <> 0) then
         begin
           chunk.effect_def := ef_Extended2;
           chunk.effect := ef_ex2_FineTuneUp*16+old_chunk.effect DIV 16;
         end
       else begin
              chunk.effect_def := ef_Extended2;
              chunk.effect := ef_ex2_FineTuneDown*16+old_chunk.effect MOD 16;
            end;

  put_chunk(patt,line,chan,chunk);
end;
