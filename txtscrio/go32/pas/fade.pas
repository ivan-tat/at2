type
  tFADE  = (first,fadeOut,fadeIn);
  tDELAY = (fast,delayed);

type
  tFADE_BUF = Record
                action: tFADE;
                pal0: array[0..255] of Record r,g,b: Byte end;
                pal1: array[0..255] of Record r,g,b: Byte end;
              end;

var
  fade_speed: Byte; cvar; external;
