// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure import_old_a2m_patterns1(block: Byte; count: Byte);

procedure get_old_chunk(pattern,line,channel: Byte; var chunk: tOLD_CHUNK);
begin chunk := old_hash_buffer[pattern][line][channel]; end;

var
  patt,line,chan: Byte;
  chunk: tOLD_CHUNK;

begin { import_old_a2m_patterns1 }
  For patt := 0 to max(PRED(count),15) do
    For line := 0 to $3f do
      For chan := 1 to 9 do
        begin
          get_old_chunk(patt,line,chan,chunk);
          import_old_a2m_event1(block*16+patt,line,chan,chunk,TRUE);
        end;
end;
