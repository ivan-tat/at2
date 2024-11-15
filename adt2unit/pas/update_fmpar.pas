// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure update_fmpar(chan: Byte);
begin
  opl3out(_instr[00]+_chan_m[chan],fmpar_table[chan].multipM+
                                   fmpar_table[chan].ksrM  SHL 4+
                                   fmpar_table[chan].sustM SHL 5+
                                   fmpar_table[chan].vibrM SHL 6+
                                   fmpar_table[chan].tremM SHL 7);
  opl3out(_instr[01]+_chan_c[chan],fmpar_table[chan].multipC+
                                   fmpar_table[chan].ksrC  SHL 4+
                                   fmpar_table[chan].sustC SHL 5+
                                   fmpar_table[chan].vibrC SHL 6+
                                   fmpar_table[chan].tremC SHL 7);

  opl3out(_instr[10]+_chan_n[chan],(fmpar_table[chan].connect+
                                    fmpar_table[chan].feedb SHL 1) OR
                                   _panning[panning_table[chan]]);

  vscale_table[chan] := concw(fmpar_table[chan].kslM SHL 6,
                              fmpar_table[chan].kslC SHL 6);
  set_ins_volume(LO(volume_table[chan]),
                 HI(volume_table[chan]),chan);
end;
