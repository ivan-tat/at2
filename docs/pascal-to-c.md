# Porting to C

| Pascal | C | Transitional C header | C header |
| --- | --- | --- | --- |
| `ExitProc := @NewExit;` | `atexit(NewExit);` | `"pascal/stdlib.h"` | `<stdlib.h>` |
| `Halt(err);` | `exit(err);` | `"pascal/stdlib.h"` | `<stdlib.h>` |
| `var s: String;` | `String s[255+1];` | `"pascal.h"` | - |
| `const s: String[78] = 'Text';` | `String s[78+1] = "\x04" "Text";` | `"pascal.h"` | - |
| `len := Length(s);` | `len = Length(s);` | `"pascal.h"` | - |
| `s[0] := Chr(len);` | `SetLength(s, len);` | `"pascal.h"` | - |
| `p := @s[1];` | `p = GetStr(s);` | `"pascal.h"` | - |
| `s := 'Text';` | `StrToString(s, "Text", 255);` | `"pascal.h"` | - |
| `s := StrPas(p);` | `StrToString(s, p, 255);` | `"pascal.h"` | - |
| `t := s;` | `CopyString(t, s, 255);` | `"pascal.h"` | - |
| `t := t + s;` | `AppendString(t, s, 255);` | `"pascal.h"` | - |
| `GetMem(p, size);` | `p = malloc(size);` | `"pascal/stdlib.h"` | `<stdlib.h>` |
| `FreeMem(p, size);` | `free(p);` | `"pascal/stdlib.h"` | `<stdlib.h>` |
| `Move(src, dest, count);` | `memmove(dest, src, count);` | `"pascal/string.h"` | `<string.h>` |
| `FillChar(buf, count, b);` | `memset(buf, b, count);` | `"pascal/string.h"` | `<string.h>` |
| `FillWord(buf, count, w);` | `memsetw(buf, w, count);` | `"common.h"` | `"common.h"` |
| `Delay(msec);` | `delay(msec);` | `"pascal/dos.h"` | `<dos.h>` |
| `Write(...);` | `printf(...);` | `"pascal/stdio.h"` | `<stdio.h>` |
| `WriteLn(...);` | `printf(...);` | `"pascal/stdio.h"` | `<stdio.h>` |

## Floating point math

| Pascal | C | Transitional C header | C header |
| --- | --- | --- | --- |
| `F := Int(F);` | `F = truncf(F);` | `"pascal/math.h"` | `<math.h>` |
| `D := Int(D);` | `D = trunc(D);` | `"pascal/math.h"` | `<math.h>` |
| `q := Trunc(F);` | `q = (int64_t)truncf(F);` | `"pascal/math.h"` | `<math.h>` |
| `q := Trunc(D);` | `q = (int64_t)trunc(D);` | `"pascal/math.h"` | `<math.h>` |
| `F := Frac(F);` | `F = F-truncf(F);` | `"pascal/math.h"` | `<math.h>` |
| `D := Frac(D);` | `D = D-trunc(D);` | `"pascal/math.h"` | `<math.h>` |
| `q := Round(F);` | `q = (int64_t)F;` | `"pascal/math.h"` | `<math.h>` |
| `q := Round(D);` | `q = (int64_t)D;` | `"pascal/math.h"` | `<math.h>` |

## Interrupts

| Pascal | C | Transitional C header | C header |
| --- | --- | --- | --- |
| `enable;` | `enable();` | `"pascal/dos.h"` | `<dos.h>` |
| `asm sti end;` | `__asm__ ("sti");` | - | - |
| `disable;` | `disable();` | `"pascal/dos.h"` | `<dos.h>` |
| `asm cli end;` | `__asm__ ("cli");` | - | - |
| `get_pm_interrupt(n, p);` | `__dpmi_get_protected_mode_interrupt_vector(n, &p);` | `"pascal/dpmi.h"` | `<dpmi.h>` |
| `set_pm_interrupt(n, p);` | `__dpmi_set_protected_mode_interrupt_vector(n, &p);` | `"pascal/dpmi.h"` | `<dpmi.h>` |

## Port I/O

| Pascal | C | Transitional C header | C header |
| --- | --- | --- | --- |
| `asm in al,dx end;` | `b = inportb(port);` | `"pascal/pc.h"` | `<pc.h>` |
| `asm in ax,dx end;` | `w = inportw(port);` | `"pascal/pc.h"` | `<pc.h>` |
| `asm in eax,dx end;` | `l = inportl(port);` | `"pascal/pc.h"` | `<pc.h>` |
| `asm rep insb end;` | `inportsb(port, buf, len);` | `"pascal/pc.h"` | `<pc.h>` |
| `asm out dx,al end;` | `outportb(port, b);` | `"pascal/pc.h"` | `<pc.h>` |
| `asm out dx,ax end;` | `outportw(port, w);` | `"pascal/pc.h"` | `<pc.h>` |
| `asm out dx,eax end;` | `outportl(port, l);` | `"pascal/pc.h"` | `<pc.h>` |
| `asm rep outsb end;` | `outportsb(port, buf, len);` | `"pascal/pc.h"` | `<pc.h>` |

## DPMI and DOS memory

| Pascal | C | Transitional C header | C header |
| --- | --- | --- | --- |
| `int31error` | `__dpmi_error` | `"pascal/dpmi.h"` | `<dpmi.h>` |
| `__v2prt0_ds_alias` | `__djgpp_ds_alias` | `"pascal/sys/exceptn.h"` | `<sys/exceptn.h>` |
| `w := get_cs;` | `w = _go32_my_cs();` | `"pascal/go32.h"` | `<go32.h>` |
| `w := get_ds;` | `w = _go32_my_ds();` | `"pascal/go32.h"` | `<go32.h>` |
| `w := dosmemselector;` | `w = _dos_ds;` | `"pascal/go32.h"` | `<go32.h>` |
| `dosmemget(seg, ofs, data, size);` | `dosmemget(seg*16+ofs, size, data);` | `"pascal/go32.h"` | `<go32.h>` |
| `dosmemput(seg, ofs, data, size);` | `dosmemput(data, size, seg*16+ofs);` | `"pascal/go32.h"` | `<go32.h>` |
| `b := mem[seg:ofs];` | `b = _farnspeekb(seg*16+ofs);` | `"pascal/sys/farptr.h"` | `<sys/farptr.h>` |
| `w := memw[seg:ofs];` | `w = _farnspeekw(seg*16+ofs);` | `"pascal/sys/farptr.h"` | `<sys/farptr.h>` |
| `l := meml[seg:ofs];` | `l = _farnspeekl(seg*16+ofs);` | `"pascal/sys/farptr.h"` | `<sys/farptr.h>` |
| `mem[seg:ofs] := b;` | `_farnspokeb(seg*16+ofs, b);` | `"pascal/sys/farptr.h"` | `<sys/farptr.h>` |
| `memw[seg:ofs] := w;` | `_farnspokew(seg*16+ofs, w);` | `"pascal/sys/farptr.h"` | `<sys/farptr.h>` |
| `meml[seg:ofs] := l;` | `_farnspokel(seg*16+ofs, l);` | `"pascal/sys/farptr.h"` | `<sys/farptr.h>` |
| `lock_data(x, s);` | `_go32_dpmi_lock_data(&x, s);` | `"pascal/dpmi.h"` | `<dpmi.h>` |
| `lock_code(@x, s);` | `_go32_dpmi_lock_code((void *)(uintptr_t)x, s);` | `"pascal/dpmi.h"` | `<dpmi.h>` |
| `unlock_data(x, s);` | `_go32_dpmi_unlock_data(&x, s);` | `"go32/dpmi.h"` | `"go32/dpmi.h"` |
| `unlock_code(@x, s);` | `_go32_dpmi_unlock_code((void *)(uintptr_t)x, s);` | `"go32/dpmi.h"` | `"go32/dpmi.h"` |

Before accessing DOS memory with `_farns*` routines:

```C
uint16_t orig_fs;
...
orig_fs = _fargetsel(); // save original FS register
_farsetsel(_dos_ds);    // set proper FS register
```

After accessing DOS memory:

```C
_farsetsel(orig_fs); // restore original FS register
```

This is not needed when linking with FPC: `FS` register is already set to `_dos_ds`.
And you should always take care of what you do with `FS` register across function calls.

## DPMI function 0000h: Allocate LDT Descriptors

### Pascal

**Unit**: go32

```Pascal
function allocate_ldt_descriptors(count: Word): Word;
```

**Return values**:

* On success:
  * Returns *base selector*.
* On error:
  * Returns DPMI error code.
  * DPMI error is ignored and not handled properly.

### C

| Transitional C header | C header |
| --- | --- |
| `"pascal/dpmi.h"` | `<dpmi.h>` |

```C
int32_t __dpmi_allocate_ldt_descriptors(int32_t count);
```

**Return values**:

* On success:
  * Returns *base selector*.
* On error:
  * Returns `-1`.
  * `__dpmi_error` is set to DPMI error code.

## DPMI function 0001h: Free LDT Descriptor

### Pascal

**Unit**: go32

```Pascal
function free_ldt_descriptor(selector: Word): Boolean;
```

**Return values**:

* On success:
  * Returns `true`.
* On error:
  * Returns `false`.
  * `int31error` is set to DPMI error code.

### C

| Transitional C header | C header |
| --- | --- |
| `"pascal/dpmi.h"` | `<dpmi.h>` |

```C
int32_t __dpmi_free_ldt_descriptor(int32_t selector);
```

**Return values**:

* On success:
  * Returns `0`.
* On error:
  * Returns `-1`.
  * `__dpmi_error` is set to DPMI error code.

## DPMI function 0003h: Get Next Selector Increment Value

### Pascal

**Unit**: go32

```Pascal
function get_next_selector_increment_value: Word;
```

**Return values**:

* On success:
  * Returns value to add to get to next selector.
* On error:
  * Returns DPMI error code.
  * DPMI error is ignored and not handled properly.

### C

| Transitional C header | C header |
| --- | --- |
| `"pascal/dpmi.h"` | `<dpmi.h>` |

```C
int32_t __dpmi_get_selector_increment_value(void);
```

**Return values**:

* On success:
  * Returns value to add to get to next selector.
* On error:
  * Returns `-1`.
  * `__dpmi_error` is set to DPMI error code.

## DPMI function 0007h: Set Segment Base Address

### Pascal

**Unit**: go32

```Pascal
function set_segment_base_address(selector: Word; address: Longint): Boolean;
```

**Return values**:

* On success:
  * Returns `true`.
* On error:
  * Returns `false`.
  * `int31error` is set to DPMI error code.

### C

| Transitional C header | C header |
| --- | --- |
| `"pascal/dpmi.h"` | `<dpmi.h>` |

```C
int32_t __dpmi_set_segment_base_address(int32_t selector, uint32_t address);
```

**Return values**:

* On success:
  * Returns `0`.
* On error:
  * Returns `-1`.
  * `__dpmi_error` is set to DPMI error code.

## DPMI function 0008h: Set Segment Limit

### Pascal

**Unit**: go32

```Pascal
function Pascal_set_segment_limit(selector: Word; limit: Longint): Boolean;
```

**Return values**:

* On success:
  * Returns `true`.
* On error:
  * Returns `false`.
  * `int31error` is set to DPMI error code.

### C

| Transitional C header | C header |
| --- | --- |
| `"pascal/dpmi.h"` | `<dpmi.h>` |

```C
int32_t __dpmi_set_segment_limit(int32_t selector, uint32_t limit);
```

**Return values**:

* On success:
  * Returns `0`.
* On error:
  * Returns `-1`.
  * `__dpmi_error` is set to DPMI error code.

## DPMI function 0100h: Allocate DOS Memory Block

### Pascal

**Unit**: go32

```Pascal
function global_dos_alloc(bytes: Longint): Longint;
```

**Return values**:

* On success:
  * Returns *selector* + *segment* * `65536`.
* On error:
  * Returns `0`.
  * `int31error` is set to DPMI error code.

### C

| Transitional C header | C header |
| --- | --- |
| `"pascal/dpmi.h"` | `<dpmi.h>` |

```C
int32_t __dpmi_allocate_dos_memory(int32_t paras, int32_t *ret);
```

**Return values**:

* On success:
  * Returns initial real mode *segment* of allocated block.
  * `*ret` is set to *selector* for allocated block.
* On error:
  * Returns `-1`.
  * `*ret` is set to a *size* of largest available block in paragraphs.
  * `__dpmi_error` is set to DPMI error code.

## DPMI function 0101h: Free DOS Memory Block

### Pascal

**Unit**: go32

```Pascal
function global_dos_free(selector: Word): Boolean;
```

**Return values**:

* On success:
  * Returns `true`.
* On error:
  * Returns `false`.
  * DPMI error code is ignored.

### C

| Transitional C header | C header |
| --- | --- |
| `"pascal/dpmi.h"` | `<dpmi.h>` |

```C
int32_t __dpmi_free_dos_memory(int32_t selector);
```

**Return values**:

* On success:
  * Returns `0`.
* On error:
  * Returns `-1`.
  * `__dpmi_error` is set to DPMI error code.

## DPMI function 0300h: Simulate Real Mode Interrupt

### Pascal

**Unit**: go32

```Pascal
function realintr(n: Word; var regs: trealregs): Boolean;
```

**Return values**:

* On success:
  * Returns `true`.
* On error:
  * Returns `false`.
  * DPMI error code is ignored.

### C

| Transitional C header | C header |
| --- | --- |
| `"pascal/dpmi.h"` | `<dpmi.h>` |

```C
int32_t __dpmi_simulate_real_mode_interrupt(int32_t n, __dpmi_regs *regs);
```

**Return values**:

* On success:
  * Returns `0`.
* On error:
  * Returns `-1`.
  * `__dpmi_error` is set to DPMI error code.

### Input values

* Parameter `n`:
  * Bits 7..0 are interrupt number (0..255).
  * If bit 8 is 1 - resets the interrupt controller and A20 line.
  * Other bits must be 0.

### Structure's members

| Pascal | C |
| :---: | :---: |
| (`trealregs`) | (`__dpmi_regs`) |
| --- | --- |
| *Longint* | *uint32_t* |
| `EDI`, `stuff[1]`, `RealEDI` | `d.edi` |
| `ESI`, `stuff[2]`, `RealESI` | `d.esi` |
| `EBP`, `stuff[3]`, `RealEBP` | `d.ebp` |
| `Res`, `stuff[4]`, `RealRES` | `d.res` |
| `EBX`, `RealEBX` | `d.ebx` |
| `EDX`, `RealEDX` | `d.edx` |
| `ECX`, `RealECX` | `d.ecx` |
| `EAX`, `RealEAX` | `d.eax` |
| --- | --- |
| *Word* | *uint16_t* |
| *low => high* | *low => high* |
| `DI`, `DI2` | `x.di`, `x.di_hi` |
| `SI`, `SI2` | `x.si`, `x.si_hi` |
| `BP`, `BP2` | `x.bp`, `x.bp_hi` |
| `R1`, `R2` | `x.res`, `x.res_hi` |
| `BX`, `BX2` | `x.bx`, `x.bx_hi` |
| `DX`, `DX2` | `x.dx`, `x.dx_hi` |
| `CX`, `CX2` | `x.cx`, `x.cx_hi` |
| `AX`, `AX2` | `x.ax`, `x.ax_hi` |
| --- | --- |
| *Word* | *uint16_t* |
| `Flags`, `RealFlags` | `x.flags` |
| `ES`, `RealES` | `x.es` |
| `DS`, `RealDS` | `x.ds` |
| `FS`, `RealFS` | `x.fs` |
| `GS`, `RealGS` | `x.gs` |
| `IP`, `RealIP` | `x.ip` |
| `CS`, `RealCS` | `x.cs` |
| `SP`, `RealSP` | `x.sp` |
| `SS`, `RealSS` | `x.ss` |
| --- | --- |
| *Byte* | *uint8_t* |
| *low => high* | *low => high* |
| - | `h.edi[0]`, `h.edi[1]`, `h.edi[2]`, `h.edi[3]` |
| - | `h.esi[0]`, `h.esi[1]`, `h.esi[2]`, `h.esi[3]` |
| - | `h.ebp[0]`, `h.ebp[1]`, `h.ebp[2]`, `h.ebp[3]` |
| - | `h.res[0]`, `h.res[1]`, `h.res[2]`, `h.res[3]` |
| `BL`, `BH`, `BL2`, `BH2` | `h.bl`, `h.bh`, `h.ebx_b2`, `h.ebx_b3` |
| `DL`, `DH`, `DL2`, `DH2` | `h.dl`, `h.dh`, `h.edx_b2`, `h.edx_b3` |
| `CL`, `CH`, `CL2`, `CH2` | `h.cl`, `h.ch`, `h.ecx_b2`, `h.ecx_b3` |
| `AL`, `AH`, `AL2`, `AH2` | `h.al`, `h.ah`, `h.eax_b2`, `h.eax_b3` |

### Implementations

| Behavior | Pascal | C |
| :--- | :---: | :---: |
| Zeroes input `Res`, `SP`, `SS` | yes | no |
| Preserves `FS` | yes | no |

## DPMI function 0800h: Physical Address Mapping

### Pascal

**Unit**: go32

```Pascal
function get_linear_addr(phys_addr: Longint; size: Longint): Longint;
```

**Return values**:

* On success:
  + Returns *linear address* that can be used to access the physical memory.
  + `int31error` is set to `0`.
* On error:
  + Return value is undefined.
  + `int31error` is set to DPMI error code.

### C

| Transitional C header | C header |
| --- | --- |
| `"pascal/dpmi.h"` | `<dpmi.h>` |

```C
int32_t __dpmi_physical_address_mapping(__dpmi_meminfo *info);
```

**Input values**:

* `info->handle` is ignored.
* `info->size` is a *size* of region to map in bytes.
* `info->address` is a *physical address* of memory.

**Return values**:

* On success:
  * Returns `0`.
  * `info->address` is set to *linear address* that can be used to access the physical memory.
* On error:
  * Returns `-1`.
  * `__dpmi_error` is set to DPMI error code.

## Issues

* For FPC version 2.6.4 and GO32V2 target: always define external variables as public (bug in FPC).
