// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure keyboard_init;
begin
  _dbg_enter ({$I %FILE%}, 'keyboard_init');

  FillChar(keydown,SizeOf(keydown),FALSE);
  user_proc_ptr := @int09_user_proc;
  lock_data(user_proc_ptr,SizeOf(user_proc_ptr));
  lock_data(DosMemSelector,SizeOf(DosMemSelector));
  lock_code(@int09_user_proc,DWORD(@int09_user_proc_end)-DWORD(@int09_user_proc));
  lock_code(@newint09_proc,DWORD(@newint09_proc_end)-DWORD(@newint09_proc));
  newint09_handler.offset := @newint09_proc;
  newint09_handler.segment := get_cs;
  get_pm_interrupt($09,oldint09_handler);
  set_pm_interrupt($09,newint09_handler);

  _dbg_leave; //EXIT //keyboard_init
end;
