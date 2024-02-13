{$IFDEF CPU64}
{ Use 8-byte alignment on 64-bit architectures, so pointers are aligned }
{$PACKRECORDS 8}
{$ELSE}
{ C default packing is dword }
{$PACKRECORDS 4}
{$ENDIF}
