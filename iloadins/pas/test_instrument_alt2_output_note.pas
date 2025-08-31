// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function output_note(chan,board_pos: Byte): Boolean;

var
  note: Byte;
  freq: Word;

begin
  _dbg_enter ({$I %FILE%}, 'test_instrument_alt2.output_note');

  note := board_pos+12*(current_octave-1);
  If NOT (note in [0..12*8+1]) then
    begin
      output_note := FALSE;
      _dbg_leave; EXIT; //test_instrument_alt2.output_note
    end;

  chan_handle[chan] := board_scancodes[board_pos];
  load_instrument(temp_instrument,chan);

  If percussion_mode and
     (temp_instrument.perc_voice in [4,5]) then
    load_instrument(temp_instrument,_perc_sim_chan[chan]);

  freq := nFreq(note-1)+$2000+
          SHORTINT(pBYTE(@Addr(temp_instrument)^)[12]);
  event_table[chan].note := note;
  freq_table[chan] := freq;
  freqtable2[chan] := freq;
  key_on(chan);
  change_freq(chan,freq);

  macro_speedup := songdata.macro_speedup;
  If (play_status = isStopped) then update_timer(songdata.tempo);

  freq_table[chan] := freq;
  freqtable2[chan] := freq;
  init_macro_table(chan,note,current_inst,freq);
  output_note := true;

  _dbg_leave; //EXIT //test_instrument_alt2.output_note
end;
