// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2026 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure MACRO_BROWSER(instrBrowser: Boolean; updateCurInstr: Boolean);
begin
  _dbg_enter ({$I %FILE%}, 'MACRO_BROWSER');

  songdata_crc := Update32(songdata,SizeOf(songdata),0);
  a2w_file_loader(FALSE,NOT instrBrowser,TRUE,FALSE,updateCurInstr); // browse internal A2W data
  If (Update32(songdata,SizeOf(songdata),0) <> songdata_crc) then
    module_archived := FALSE;

  _dbg_leave; //EXIT //MACRO_BROWSER
end;
