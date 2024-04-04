# Porting to C

| Pascal | C | Transitional C header | C header |
| --- | --- | --- | --- |
| `len := Length(s);` | `len = Length(s);` | `"pascal.h"` | - |
| `s[0] := Chr(len);` | `SetLength(s, len);` | `"pascal.h"` | - |
| `p := @s[1];` | `p = GetStr(s);` | `"pascal.h"` | - |
| `s := 'Text';` | `StrToString(s, "Text", 255);` | `"pascal.h"` | - |
| `t := s;` | `CopyString(t, s, 255);` | `"pascal.h"` | - |
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

## Issues

* For FPC version 2.6.4 and GO32V2 target: always define external variables as public (bug in FPC).
