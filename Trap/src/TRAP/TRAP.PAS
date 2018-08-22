{Trap copyright (c) Kamil Korolkiewicz 2003/2004}
{-----------------------------------------------}
program TRAP;

{$G+,E-,N-,S-,I-}
{$M 2048,0,655360}
{$I TRAPCORE.PAS}
{a$DEFINE DEBUG}


{-----------------------------------------------}

{staˆe nieformalne}

const
entrystr =
{'                                                                               ';}
'              Trap v.'+ver+' copyright (c) Kamil Korolkiewicz 2003/2004             ';
MEMTOSYSALLOC = 16000 div 16;
TRAPMEMSTART = $4000-bufsize shr 4-(11200 shr 4)-1;
EXTMEMREQ = 1000000 div 1000;
REQDOSVER = 5;
FILEPACKNAME = 'TRAP.PAK';

{zmienne nieformalne}

var
__ppchar1 : array[0..49] of char;
__ppchar2 : pchar;
__proc : byte;
__name : array [0..12] of byte;
__proccnt : longint;
__procclk,__procti : word;
__tresult1,__tresult2 : longint;
__pmenabled : byte;
__pp : pointer;
__freedm,__freexm : longint;
__dsegstart : integer;
__hastsc : byte;
__rdtsc1,__rdtsc2 : longint;
__dosver,__winver : word;
__params : word;

{Pocz¥tek programu TRAP}
begin
     clrscr;
     trapcrtprint (0,0,16+15,entrystr);
     trapcrtprint (0,trapcrtcy+1,7,'Geting system information...');
     params := getparams;
     {-----------------------------------------------}
     {Pobieranie informacji o procesorze}
     if test8086 < 2 then syserror ('main()','80386 processor requed');
     asm
        {pushfd}                  db 66h; db 9ch;
        {pop eax}                 db 66h; db 58h;
        {mov ecx,eax}             db 66h; db 8bh; db 0c8h;
        {xor eax,00040000h}       db 66h; db 35h; dd 00040000h;
        {push eax}                db 66h; db 50h;
        {popfd}                   db 66h; db 9dh;
        {pushfd}                  db 66h; db 9ch;
        {pop eax}                 db 66h; db 58h;
        {xor eax,ecx}             db 66h; db 33h; db 0c1h;
        jne @d1
        {mov __proc,0}            db 66h; db 0c7h; db 6h; dw offset __proc; dd 00000000h;
        jmp @q
        @d1:
        {pushfd}                  db 66h; db 9ch;
        {pop eax}                 db 66h; db 58h;
        {mov ecx,eax}             db 66h; db 8bh; db 0c8h;
        {or eax,00200000h}        db 66h; db 0dh; dd 00200000h;
        {push eax}                db 66h; db 50h;
        {popfd}                   db 66h; db 9dh;
        {pushfd}                  db 66h; db 9ch;
        {pop eax}                 db 66h; db 58h;
        {and eax,00200000h}       db 66h; db 25h; dd 00200000h;
        jnz @d2
        {mov __proc,1}            db 66h; db 0c7h; db 6h; dw offset __proc; dd 00000001h;
        jmp @q
        @d2:
        {mov __proc,2}            db 66h; db 0c7h; db 6h; dw offset __proc; dd 00000002h;
        {mov eax,0}               db 66h; db 0b8h; dd 00000000h;
        {cpuid}                   db 0fh; db 0a2h;
        {mov __name,ebx}          db 66h; db 89h; db 1eh; dw offset __name;
        {mov __name+4,edx}        db 66h; db 89h; db 16h; dw offset __name+4;
        {mov __name+8,ecx}        db 66h; db 89h; db 0eh; dw offset __name+8;
        mov __hastsc,0
        {mov eax,1}               db 66h; db 0b8h; dd 00000001h;
        {cpuid}                   db 0fh; db 0a2h;
        {mov sysinf.proccaps,edx} db 66h; db 89h; db 16h; dw offset sysinf.proccaps;
        test dl,00010000b
        jz @q
        mov __hastsc,1
        @q:
     end;
     {-----------------------------------------------}
     {Pobieranie szybko˜ci procesora}
     asm
        in al,21h
        push ax
        and al,0feh
        out 21h,al
        mov ax,0
        mov es,ax
        mov di,1132
        mov dl,es:[di]
        @lop:
        cmp dl,es:[di]
        je @lop
        cmp __hastsc,0
        jz @dt1
        {rdtsc}                  db 0fh; db 31h;        {pentium+}
        {mov __rdtsc1,eax}       db 66h; db 0a3h; dw offset __rdtsc1
        @dt1:
        mov dl,es:[di]
        {xor eax,eax}            db 66h; db 33h; db 0c0h;
        @lopt:
        {inc eax}                db 66h; db 40h;
        cmp dl,es:[di]
        je @lopt
        {mov __proccnt,eax}      db 66h; db 0a3h; dw offset __proccnt;
        cmp __hastsc,0
        jz @dt2
        {rdtsc}                  db 0fh; db 31h;        {pentium+}
        {mov __rdtsc2,eax}       db 66h; db 0a3h; dw offset __rdtsc2
        @dt2:
        pop ax
        out 21h,al
     end;
     {-----------------------------------------------}
     {Czy program dziala w trybie chronionym (V86)}
     asm
        smsw ax
        and ax,1
        jz @d
        mov __pmenabled,1
        jmp @q
        @d:
        mov __pmenabled,0
        @q:
     end;
     {-----------------------------------------------}
     if __hastsc = 0 then
     begin
     case __proc of
     0:__procti := 2300; {386-23.00}
     1:__procti := 500;  {486-5.00}
     2:__procti := 300;  {586+-3.00}
     end;
     if __pmenabled = 1 then inc (__procti,9);
     __proccnt := __proccnt div 1000;
     end;
     {-----------------------------------------------}
     if __hastsc = 1 then
     __procclk := ((__rdtsc2-__rdtsc1) div 1000)*182 div 10000
     else
     __procclk := (__proccnt*__procti)*182 div 1000000; {18.2}
     sysinf.procclk := __procclk;
     if __proc < 2 then
     sysinf.procname := processornames[__proc]
     else
     sysinf.procname := @__name;
     {writeln (pchar (addr (__name)),' mhz: ',__procclk);}
     {-----------------------------------------------}
     {Pobieranie informacji o pamieci dos}
     getmem (__pp,1);
     __dsegstart := TRAPMEMSTART-seg (__pp^);
     __freedm := memavail; {{$a0000-TRAPMEMSTART*16; {(longint (seg (__pp))*$10);}
     freemem (__pp,1);
     if __dsegstart < MEMTOSYSALLOC then syserror ('main()','No enough dos memory');
     dossegcnt := TRAPMEMSTART;
     sysinf.dosmemfree := __freedm;
     {-----------------------------------------------}
     {Pobieranie informacji o pamieci xms}
     if not detectxms then syserror ('main()','XMS driver not found');
     __freexm := xmsmemavail;
     {writeln (__freexm);}
     if __freexm < EXTMEMREQ then syserror ('main()','No enough XMS memory');
     sysinf.xmsmemfree := __freexm;
     {-----------------------------------------------}
     {Pobieranie informacji o systemie operacyjnym}
     __dosver := getdosver;
     __winver := getwinver;
     if lo (__dosver) < REQDOSVER then syserror ('main()','Req DOS version >= 5');
     sysinf.dosver := __dosver;
     sysinf.winver := __winver;
     {-----------------------------------------------}
     {Pobieranie informacji o karcie graficznej i karcie dzwiekowej}
     if not parameq (params,SAFE_MODE) then
     begin
     if getvesainfo = false then vgaonly := true;
     sbinforec.p := 0;
     if (not parameq (params,NOSOUND_MODE)) and (sbreset ($220)) then
     begin
          getsbenv (sbinforec.p,sbinforec.d,sbinforec.i,sbinforec.m);
          if (sbinforec.p = 0) and (sbreset ($220)) then
          begin
               sbinforec.p := $220;
               sbinforec.d := 1;
               sbinforec.i := 7;
               sbinforec.m := $330;
          end;
     end;
     end
     else
     vgaonly := true;
     if sbinforec.p = 0 then ussndt := false;
     {-----------------------------------------------}
     {Wypisanie informacji o systemie}
     getmem (__ppchar2,60);
     trapcrtprint (0,trapcrtcy+1,9,'--------------------------------------------------------------------------------');
     strcopy (@__ppchar1,'Processor type: ');
     strcat (@__ppchar1,sysinf.procname);
     trapcrtprint (0,trapcrtcy+1,9,@__ppchar1);
     {---}
     strcopy (@__ppchar1,'Processor clock (MHZ): ');
     val2pchar (__procclk,__ppchar2);
     strcat (@__ppchar1,__ppchar2);
     trapcrtprint (0,trapcrtcy+1,9,@__ppchar1);
     {---}
     strcopy (@__ppchar1,'Free DOS memory (bytes): ');
     val2pchar (__freedm,__ppchar2);
     strcat (@__ppchar1,__ppchar2);
     trapcrtprint (0,trapcrtcy+1,9,@__ppchar1);
     {---}
     strcopy (@__ppchar1,'Free XMS memory (kbytes): ');
     val2pchar (__freexm,__ppchar2);
     strcat (@__ppchar1,__ppchar2);
     trapcrtprint (0,trapcrtcy+1,9,@__ppchar1);
     {---}
     strcopy (@__ppchar1,'DOS version: ');
     val2pchar (lo (__dosver),__ppchar2);
     strcat (@__ppchar1,__ppchar2);
     strcat (@__ppchar1,'.');
     val2pchar (hi (__dosver),__ppchar2);
     strcat (@__ppchar1,__ppchar2);
     trapcrtprint (0,trapcrtcy+1,9,@__ppchar1);
     {---}
     strcopy (@__ppchar1,'WIN version: ');
     val2pchar (hi (__winver),__ppchar2);
     strcat (@__ppchar1,__ppchar2);
     strcat (@__ppchar1,'.');
     val2pchar (lo (__winver),__ppchar2);
     strcat (@__ppchar1,__ppchar2);
     trapcrtprint (0,trapcrtcy+1,9,@__ppchar1);
     {---}
     if vgaonly then trapcrtprint (0,trapcrtcy+1,9,'VGA card')
     else
     begin
     strcopy (@__ppchar1,'VESA card (name,version,RAM): ');
     strcat (@__ppchar1,vesacaps.name);
     strcat (@__ppchar1,',');
     val2pchar (hi (vesacaps.ver),__ppchar2);
     strcat (@__ppchar1,__ppchar2);
     strcat (@__ppchar1,'.');
     val2pchar (lo (vesacaps.ver),__ppchar2);
     strcat (@__ppchar1,__ppchar2);
     strcat (@__ppchar1,',');
     val2pchar (longint (vesacaps.mem)*64 div 1024,__ppchar2);
     strcat (@__ppchar1,__ppchar2);
     strcat (@__ppchar1,'MB');
     trapcrtprint (0,trapcrtcy+1,9,@__ppchar1);
     end;
     {---}
     if ussndt = false then trapcrtprint (0,trapcrtcy+1,9,'No SB card')
     else
     begin
     strcopy (@__ppchar1,'SB Port,DMA,IRQ,MIDI Port: ');
     word2hex (sbinforec.p,__ppchar2);
     strcat (@__ppchar1,__ppchar2);
     strcat (@__ppchar1,',');
     val2pchar (sbinforec.d,__ppchar2);
     strcat (@__ppchar1,__ppchar2);
     strcat (@__ppchar1,',');
     val2pchar (sbinforec.i,__ppchar2);
     strcat (@__ppchar1,__ppchar2);
     strcat (@__ppchar1,',');
     word2hex (sbinforec.m,__ppchar2);
     strcat (@__ppchar1,__ppchar2);
     trapcrtprint (0,trapcrtcy+1,9,@__ppchar1);
     end;
     {---}
     trapcrtprint (0,trapcrtcy+1,9,'--------------------------------------------------------------------------------');
     {-----------------------------------------------}
     {Inicjacja pamieci XMS}
     trapcrtprint (0,trapcrtcy+1,7,'Initializing XMS heap...');
     initxmsheap (EXTMEMREQ);
     if iserr <> 0 then syserror ('main()','Can''t alloc XMS heap');
     {-----------------------------------------------}
     {Pobieranie pamieci}
     trapcrtprint (0,trapcrtcy+1,7,'Geting memory...');
     sndbuf := allocmem (bufsize,0);
     _32fill (sndbuf,bufsize,128);
     {$IFDEF DEBUG}
     val2pchar ($a0000-longint (dossegcnt) shl 4,__ppchar2);
     strcopy (@__ppchar1,__ppchar2);
     word2hex (dossegcnt,__ppchar2);
     strcat (@__ppchar1,' ');
     strcat (@__ppchar1,__ppchar2);
     word2hex (seg (sndbuf^),__ppchar2);
     strcat (@__ppchar1,' ');
     strcat (@__ppchar1,__ppchar2);
     trapcrtprint (0,trapcrtcy+1,7,__ppchar1);
     {$ENDIF}
     txttbl := allocmem (11200,0);
     {$IFDEF DEBUG}
     val2pchar ($a0000-longint (dossegcnt) shl 4,__ppchar2);
     strcopy (@__ppchar1,__ppchar2);
     word2hex (dossegcnt,__ppchar2);
     strcat (@__ppchar1,' ');
     strcat (@__ppchar1,__ppchar2);
     word2hex (seg (txttbl^),__ppchar2);
     strcat (@__ppchar1,' ');
     strcat (@__ppchar1,__ppchar2);
     trapcrtprint (0,trapcrtcy+1,7,__ppchar1);
     {$ENDIF}
     buf := allocmem (64000,0);
     buftoclr;
     {$IFDEF DEBUG}
     val2pchar ($a0000-longint (dossegcnt) shl 4,__ppchar2);
     strcopy (@__ppchar1,__ppchar2);
     word2hex (dossegcnt,__ppchar2);
     strcat (@__ppchar1,' ');
     strcat (@__ppchar1,__ppchar2);
     word2hex (seg (buf^),__ppchar2);
     strcat (@__ppchar1,' ');
     strcat (@__ppchar1,__ppchar2);
     trapcrtprint (0,trapcrtcy+1,7,__ppchar1);
     {$ENDIF}
     trapmidimusic := allocmem (midimusicmaxsize,0);
     getmem (trapcurrmapname,256);
     getmem (trapcurrmapmusic,256);
     {-----------------------------------------------}
     {Inicjacja klawiatury}
     trapcrtprint (0,trapcrtcy+1,7,'Initializing key interrupt...');
     keyis (on);
     pressnumlock;
     stdkeyint := false;
     trapmenuon := false;
     {-----------------------------------------------}
     {Inicjacja dzwieku}
     trapcrtprint (0,trapcrtcy+1,7,'Initializing sound...');
     if ussndt then ussndt := initsnd (sbinforec.p,sbinforec.d,sbinforec.i);
     if ussndt then trapcrtprint (trapcrtcx,trapcrtcy,7,'ok');
     {-----------------------------------------------}
     {Odczytywanie konfiguracji}
     trapcrtprint (0,trapcrtcy+1,7,'Reading config...');
     loadconfig;
     {-----------------------------------------------}
     {Otwozenie paka z potrzebnymi plikami}
     trapcrtprint (0,trapcrtcy+1,7,'Opening pak file...');
     if not uspack (trappfrec,FILEPACKNAME) then syserror ('main()',FILEPACKNAME+' not found!!!');
     assignpfile (trapfilehandle,trappfrec,'TXTTBL.ASC');
     if pfileior <> 0 then syserror ('main()',getpfior);
     resetpfile (trapfilehandle);
     blockreadpfile (trapfilehandle,txttbl^,11200);
     closepfile (trapfilehandle);
     pfileior := 0;
     textdefaults;
     {-----------------------------------------------}
     trapcrtprint (0,trapcrtcy+1,7,'Geting vsync freq...');
     sysinf.vsyncfreq := getvsyncfreq;
     val2pchar (sysinf.vsyncfreq,__ppchar2);
     strcopy (@__ppchar1,__ppchar2);
     strcat (@__ppchar1,'hz');
     trapcrtprint (trapcrtcx,trapcrtcy,7,__ppchar1);
     exitproc := @trapexitproc;
     freemem (__ppchar2,60);
     {-----------------------------------------------}
     {Inicjacja trybu graficznego i palety kolorow}
     if configrec.scrmode = 0 then initvgascr
     else
     if not setvesamode (configrec.xscrsx,configrec.xscrsy) then
     begin
          initvgascr;
          configrec.scrmode := 0;
     end
     else
     begin
          configrec.xsbx1 := configrec.xscrsx div 2-160;
          configrec.xsbx2 := configrec.xscrsx div 2+159;
          configrec.xsby1 := configrec.xscrsy div 2-100;
          configrec.xsby2 := configrec.xscrsy div 2+99;
          scrox := configrec.xscrsx div 2;
          scroy := configrec.xscrsy div 2;
     end;
     {}
     set_8_32_pal;
     setgammapal (configrec.gamma);
     loadallfiles;
     {glowna petla programu}
     while (true) do
     begin
          startframe;
          processtimers;
          verifykeys;
          update;
          render;
          gettrapfps;
          inc (trapframes);
     end;
end.