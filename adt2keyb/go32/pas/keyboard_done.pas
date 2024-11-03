// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

procedure keyboard_done;
begin
  _dbg_enter ({$I %FILE%}, 'keyboard_done');

  set_pm_interrupt($09,oldint09_handler);
  unlock_data(DosMemSelector,SizeOf(DosMemSelector));
  unlock_data(user_proc_ptr,SizeOf(user_proc_ptr));
  unlock_code(@int09_user_proc,DWORD(@int09_user_proc_end)-DWORD(@int09_user_proc));
  lock_code(@newint09_proc,DWORD(@newint09_proc_end)-DWORD(@newint09_proc));
  keyboard_reset_buffer;

  _dbg_leave; //EXIT //keyboard_done
end;
