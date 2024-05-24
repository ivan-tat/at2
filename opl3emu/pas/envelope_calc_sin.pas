// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function envelope_calc_sin(wf: Byte; phase: Word; eg_out: Smallint): Smallint;

var
  output,
  level,
  invert: Word;

begin
  phase := phase AND $3ff;
  output := 0;
  invert := 0;

  Case wf of
    // Sine
    0: begin
         If (phase AND $200 <> 0) then
           invert := NOT invert;
         If (phase AND $100 <> 0) then
           output := LOG_SIN_VAL[(phase AND $0ff) XOR $0ff]
         else output := LOG_SIN_VAL[phase and $0ff];
       end;

    // Half-Sine
    1: begin
         If (phase AND $200 <> 0) then
           output := $1000
         else If (phase AND $100 <> 0) then
                output := LOG_SIN_VAL[(phase AND $0ff) xor $0ff]
              else output := LOG_SIN_VAL[phase AND $0ff];
       end;

    // Abs-Sine
    2: begin
         If (phase AND $100 <> 0) then
           output := LOG_SIN_VAL[(phase AND $0ff) XOR $0ff]
         else output := LOG_SIN_VAL[phase AND $0ff];
       end;

    // Pulse-Sine
    3: begin
         If (phase AND $100 <> 0) then
           output := $1000
         else output := LOG_SIN_VAL[phase AND $0ff];
       end;

    // Sine (EPO)
    4: begin
         If (phase AND $300 = $100) then
           invert := NOT invert;
         If (phase AND $200 <> 0) then
           output := $1000
         else If (phase AND $80 <> 0) then
                output := LOG_SIN_VAL[((phase XOR $0ff) SHL 1) AND $0ff]
              else output := LOG_SIN_VAL[(phase SHL 1) AND $0ff];
       end;

    // Abs-Sine (EPO)
    5: begin
         If (phase AND $200 <> 0) then
           output := $1000
         else If (phase AND $80 <> 0) then
                output := LOG_SIN_VAL[((phase XOR $0ff) SHL 1) AND $0ff]
              else output := LOG_SIN_VAL[(phase SHL 1) AND $0ff];
       end;

    // Square
    6: begin
         If (phase AND $200 <> 0) then
           invert := WORD_NULL;
         output := 0;
       end;

    // Derived Square
    7: begin
         If (phase AND $200 <> 0) then
           begin
             invert := NOT invert;
             phase := (phase AND $1ff) XOR $1ff;
           end;
         output := phase SHL 3;
       end;
  end;

  level := limit_value(output + (eg_out SHL 3),0,$1fff);
  envelope_calc_sin := SMALLINT(((EXP_VAL[(level AND $0ff) XOR $0ff] OR $400) SHL 1) SHR
                                (level SHR 8)) XOR invert;
end;
