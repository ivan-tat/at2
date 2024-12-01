// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure decay_bars_refresh;

var
  chan,temp: Byte;
  speedup_factor,speedown_factor: Real;

begin
  _dbg_enter ({$I %FILE%}, 'decay_bars_refresh');

  show_bar(1,overall_volume,05,1,2,
           analyzer_ovrllvol+analyzer_ovrllvol SHL 4);
  show_bar(1,global_volume,MAX_COLUMNS-7,1,2,
           analyzer_volumelvl+analyzer_volumelvl SHL 4);

{$IFDEF GO32V2}
  speedup_factor := 4.0*SUCC(fps_down_factor);
  speedown_factor :=4.0*SUCC(fps_down_factor);
{$ELSE}
  speedup_factor := 2.5*(100/sdl_frame_rate);
  speedown_factor := 2.5*(100/sdl_frame_rate);
{$ENDIF}

  chan := chan_pos;
  For temp := 1 to MAX_TRACKS do
    begin
      If (decay_bar[chan].dir1 = 1) then
        decay_bar[chan].lvl1 := decay_bar[chan].lvl1+
                 decay_bar[chan].dir1*(decay_bar_rise*speedup_factor)
      else
        decay_bar[chan].lvl1 := decay_bar[chan].lvl1+
                 decay_bar[chan].dir1*(decay_bar_fall*speedown_factor);

      If (decay_bar[chan].dir2 = 1) then
        decay_bar[chan].lvl2 := decay_bar[chan].lvl2+
                 decay_bar[chan].dir2*(decay_bar_rise*speedup_factor)
      else
        decay_bar[chan].lvl2 := decay_bar[chan].lvl2+
                 decay_bar[chan].dir2*(decay_bar_fall*speedown_factor);

      If (decay_bar[chan].lvl1 < 0) then decay_bar[chan].lvl1 := 0;
      If (decay_bar[chan].lvl1 > 63) then decay_bar[chan].lvl1 := 63;
      If (decay_bar[chan].lvl1 >= carrier_vol[chan]) then
        decay_bar[chan].dir1 := -1;

      If (decay_bar[chan].lvl2 < 0) then decay_bar[chan].lvl2 := 0;
      If (decay_bar[chan].lvl2 > 63) then decay_bar[chan].lvl2 := 63;
      If (decay_bar[chan].lvl2 >= modulator_vol[chan]) then
        decay_bar[chan].dir2 := -1;

      If (volum_bar[chan].dir = 1) then
        volum_bar[chan].lvl := volum_bar[chan].lvl+
                 volum_bar[chan].dir*(decay_bar_rise*speedup_factor)
      else
        volum_bar[chan].lvl := volum_bar[chan].lvl+
                 volum_bar[chan].dir*(decay_bar_fall*speedown_factor);

      If (volum_bar[chan].lvl < 0) then volum_bar[chan].lvl := 0;
      If (volum_bar[chan].lvl > 63) then volum_bar[chan].lvl := 63;
      If NOT (percussion_mode and (chan in [17..20])) then
        If (ins_parameter(voice_table[chan],10) AND 1 = 1) then
          If (volum_bar[chan].lvl >= (carrier_vol[chan]+modulator_vol[chan]) DIV 2) then
            volum_bar[chan].dir := -1
          else
        else If (volum_bar[chan].lvl >= carrier_vol[chan]) then
               volum_bar[chan].dir := -1
             else
      else If (volum_bar[chan].lvl >= modulator_vol[chan]) then
             volum_bar[chan].dir := -1;

      If (ins_parameter(voice_table[chan],10) AND 1 = 1) or
         (percussion_mode and (chan in [17..20])) then
        show_bar(temp,Round(decay_bar[chan].lvl2),08,1,6,
                 analyzer+analyzer_modulator SHL 4)
      else
        show_bar(temp,
                 63-(scale_volume(63-Round(decay_bar[chan].lvl2),
                                  63-Round(decay_bar[chan].lvl1))),
                 08,1,6,
                 analyzer+analyzer_modulator SHL 4);

      If NOT (percussion_mode and (chan in [17..20])) then
        show_bar(temp,Round(decay_bar[chan].lvl1),08,8,14,
                 analyzer+analyzer_carrier SHL 4)
      else
        show_bar(temp,0,08,8,14,
                 analyzer+analyzer_carrier SHL 4);
      Inc(chan);
    end;

  _dbg_leave; //EXIT //decay_bars_refresh
end;
