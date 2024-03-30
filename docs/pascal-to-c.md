# Porting to C

| Pascal | C |
| --- | --- |
| `len := Length(s);` | `len = Length(s);` |
| `p := @s[1];` | `p = GetString(s);` |
| `Move(source, dest, count);` | `memmove(dest, source, count);` |
| `FillChar(x, count, value);` | `memset(x, value, count);` |
| `FillWord(x, count, value);` | `memsetw(x, value, count);` |
