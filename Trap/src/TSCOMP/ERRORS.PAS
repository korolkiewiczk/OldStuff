const
maxerror = 14;
errors : array [0..maxerror-1] of pchar =
('Procedura poza dyrektyw¥ /script',
 'Deklaracja zmiennej poza dyrektyw¥ /var',
 'Etykieta poza dyrektyw¥ /script',
 'Niewˆa˜ciwy operator',
 'Brak operatora "="',
 'Niewˆa˜ciwy operator',
 'Niepoprawna liczba',
 'Deklaracja skryptu w dyrektywie /var lub w dyrektywie /script',
 'Deklaracja zmiennych w dyrektywie /script',
 'Nieznana zmienna lub etykieta',
 'Wymagany operator /endscript',
 'Wymagany operator /script',
 'Wymagany operator /var',
 'Wymagane " na koäcu');