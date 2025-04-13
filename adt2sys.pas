//  This file is part of Adlib Tracker II (AT2).
//
//  AT2 is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  AT2 is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with AT2.  If not, see <http://www.gnu.org/licenses/>.

unit AdT2sys;
{$S-,Q-,R-,V-,B-,X+}
{$PACKRECORDS 1}
{$MODESWITCH CVAR}
{$L adt2sys.o}
interface

var
  virtual_screen__first_row: Longint; cvar; external;
  virtual_cur_shape: Word; cvar; external;
  virtual_cur_pos: Word; cvar; external;
  slide_ticks: Longint; cvar; external;
  reset_slide_ticks: Boolean; cvar; external;
{$IFDEF GO32V2}
  gfx_ticks: Longint; cvar; external;
  reset_gfx_ticks: Boolean; cvar; external;
  scroll_ticks: Single; cvar; external;
  mouse_active: Boolean; cvar; external;
{$ENDIF}
{$IFNDEF GO32V2}
  _WAVREC_blink_flag: Boolean; cvar; external;
  _WAVREC_blink_ticks: Longint; cvar; external;
{$ENDIF}
  _NRECM_blink_flag: Boolean; cvar; external;
  _NRECM_blink_ticks: Longint; cvar; external;
  _IRQ_freq_shift_reset_flag: Boolean; cvar; external;
  _IRQFREQ_update_event: Boolean; cvar; external;
  _IRQFREQ_blink_flag: Boolean; cvar; external;
  _IRQFREQ_blink_ticks: Longint; cvar; external;
  blink_flag: Boolean; cvar; external;
  blink_ticks: Longint; cvar; external;
  cursor_sync: Boolean; cvar; external;
  _show_bpm_realtime_proc: procedure; cvar; external;
{$IFDEF GO32V2}
  _draw_screen_without_vsync: Boolean; cvar; external;
  _draw_screen_without_delay: Boolean; cvar; external;
{$ELSE}
  _draw_screen_without_delay: Boolean; cvar; external;
  _update_sdl_screen: Boolean; cvar; external;
  _name_scrl_shift_ctr: Shortint; cvar; external;
  _name_scrl_shift: Byte; cvar; external;
  _name_scrl_pending_frames: Longint; cvar; external;
{$ENDIF}
  _cursor_blink_factor: Longint; cvar; external;
  _cursor_blink_pending_frames: Longint; cvar; external;
  _realtime_gfx_no_update: Boolean; cvar; external;
{$IFDEF GO32V2}
  _screen_refresh_pending_frames: Longint; cvar; external;
  _custom_svga_cfg: array[1..31] of Record
                                      flag: Boolean;
                                      value: Longint;
                                    end; cvar; external;
{$ENDIF}

var
  _force_program_quit: Boolean; cvar; external;
  _traceprc_last_order: Byte; cvar; external;
  _traceprc_last_pattern: Byte; cvar; external;
  _traceprc_last_line: Byte; cvar; external;
  _pattedit_lastpos: Byte; cvar; external;

procedure sys_init; cdecl; external;
procedure sys_done; cdecl; external;
procedure draw_screen; cdecl; external;

{$IFNDEF GO32V2}

var
  _FrameBuffer: Pointer; cvar; external;

procedure vid_Init; cdecl; external;
procedure vid_Deinit; cdecl; external;
procedure vid_SetVideoMode(do_delay: Boolean); cdecl; external;
procedure vid_SetRGBPalette(var palette); cdecl; external;
procedure vid_FadeOut; cdecl; external;

{$ELSE}

var
  _FrameBuffer_mirror: array[0..PRED(1024*768)] of Byte; cvar; external;

{$ENDIF}

procedure ResetF_RW(var f: File); cdecl; external;
procedure ResetF(var f: File); cdecl; external;
procedure RewriteF(var f: File); cdecl; external;
procedure BlockReadF(var f: File; var data; size: Longint; var bytes_read: Longint); cdecl; external;
procedure BlockWriteF(var f: File; var data; size: Longint; var bytes_written: Longint); cdecl; external;
procedure SeekF(var f: File; fpos: Longint); cdecl; external;
procedure EraseF(var f: File); cdecl; external;
procedure CloseF(var f: File); cdecl; external;

implementation

uses
  debug,
  pascal,
  DOS,
{$IFDEF GO32V2}
  GO32,
  VGA,
  AdT2vesa,
{$ELSE}
  SDL,
  SDL_Video,
  SDL_Timer,
  SDL__rwops,
  AdT2opl3,
{$ENDIF}
  AdT2unit,
  AdT2text,
  AdT2keyb,
  AdT2data,
  TxtScrIO,
  StringIO,
  ParserIO;

{$IFNDEF GO32V2}
//screen
//rgb_color_alt
{$ENDIF} // DEFINED(GO32V2)

//sys_init
//sys_done

{$IFDEF GO32V2}

//draw_VESA_screen_800x600_1
//draw_VESA_screen_800x600_2
//draw_VESA_screen_1024x768
//dump_VESA_buffer
//shift_text_screen
//draw_screen

{$ELSE}

{$IFNDEF CPU64}

//draw_SDL_screen_720x480
//draw_SDL_screen_960x800
//draw_SDL_screen_1440x960

{$ELSE}

//draw_SDL_screen

{$ENDIF}

//draw_screen_proc

//vid_Init
//vid_Deinit
//vid_SetRGBPalette
//draw_screen
//vid_SetVideoMode
//vid_FadeOut

{$ENDIF}

//ResetF_RW
//ResetF
//RewriteF
//BlockReadF
//BlockWriteF
//SeekF
//EraseF
//CloseF

end.
