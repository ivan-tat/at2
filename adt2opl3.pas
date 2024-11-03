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

unit AdT2opl3;
{$S-,Q-,R-,V-,B-,X+}
{$PACKRECORDS 1}
{$MODESWITCH CVAR}
{$L adt2opl3.o}
interface

procedure opl2out(reg,data: Word); cdecl; external;
procedure opl3exp(data: Word); cdecl; external;

type
  tOPL3OUT_proc = procedure(reg,data: Word); cdecl;

var
  opl3out: tOPL3OUT_proc; cvar; external;

{$IFDEF GO32V2}

var
  opl3port: Word; cvar; external;
  opl_latency: Byte; cvar; external;

function detect_OPL3: Boolean; cdecl; external;

{$ELSE} { NOT DEFINED(GO32V2) }

var
  renew_wav_files_flag: Boolean; cvar; external;
  opl3_channel_recording_mode: Boolean; cvar; external;
  opl3_record_channel: array[1..20] of Boolean; cvar; external;

procedure flush_WAV_data; cdecl;
procedure opl3_init; cdecl; external;
procedure opl3_done; cdecl; external;
procedure snd_init; cdecl; external;
procedure snd_done; cdecl; external;
procedure snd_SetTimer(value: Longint); cdecl; external;
procedure update_recorded_channels; cdecl; external;

var
  opl3_flushmode: Boolean; cvar; external;

const
  WAV_BUFFER_SIZE = 512*1024; // cache buffer size -> 512k per file

var
  wav_buffer_len: Longint; cvar; external;
  wav_buffer: array[0..18,0..PRED(18*WAV_BUFFER_SIZE)] of Byte; cvar; external;

{$ENDIF} { NOT DEFINED(GO32V2) }

implementation

uses
  debug,
  pascal,
{$IFDEF GO32V2}
  AdT2sys;
{$ELSE} { NOT DEFINED(GO32V2) }
  SysUtils,
  AdT2unit,
  AdT2sys,
  StringIO,
  SDL_Types,
  SDL_Audio,
  OPL3EMU;
{$ENDIF} { NOT DEFINED(GO32V2) }

{$IFNDEF GO32V2}

procedure flush_WAV_data; cdecl;
public name PUBLIC_PREFIX + 'flush_WAV_data';

type
  tWAV_HEADER = Record
     file_desc: array[0..3] of Char;    // =="RIFF"
     file_size: Dword;                  // ==filesize-8
     wav_desc: array[0..3] of Char;     // =="WAVE"
     format_desc: array[0..3] of Char;  // =="fmt "
     wav_size: Dword;                   // ==16
     wav_type: Word;                    // ==1 (PCM)
     num_channels: Word;                // ==2 (Stereo)
     samples_sec: Dword;                // sampling frequency
     bytes_sec: Dword;                  // ==num_channels*samples_sec*bits_sample/8
     block_align: Word;                 // ==num_channels*bits_sample/8
     bits_sample: Word;                 // ==16
     data_desc: array[0..3] of Char;    // "data"
     data_size: Dword;                  // size of data
   end;

const
  wav_header: tWAV_HEADER = (file_desc:    'RIFF';
                             file_size:    SizeOf(tWAV_HEADER)-8;
                             wav_desc:     'WAVE';
                             format_desc:  'fmt ';
                             wav_size:     16;
                             wav_type:     1;
                             num_channels: 2;
                             samples_sec:  49716;
                             bytes_sec:    2*49716*16 DIV 8;
                             block_align:  2*16 DIV 8;
                             bits_sample:  16;
                             data_desc:    'data';
                             data_size:    0);

var
  wav_file: File;
  temp,bytes_to_write: Longint;
  idx,from_ch,to_ch: Byte;
  filename_suffix: String;

begin
  bytes_to_write := wav_buffer_len;
  // flush when at least 1 sec of recorded data
  If (bytes_to_write < 2*sdl_sample_rate*16 DIV 8) then
    EXIT; //flush_WAV_data
  If NOT ((play_status = isPlaying) and (sdl_opl3_emulator <> 0)) then
    EXIT; //flush_WAV_data

  // prepare output directory
  If NOT DirectoryExists(Copy(sdl_wav_directory,1,Length(sdl_wav_directory)-Length(NameOnly(sdl_wav_directory)))) then
    If NOT CreateDir(Copy(sdl_wav_directory,1,Length(sdl_wav_directory)-Length(NameOnly(sdl_wav_directory)))) then
      EXIT; //flush_WAV_data

  wav_buffer_len := 0;
  If NOT opl3_channel_recording_mode then
    begin
      from_ch := 0;
      to_ch := 0;
    end
  else
    begin
      from_ch := 1;
      If NOT percussion_mode then to_ch := songdata.nm_tracks
      else to_ch := 18;
    end;

  For idx := from_ch to to_ch do
    begin
      filename_suffix := '';
      If (idx <> 0) then
        If NOT (opl3_record_channel[idx]) or (is_4op_chan(idx) and NOT (idx in _4op_tracks_hi)) then CONTINUE
        else If NOT is_4op_chan(idx) then
               If percussion_mode then
                 Case idx of
                   16: filename_suffix := ' ('+ExpStrL(Num2str(idx,10),2,'0')+'_BD)';
                   17: filename_suffix := ' ('+ExpStrL(Num2str(idx,10),2,'0')+'_SD_HH)';
                   18: filename_suffix := ' ('+ExpStrL(Num2str(idx,10),2,'0')+'_TT_TC)';
                   else filename_suffix := ' ('+ExpStrL(Num2str(idx,10),2,'0')+')';
                 end
               else filename_suffix := ' ('+ExpStrL(Num2str(idx,10),2,'0')+')'
             else filename_suffix := ' ('+ExpStrL(Num2str(idx,10),2,'0')+'_'
                                         +ExpStrL(Num2str(idx+1,10),2,'0')+'_4OP)';

      If opl3_flushmode then
        Assign(wav_file,Copy(sdl_wav_directory,1,Length(sdl_wav_directory)-Length(NameOnly(sdl_wav_directory)))+
                        BaseNameOnly(sdl_wav_directory)+filename_suffix+'.wav')
      else Assign(wav_file,sdl_wav_directory+BaseNameOnly(songdata_title)+filename_suffix+'.wav');

      // update WAV header
      {$i-}
      If renew_wav_files_flag then
        begin
          RewriteF(wav_file);
          wav_header.file_size := 0;
          wav_header.data_size := 0;
        end
      else ResetF(wav_file);
      {$i+}
      If renew_wav_files_flag or (IOresult <> 0) then
        begin
          {$i-}
          RewriteF(wav_file);
          {$i+}
          If (IOresult <> 0) then
            begin
              Close(wav_file);
              {$i-}
              EraseF(wav_file);
              {$i+}
              If (IOresult <> 0) then ;
              EXIT; //flush_WAV_data
            end;
          wav_header.samples_sec := sdl_sample_rate;
          wav_header.bytes_sec := 2*sdl_sample_rate*16 DIV 8;
          wav_header.file_size := bytes_to_write;
          wav_header.data_size := bytes_to_write;
          {$i-}
          BlockWriteF(wav_file,wav_header,SizeOf(wav_header),temp);
          {$i+}
          If (IOresult <> 0) or
             (temp <> SizeOf(wav_header)) then
            begin
              CloseF(wav_file);
              {$i-}
              EraseF(wav_file);
              {$i+}
              If (IOresult <> 0) then ;
              EXIT; //flush_WAV_data
            end;
        end
      else begin
             {$i-}
             BlockReadF(wav_file,wav_header,SizeOf(wav_header),temp);
             {$i+}
             If (IOresult <> 0) or
                (temp <> SizeOf(wav_header)) then
               begin
                 CloseF(wav_file);
                 {$i-}
                 EraseF(wav_file);
                 {$i+}
                 If (IOresult <> 0) then ;
                 EXIT; //flush_WAV_data
               end;
             wav_header.file_size := wav_header.file_size+bytes_to_write;
             wav_header.data_size := wav_header.data_size+bytes_to_write;
             {$i-}
             ResetF_RW(wav_file);
             {$i+}
             If (IOresult <> 0) then
               begin
                 CloseF(wav_file);
                 {$i-}
                 EraseF(wav_file);
                 {$i+}
                 If (IOresult <> 0) then ;
                 EXIT; //flush_WAV_data
               end;
             {$i-}
             BlockWriteF(wav_file,wav_header,SizeOf(wav_header),temp);
             {$i+}
             If (IOresult <> 0) or
                (temp <> SizeOf(wav_header)) then
               begin
                 CloseF(wav_file);
                 {$i-}
                 EraseF(wav_file);
                 {$i+}
                 If (IOresult <> 0) then ;
                 EXIT; //flush_WAV_data
               end;
             {$i-}
             SeekF(wav_file,FileSize(wav_file));
             {$i+}
             If (IOresult <> 0) then
               begin
                 CloseF(wav_file);
                 {$i-}
                 EraseF(wav_file);
                 {$i+}
                 If (IOresult <> 0) then ;
                 EXIT; //flush_WAV_data
               end;
           end;

      // write sample data
      {$i-}
      BlockWriteF(wav_file,wav_buffer[idx],bytes_to_write,temp);
      {$i+}
      If (IOresult <> 0) or (temp <> bytes_to_write) then
        begin
          CloseF(wav_file);
          {$i-}
          EraseF(wav_file);
          {$i+}
          If (IOresult <> 0) then ;
        end
      else
        CloseF(wav_file);
    end;

  renew_wav_files_flag := FALSE;

  //EXIT //flush_WAV_data
end;

{$ENDIF} { NOT DEFINED(GO32V2) }

{$IFDEF GO32V2}
procedure init_adt2opl3; cdecl; external;

begin
  init_adt2opl3;
{$ENDIF} { DEFINED(GO32V2) }
end.
