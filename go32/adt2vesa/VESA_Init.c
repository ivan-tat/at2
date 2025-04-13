// This file is part of Adlib Tracker II (AT2).
//
// SPDX-FileType: SOURCE
// SPDX-FileCopyrightText: 2014-2025 The Adlib Tracker 2 Authors
// SPDX-License-Identifier: GPL-3.0-or-later

#define FARPTR_TO_LINEAR(x) (((x >> 12) & 0xFFFF0) + (x & 0xFFFF))

// s = pointer to string in DOS memory (seg:ofs)
static size_t dos_strnlen (uint32_t s, size_t n)
{
  uint32_t p = FARPTR_TO_LINEAR (s);
  size_t count = n;
  uint16_t orig_fs = _fargetsel ();

  _farsetsel (_dos_ds);
  while (_farnspeekb (p) && count)
  {
    p++; // be careful: no 1MB limit
    count--;
  }
  _farsetsel (orig_fs);

  return n - count;
}

// src = pointer to string in DOS memory (seg:ofs)
static void get_dos_str (char *dst, uint32_t src, size_t n)
{
  dosmemget (FARPTR_TO_LINEAR (src), dos_strnlen (src, n), dst);
}

// Returns 0 on success, error code otherwise.
int VESA_Init (void)
{
  int status = 0;
  uint8_t i, j;
  int32_t dos_sel, dos_seg = -1;
  char t[255+1];

  DBG_ENTER ("VESA_Init");

  if (VESA_GraphicsSysInited)
    goto _exit;

  VESA_ModeList[0].ModeNumber = 0x13;
  VESA_ModeList[0].XResolution = 320;
  VESA_ModeList[0].YResolution = 200;
  VESA_ModeList[0].BufferAddress = (void *) 0xA0000;

  status = VESA_GetInfo (&VESA_Info);
  if (status != 0)
    goto _error;

  if (VESA_Info.Signature != MK_UINT32 ('V', 'E', 'S', 'A'))
  {
    status = 3; // ERROR: VESA BIOS extensions not found
    goto _error;
  }

  VESA_Version = VESA_Info.Version;

  if ((VESA_Version >> 8) < 2)
  {
    status = 4; // ERROR: VESA 2.0 required
    goto _error;
  }

  get_dos_str (t, VESA_Info.OEM_StringPtr, 255);
  StrToString (VESA_OEM_String, t, 255);
  VESA_Capabilities = VESA_Info.Capabilities;
  VESA_VideoMemory = VESA_Info.VideoMemory << 6;
  VESA_OEM_SoftwareRevision = VESA_Info.OEM_SoftwareRevision;
  get_dos_str (t, VESA_Info.OEM_VendorNamePtr, 255);
  StrToString (VESA_OEM_VendorName, t, 255);
  get_dos_str (t, VESA_Info.OEM_ProductNamePtr, 255);
  StrToString (VESA_OEM_ProductName, t, 255);
  get_dos_str (t, VESA_Info.OEM_ProductRevisionPtr, 255);
  StrToString (VESA_OEM_ProductRevision, t, 255);
  dosmemget (FARPTR_TO_LINEAR (VESA_Info.ModeListPtr),
             sizeof (tModeList), &ModeList);

  dos_seg = __dpmi_allocate_dos_memory ((sizeof (tModeInfoBlock) + 15) / 16,
                                        &dos_sel);
  if (dos_seg < 0)
  {
    status = 5; // ERROR: failed to allocate `tModeInfoBlock' structure
    goto _error;
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
  dos_seg = -1;
  VESA_NumberOfModes = j - 1;

  if (VESA_NumberOfModes >= 1) {
    __dpmi_meminfo info = {
      .size = 4096 * 1024,
      .address = (uint32_t) VESA_ModeList[1].BufferAddress
    };

    if (__dpmi_physical_address_mapping (&info)
    ||  (!info.address))
    {
      status = 7; // ERROR: cannot remap LFB to linear address space
      goto _error;
    }

    VESA_ModeList[1].BufferAddress = (void *) info.address;

    for (i = 2; i <= VESA_NumberOfModes; i++)
      VESA_ModeList[i].BufferAddress = VESA_ModeList[1].BufferAddress;
  }

  VESA_Mode = 0;
  VESA_GraphicsSysInited = true;

_exit:
  DBG_LEAVE (); //EXIT //VESA_Init
  return status;

_error:
  if (dos_seg >= 0)
    __dpmi_free_dos_memory (dos_sel);

  goto _exit;
}
