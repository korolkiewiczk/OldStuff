EXPR v0.91 for WIN32

POMOC PROGRAMU

Spis tresci:
1.STEROWANIE
2.KONSOLA
3.TRYB WYKRESU
4.TRYB DZWIEKU
5.WYRAZENIA
6.STANDARDOWA BIBLIOTEKA FUNKCJI
7.FUNKCJE ZEWNETRZNE I UZYTKOWNIKA

1. STEROWANIE
Nizej znajduje sie opis dzialania poszczegolnych klawiszy w roznych trybach dzialania programu. Funkcje klawiszy odpowiadaja opcjom menu i ich skrotom.

1.1 Ogolne

[F1]-pomoc
[F4]-konsola
[F5]-okno wykresu
[F6]-okno dziweku
[F8]-zakoncz
[F9]-zrzut ekranu
[Shift]+[F9]-zrzut ekranu - negatyw
[Ctrl]+[F1-F8]-tryb graficzny
[Ctrl]+[F9]-natywny tryb graficzny
[Ctrl]+[W]-przelacz miedzy trybem ekranowym a pelnym ekranem
[Ctrl]+[1-8]-wybierz kolor rysowania
[Shift]+[Ctrl]+[1-4]-tryb rysowania:
 1-funkcja y=f(x) na plaszczyznie (2D)
 2-funkcja z=f(x,y) na plaszczyznie (2D)
 3-funkcja z=f(x,y) w przestrzeni (3D)
 4-funkcja parametryczna x=x(t), y=y(t) na plaszczyznie (2D)
UWAGA: Aby zmienic sposob przedstawiania wykresu w trybie wykresu wystarczy wcisnac jedynie klawisz z cyfra ktora odpowiada wyzej wymienionym.
Srodkowy klawisz myszy ukrywa/pokazuje kursor myszy.


1.2 W trybie konsoli

[Enter]/[Lewy klawisz myszy]-oblicza wartosc wyrazenia i ustawia je do rysowania w trybie wykresu oraz odtwarzania w trybie dzwieku
[Esc]/[Prawy klawisz myszy]-czysci pole tekstowe
[Tab]-wyswietla liste funkcji
[Gora/Dol]-dostep do poprzednio wpisywanych wyrazen
[Page Up/Down]-przewijanie konsoli w gore/dol
[Shift]+[Lewo/Prawo]-przewijanie konsoli w lewo/prawo


1.3 W trybie wykresu

UWAGA: Klawisz [Shift] zwieksza predkosc zmian (X4), albo odwraca ich kierunek
[Kursory]-przesuwanie wykresu
[Page Up]/[Rolka myszy]-powieksz
[Page Down]/[Rolka myszy]-pomniejsz
[Home]-standardowe ustawienia
[Tab]-zmiana analizowanego wykresu
[Del]-usuniecie analizowanego wykresu
[Q]-zmiana dokladnosc wynikow
[R]-tryb cyklicznego odswierzania obrazu (wykorzystywane w animacji)
[F]-zmienia sposob dopasowania wykresu do ekranu
[Z]-wlacza/wylacza napisy
[X]-pokazuje/chowa osie wykresu
[C]-pokazuje/chowa siatke
[0]-szuka miejsca zerowego funkcji
[\]-zmienia tryb wyswietlania liczby klatek na sekunde (chwilowa/srednia)
Za pomoca myszy rowniez mozemy przesuwac wykres oraz okreslac wartosc funkcji w danym punkcie.


1.4 W trybie wykesu 3D (dodatkowo)

[Mysz]-rozgladanie sie
[Przyciski myszy]-ruch do przodu/tylu
[W]/[S]/[A]/[D]-ruch gora/tyl/w lewy bok/w prawy bok
[Page Up]-w gore
[Page Down]-w dol
[N]-gestosc siatki (zmiana dwutrotna)
[M]-promien widocznosci (zmiana o wartosc gestosci siatki)
[H]-wstrzymanie obliczen wykresu dla trybu 3D
[J]-wlacz/wylacz zmiane kierunku swiatla
[K]-wlacz/wylacz tryb jasnosci wykresu w zaleznosci od odleglosci
[L]-promien swiatla widocznosci/jasnosc wykresu
[V]-zwieksz/zmiejsz kat widzenia
[U]-zmien metode rysowania (siatka lub wypelnione)
[I]-wlacz/wylacz oswietlenie
[P]-szybkie/wolne przesuwanie sie
[O]-wlacz/wylacz opengl
[G]-wlacz/wylacz oddzielne oswietlenie dla przedniej i tylnej powierzchni (opengl)
[Y]-ukrywaj tylne powierzchnie (opengl)
[T]-wlacz/wylacz teksturowanie
[B]-wlacz/wylacz tryb gry
[/]-model fizyki w trybie gry
[E]-wlacz/wylacz mape srodowiska


1.5 W trybie funkcji parametrycznej

[-]/[=]-zmniejsz/zwieksz liczbe wezlow
[ lub ]-zmniejsz/zwieksz poczatkowe t1
[;]/[']-zmniejsz/zwieksz koncowe t2


1.6 W trybie dzwieku

[Gora]/[Dol]-zmiana glosnosci dzwieku
[Lewo]/[Prawo]-zmiana balansu
[Shift]+[Gora]/[Dol]-zmiana mnoznika amplitudy
[Shift]+[Lewo]/[Prawo]-zmiana mnoznika czestotliwosci
[Page Up]/[Page Down]-zmiana skali oscyloskopu
[Spacja]-wstrzymaj odtwarzanie
[Tab]-nastepne wyrazenie
[Home]-ustawienia domyslne
[I]-wlacz/wylacz mikrofon
[Enter]-odtwarzaj od pozycji poczatkowej (t1)
[Del]-usun dana funkcje
[S]-wlacz/wylacz tryb nasycenia
[R]-wlacz/wylacz tryb powtarzania w przedziale <t1,t2>
[ / ]-zmiana t1
[;]/[']-zmiana t2


2. KONSOLA
Konsola umozliwia wprowadzanie wyrazen do programu. W polu edycji mozemy edytowac wyrazenie. Klawisz [Tab] wyswietla liste dostepnych w programie funkcji. Klawisz [Enter] powoduje wykonanie (obliczenie) wprowadzonego wyrazenia oraz przekazanie go do trybu wykresu i dzwieku. Zaczynajac wpisywanie od znaku '/', wyrazenie bedzie tylko obliczane i nie przekazywane do rysowania/odtwarzania. Z kolei znak '>' lub '<' na poczatku powoduje odpowiednio wykonywanie danego wyrazenia przed rozpoczeciem i po zakonczeniu rysowania funckji w trybie wykresu. Znak ';' oznacza komentarz.
Uzytkownik ma dostep do poprzednio wpisywanych wyrazen za pomoca klawiszy kursora (gora/dol).
Oprocz liczb w zapisie dziesietnym, mozemy uzywac liczb w zapisie szesnastkowym zapisujac znak $ przed pierwsza cyfra liczby.

3.TRYB WYKRESU
Tryb wykresu uaktywniamy klawiszem [F5]. Maksymalnie moze byc wyswietlanych 8 funkcji jednoczesnie.
W standardowym trybie (nr 1) rysowane sa funkcje postaci y=f(x).
Z kolei w trybie nr 2 rysowanie rowniez odbywa sie na plaszczyznie, jednak przebiega w inny sposob. Dla kazdego punktu plaszczyzny (x,y) obliczana jest wartosc funkcji i okresla ona natezenie koloru.
Tryb nr 3 jest podobny do trybu nr 2, jednak wartosc funkcji okresla wysokosc plaszczyzny w 3D. W tym trybie dostepne sa standardowe metody rysowania: linie (domyslnie), trojkaty wypelnione kolorem (klawisz [U]), oraz tekstura ([T]). Mozemy rowniez uaktywnic swiatlo ([I]). Zmiane wektora swiatla uaktywniamy klawiszem [J]. Mozemy rowniez okreslic, czy wraz ze wzrostem odleglosc od obserwatora, wykres staje sie ciemniejszy ([K]). Promien obszaru wykresu zmieniamy klawiszem [M]. Gestosc siatki zas zmieniamy klawiszem [N]. W trybie gry ([B]) wykrywane sa kolizje z plaszczyzna wykresu oraz dzialaja na obserwatora zasady fizyki.
Tryb nr 4 umozliwia rysowanie funkcji okreslonych rownaniem parametrycznym. Funkcja par(a,b) powoduje przypisanie x:=a, y:=b. Np. par(cos(t),sin(t)) rysuje okrag. Funkcja pol(a) powoduje wywolanie par(cos(t)*a,sin(t)*a), jest to odwzorowanie funkcji w ukladzie biegunowym na kartezjanski. Np. pol(t) rysuje spirale Archimedesa.

4. TRYB DZWIEKU
Tryb dzwieku uaktywniamy klawiszem [F6]. Odtwarzany jest dzwiek, ktorego charakterystyke okresla wprowadzone wczesniej wyrazenie. Moze byc okreslonych do 8 funkcji jednoczesnie. Jesli wlaczony jest mikrofon, to w zmiennej t zapisywana jest amplituda dzwieku wejsciowego.

5. WYRAZENIA
Kazde wyrazenie jest pewnym zlozeniem funkcji. Operatory (np. +,-,*,/) rowniez sa funkcjami, z tym ze zapisujemy je w tradycyjnej postaci np. 5*4. Odpowiada to wywolaniu funkcji MUL(5,4). Z kolei wyrazenie 5+SIN(PI)*(4+5) odpowiada ADD(5,MUL(SIN(PI),ADD(4,5))). Jak widac wiec wyrazenie to zlozenie skonczonej liczby funckji.
Klawiszem [Tab] uaktywniamy (w trybie konsoli) liste dostepnych funkcji oraz zmiennych. Oprocz standardowych funkcji i zmiennych, mozna definiowac wlasne oraz wczytywac je z zewnetrznych bibliotek DLL (pluginy). Wyrazenia wpisujemy w konsoli, jednak mozemy je wczytywac z zewnetrznych plikow (funkcja <EXEC("nazwa pliku")>). Ponadto, przy uruchamianiu programu, domyslnie wykonywane sa wyrazenia zawarte w pliku <autoexec.txt>.

6. STANDARDOWA BIBLIOTEKA FUNKCJI
Ponizej znajduje sie opis funkcji z podzialem na grupy.

6.1 Funkcje matematyczne

6.1.1 Funkcje jedno-argumentowe: 
ABS(A)         - modul z liczby A
ACOS(A)        - arcus cosinus z liczby A
ACOSH(A)       - arcus cosinus hiperboliczny z liczby A
ACTAN(A)       - arcus cotangens z liczby A
ASIN(A)        - arcus sinus z liczby A
ASINH(A)       - arcus sinus hiperboliczny z liczby A
ATAN(A)        - arcus tangens z liczby A
ATANH(A)       - arcus tangens hiperboliczny z liczby A
CEIL(A)        - najmniejsza liczba calkowita nie mniejsza niz A
CHS(A)         - zwraca -A (operator -)
COSH(A)        - cosinus hiperboliczny z liczby A
COS(A)         - cosinus z A
CTAN(A)        - cotangens z A
CTANH(A)       - cotangens hiperboliczny z liczby A
EXP(A)         - e^A
FLOOR(A)       - najwieksza liczba calkowita nie wieksza niz liczba A
FRAC(A)        - czesc ulamkowa liczby
GAMMA(A)       - funkcja gamma z liczby A (jesli A naturalne to = A!)
LN(A)          - logarytm naturalny z liczby A
LOG(A)         - logarytm dziesietny z liczby A
ROUND(A)       - zaokraglenie liczby A do najblizszej liczby calkowitej
SIN(A)         - sinus z liczby A
SINH(A)        - sinus hiperboliczny z liczby A
SQRT(A)        - pierwiastek kwadratowy z liczby A
TAN(A)         - tangens z liczby A
TANH(A)        - tangens hiperboliczny  z liczby A

6.1.2 Funkcje dwu-argumentowe
Czesc funkcji odpowiada operatorom binarnym (dwuargumentowym).
ADD(A,B)       - A+B (operator +)
DIST(A,B)      - SQRT(A*A+B*B)
DIV(A,B)       - A/B (operator -)
EQUP(A,B)      - A==B (operator ==) - zwraca 1 <=> gdy A=B
EQU(A,B)       - A=B (operator =) - zwraca max{(1-ABS(A-B))/_EPS,0}
GT(A,B)        - A>B (operator >)
GTE(A,B)       - A>=B (operator >=)
LAND(A,B)      - A&&B (operator &&) - logiczna koniunkcja
LEQU(A,B)      - A<=>B (operator <=>) - logiczna rownowaznosc
LIMP(A,B)      - A=>B (operator =>) - logiczna implikacja
LOGB(A,B)      - logarytm przy podstawie B z liczby A
LOR(A,B)       - A||B (operator ||) - logiczna alternatywa
LW(A,B)        - A<B (operator <)
LWE(A,B)       - A<=B (operator <=)
LXOR(A<B)      - A^^B (operator ^^) - logiczne zaprzeczenie rownowaznosci
MAX(A,B)       - max{A,B}
MIN(A,B)       - min{A,B}
MOD(A,B)       - A mod B (operator %)
MUL(A,B)       - A*B (operator *)
NEQU(A,B)      - A nie rowne B (operator !=)
POW(A,B)       - A do potegi B (operator ^)

6.1.3 Pozostale
DIFF("A")      - numerycznie oblicza pochodna wyrazenia A dla zmiennej X
INTEGRAL("A",B,C) - numerycznie calkuje wyrazenie A na przedziale [B,C]
IFV(A,B,C)     - jesli A!=0 to zwraca B, w przeciwnym wypadku zwraca C
PAR(A,B)       - X:=A, Y:=B
POL(A)         - X:=A*COS(T), Y:=A*SIN(T)
RND()          - zwraca wartosc losowa z pzedzialu [0,1]
SRND(A)        - ustawia generator liczb losowych na liczbe calkowita A
                 jesli A jest 0 to ustawia na wartosc poczatkowa
SET(@A,B)      - przypisz zmiennej A liczbe B (operator :=) (*)
SUM("A",B,C)   - oblicz sume wyrazen A dla zmiennej K w przedziale [B,C]
TABLE("A",B,C,D) - wypisuje tabelke wartosci wyrazenia A dla X zmieniajacego
                   sie o D w przedziale [B,C]
(*) - wyrazenie @A oznacza adres do zmiennej A. Dlatego tez nalezy pisac @A:=B, a nie A:=B.

6.2 Funkcje obslugowe
BPRINT(A,B)    - wypisuje liczbe A w systemie o podstawie B
CLS()          - czysci okno konsoli
COLOR(A)       - ustala kolor rysowania na A
ECHO(A)        - jesli A!=0 to wypisuj w konsoli wyrazen wczytywanych
                 z zewnetrznego pliku
EXEC("A")      - wykonaj wyrazenia z pliku o nazwie A
FILE("A",B)    - zwraca bajt o numerze B z pliku o nazwie A (mozna wykorzystac
                 np. do mapy wysokosci, albo do odtwarzania pliku WAV)
LOADLIB("A")   - laduje zewnetrzna biblioteke z pliku o nazwie A
MODE(A)        - przelacza na tryb rysowania o numerze A+1 
                 (np. MODE(2) oznacza tryb nr 3, czyli tryb 3D)
PRINT("A")     - wypisuje ciag znakow A do konsoli
PTR(A,B)       - wartosc z adresu A o przesunieciu B. Mozna rowniez zapisac
                 A[B]. Np. @X[23] podaje wartosc przesunieta o 23 bajty 
                 w stosunku do zmiennej X
SOUNDMODE(A,B,C) - ustawia czestotliwosc probkowania dzwieku na A (np. 44100),
                   ilosc bitow na probke na B (np. 16) i  ilosc kanalow na C 
                   (1-mono, 2-stereo)
SYSINFO()      - wypisuje w konsoli infomacje nt. systemu.
TEX_LOAD(A,"B")       - laduje teksture o numerze A+1, z pliku o nazwie B
TEX_SETPAR(A,B,C,D,E) - ustawia parametry tekstury nr A+1, gdzie odpowiednio
                        zmienne to: przesuniecie x i y oraz szerokosc 
                        i wysokosc tekstury w skali jednostki
TEX_SETALPHA(A,B)     - ustawia kanal alfa tekstury nr A+1 na B
TEX_FUNC[1,3,4]
(A,"B"[,"C","D","E"],F,G,H,I) 
                      - dynamiczne generowanie tekstury nr A+1 z odp. 1,3,4 
                        kanalami kolorow gdzie B,C,D,E to wyrazenia,
                        F,G,H,I to wspolrzedne w jakich przebiegaja 
                        zmienne x i y, typowo sa to -1, 1, 1, -1
TEX_GENNORMALMAP(A,B) - generuje teksture normalnych. Traktuje teksture nr A
                        z obrazem jako mape wysokosci gdzie B to wysokosc
                        odpowiadajaca najjasniejszemu kolorowi (w odcieniach
                        szarosci). Wywolanie funkcji nie jest konieczne, jednak
                        umozliwia zmiane domyslnej wysokosci, oraz moze byc
                        przydatne przy teksturach dynamicznie generowanych.
                        Funkcja tylko dla wersji z Cg
TEX_SETNORMALMAP("A",B) - Laduje z pliku B mape normalnych do tekstury nr A+1
                        Funkcja tylko dla wersji z Cg
VARGET(A,B)    - pobiera wartosc zmiennej systemowej A definiowanej 
                 w pliku config.cfg z przesunieciem B
VARSET(A,B,C)  - ustawia wartosc zmiennej systemowej A definiowanej 
                 w pliku config.cfg na wartosc (znakowa) B z przesunieciem C
VARSETF(A,B,C) - ustawia wartosc zmiennej systemowej A definiowanej 
                 w pliku config.cfg na wartosc (liczbowa) B z przesunieciem C
VIDMODE(A,B)   - ustawia tryb graficzny na AXB (np. VIDMODE(640,480))
SHELL("A")     - wykonuje polecenie powloki systemowej
STRIB("A",B)   - zwraca liczbe A (ktora jest w systemie B) w systemie 10

6.3 Funkcje definiowania nowych funkcji (uzytkownika)
Wiecej o definiowaniu wlasnych funkcji w kolejnej sekcji
F_BEGIN("A",B) - rozpoczyna definiowanie nowej funkcji o nazwie A z liczba
                 parametrow rowna B.
F_END()        - konczy definiowanie funkcji
F_VAR("A")     - tworzy nowa zmienna
F_DEL("A")     - usuwa zmienna/funkcje

6.4 Zmienne
X              - podstawowa zmienna dla wszystkich trybow wykresu i dzwieku
Y              - jw.
T              - zmienna dla funkcji parametrycznej
TIME           - aktualna wartosc czasu systemowego (w sek.)
E              - liczba e
PI             - liczba pi
K              - zmienna pomocnicza (wykorzystywana pzez funkcje sumowania)
_1,_2,_3,...   - kolejne parametry przekazywane do funkcji uzytkownika
_EPS           - patrz funkcja EQU

7.FUNKCJE ZEWNETRZNE I UZYTKOWNIKA
Oprocz standardowej biblioteki funkcji program EXPR umozliwia dodawanie nowych funkcji na 2 sposoby: poprzez wczytywanie zewnetrzych bibiotek z plikow DLL (przygotowanych wczesniej i skompilowanych) oraz poprzez zapisywania ich jako ciag pewnych wyrazen, na etapie uruchomionego programu.
Funkcje uzytkownika tworzone sa wedlug prostego schematu. Mozemy je zapisywac zarowno konsoli programu EXPR, jak i zapisywac je w pliku tekstowym, a nastepnie wczytywac uzywajac funkcji EXEC("A"), gdzie A to nazwa naszego pliku (albo zapisywac do pliku autoexec.txt).
Schemat definiowania funkcji jest nastepujacy:
1)Wywolujemy funkcje F_BEGIN("NAZWA",ILOSC_PARAMETROW). Ilosc parametrow jest oganiczona do 8. Nazwa moze zawierac cyfry tylko na koncu.
2)Od teraz kolejne wyrazenia beda stanowily ciag instrukcji naszej funkcji. Mozemy rownierz uzywac specjalnych dyrektyw, kazda zaczyna sie od znaku #:
#IF <wyrazenie> - jesli <wyrazenie> jest rozne od 0 to wykonuje ciag instrukcji az do napotkania #ENDIF, #ELSE albo #ELSEIF <wyrazenie>.
#WHILE <wyrazenie> - wykonuje instrukcje az do #ENDWHILE jesli <wyrazenie> jest rozne od 0.
#REP <wyrazenie> - wykonuje instrukcje az do #ENDREP <wyrazenie> razy. Istotne jest to ze wartosc wyrazenia jest obliczana tylko na wejsciu do petli.
#ELSE, #ELSEIF uzywamy tylko w blokach #IF ... #ENDIF
#ELSEIF to polaczenie #IF ... #ELSE #IF ... #ELSE
3)Wartosc zwracana to wartosc ostatniego wyrazenia jakie zostanie wykonane w funkcji
4)Definicje funkcji konczymy funkcja F_END()

Argumenty przekazywane do funkcji to kolejno zmienne _1, _2, ..., _8.
UWAGA: przy przypisywaniu zmiennej wartosci uzywamy konstrukcji @zmienna:=wartosc

Przykladowy program:
//************************
//oblicza pierwiastek stopnia podanego w parametrze nr 2
//deklaracje zmiennych pomocniczych
f_var('pom')
//poczatek programu
f_begin("sqrtn",2)
#if _1>=0
 @pom:=_1
//wykonaj operacje 10 razy
 #rep 10
  @pom:=(1/_2)*((_2-1)*pom+_1/pom^(_2-1))
 #endrep
//zwroc wartosc pom
 pom
#else
//zwroc 0
 0
#endif
f_end()
//usun zmienna pom
f_del("pom")
//wywolanie funkcji
sqrtn(x,2)
//************************