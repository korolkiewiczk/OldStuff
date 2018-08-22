IFNDEF __ENGINE_INC
__ENGINE_INC=1

;Engine.inc-struktury, definicje i dane globalne

;definicje

bool            typedef dword
float           typedef real4
double          typedef real8
__int8          typedef sbyte
__int16         typedef sword
__int32         typedef sdword
__int64         typedef qword

;strukrury

;prosta struktora przechowujaca punkt
EnPoint struct
        x double ?      ;wspolrzedne x punktu
        y double ?      ;wspolrzedne y punktu
        z double ?      ;wspolrzedne z punktu
        w bool ?        ;czy punkt przetransformowany
EnPoint ends

;struktura przechowujaca wierzcholek
EnVertex struct
        x double ?      ;wspolrzedne x wierzcholka
        y double ?      ;wspolrzedne y wierzcholka
        z double ?      ;wspolrzedne z wierzcholka
        w bool ?        ;czy wierzcholek przetransformowany
        color dd ?      ;kolor wierzcholka (w zaleznosci od trybu graf.)
        u float ?       ;wspolrzedne x tekstury (0..1)
        v float ?       ;wspolrzedne y tekstury (0..1)
EnVertex ends

EnTriangle struct
        TrVertex1 EnVertex <>   ;pierwszy wierzcholek
        TrVertex2 EnVertex <>   ;drugi wierzcholek
        TrVertex3 EnVertex <>   ;trzeci wierzcholek
EnTriangle ends

En3DObject struct       ;struktora przechowujaca objekt 3d (lub scene)
        ObjnTriangles dd ?      ;ilosc trojkatow dla tego objektu
        ObjTriangles dd ?       ;wskaznik do trojkatow
        ObjnObjects dd ?        ;ilosc objektow podrzednych dla tego objetku
        ObjObjects dd ?         ;wskaznik do objetkow podrzednych
        ObjParentObj dd ?       ;wskaznik do objektu rodzica
En3DObject ends

;dane

Engine_Camera_Dist float 128.0
Engine_Scr_XCenter float 160.0
Engine_Scr_YCenter float 100.0

ENDIF
