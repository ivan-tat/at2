// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

function GetKey: Word;

function getkey_proc: Word;

var
  event: SDL_Event;
  i,j: Integer;

begin
  Repeat
    draw_screen;
    If (SDL_PollEvent(@event) <> 0) then
      begin
        If (event.eventtype = SDL_EVENTQUIT) or _force_program_quit then
          begin
            _force_program_quit := TRUE;
            getkey_proc := kESC;
            EXIT;
          end;
        // skip all other event except key presses
        If (event.eventtype <> SDL_KEYDOWN) then CONTINUE
        else
          begin
            // skip all modifier keys
            If (event.key.keysym.sym >= SDLK_NUMLOCK) then CONTINUE;
            // roll thru symtab, form correct GetKey value
            For j := 0 to PRED(SYMTABSIZE) do
              begin
                If (event.key.keysym.sym = symtab[j*10+9]) then
                  begin // first check with modifier keys, order: ALT, CTRL, SHIFT (as DOS does)
                    { ALT }
                    If (keydown[SC_LALT] = TRUE) or (keydown[SC_RALT] = TRUE) then
                      begin
                        // impossible combination
                        If (symtab[j*10+4] = WORD_NULL) then CONTINUE;
                        If (symtab[j*10+4] > BYTE_NULL) then
                          begin
                            getkey_proc := symtab[j*10+4];
                            EXIT;
                          end;
                        getkey_proc := (symtab[j*10] SHL 8) OR symtab[j*10+4];
                        EXIT;
                      end;
                    { CTRL }
                    If (keydown[SC_LCTRL] = TRUE) or (keydown[SC_RCTRL] = TRUE) then
                      begin
                        // impossible combination
                        If (symtab[j*10+3] = WORD_NULL) then CONTINUE;
                        If (symtab[j*10+3] > BYTE_NULL) then
                          begin
                            getkey_proc := symtab[j*10+3];
                            EXIT;
                          end;
                        getkey_proc := (symtab[j*10] SHL 8) OR symtab[j*10+3];
                        EXIT;
                      end;
                    { SHIFT }
                    If (keydown[SC_LSHIFT] = TRUE) or (keydown[SC_RSHIFT] = TRUE) then
                      begin
                        i := 2; // SHIFT
                        If (_capslock = TRUE) then i := 7 // caps lock
                        else If (_numlock = TRUE) then i := 8; // num lock
                        // impossible combination
                        If (symtab[j*10+i] = WORD_NULL) then CONTINUE;
                        If (symtab[j*10+i] > BYTE_NULL) then getkey_proc := symtab[j*10+i]
                        else getkey_proc := (symtab[j*10] SHL 8) OR symtab[j*10+i];
                        EXIT;
                      end;
                    { normal ASCII }
                    i := 1;
                    If (_capslock = TRUE) then i := 6 // caps lock
                    else If (_numlock = TRUE) then i := 5; // num lock
                    // impossible combination
                    If (symtab[j*10+i] = WORD_NULL) then CONTINUE;
                    If (symtab[j*10+i] > BYTE_NULL) then getkey_proc := symtab[j*10+i]
                    else getkey_proc := (symtab[j*10] SHL 8) OR symtab[j*10+i]; // (scancode << 8) + ASCII
                    EXIT;
                  end;
              end;
          end;
      end;
  until FALSE;
end;

begin
  Repeat draw_screen until keypressed;
  // filter out CTRL+TAB combo as it is handled within timer routine
  If ctrl_tab_pressed then
    begin
      draw_screen;
      keyboard_reset_buffer;
      GetKey := WORD_NULL;
    end
  else GetKey := getkey_proc;
end;
