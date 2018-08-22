.:VFUNC:.
.:V 0.1:.

1. WSTÊP
2. DEFINIOWANIE FUNKCJI
3. STEROWANIE
4. WSKAZÓWKI
5. O PROGRAMIE


1. WSTÊP
--------

Program umo¿liwia wyœwietlanie trójwymiarowych wykresów funkcji wektorowych 
(wyœwietlanie krzywych i p³aszczyzn w przestrzeni 3D). Umo¿liwia tak¿e 
tworzenie ich animacji.

2. DEFINIOWANIE FUNCKJI
-----------------------

Jako parametr wywo³ania programu podajemy nazwê pliku przechowuj¹cego opis 
naszego wykresu (w systemie Windows wystarczy "przeci¹gn¹æ" dany plik 
tekstowy na plik wykonywalny programu). Domyœlnym plikiem jest default.txt.
Schemat pliku opisuj¹cego wykres wygl¹da nastêpuj¹co:
C/S [kolor_w_hex]
t1 t2 [s1 s2] iloœæ_wêz³ów
x(t[,s])
y(t[,s])
z(t[,s])
C/S [kolor_w_hex]
...
...

Przyk³ad:

S 80ff0000
-pi pi -pi pi 50
(3+cos(s))*cos(t)
(3+cos(s))*sin(t)
sin(s)
C 00ff00
-pi pi 1000*2
cos(t)*cos(t)
sin(t)*cos(t)
sin(t)

(opisuje torus w kolorze czerwonym (o przeŸroczystoœci 0.5) oraz krzyw¹ 
vivaniego w kolorze zielonym)
Liczne przyk³ady do³¹czone s¹ do programu.

Wiersz 5n+1 okreœla czy definiujemy krzyw¹ (C), czy p³aszczyznê (S). 
Opcjonalnie mo¿emy podaæ kolor obiektu (wartoœæ szesnastkowa np. ff8020).
Wiersz 5n+2 okreœla wartoœci pocz¹tkowe i koñcowe zmiennych t oraz s (w 
przypadku p³aszczyzny). Na koñcu wiersza podajemy iloœæ punktów w przypadku 
krzywej lub iloœæ punktów w jednym wierszu siatki w przypadku p³aszczyzny.
Wiersze 5n+3, 5n+4 i 5n+5 okreœlaj¹ odpowiednio funkcje zmiennych x, y i z, 
do których przekazywane s¹ zmienne t i s (w przypadku krzywej tylko zmienna 
t).

W wyra¿eniach opisuj¹cych funkcje mo¿emy u¿ywaæ nastêpuj¹cych funkcji:
ABS, SGN, SIN, COS, TAN, CTAN, FLOOR, CEIL, LN, RND, PI, E
oraz u¿ywaæ operatorów +, -, *, /, ^ i zmiennych T, S oraz TIME. Ostatnia 
zmienna umo¿liwia tworzenie animowanych wykresów funkcji. Wielkoœæ znaków 
nie ma znaczenia.
np. 3abs(e^3-t-sin(time)*2)

3. STEROWANIE
-------------

UP/DOWN, LEFT/RIGHT, PAGE UP/PAGE DOWN - translacja kamery równolegle od osi 
(odpowiednio) OY, OX, OZ.
Mysz - zmiana k¹ta kamery
Lewy/Prawy klawisz myszy - w przód/ty³
ENTER - ponowne za³adowanie danych z pliku
TAB - zmiana trybu wyœwietlania
SHIFT - przyœpieszenie zmian
F - cieniowanie g³adkie/p³askie
I - w³¹czenie/wy³¹czenie wyœwietlenia informacji
J - ustawienie wektora œwiat³a zgodnego z wektorem kamery
B - w³¹czenie/wy³¹czenie efektu przeŸroczystoœci
HOME - ustawienia pocz¹tkowe
ESC - wyjœcie

4. WSKAZÓWKI
------------

Klawiszem TAB mo¿emy zmieniaæ jeden z trzech sposobów wyœwietlania funkcji:
-Siatka
-Wype³nienie jednolitym kolorem
-Cieniowanie z uwzglêdnieniem wektora œwiat³a
Klawisz SHIFT przyœpiesza przemieszczanie kamery. Najszybciej przemieszczamy 
kamerê dokonuj¹c jej translacji (patrz wy¿ej - sterowanie)

5. O PROGRAMIE
--------------

Program napisany w jêzyku C++ z wykorzystaniem autorskiego projektu fEval 
(do obliczania wartoœci wyra¿eñ funkcji) oraz biblioteki OpenGL do 
wyœwietlania grafiki 3D. Zosta³o wykorzystane œrodowisko CodeBlocks i 
kompilator g++.

Strona domowa autora: http://kk.plenty.vbiz.pl
Autor: Kamil Korolkiewicz 