// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function insert_command(cmd,cmd2: Word; patterns: Byte; chan: Byte; exceptions: tByteSet): Boolean;

var
  chunk: tCHUNK;
  temp2,temp3: Byte;
  patt_break: Byte;
  order,patt: Byte;
  patts: String;

begin
  patts := '';
  order := 0;
  patt := BYTE_NULL;
  insert_command := FALSE;

  Repeat
    If (Pos(CHR(songdata.pattern_order[order]),patts) <> 0) or
       (songdata.pattern_order[order] >= $80) then Inc(order)
    else
      begin
        patt := songdata.pattern_order[order];
        patt_break := songdata.patt_len;
        For temp3 := 1 to songdata.nm_tracks do
          For temp2 := 0 to PRED(songdata.patt_len) do
            begin
              get_chunk(patt,temp2,temp3,chunk);
              If (chunk.effect_def in [ef_PositionJump,ef_PatternBreak]) or
                 (chunk.effect_def2 in [ef_PositionJump,ef_PatternBreak]) then
                patt_break := temp2;

              If (temp3 = chan) and (temp2 <= patt_break) then
                If (cmd2 = 0) then
                  If (chunk.effect_def+chunk.effect = 0) or
                     (chunk.effect_def SHL 8 + chunk.effect = cmd) or
                     (chunk.effect_def in exceptions) then
                    begin
                      chunk.effect_def := HI(cmd);
                      chunk.effect := LO(cmd);
                      put_chunk(patt,temp2,temp3,chunk);
                      insert_command := TRUE;
                      EXIT; //insert_command
                    end
                  else If (chunk.effect_def2+chunk.effect2 = 0) or
                          (chunk.effect_def2 SHL 8 + chunk.effect2 = cmd2) or
                          (chunk.effect_def2 in exceptions) then
                         begin
                           chunk.effect_def2 := HI(cmd);
                           chunk.effect2 := LO(cmd);
                           put_chunk(patt,temp2,temp3,chunk);
                           insert_command := TRUE;
                           EXIT; //insert_command
                         end
                       else
                else If ((chunk.effect_def+chunk.effect = 0) or
                         (chunk.effect_def SHL 8 + chunk.effect = cmd) or
                         (chunk.effect_def in exceptions)) and
                        ((chunk.effect_def2+chunk.effect2 = 0) or
                         (chunk.effect_def2 SHL 8 + chunk.effect2 = cmd2) or
                         (chunk.effect_def2 in exceptions)) then
                       begin
                         chunk.effect_def := HI(cmd);
                         chunk.effect := LO(cmd);
                         chunk.effect_def2 := HI(cmd2);
                         chunk.effect2 := LO(cmd2);
                         put_chunk(patt,temp2,temp3,chunk);
                         insert_command := TRUE;
                         EXIT; //insert_command
                       end;
            end;
        Inc(order);
        patts := patts+CHR(patt);
      end;
  until (patt >= patterns) or (order > $7f);

  //EXIT //insert_command
end;
