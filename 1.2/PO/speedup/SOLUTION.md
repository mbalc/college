Algorytm Floyda to prosty algorytm grafowy o złożoności czasowej T(n) = O(n^3) pozwalający dla każdej pary wierzchołków w skierowanym grafie bez ujemnych cykli znaleźć długość najkrótszej ścieżki między nimi. Zainteresowanych odsyłam [tutaj](http://www.algorytm.org/algorytmy-grafowe/algorytm-floyda.html)

**Porównywany język:** C++

Środowisko: Windows 10 Home, version 1607, build 14393.222, 64-bit

Platforma testowa: specyfikacja dostępna [tutaj](https://www.x-kom.pl/p/160835-notebook-laptop-156-lenovo-z510-i5-4200m-8gb-1000-dvd-rw-gt740m-braz.html#specification)

Wersja java: javac 1.8.0_121

**Wersja c++:** g++.exe (tdm64-1) 5.1.0

**Procedura testowa:** do testów wykorzystałem środowisko PowerShell (Get-Host Version: 5.1.14393.206, 64bit). Podczas testowania starałem się zachować minimalne, jednorodne obciążenie komputera innymi procesami.   

Załączony skrypt "test.ps1" wypisuje czasy wykonania poszczególnych testów w sekundach za każdym z dwudziestu wywołań. Pozostałe skrypty służą do ustawiania zmiennych środowiskowych odpowiednio dla każdego z rozważanych przypadków testowych (różniących się językiem i flagami kompilacji/wywoływania). 

Do zgrania obydwu skryptów wykorzystałem procedury Begin i Process: przykładowo do przetestowania przypadku "c++A" wywołuję:   

```
Begin
    { . .\\c++A.ps1 }
Process
    { .\\test.ps1 }
```

To, co skrypt dokładnie wypisał na moim komputerze zamieściłem w plikach *_out.txt, w tabelce zamieściłem natomiast mediany z 20 wyników czasowych dla każdego testu (obliczone za pomocą [narzędzia on-line](http://www.alcula.com/calculators/statistics/median/)). 

**Testy:** Rozwiązania testowałem za pomocą pięciu testów o różnych parametrach wygenerowanych za pomocą załączonego generatora testów. Dokładne wartości parametrów są zawarte w skrypcie generującym. 

Ostatni, największy z testów nie został załączony w przesłanej paczce ze względu na limity rozmiarów plików narzucane przez Moodle. Zachęcam do wygenerowania go za pomocą załączonego generatora testów - powinien być deterministyczny i wygenerować identyczny plik (skrypt ten na wejściu oczekuje jednej liczby z zakresu 1..5). W razie potrzeby - test ten zamieszczam też [tutaj](https://drive.google.com/open?id=0B2rLmnUCrfaOakFaeUxOUUdCcWM). 

Krótka charakterystyka testów: 

-grafy zawarte w testasch o numerach 3 i 4 mają tę samą liczbę wierzchołków, różnią się natomiast gęstością - graf z test4.in jest znacznie gęstszy  
-pozostałe testy różnią się przede wszystkim liczbą wierzchołków - tym większą, im większy numer testu. 

  

Czasy (mediana z 20 wyników czasowych w sekundach):

|          |  1          |  2          |  3          |  4          | 5           |
|----------|-------------|-------------|-------------|-------------|-------------|
|c++ A     |  0.01114285 | 0.08772715  | 1.61224755  | 1.61001195  | 7.1012841   |
|c++ B     |  0.01023535 | 0.04129515  | 0.3985631   | 0.40535765  | 1.5886691   |
|java A    |  0.10716065 | 0.2845064   | 1.02733735  | 0.977343    | 3.6443019   |
|java B    |  0.10087105 | 1.63531665  | 23.476578   | 24.18081465 | 92.45682425 |
    


Główne różnice między przypadkami testowymi:

  

c++ A:    `-O0`

c++ B:    `-O2`

  

java A:    bez flag wykonania

java B:    `-Xint -Xprof`
