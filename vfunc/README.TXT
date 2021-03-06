.:VFUNC:.
.:V 0.1:.

1. WST�P
2. DEFINIOWANIE FUNKCJI
3. STEROWANIE
4. WSKAZ�WKI
5. O PROGRAMIE


1. WST�P
--------

Program umo�liwia wy�wietlanie tr�jwymiarowych wykres�w funkcji wektorowych 
(wy�wietlanie krzywych i p�aszczyzn w przestrzeni 3D). Umo�liwia tak�e 
tworzenie ich animacji.

2. DEFINIOWANIE FUNCKJI
-----------------------

Jako parametr wywo�ania programu podajemy nazw� pliku przechowuj�cego opis 
naszego wykresu (w systemie Windows wystarczy "przeci�gn��" dany plik 
tekstowy na plik wykonywalny programu). Domy�lnym plikiem jest default.txt.
Schemat pliku opisuj�cego wykres wygl�da nast�puj�co:
C/S [kolor_w_hex]
t1 t2 [s1 s2] ilo��_w�z��w
x(t[,s])
y(t[,s])
z(t[,s])
C/S [kolor_w_hex]
...
...

Przyk�ad:

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

(opisuje torus w kolorze czerwonym (o prze�roczysto�ci 0.5) oraz krzyw� 
vivaniego w kolorze zielonym)
Liczne przyk�ady do��czone s� do programu.

Wiersz 5n+1 okre�la czy definiujemy krzyw� (C), czy p�aszczyzn� (S). 
Opcjonalnie mo�emy poda� kolor obiektu (warto�� szesnastkowa np. ff8020).
Wiersz 5n+2 okre�la warto�ci pocz�tkowe i ko�cowe zmiennych t oraz s (w 
przypadku p�aszczyzny). Na ko�cu wiersza podajemy ilo�� punkt�w w przypadku 
krzywej lub ilo�� punkt�w w jednym wierszu siatki w przypadku p�aszczyzny.
Wiersze 5n+3, 5n+4 i 5n+5 okre�laj� odpowiednio funkcje zmiennych x, y i z, 
do kt�rych przekazywane s� zmienne t i s (w przypadku krzywej tylko zmienna 
t).

W wyra�eniach opisuj�cych funkcje mo�emy u�ywa� nast�puj�cych funkcji:
ABS, SGN, SIN, COS, TAN, CTAN, FLOOR, CEIL, LN, RND, PI, E
oraz u�ywa� operator�w +, -, *, /, ^ i zmiennych T, S oraz TIME. Ostatnia 
zmienna umo�liwia tworzenie animowanych wykres�w funkcji. Wielko�� znak�w 
nie ma znaczenia.
np. 3abs(e^3-t-sin(time)*2)

3. STEROWANIE
-------------

UP/DOWN, LEFT/RIGHT, PAGE UP/PAGE DOWN - translacja kamery r�wnolegle od osi 
(odpowiednio) OY, OX, OZ.
Mysz - zmiana k�ta kamery
Lewy/Prawy klawisz myszy - w prz�d/ty�
ENTER - ponowne za�adowanie danych z pliku
TAB - zmiana trybu wy�wietlania
SHIFT - przy�pieszenie zmian
F - cieniowanie g�adkie/p�askie
I - w��czenie/wy��czenie wy�wietlenia informacji
J - ustawienie wektora �wiat�a zgodnego z wektorem kamery
B - w��czenie/wy��czenie efektu prze�roczysto�ci
HOME - ustawienia pocz�tkowe
ESC - wyj�cie

4. WSKAZ�WKI
------------

Klawiszem TAB mo�emy zmienia� jeden z trzech sposob�w wy�wietlania funkcji:
-Siatka
-Wype�nienie jednolitym kolorem
-Cieniowanie z uwzgl�dnieniem wektora �wiat�a
Klawisz SHIFT przy�piesza przemieszczanie kamery. Najszybciej przemieszczamy 
kamer� dokonuj�c jej translacji (patrz wy�ej - sterowanie)

5. O PROGRAMIE
--------------

Program napisany w j�zyku C++ z wykorzystaniem autorskiego projektu fEval 
(do obliczania warto�ci wyra�e� funkcji) oraz biblioteki OpenGL do 
wy�wietlania grafiki 3D. Zosta�o wykorzystane �rodowisko CodeBlocks i 
kompilator g++.

Strona domowa autora: http://kk.plenty.vbiz.pl
Autor: Kamil Korolkiewicz 