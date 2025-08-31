// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Local to `test_instrument_alt' procedure.

function output_note(chan,board_pos: Byte): Boolean;

var
  note: Byte;
  freq: Word;

begin
  _dbg_enter ({$I %FILE%}, 'test_instrument_alt.output_note');

  note := board_pos+12*(current_octave-1);
  If NOT (note in [0..12*8+1]) then
    begin
      output_note := FALSE;
      _dbg_leave; EXIT; //test_instrument_alt.output_note
    end;

  chan_handle[chan] := board_scancodes[board_pos];
  If (test_ins2 <> 0) then chan := _4op_main_chan[chan];
  load_instrument(temp_instrument,chan);

  If percussion_mode and
     (temp_instrument.perc_voice in [4,5]) then
    load_instrument(temp_instrument,_perc_sim_chan[chan]);

  If (test_ins2 <> 0) then
    load_instrument(temp_instrument2,PRED(chan));

  macro_speedup := songdata.macro_speedup;
  If (play_status = isStopped) then update_timer(songdata.tempo);

  freq := nFreq(note-1)+$2000+
          SHORTINT(pBYTE(@Addr(temp_instrument)^)[12]);
  event_table[chan].note := note;
  freq_table[chan] := freq;
  freqtable2[chan] := freq;
  key_on(chan);
  change_freq(chan,freq);

  If (test_ins2 = 0) then init_macro_table(chan,note,test_ins1,freq)
  else begin
         init_macro_table(chan,note,test_ins1,freq);
         freq_table[PRED(chan)] := freq;
         freqtable2[PRED(chan)] := freq;
         init_macro_table(PRED(chan),note,test_ins2,freq);
       end;

  output_note := true;

  _dbg_leave; //EXIT //test_instrument_alt.output_note
end;
