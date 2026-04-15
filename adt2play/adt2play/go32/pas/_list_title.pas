// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure _list_title;
begin
  If iVGA then
    begin
      CWriteLn('',$07,0);
      CWriteLn('   subz3ro''s',$09,0);
      CWriteLn('       ÄÂÄ       ÄÄ',$09,0);
      CWriteLn('  /´DLiB³R/´CK3R ³³ PLAYER',$09,0);
      CWriteLn('   ³       ³     ÄÄ   '+VERSION_STR,$09,0);
      CWriteLn('',$07,0);
    end
  else begin
         WriteLn;
         WriteLn('   subz3ro''s');
         WriteLn('       ÄÂÄ       ÄÄ');
         WriteLn('  /´DLiB³R/´CK3R ³³ PLAYER');
         WriteLn('   ³       ³     ÄÄ   '+VERSION_STR);
         WriteLn;
       end;
end;
