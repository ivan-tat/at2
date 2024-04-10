// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2024 Ivan Tatarinov
// SPDX-License-Identifier: GPL-3.0-or-later

procedure __dpmi_allocate_dos_memory; cdecl; external;
procedure __dpmi_free_dos_memory; cdecl; external;
procedure __dpmi_simulate_real_mode_interrupt; cdecl; external;
procedure __dpmi_physical_address_mapping; cdecl; external;
