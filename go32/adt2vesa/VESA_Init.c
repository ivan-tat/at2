// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2024 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

void VESA_Init (void) {
  uint8_t i, j;
  __dpmi_regs regs;
  int32_t dos_sel, dos_seg;

  if (VESA_GraphicsSysInited)
    return;

  VESA_ModeList[0].ModeNumber = 0x13;
  VESA_ModeList[0].XResolution = 320;
  VESA_ModeList[0].YResolution = 200;
  VESA_ModeList[0].BufferAddress = (void *) 0xA0000;

  dos_seg = __dpmi_allocate_dos_memory ((sizeof (tVESA_Info) + 15) / 16,
                                        &dos_sel);
  if (dos_seg < 0) {
    // FIXME: no check for error in original code
  }

  VESA_Info.Signature = MK_UINT32 ('V', 'B', 'E', '2');
  memset ((uint8_t *) &VESA_Info + 4, 0, sizeof (tVESA_Info) - 4);
  // Indicate VBE 2.0 information is desired and the information
  // block is 512 bytes in size
  dosmemput (&VESA_Info, 4, dos_seg * 16);

  regs.d.eax = 0x4F00; // Return VBE Controller Information
  regs.x.ds = dos_seg;
  regs.x.es = dos_seg; // ES:DI = Pointer to buffer in which to place
  regs.d.edi = 0;      // VbeInfoBlock structure
  if (__dpmi_simulate_real_mode_interrupt (0x10, &regs)) {
    // FIXME: no check for error in original code
  }

  dosmemget (dos_seg * 16, sizeof (tVESA_Info), &VESA_Info);
  __dpmi_free_dos_memory (dos_sel);

  if (VESA_Info.Signature != MK_UINT32 ('V', 'E', 'S', 'A'))
    return; // ERROR: VESA BIOS extensions not found!

  VESA_Version = VESA_Info.Version;

  if ((VESA_Version >> 8) < 2)
    return; // ERROR: VESA 2.0 required!

  StrToString (VESA_OEM_String,
               (char *) &VESA_Info + VESA_Info.OEM_StringPtr, 255);
  VESA_Capabilities = VESA_Info.Capabilities;
  VESA_VideoMemory = VESA_Info.VideoMemory << 6;
  VESA_OEM_SoftwareRevision = VESA_Info.OEM_SoftwareRevision;
  StrToString (VESA_OEM_VendorName,
               (char *) &VESA_Info + VESA_Info.OEM_VendorNamePtr, 255);
  StrToString (VESA_OEM_ProductName,
               (char *) &VESA_Info + VESA_Info.OEM_ProductNamePtr, 255);
  StrToString (VESA_OEM_ProductRevision,
               (char *) &VESA_Info + VESA_Info.OEM_ProductRevisionPtr, 255);
  dosmemget (((VESA_Info.ModeListPtr >> 12) & 0xFFFF0)
             + (VESA_Info.ModeListPtr & 0xFFFF),
             sizeof (tModeList), ModeList);

  dos_seg = __dpmi_allocate_dos_memory ((sizeof (tModeInfoBlock) + 15) / 16,
                                        &dos_sel);
  if (dos_seg < 0) {
    // FIXME: no check for error in original code
  }

  i = 0;
  j = 1;

  do {
    VESA_GetModeInfo (ModeList[i], dos_seg * 16);
    dosmemget (dos_seg * 16, sizeof (tModeInfoBlock), &ModeInfoBlock);
    i++;

    // Mode attributes to end loop:
    // [+] Mode supported in hardware (bit 0 = 1)
    // [+] Graphics mode (bit 4 = 1)
    // [+] Linear frame buffer mode (bit 7 = 1)
  } while ((((ModeInfoBlock.ModeAttributes & 0x0091) != 0x0091)
            || (ModeInfoBlock.NumberOfPlanes != 1)
            || (ModeInfoBlock.BitsPerPixel != 8))
           && (ModeList[i - 1] != 0xFFFF));

  if (ModeList[i - 1] != 0xFFFF) {
    j++;

    VESA_ModeList[1].ModeNumber = ModeList[i - 1];
    VESA_ModeList[1].XResolution = ModeInfoBlock.XResolution;
    VESA_ModeList[1].YResolution = ModeInfoBlock.YResolution;
    VESA_ModeList[1].BufferAddress = ModeInfoBlock.PhysBasePtr;

    while ((i <= 127) && (ModeList[i] != 0xFFFF)) {
      VESA_GetModeInfo (ModeList[i], dos_seg * 16);
      dosmemget (dos_seg * 16, sizeof (tModeInfoBlock), &ModeInfoBlock);

      // Mode attributes:
      // [+] Mode supported in hardware (bit 0 = 1)
      // [+] Graphics mode (bit 4 = 1)
      // [+] Linear frame buffer mode (bit 7 = 1)
      if (((ModeInfoBlock.ModeAttributes & 0x0091) == 0x0091)
          && (ModeInfoBlock.NumberOfPlanes == 1)
          && (ModeInfoBlock.BitsPerPixel == 8)) {
        VESA_ModeList[j].ModeNumber = ModeList[i];
        VESA_ModeList[j].XResolution = ModeInfoBlock.XResolution;
        VESA_ModeList[j].YResolution = ModeInfoBlock.YResolution;
        j++;
      }

      i++;
    }
  }

  __dpmi_free_dos_memory (dos_sel);
  VESA_NumberOfModes = j - 1;

  if (VESA_NumberOfModes >= 1) {
    __dpmi_meminfo info = {
      .size = 4096 * 1024,
      .address = (uint32_t) VESA_ModeList[1].BufferAddress
    };

    if (__dpmi_physical_address_mapping (&info)
    ||  (!info.address))
      return; // ERROR: Cannot remap LFB to linear address space!

    VESA_ModeList[1].BufferAddress = (void *) info.address;

    for (i = 2; i <= VESA_NumberOfModes; i++)
      VESA_ModeList[i].BufferAddress = VESA_ModeList[1].BufferAddress;
  }

  VESA_Mode = 0;
  VESA_GraphicsSysInited = true;
}
