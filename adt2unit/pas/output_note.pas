// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure output_note(note,ins,chan: Byte;
                      restart_macro,restart_adsr: Boolean);
var
{$IFDEF ADT2PLAY}
  pos: Byte;
{$ENDIF} // DEFINED(ADT2PLAY)
  freq: Word;

begin
  If (note = 0) and (ftune_table[chan] = 0) then
    EXIT; //output_note
  If NOT (note in [1..12*8+1]) then freq := freq_table[chan]
  else begin
         freq := nFreq(note-1)+SHORTINT(ins_parameter(ins,12));
         If restart_adsr then key_on(chan);

         freq_table[chan] := concw(LO(freq_table[chan]),
                                   HI(freq_table[chan]) OR $20);

{$IFDEF ADT2PLAY}
         pos := 1 + Round(note*(DECAY_BARS-1)/(12*8+1));
         If (decay_bar[pos].lvl <> 0) then
           If (pos > 1) and
              (decay_bar[pos-1].dir <> 1) then
             Dec(pos)
           else If (pos < DECAY_BARS) and
                   (decay_bar[pos+1].dir <> 1) then
                  Inc(pos);

         If is_4op_chan(chan) then
           begin
             decay_bar[pos].dir := 1;
             If (ins_parameter(voice_table[chan],10) AND 1 = 0) then
               decay_bar[pos].max_lvl :=
                 (carrier_vol[PRED(chan)]+carrier_vol[chan]) DIV 2
             else decay_bar[pos].max_lvl :=
                    (carrier_vol[PRED(chan)]+modulator_vol[PRED(chan)]+
                     carrier_vol[chan]+modulator_vol[chan]) DIV 4;
           end
         else
           begin
             decay_bar[pos].dir := 1;
             If (ins_parameter(voice_table[chan],10) AND 1 = 0) then
               decay_bar[pos].max_lvl :=
                 carrier_vol[chan]
             else decay_bar[pos].max_lvl :=
                    (carrier_vol[chan]+modulator_vol[chan]) DIV 2;
           end;
{$ELSE} // NOT DEFINED(ADT2PLAY)
         If channel_flag[chan] then
           If is_4op_chan(chan) then
             begin
               If NOT (percussion_mode and (chan in [17..20])) then
                 If (ins_parameter(voice_table[chan],10) AND 1 = 1) then
                   If (volum_bar[chan].lvl < (carrier_vol[chan]+modulator_vol[chan]) DIV 2) then
                     volum_bar[chan].dir := 1
                   else
                 else If (volum_bar[chan].lvl < carrier_vol[chan]) then
                        volum_bar[chan].dir := 1
                      else
               else If (volum_bar[chan].lvl < modulator_vol[chan]) then
                      volum_bar[chan].dir := 1;

               If NOT (percussion_mode and (PRED(chan) in [17..20])) then
                 If (ins_parameter(voice_table[PRED(chan)],10) AND 1 = 1) then
                   If (volum_bar[PRED(chan)].lvl < (carrier_vol[PRED(chan)]+modulator_vol[PRED(chan)]) DIV 2) then
                     volum_bar[PRED(chan)].dir := 1
                   else
                 else If (volum_bar[PRED(chan)].lvl < carrier_vol[PRED(chan)]) then
                        volum_bar[PRED(chan)].dir := 1
                      else
               else If (volum_bar[PRED(chan)].lvl < modulator_vol[PRED(chan)]) then
                      volum_bar[PRED(chan)].dir := 1;

               If (decay_bar[chan].lvl1 < carrier_vol[chan]) then
                 decay_bar[chan].dir1 := 1;

               If (decay_bar[chan].lvl2 < modulator_vol[chan]) then
                 decay_bar[chan].dir2 := 1;

               If (decay_bar[PRED(chan)].lvl1 < carrier_vol[PRED(chan)]) then
                 decay_bar[PRED(chan)].dir1 := 1;

               If (decay_bar[PRED(chan)].lvl2 < modulator_vol[PRED(chan)]) then
                 decay_bar[PRED(chan)].dir2 := 1;

               If (play_status <> isPlaying) then
                 begin
                   volum_bar[chan].dir := -1;
                   decay_bar[chan].dir1 := -1;
                   decay_bar[chan].dir2 := -1;
                   decay_bar[PRED(chan)].dir1 := -1;
                   decay_bar[PRED(chan)].dir2 := -1;
                 end;
             end
           else begin
                  If NOT (percussion_mode and (chan in [17..20])) then
                    If (ins_parameter(voice_table[chan],10) AND 1 = 1) then
                      If (volum_bar[chan].lvl < (carrier_vol[chan]+modulator_vol[chan]) DIV 2) then
                        volum_bar[chan].dir := 1
                      else
                    else If (volum_bar[chan].lvl < carrier_vol[chan]) then
                           volum_bar[chan].dir := 1
                         else
                  else If (volum_bar[chan].lvl < modulator_vol[chan]) then
                         volum_bar[chan].dir := 1;

                  If (decay_bar[chan].lvl1 < carrier_vol[chan]) then
                    decay_bar[chan].dir1 := 1;

                  If (decay_bar[chan].lvl2 < modulator_vol[chan]) then
                    decay_bar[chan].dir2 := 1;

                  If (play_status <> isPlaying) then
                    begin
                      volum_bar[chan].dir := -1;
                      decay_bar[chan].dir1 := -1;
                      decay_bar[chan].dir2 := -1;
                    end;
                end;
{$ENDIF} // NOT DEFINED(ADT2PLAY)
       end;

  If (ftune_table[chan] = -127) then ftune_table[chan] := 0;
  freq := freq+ftune_table[chan];
  change_frequency(chan,freq);

  If (note <> 0) then
    begin
      event_table[chan].note := note;
      If is_4op_chan(chan) then
        event_table[PRED(chan)].note := note;
      If restart_macro then
        With event_table[chan] do
           If NOT (((effect_def = ef_Extended) and
                   (effect DIV 16 = ef_ex_ExtendedCmd2) and
                   (effect MOD 16 = ef_ex_cmd2_NoRestart)) or
                  ((effect_def2 = ef_Extended) and
                   (effect2 DIV 16 = ef_ex_ExtendedCmd2) and
                   (effect2 MOD 16 = ef_ex_cmd2_NoRestart))) then
             init_macro_table(chan,note,ins,freq)
           else macro_table[chan].arpg_note := note;
    end;

  //EXIT //output_note
end;
