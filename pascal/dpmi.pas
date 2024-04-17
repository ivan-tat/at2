// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

var
  __dpmi_error_ptr: ^Word; cvar; external;

procedure __dpmi_allocate_ldt_descriptors; cdecl; external;
procedure __dpmi_free_ldt_descriptor; cdecl; external;
procedure __dpmi_get_selector_increment_value; cdecl; external;
procedure __dpmi_set_segment_base_address; cdecl; external;
procedure __dpmi_set_segment_limit; cdecl; external;
procedure __dpmi_allocate_dos_memory; cdecl; external;
procedure __dpmi_free_dos_memory; cdecl; external;
procedure __dpmi_get_protected_mode_interrupt_vector; cdecl; external;
procedure __dpmi_set_protected_mode_interrupt_vector; cdecl; external;
procedure __dpmi_simulate_real_mode_interrupt; cdecl; external;
procedure __dpmi_physical_address_mapping; cdecl; external;
procedure _go32_dpmi_lock_data; cdecl; external;
procedure _go32_dpmi_lock_code; cdecl; external;
