;TIME.INC for DJGPP by Kamil Korolkiewicz 2005/2006
;Funkcje do obslugi czasu

;stale

TIME_FLAGS_ABSOLUTE_TIME=0
TIME_FLAGS_MILLISEC_TIME=1
TIME_FLAGS_SEC_TIME=2
TIME_FLAGS_MIN_TIME=3
TIME_FLAGS_HOUR_TIME=4
TIME_FLAGS_CLOCK_ONLY=8


;dane

.data

__cpu__tsc__freq dq 0           ;czestotliwosc taktowania procesora
Time_Get dd 0
__float__time__0_001 real4 0.001
__float__time__1_0 real4 1.0
__float__time__60_0 real4 60.0
__float__time__3600_0 real4 3600.0
__float__time__0_018 real4 0.018
__float__time__18_2 real4 18.2
__float__time__1092 real4 1092.0
__float__time__65520 real4 65520.0
__float__time__temp real4 0.0


public __cpu__tsc__freq,Time_Get
public __float__time__0_001,\
__float__time__1_0,\
__float__time__60_0,\
__float__time__3600_0,\
__float__time__0_018,\
__float__time__18_2,\
__float__time__1092,\
__float__time__65520,\
__float__time__temp

.code

Time struct
        current_time dq 0
        flags dd 0
Time ends

;flags:
;[0..2]-rodzaj zwracanego czasu:000-absolutny,001-w milisek,010-w sek,011-w min,100-w godz
;[3..3]-rodzaj timera:0-najlepszy,1-zawsze clock (zegar systemowy)

extern __cpu__tsc__freq:qword,Time_Get:dword
extern __float__time__0_001:real4,\
__float__time__1_0:real4,\
__float__time__60_0:real4,\
__float__time__3600_0:real4,\
__float__time__0_018:real4,\
__float__time__18_2:real4,\
__float__time__1092:real4,\
__float__time__65520:real4,\
__float__time__temp:real4


;--------------------------------
;_InitTime ()
;       Initializuje procedury do obslugi czasu
;       Wyjscie: eax=0-czas jest pobierany z uzyciem zegara systemowego (mala dokladnosc)
;                eax=1-czas jest pobierany z uzyciem zegara procesora (bardzo duza dokladnosc)
;       Uwaga: Funkcja musi byc wywolana na poczatku
;--------------------------------

_InitTime proc C uses edi es
        local t1:dword,t2:dword
        mov eax,cpu_feat
        and eax,10000b
        jnz InitTime_tsc
        xor eax,eax
        mov dword ptr __cpu__tsc__freq,eax
        mov dword ptr __cpu__tsc__freq+4,eax
        xor eax,eax
        ret
InitTime_tsc:
        mov es,stub32zerosel
        mov edi,1132
        mov eax,es:[edi]
InitTime_waitt1:
        cmp eax,es:[edi]
        je InitTime_waitt1
.586p
        rdtsc
        mov t1,eax
        mov t2,edx
        mov ecx,4
InitTime_waittloop:
        mov eax,es:[edi]
InitTime_waitt2:
        cmp eax,es:[edi]
        je InitTime_waitt2
        loop InitTime_waittloop
        rdtsc
.386
        sub eax,t1
        sbb edx,t2
        mov ebx,298188
        mul ebx
        shrd eax,edx,16
        shr edx,16
        mov dword ptr __cpu__tsc__freq,eax
        mov dword ptr __cpu__tsc__freq+4,edx
        xor eax,eax
        inc eax
        ret
_InitTime endp

;--------------------------------
;Time_Get (eax)
;       Pobiera do struktory Time aktualny czas oraz zwraca roznice czasu w danej jednostce (Time.flags)
;       (liczba zmiennopozycyjna 32bit)
;       Wejscie: eax-offset do strutkury Time
;       Wyjscie: st(0)-czas oraz zmieniona struktora
;--------------------------------

.586p

assume eax:ptr Time

__gettime__clock__ proc C uses ebx ,ttime:dword
        mov eax,ttime
        mov ebx,[eax].flags
        and ebx,7
        or ebx,ebx
        jz __gettime__clock__absolute
        cmp ebx,1
        je __gettime__clock__ms
        cmp ebx,2
        je __gettime__clock__s
        cmp ebx,3
        je __gettime__clock__min
        cmp ebx,4
        je __gettime__clock__h
        ffree st(0)
        ret

__gettime__clock__absolute:
        fild [eax].current_time
        mov es,stub32zerosel
        mov ebx,1132
        mov ebx,es:[ebx]
        mov dword ptr [eax].current_time,ebx
        xor ebx,ebx
        mov dword ptr [eax].current_time+4,ebx
        fild [eax].current_time
        fxch
        fsubp st(1),st(0)
        ret

__gettime__clock__ms:
        fld __float__time__0_018
        jmp __gettime__clock__calc_time

__gettime__clock__s:
        fld __float__time__18_2
        jmp __gettime__clock__calc_time

__gettime__clock__min:
        fld __float__time__1092
        jmp __gettime__clock__calc_time

__gettime__clock__h:
        fld __float__time__65520
        jmp __gettime__clock__calc_time

__gettime__clock__calc_time:
        fstp __float__time__temp
        fild [eax].current_time
        mov es,stub32zerosel
        mov ebx,1132
        mov ebx,es:[ebx]
        mov dword ptr [eax].current_time,ebx
        xor ebx,ebx
        mov dword ptr [eax].current_time+4,ebx
        fild [eax].current_time
        fxch
        fsubp st(1),st(0)
        fdiv __float__time__temp
        ret
__gettime__clock__ endp


__gettime__rdtsc__ proc C uses ebx ,ttime:dword
        mov eax,ttime
        mov ebx,[eax].flags
        test ebx,1000b
        jz __gettime__rdtsc__no_clock
        push eax
        call __gettime__clock__
        add esp,4
        ret
__gettime__rdtsc__no_clock:
        and ebx,7
        or ebx,ebx
        jz __gettime__rdtsc__absolute
        cmp ebx,1
        je __gettime__rdtsc__ms
        cmp ebx,2
        je __gettime__rdtsc__s
        cmp ebx,3
        je __gettime__rdtsc__min
        cmp ebx,4
        je __gettime__rdtsc__h
        ffree st(0)
        ret

__gettime__rdtsc__absolute:
        mov ebx,eax
        assume ebx:ptr Time
        fild [ebx].current_time
        rdtsc
        mov dword ptr [ebx].current_time,eax
        mov dword ptr [ebx].current_time+4,edx
        fild [ebx].current_time
        assume ebx:nothing
        fxch
        fsubp st(1),st(0)
        ret

__gettime__rdtsc__ms:
        fld __float__time__0_001
        jmp __gettime__rdtsc__calc_time

__gettime__rdtsc__s:
        fld __float__time__1_0
        jmp __gettime__rdtsc__calc_time

__gettime__rdtsc__min:
        fld __float__time__60_0
        jmp __gettime__rdtsc__calc_time

__gettime__rdtsc__h:
        fld __float__time__3600_0
        jmp __gettime__rdtsc__calc_time

__gettime__rdtsc__calc_time:
        fild __cpu__tsc__freq
        fmul st(0),st(1)
        fstp __float__time__temp
        ffree st(0)
        fincstp
        mov ebx,eax

        assume ebx:ptr Time
        fild [ebx].current_time
        rdtsc
        mov dword ptr [ebx].current_time,eax
        mov dword ptr [ebx].current_time+4,edx
        fild [ebx].current_time
        assume ebx:nothing
        fxch
        fsubp st(1),st(0)
        fdiv __float__time__temp
        ret
__gettime__rdtsc__ endp

assume eax:nothing

.386
