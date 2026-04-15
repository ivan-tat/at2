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

program AdT2_Player;
{$S-,Q-,R-,V-,B-,X+}
{$PACKRECORDS 1}
{$MODESWITCH CVAR}
{$L adt2play.o}
uses
  ctypes,
  debug,
  DOS,
  GO32,
  parserio,
  adt2keyb,
  adt2unit,
  adt2opl3,
  adt2ext3,
  adt2pack,
  depackio,
  A2scrIO,
  StringIO,
  TxtScrIO;

var
  modname: array[0..14] of PChar; cvar; external;

var
  status: CInt;
  gfx_mode: Boolean; cvar; external;
  jukebox: Boolean; cvar; external;
  error_code: Integer; cvar; external;
  fkey: Word; cvar; external;
  index,last_order: Byte;
  dirinfo: SearchRec;
  mem_info: TFPCHeapStatus;
  free_mem: Longint;
  temp,temp2: Byte;
  loader_result: Shortint; cvar; external;
  loader_error: PChar; cvar; external;

procedure _list_title; cdecl; external;
function load_song (fname: String; var error: PChar): Shortint; cdecl; external;
procedure terminate (status: Word); cdecl; external;
function run_adt2play (argc: CInt; argv: PPChar): CInt; cdecl; external;

begin
  status := run_adt2play (argc, argv);
  if (status <> 0) then Halt (status);

  mem_info := GetFPCHeapStatus;
  free_mem := mem_info.CurrHeapFree*1000;
  error_code := 0;
  temp := $80;

  Repeat
    If (free_mem > PATTERN_SIZE*temp) then
      begin
        max_patterns := temp;
        BREAK;
      end
    else If (temp >= $10 + $10) then Dec(temp,$10)
         else begin
                error_code := -2;
                BREAK;
              end;
  until FALSE;

  If (error_code <> -2) then
    GetMem(pattdata,PATTERN_SIZE*max_patterns);
  if (pattdata = NIL) then
  begin
    WriteLn ('ERROR: Memory allocation failed.');
    Halt (1);
  end;

  FillChar(decay_bar,SizeOf(decay_bar),0);
  play_status := isStopped;
  init_songdata;
  init_timer_proc;

  if gfx_mode then
    toggle_picture_mode
  else begin
         FillWord(screen_ptr^,MAX_SCREEN_MEM_SIZE DIV 2,$0700);
         dosmemput($0b800,0,screen_ptr^,MAX_SCREEN_MEM_SIZE);
         GotoXY(1,1);
         _list_title;
       end;

  index := 0;
  Repeat
    If (index = 0) then
      begin
        CWriteLn(FilterStr(DietStr('úù-Ä--ùú   úù-ÄÄÄÄÄÄÄÄÄÄÄÄÄÄ--ùú       úù-ÄÄÄ--ùú úù-ÄÄ-Äùú',
                                   PRED(MaxCol)),
                           '.',' '),$01,0);
        CWriteLn(                  '  ~[~SPACE~]~ Fast-Forward ~[~Ä~]~ Restart ~[~ÄÙ~]~ Next ~[~ESC~]~ Quit',$09,$01);
        CWriteLn(FilterStr(DietStr('úù-ÄÄÄÄÄÄÄÄÄÄ--ùú      úù-ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ-Äùú',
                                  PRED(MaxCol)),
                          '.',' '),$01,0);

        CWriteLn('',$07,0);
        _window_top := WhereY;
      end;

    Inc(index);
    If (index < argc) and (argv[index][0] <> '/') then
      begin
        FindFirst(StrPas(argv[index]),AnyFile-VolumeID-Directory,dirinfo);
        If (DosError <> 0) then
          begin
            CWriteLn(DietStr('ERROR(2) - No such file "'+
                             Lower(StrPas(argv[index]))+'"',
                     PRED(MaxCol)),$07,0);
            CWriteLn('',$07,0);
            FindNext(dirinfo);
            CONTINUE;
          end;

        While NOT (DosError <> 0) do
          begin
            If (PathOnly(StrPas(argv[index])) <> '') then
              songdata_source := Upper(PathOnly(StrPas(argv[index]))+dirinfo.name)
            else songdata_source := Upper(dirinfo.name);

            C3Write(DietStr('Loading "'+songdata_source+'" (please wait)',
                             PRED(MaxCol)),$07,0,0);
            wtext2(_timer_xpos,_timer_ypos,_timer_str,_timer_color);
            wtext(_progress_xpos,_progress_ypos,_progress_str,_progress_color);
            wtext(_pos_str_xpos,_pos_str_ypos,_position_str2+'  ',_pos_str_color);
            wtext2(_fname_xpos,_fname_ypos,ExpStrR(NameOnly(songdata_source),12,' '),_fname_color);
            wtext(_pos_str_xpos,_pos_str_ypos,ExpStrR('Loading...',35,' '),_pos_str_color);
            For temp := 1 to 10 do WaitRetrace;

            limit_exceeded := FALSE;
            _decay_bars_initialized := FALSE;
//            if gfx_mode then decay_bars_refresh;

            loader_error := NIL;
            loader_result := load_song (songdata_source, loader_error);
            if (loader_result < 0) then
            begin
              if ((loader_result = -1) or (loader_result = -2)) then
              begin
                CWriteLn(DietStr(ExpStrR('ERROR(3) - Invalid module ('+songdata_source+')',
                                         PRED(MaxCol),' '),
                                 PRED(MaxCol)),$07,0);
              end
              else
              begin
                last_order := 0;
                entries := 0;
                if (limit_exceeded) then
                  CWriteLn(DietStr(ExpStrR('ERROR(1) - Insufficient memory!',
                                           PRED(MaxCol),' '),
                                   PRED(MaxCol)),
                           $07,0)
                else
                  CWriteLn(DietStr(ExpStrR('ERROR: '+StrPas(loader_error),
                                           PRED(MaxCol),' '),
                                   PRED(MaxCol)),
                           $07,0);
              end;
              CWriteLn('',$07,0);
              FindNext(dirinfo);
              CONTINUE;
            end;

            last_order := 0;
            entries := 0;
            count_order(entries);
            correction := calc_following_order(0);
            entries2 := entries;
            If (correction <> -1) then Dec(entries,correction)
            else entries := 0;
            CWriteLn(DietStr(ExpStrR('Playing '+StrPas(modname[loader_result])+' "'+
                                     songdata_source+'"',
                                     PRED(MaxCol),' '),
                             PRED(MaxCol)),$07,0);
            temp2 := PRED(WhereY);

            If (entries = 0) then
              begin
                If NOT _picture_mode then GotoXY(1,temp2);
                CWriteLn(DietStr(ExpStrR('Playing '+StrPas(modname[loader_result])+' "'+
                                         songdata_source+'"',
                                         PRED(MaxCol),' '),
                                 PRED(MaxCol)),$08,0);
                CWriteLn(DietStr(ExpStrR(''+NameOnly(songdata_source)+' [stopped] ['+
                                         ExpStrL(Num2str(TRUNC(time_playing) DIV 60,10),2,'0')+
                                         ':'+ExpStrL(Num2str(TRUNC(time_playing) MOD 60,10),2,'0')+']',
                                         PRED(MaxCol),' '),
                                 PRED(MaxCol)),$07,0);
                CWriteLn('',$07,0);
                FindNext(dirinfo);
                CONTINUE;
              end;

            stop_playing;
            if (error_code = 0) then start_playing;
            set_overall_volume(63);

            Repeat
              If (overall_volume = 63) then
                C3Write(DietStr(_position_str+'  ',PRED(MaxCol)),$0f,0,0);
              wtext2(_timer_xpos,_timer_ypos,_timer_str,_timer_color);
              wtext(_progress_xpos,_progress_ypos,_progress_str,_progress_color);
              wtext(_pos_str_xpos,_pos_str_ypos,_position_str2+'  ',_pos_str_color);

              If (inportb($60) = $39) { SPACE pressed } then
                begin
                  If (overall_volume > 32) then
                    For temp := 63 downto 32 do
                      begin
                        set_overall_volume(temp);
                        delay_counter := 0;
                        While (delay_counter < overall_volume DIV 20) do
                          begin
                            If timer_200hz_flag then
                              begin
                                timer_200hz_flag := FALSE;
                                Inc(delay_counter);
                                C3Write(DietStr(_position_str+'',PRED(MaxCol)),$0f,0,0);
                                wtext2(_timer_xpos,_timer_ypos,_timer_str,_timer_color);
                                wtext(_progress_xpos,_progress_ypos,_progress_str,_progress_color);
                                wtext(_pos_str_xpos,_pos_str_ypos,_position_str2+'',_pos_str_color);
                                If timer_50hz_flag then
                                  begin
                                    timer_50hz_flag := FALSE;
                                    If NOT fast_forward then
                                      decay_bars_refresh;
                                  end;
                              end;
                            MEMW[0:$041c] := MEMW[0:$041a];
                          end;
                      end
                  else begin
                         If timer_200hz_flag then
                           begin
                             timer_200hz_flag := FALSE;
                             C3Write(DietStr(_position_str+'',PRED(MaxCol)),$0f,0,0);
                             wtext2(_timer_xpos,_timer_ypos,_timer_str,_timer_color);
                             wtext(_progress_xpos,_progress_ypos,_progress_str,_progress_color);
                             wtext(_pos_str_xpos,_pos_str_ypos,_position_str2+'',_pos_str_color);
                             If timer_50hz_flag then
                               begin
                                 timer_50hz_flag := FALSE;
                                 If NOT fast_forward then
                                   decay_bars_refresh;
                               end;
                           end;
                         MEMW[0:$041c] := MEMW[0:$041a];
                       end;
                  fast_forward := TRUE;
                end
              else If (inportb($60) = $0b9) { SPACE released } then
                     begin
                       fast_forward := FALSE;
                       If (overall_volume < 63) then
                         For temp := 32 to 63 do
                           begin
                             set_overall_volume(temp);
                             delay_counter := 0;
                             While (delay_counter < overall_volume DIV 20) do
                               begin
                                 If (timer_200hz_counter = 0) then
                                   begin
                                     Inc(delay_counter);
                                     C3Write(DietStr(_position_str+'  ',PRED(MaxCol)),$0f,0,0);
                                     wtext2(_timer_xpos,_timer_ypos,_timer_str,_timer_color);
                                     wtext(_progress_xpos,_progress_ypos,_progress_str,_progress_color);
                                     wtext(_pos_str_xpos,_pos_str_ypos,_position_str2+'  ',_pos_str_color);
                                     If timer_50hz_flag then
                                       begin
                                         timer_50hz_flag := FALSE;
                                         decay_bars_refresh;
                                       end;
                                   end;
                                 MEMW[0:$041c] := MEMW[0:$041a];
                               end;
                           end;
                     end;

              If (NOT fast_forward and timer_50hz_flag) or
                 (fast_forward and timer_20hz_flag) then
                begin
                  If NOT fast_forward then timer_50hz_flag := FALSE
                  else timer_20hz_flag := FALSE;
                  decay_bars_refresh;
                end;

              If NOT keypressed then fkey := WORD_NULL
              else asm
                       xor      ax,ax
                       int      16h
                       mov      fkey,ax
                   end;

              MEMW[0:$041c] := MEMW[0:$041a];
              If jukebox and (last_order <> current_order) then
                begin
                  If (last_order > current_order) and
                     (last_order = PRED(entries2)) then BREAK
                  else last_order := current_order;
                end;

              If (fkey = kBkSPC) then
                begin
                  fade_out;
                  stop_playing;
                  set_overall_volume(63);
                  if (error_code = 0) then start_playing;
                end;
            until (fkey = kENTER) or
                  (fkey = kESC);

            fade_out;
            stop_playing;
            If NOT _picture_mode then GotoXY(1,temp2);
            CWriteLn(DietStr(ExpStrR('Playing '+StrPas(modname[loader_result])+' "'+
                                     songdata_source+'"',
                                     PRED(MaxCol),' '),
                             PRED(MaxCol)),$08,0);
            CWriteLn(DietStr(ExpStrR(''+NameOnly(songdata_source)+' [stopped] ['+
                                     ExpStrL(Num2str(TRUNC(time_playing) DIV 60,10),2,'0')+
                                     ':'+ExpStrL(Num2str(TRUNC(time_playing) MOD 60,10),2,'0')+']',
                                     PRED(MaxCol),' '),
                             PRED(MaxCol)),$07,0);
            CWriteLn('',$07,0);
            If (fkey = kESC) then BREAK;
            FindNext(dirinfo);
          end;
      end;
  until (index = argc);
  done_timer_proc;
end.
