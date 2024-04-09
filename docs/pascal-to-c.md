# Porting to C

| Pascal | C | Transitional C header | C header |
| --- | --- | --- | --- |
| `var s: String;` | `String s[255+1];` | `"pascal.h"` | - |
| `const s: String[78] = 'Text';` | `String s[78+1] = "\x04" "Text";` | `"pascal.h"` | - |
| `len := Length(s);` | `len = Length(s);` | `"pascal.h"` | - |
| `s[0] := Chr(len);` | `SetLength(s, len);` | `"pascal.h"` | - |
| `p := @s[1];` | `p = GetStr(s);` | `"pascal.h"` | - |
| `s := 'Text';` | `StrToString(s, "Text", 255);` | `"pascal.h"` | - |
| `s := StrPas(p);` | `StrToString(s, p, 255);` | `"pascal.h"` | - |
| `t := s;` | `CopyString(t, s, 255);` | `"pascal.h"` | - |
| `t := t + s;` | `AppendString(t, s, 255);` | `"pascal.h"` | - |
| `Move(src, dest, count);` | `memmove(dest, src, count);` | `"pascal/string.h"` | `<string.h>` |
| `FillChar(buf, count, b);` | `memset(buf, b, count);` | `"pascal/string.h"` | `<string.h>` |
| `FillWord(buf, count, w);` | `memsetw(buf, w, count);` | `"common.h"` | `"common.h"` |
| `Delay(msec);` | `delay(msec);` | `"pascal/dos.h"` | `<dos.h>` |

## Interrupts

| Pascal | C | Transitional C header | C header |
| --- | --- | --- | --- |
| `enable;` | `enable();` | `"pascal/dos.h"` | `<dos.h>` |
| `asm sti end;` | `enable();` | `"pascal/dos.h"` | `<dos.h>` |
| `disable;` | `disable();` | `"pascal/dos.h"` | `<dos.h>` |
| `asm cli end;` | `disable();` | `"pascal/dos.h"` | `<dos.h>` |

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

## DOS memory

| Pascal | C | Transitional C header | C header |
| --- | --- | --- | --- |
| `int31error` | `__dpmi_error` | `"pascal/dpmi.h"` | `<dpmi.h>` |
| `dosmemselector` | `_dos_ds` | `"pascal/go32.h"` | `<go32.h>` |
| `dosmemget(seg, ofs, data, size);` | `dosmemget(seg*16+ofs, size, data);` | `"pascal/go32.h"` | `<go32.h>` |
| `dosmemput(seg, ofs, data, size);` | `dosmemput(data, size, seg*16+ofs);` | `"pascal/go32.h"` | `<go32.h>` |
| `b := mem[seg:ofs];` | `b = _farnspeekb(seg*16+ofs);` | `"pascal/farptr.h"` | `<farptr.h>` |
| `w := memw[seg:ofs];` | `w = _farnspeekw(seg*16+ofs);` | `"pascal/farptr.h"` | `<farptr.h>` |
| `l := meml[seg:ofs];` | `l = _farnspeekl(seg*16+ofs);` | `"pascal/farptr.h"` | `<farptr.h>` |
| `mem[seg:ofs] := b;` | `_farnspokeb(seg*16+ofs, b);` | `"pascal/farptr.h"` | `<farptr.h>` |
| `memw[seg:ofs] := w;` | `_farnspokew(seg*16+ofs, w);` | `"pascal/farptr.h"` | `<farptr.h>` |
| `meml[seg:ofs] := l;` | `_farnspokel(seg*16+ofs, l);` | `"pascal/farptr.h"` | `<farptr.h>` |

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

## DPMI function 0100h: Allocate DOS Memory Block

### Pascal

**Unit**: go32

```Pascal
function global_dos_alloc(bytes: Longint): Longint;
```

**Return values**:

* On success: *selector* + *segment* * 65536.
* On error: `0`.
  * `int31error` is set to DOS error code.

### C

| Transitional C header | C header |
| --- | --- |
| `"pascal/dpmi.h"` | `<dpmi.h>` |

```C
int32_t __dpmi_allocate_dos_memory(int32_t paras, int32_t *ret);
```

**Return values**:

* On success: *segment*.
  * `*ret` is set to *selector*.
* On error: `-1`.
  * `*ret` is set to a size of largest available block in paragraphs.
  * `__dpmi_error` is set to DOS error code.

## DPMI function 0101h: Free DOS Memory Block

### Pascal

**Unit**: go32

```Pascal
function global_dos_free(selector: Word): Boolean;
```

**Return values**:

* On success: `true`.
* On error: `false`.
  * DOS error code is ignored.

### C

| Transitional C header | C header |
| --- | --- |
| `"pascal/dpmi.h"` | `<dpmi.h>` |

```C
int32_t __dpmi_free_dos_memory(int32_t selector);
```

**Return values**:

* On success: `0`.
* On error: `-1`.
  * `__dpmi_error` is set to DOS error code.

## DPMI function 0300h: Simulate Real Mode Interrupt

### Pascal

**Unit**: go32

```Pascal
function realintr(n: Word; var regs: trealregs): Boolean;
```

**Return values**:

* On success: `true`.
* On error: `false`.
  * DOS error code is ignored.

### C

| Transitional C header | C header |
| --- | --- |
| `"pascal/dpmi.h"` | `<dpmi.h>` |

```C
int32_t __dpmi_simulate_real_mode_interrupt(int32_t n, __dpmi_regs *regs);
```

**Return values**:

* On success: `0`.
* On error: `-1`.
  * `__dpmi_error` is set to DOS error code.

### Input values

* Parameter `n`:
  * Bits 7..0 is interrupt number (0..255).
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

## Issues

* For FPC version 2.6.4 and GO32V2 target: always define external variables as public (bug in FPC).
