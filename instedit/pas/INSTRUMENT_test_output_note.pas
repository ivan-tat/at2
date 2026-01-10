// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

// Local to `INSTRUMENT_test' procedure

function output_note(chan,board_pos: Byte): Boolean;

var
  note: Byte;
  freq: Word;
  ins: tADTRACK2_INS;

begin
  _dbg_enter ({$I %FILE%}, 'INSTRUMENT_test.output_note');

  note := board_pos+12*(current_octave-1);
  If NOT (note in [0..12*8+1]) then
    begin
      output_note := FALSE;
      _dbg_leave; EXIT; //INSTRUMENT_test.output_note
    end;

  chan_handle[chan] := board_scancodes[board_pos];
  If _4op_mode then
    chan := _4op_main_chan[chan];

  If _1op_preview then
    begin
      If _operator_enabled[1] or _operator_enabled[2] then
        ins := songdata.instr_data[instr]
      else ins := songdata.instr_data[instr2];
      pBYTE(@ins)[10] := pBYTE(@ins)[10] OR 1;
      load_instrument(ins,chan);
      If _operator_enabled[1] or _operator_enabled[2] then
        set_ins_volume($3f-ORD(_operator_enabled[1])*($3f-LO(volume_table[chan])),
                       $3f-ORD(_operator_enabled[2])*($3f-HI(volume_table[chan])),
                       chan)
      else set_ins_volume($3f-ORD(_operator_enabled[3])*($3f-LO(volume_table[chan])),
                          $3f-ORD(_operator_enabled[4])*($3f-HI(volume_table[chan])),
                          chan);
    end
  else
    begin
      load_instrument(songdata.instr_data[instr],chan);
      set_ins_volume($3f-ORD(_operator_enabled[1])*($3f-LO(volume_table[chan])),
                     $3f-ORD(_operator_enabled[2])*($3f-HI(volume_table[chan])),
                     chan);
      If percussion_mode and
         (songdata.instr_data[instr].perc_voice in [4,5]) then
        load_instrument(songdata.instr_data[instr],_perc_sim_chan[chan]);
      If _4op_mode then
        begin
          load_instrument(songdata.instr_data[instr2],PRED(chan));
          set_ins_volume($3f-ORD(_operator_enabled[3])*($3f-LO(volume_table[PRED(chan)])),
                         $3f-ORD(_operator_enabled[4])*($3f-HI(volume_table[PRED(chan)])),
                         PRED(chan));
        end;
    end;

  macro_speedup := songdata.macro_speedup;
  If (play_status = isStopped) then update_timer(songdata.tempo);

  freq := nFreq(note-1)+$2000+
          SHORTINT(pBYTE(@Addr(songdata.instr_data[instr])^)[12]);
  event_table[chan].note := note;
  freq_table[chan] := freq;
  freqtable2[chan] := freq;
  key_on(chan);
  change_freq(chan,freq);

  If process_macros then
    If NOT (_1op_preview and (_operator_enabled[3] or _operator_enabled[4])) then
      init_macro_table(chan,note,instr,freq)
    else init_macro_table(chan,note,instr2,freq)
  else begin
         macro_table[chan].fmreg_table := 0;
         macro_table[chan].arpg_table := 0;
         macro_table[chan].vib_table := 0;
       end;

  If _4op_mode and NOT _1op_preview then
    begin
      If process_macros then init_macro_table(PRED(chan),note,instr2,freq)
      else begin
             macro_table[PRED(chan)].fmreg_table := 0;
             macro_table[PRED(chan)].arpg_table := 0;
             macro_table[PRED(chan)].vib_table := 0;
           end;
    end;

  _dbg_leave; //EXIT //INSTRUMENT_test.output_note
end;
