// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Saved from "adt2unit.pas"

procedure centered_frame(var xstart,ystart: Byte; hsize,vsize: Byte;
                         name: String; atr1,atr2: Byte; border: String);
begin
  xstart := (work_MaxCol-hsize) DIV 2;
  ystart := (work_MaxLn -vsize) DIV 2+(work_MaxLn-vsize) MOD 2;

  Frame(centered_frame_vdest,xstart,ystart,xstart+hsize,ystart+vsize,
        atr1,name,atr2,border);
end;
