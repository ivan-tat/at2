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

unit DialogIO;
{$S-,Q-,R-,V-,B-,X+}
{$PACKRECORDS 1}
{$MODESWITCH CVAR}
{$L dialogio.o}
interface

uses
  DOS,
{$IFDEF GO32V2}
  GO32,
{$ENDIF}
{$IFDEF WINDOWS}
  WINDOWS,
{$ENDIF}
  StrUtils,
  AdT2unit,
  AdT2sys,
  AdT2keyb,
  AdT2text,
  TxtScrIO,
  StringIO,
  ParserIO;

type
  tDIALOG_SETTING = Record
                      frame_type:     String;
                      shadow_enabled: Boolean;
                      title_attr:     Byte;
                      box_attr:       Byte;
                      text_attr:      Byte;
                      text2_attr:     Byte;
                      keys_attr:      Byte;
                      keys2_attr:     Byte;
                      short_attr:     Byte;
                      short2_attr:    Byte;
                      disbld_attr:    Byte;
                      contxt_attr:    Byte;
                      contxt2_attr:   Byte;
                      xstart:         Byte;
                      ystart:         Byte;
                      center_box:     Boolean;
                      center_text:    Boolean;
                      cycle_moves:    Boolean;
                      all_enabled:    Boolean;
                      terminate_keys: array[1..50] of Word;
                    end;

  tDIALOG_ENVIRONMENT = Record
                          keystroke:   Word;
                          context:     String;
                          input_str:   String;
                          xpos,ypos:   Byte;
                          xsize,ysize: Byte;
                          cur_item:    Byte;
                          ext_proc:    procedure; cdecl;
                        end;

var
  dl_setting: tDIALOG_SETTING; cvar; external;
  dl_environment: tDIALOG_ENVIRONMENT; cvar; external;

function Dialog(text,keys,title: String; spos: Byte): Byte; cdecl; external;

type
  tMENU_SETTING = Record
                    frame_type:     String;
                    frame_enabled:  Boolean;
                    shadow_enabled: Boolean;
                    posbar_enabled: Boolean;
                    title_attr:     Byte;
                    menu_attr:      Byte;
                    text_attr:      Byte;
                    text2_attr:     Byte;
                    default_attr:   Byte;
                    short_attr:     Byte;
                    short2_attr:    Byte;
                    disbld_attr:    Byte;
                    contxt_attr:    Byte;
                    contxt2_attr:   Byte;
                    topic_attr:     Byte;
                    hi_topic_attr:  Byte;
                    topic_mask_chr: Set of Char;
                    center_box:     Boolean;
                    cycle_moves:    Boolean;
                    edit_contents:  Boolean;
                    reverse_use:    Boolean;
                    show_scrollbar: Boolean;
                    topic_len:      Byte;
                    fixed_len:      Byte;
                    homing_pos:     Longint;
                    terminate_keys: array[1..50] of Word;
                  end;

type
  tMENU_ENVIRONMENT = Record
                        v_dest:      tSCREEN_MEM_PTR;
                        keystroke:   Word;
                        context:     String;
                        unpolite:    Boolean;
                        winshade:    Boolean;
                        intact_area: Boolean;
                        edit_pos:    Byte;
                        curr_page:   Word;
                        curr_pos:    Word;
                        curr_item:   String;
                        ext_proc:    procedure; cdecl;
                        ext_proc_rt: procedure; cdecl;
                        refresh:     procedure; cdecl;
                        do_refresh:  Boolean;
                        own_refresh: Boolean;
                        preview:     Boolean;
                        fixed_start: Byte;
                        descr_len:   Byte;
                        descr:       Pointer;
                        is_editing:  Boolean;
                        xpos,ypos:   Byte;
                        xsize,ysize: Byte;
                        desc_pos:    Byte;
                        hlight_chrs: Byte;
                      end;

var
  mn_setting: tMENU_SETTING; cvar; external;
  mn_environment: tMENU_ENVIRONMENT; cvar; external;

function Menu(var data; x,y: Byte; spos: Word;
              len,len2: Byte; count: Word; title: String): Word; cdecl; external;

var
  MenuLib1_mn_setting: Record
                         frame_type:     String;
                         shadow_enabled: Boolean;
                         posbar_enabled: Boolean;
                         title_attr,
                         menu_attr,
                         text_attr,
                         text2_attr,
                         default_attr,
                         short_attr,
                         short2_attr,
                         disbld_attr,
                         contxt_attr,
                         contxt2_attr:   Byte;
                         center_box:     Boolean;
                         cycle_moves:    Boolean;
                         edit_contents:  Boolean;
                         reverse_use:    Boolean;
                         fixed_len:      Byte;
                         terminate_keys: array[1..50] of Word;
                       end; cvar; external;

var
  MenuLib1_mn_environment: Record
                             v_dest:     tSCREEN_MEM_PTR;
                             keystroke:  Word;
                             context:    String;
                             unpolite:   Boolean;
                             winshade:   Boolean;
                             edit_pos:   Byte;
                             curr_pos:   Word;
                             ext_proc:   procedure;
                             Menu1_refresh:    procedure; cdecl;
                             do_refresh: Boolean;
                             preview:    Boolean;
                             descr_len:  Byte;
                             descr:      Pointer;
                           end; cvar; external;

function MenuLib1_Menu(var data; x,y: Byte; spos: Word;
                       len,len2: Byte; count: Word; title: String): Word; cdecl; external;

const
{$IFDEF GO32V2}
  FILENAME_SIZE = 12;
  DIR_SIZE = 80;
  PATH_SIZE = 80;
{$ELSE}
  FILENAME_SIZE = 255;
  DIR_SIZE = 170;
  PATH_SIZE = 255;
{$ENDIF}

type
  tFSELECT_ENVIRONMENT = Record
                           last_file: String[FILENAME_SIZE];
                           last_dir:  String[DIR_SIZE];
                         end;
var
  fs_environment: tFSELECT_ENVIRONMENT; cvar; external;

function Fselect(mask: String): String; cdecl; external;

procedure DialogIO_Init; cdecl; external;

implementation

uses
  pascal,
  platform,
  debug;

end.
