PO 1. zadanie laboratoryjne

WERSJA: 1.03

Historia zmian:

*   10.05.2017 (wer. 1.01): Dopisanie informacji o stosowaniu tablic lub kolekcji oraz dopisanie przykładowego kodu tworzącego listę graczy z tablicą.
*   16.05.2017 (wer. 1.02): Uściślenie sprytnej strategii bandytów, poprawka błędu w opisie rzucania dynamitu przez pomocników szeryfa
*   16.05.2017 (wer: 1.03): Uściślenie reguł leczenia, aktualizacja komunikatu o wybuchu dynamitu

W miasteczku na Dzikim Zachodzie znajdują się trzy typy osób:

*   szeryf,
*   pomocnicy szeryfa,
*   bandyci.

Szeryf\- Celem szeryfa jest zastrzelenie bandytów. Wszyscy wiedzą, kim jest szeryf.

Pomocnicy szeryfa \- starają się pomóc szeryfowi, jednak nie znają swoich tożsamości, ani tożsamości innych za wyjątkiem szeryfa.

Bandyci \- chcą zabić szeryfa. Bandyci znają tożsamości pozostałych bandytów.

Rozgrywka toczy się w turach. Gracze są ustawieni w okrąg (kolejność jest losowana na początku rozgrywki). W czasie tury wszyscy gracze po kolei, zaczynając od szeryfa, losują kartyakcji z puli i wykonują odpowiadające im działania. W ramach swojej tury gracz losuje zestaw pięciu kart akcji. W puli znajdują się karty następujących rodzajów:

*   Uleczyć jedno życie,
*   Strzelić,
*   Zwiększyć zasięg,
*   Rzucić dynamit (w puli może być tylko jedna karta dynamitu. Po jej użyciu, karta dynamitu nie wraca do puli).

Skład puli jest ustalany przy tworzeniu rozgrywki (por. przykładowy kod). Jeśli akcje w puli się skończą, pula jest tasowana ponownie z wyłączeniem dynamitu (dynamit nie wraca do puli po wylosowaniu przez gracza).

Wykonując strzał gracz odejmuje innemu jeden punkt życia. Początkowo gracze mają zasięg 1, co oznacza, że mogą atakować tylko sąsiednich graczy. Jeśli otrzymają możliwość zwiększenia zasięgu, mogą atakować dalszych graczy.

Uleczenie życia pozwala zwiększyć swoje punkty zdrowia o 1 lub uleczyć sąsiada (również o jeden punkt). Nie można zwiększyć liczby punktów zdrowia powyżej początkowej wartości.

Dynamit powoduje pojawienie się na stole przed kolejnym graczem dynamitu. W każdej turze gracz, przed którym leży dynamit, rzuca kostką zanim wykona pierwszy ruch. Jeśli wyrzuci 1, to dynamit wybucha i zadaje mu 3 punkty obrażenia. W przeciwnym przypadku dynamit przechodzi do kolejnej osoby.

Jeśli ze względu na swoją strategię, lub brak możliwości zastosowania gracz nie wykorzysta akcji zostają mu na one na kolejną turę. W takiej sytuacji na początku tury losuje tylko brakujące akcje.

Gracz ginie, gdy liczba jego punktów życia wyniesie 0. Gdy dany gracz ginie,jego tożsamość jest pokazywana wszystkimgraczom. Zabity gracz jest usuwany z rozgrywki, więc nie wlicza się do zasięgu. Karty zabitego gracza wracają do puli. Gracz ginie natychmiast po otrzymaniu decydującego strzału. Jeśli tym graczem jest Szeryf lub ostatni z Bandytów, gra jest automatycznie kończona. Jeśli po 42 turach gra nadal nie jest rozstrzygnięta to rozgrywka jest zakończona remisem.

Wszyscy gracze mają 3 lub 4 punkty życia (wartość jest ustalana losowo przy tworzeniu gracza), za wyjątkiem szeryfa, który ma 5 punktów życia. Gracze wiedzą o aktualnych wartościach punktów życia wszystkichgraczy.

Celem zadania jest napisanie programu rozgrywającego grę z graczami o podanych strategiach działania. Strategia gracza ustalana jest przy tworzeniu nowego gracza. Skład graczy ustalany jest przy tworzeniu rozgrywki.

Strategie graczy

Każdy gracz:

*   wykorzystuje możliwość zwiększenia zasięgu
*   korzysta leczy siebie jeśli może. Jeśli gracz jest pomocnikiem szeryfa leczy najpierw szeryfa, o ile to potrzebne

Strategie szeryfa

*   (domyślna) atakuje losową osobę z puli osób w zasięgu, jeśli nikt do niego nie strzelił, lub losową z tych, które do niego strzeliły
*   (zliczająca) atakuje tylko te osoby, które go zaatakowały lub zabiły więcej pomocników szeryfa niż bandytów

Strategie bandytów

*   (domyślna)Atakują szeryfa lub losową niebędącą bandytą osobę między nimi a szeryfem celem skrócenia dystansu, o ile taka istnieje.
*   (cierpliwa) Czekają aż szeryf znajdzie się w ich zasięgu i wtedy go atakują
*   (sprytna) Jeśli mają możliwość zrobienia tego w jednej turze, zabijają najpierw jednego ze swoich (wkupując się w łaski graczy, którzy zwracają na to uwagę[\[1\]](#ftnt1)) a potem działają zgodnie z domyślną strategią. Jeśli w ich zasięgu jest szeryf, strzelają do szeryfa (nie atakują innych graczy).

*   Jeśli między bandytą, a szeryfem jest mniej niż 3 osoby (licząc zgodnie z kierunkiem przebiegu tury) i ma taką możliwość, wypuszcza dynamit

Strategie pomocników

*   (domyślna)Atakują losowe osoby poza szeryfem
*   (zliczająca) Atakują osoby, które strzeliły do szeryfa lub zabiły więcej pomocników szeryfa niż bandytów

*   Jeśli między pomocnikiem, a szeryfem jest więcej niż 3 osoby (licząc zgodnie z kierunkiem przebiegu tury), z czego co najmniej dwie trzecie jest podejrzewane o bycie bandytą (zgodnie ze strategią zliczającą), wypuszczają dynamit, o ile mogą

Gracze po otrzymaniu z puli kart akcji, rozpatrują je w następującej kolejności:

1.  Leczenie,
2.  Zwiększenie zasięgu,
3.  Strzał,
4.  Dynamit.

Polecenie
=========

Przygotuj rozwiązanie zawierające pakiet dzikizachod, w którego skład wchodzą klasy:

Gra

PulaAkcji

Gracz (A)

  Szeryf

  Bandyta

  PomocnikSzeryfa

Strategia (A)

  StrategiaSzeryfa (A)

    StrategiaSzeryfaDomyslna

    StrategiaSzeryfaZliczajaca

  StrategiaPomocnikaSzeryfa (A)

    StrategiaPomocnikaSzeryfaDomyslna

    StrategiaPomocnikaSzeryfaZliczajaca

  StrategiaBandyty (A)

    StrategiaBandytyDomyslna

    StrategiaBandytyCierpliwa

    StrategiaBandytySprytna

oraz typ wyliczeniowy

*   Akcja o wartościach: ULECZ, STRZEL, ZASIEG\_PLUS\_JEDEN, ZASIEG\_PLUS\_DWA, DYNAMIT        

Pula akcji powinna implementować metodę dodaj(Akcja typ, int liczba), dodającą do puli określoną liczbę akcji danego typu.

Gracze powinni implementować dwa konstruktory: jeden bez parametrów tworzący gracza o domyślnej strategii i drugi, przyjmujący jako parametr klasę strategii.

Tworzenie puli graczy może być zrealizowane zarówno przy pomocy tablic jak i kolekcji (przedstawiamy dwie wersje przykładowego kodu).

Poniższy kod powinien przeprowadzić przykładową rozgrywkę (z 1 szeryfem, 2 pomocnikami i 3 bandytami posługującymi się domyślnymi strategiami):

// Wersja z listą:

List<Gracz> gracze = new ArrayList<Gracz>();

gracze.add(new Szeryf());

for(int i=0;i<2;i++) gracze.add(new PomocnikSzeryfa());

for(int i=0;i<3;i++) gracze.add(new Bandyta());

// Wersja z tablicą:

Gracz gracze\[\] = {new Szeryf(), new PomocnikSzeryfa(), new PomocnikSzeryfa(), new Bandyta(), new Bandyta(), new Bandyta())};

// Kod wspólny dla obu wersji:

PulaAkcji pulaAkcji = new PulaAkcji();

pulaAkcji.dodaj(Akcja.ULECZ, 20);

pulaAkcji.dodaj(Akcja.STRZEL, 60);

pulaAkcji.dodaj(Akcja.ZASIEG\_PLUS\_JEDEN, 3);

pulaAkcji.dodaj(Akcja.ZASIEG\_PLUS\_DWA, 1);

pulaAkcji.dodaj(Akcja.DYNAMIT, 1);

Gra gra = new Gra();

gra.rozgrywka(gracze, pulaAkcji);

Uwaga! Przy ocenianiu zadanie będzie testowane z różnymi konfiguracjami pul akcji, składu graczy i ich strategii.

Do śledzenia rozgrywki na standardowym wyjściu należy wypisywać komunikaty pokazujące przebieg rozgrywki. Pełny opis komunikatów znajduje się poniżej. Przykładowa rozgrywka::

\*\* START

  Gracze:

    1: Pomocnik Szeryfa (liczba żyć: 4)

    2: Bandyta (liczba żyć: 3)

    3: Bandyta (liczba żyć: 4)

    4: Szeryf (liczba żyć: 5)

    5: Bandyta (liczba żyć: 3)

    6: Pomocnik Szeryfa (liczba żyć: 3)

\*\* TURA 1

  GRACZ 4 (Szeryf):

    Akcje: \[STRZEL, STRZEL, STRZEL, STRZEL, STRZEL\]

    Ruchy:

      STRZEL 3

      STRZEL 5

      STRZEL 5

      STRZEL 3

      STRZEL 5

  Gracze:

    1: Pomocnik Szeryfa (liczba żyć: 4)

    2: Bandyta (liczba żyć: 3)

    3: Bandyta (liczba żyć: 2)

    4: Szeryf (liczba żyć: 5)

    5: X (Bandyta)

    6: Pomocnik Szeryfa (liczba żyć: 3)

GRACZ 5 (Bandyta):

    MARTWY

GRACZ 6 (Pomocnik Szeryfa):

    Akcje: \[STRZEL, STRZEL, ULECZ, ULECZ, STRZEL\]

    Ruchy:

      STRZEL 1

      STRZEL 1

      STRZEL 1

Gracze:

  1: Pomocnik Szeryfa (liczba żyć: 1)

  2: Bandyta (liczba żyć: 3)

  3: Bandyta (liczba żyć: 2)

  4: Szeryf (liczba żyć: 5)

  5: X (Bandyta)

  6: Pomocnik Szeryfa (liczba żyć: 3)

GRACZ 1 (Pomocnik Szeryfa):

    Akcje: \[STRZEL, STRZEL, ULECZ, ULECZ, STRZEL\]

    Ruchy:

      ULECZ

      ULECZ

      STRZEL 6

      STRZEL 2

      STRZEL 6

Gracze:

  1: Pomocnik Szeryfa (liczba żyć: 3)

  2: Bandyta (liczba żyć: 2)

  3: Bandyta (liczba żyć: 2)

  4: Szeryf (liczba żyć: 5)

  5: X (Bandyta)

  6: Pomocnik Szeryfa (liczba żyć: 1)

GRACZ 2 (Bandyta):

    Akcje: \[STRZEL, STRZEL, STRZEL, ULECZ, ULECZ\]

    Ruchy:

      ULECZ

      STRZEL 1

      STRZEL 1

      STRZEL 1

Gracze:

  1: X (Pomocnik Szeryfa)

  2: Bandyta (liczba żyć: 3)

  3: Bandyta (liczba żyć: 2)

  4: Szeryf (liczba żyć: 5)

  5: X (Bandyta)

  6: Pomocnik Szeryfa (liczba żyć: 1)

GRACZ 3 (Bandyta):

    Akcje: \[STRZEL, STRZEL, STRZEL, ULECZ, ULECZ\]

    Ruchy:

      ULECZ

      ULECZ

      STRZEL 4

      STRZEL 4

      STRZEL 4

Gracze:

  1: X (Pomocnik Szeryfa)

  2: Bandyta (liczba żyć: 3)

  3: Bandyta (liczba żyć: 4)

  4: Szeryf (liczba żyć: 2)

  5: X (Bandyta)

  6: Pomocnik Szeryfa (liczba żyć: 1)

\*\* TURA 2

  GRACZ 4 (Szeryf):

    Akcje: \[STRZEL, STRZEL, ULECZ, STRZEL, STRZEL\]

    Ruchy:

      ULECZ

      STRZEL 3

      STRZEL 3

      STRZEL 3

      STRZEL 3

  Gracze:

    1: X (Pomocnik Szeryfa)

    2: Bandyta (liczba żyć: 3)

    3: X (Bandyta)

    4: Szeryf (liczba żyć: 3)

    5: X (Bandyta)

    6: Pomocnik Szeryfa (liczba żyć: 1)

GRACZ 5 (Bandyta):

    MARTWY

GRACZ 6 (Pomocnik Szeryfa):

    Akcje: \[ULECZ, ULECZ, STRZEL, STRZEL, STRZEL\]

    Ruchy:

      ULECZ 4

      ULECZ 4

      STRZEL 2

      STRZEL 2

      STRZEL 2

  Gracze:

    1: X (Pomocnik Szeryfa)

    2: X (Bandyta)

    3: X (Bandyta)

    4: Szeryf (liczba żyć: 5)

    5: X (Bandyta)

    6: Pomocnik Szeryfa (liczba żyć: 1)

\*\* KONIEC

  WYGRANA STRONA: szeryf i pomocnicy

Specyfikacja komunikatów
========================

W ramach przeprowadzania rozgrywki program powinien drukować komunikaty opisujące kolejne etapy rozgrywki. Rozróżniamy trzy typy etapów: początek, tury i zakończenie.

Opis każdego etapu zaczyna się odpowiednim nagłówkiem.

Treści części składowych komunikatów powinny mieć wcięcie na dwie spacje względem swoich nagłówków.

Nagłówki:

*   \*\* START - nagłówek początku
*   \*\* TURA <numer_tury> - nagłówek tury. Tury numerowane są od 1.
*   \*\* KONIEC - nagłówek zakończenia

Format początku:

 <lista_graczy>

Format listy graczy:

*   Gracze: - nagłówek listy graczy
*   <numer_porządkowy>: <rola> (liczba żyć: <liczba_żyć>) - opis żywego gracza. Pozycje graczy numerujemy od 1
*   <numer_porządkowy>: X (<rola>) \- opis zabitego gracza. Pozycje graczy numerujemy od 1

Format roli gracza:

 Szeryf, Pomocnik Szeryfa, Bandyta

Format tury:

W ramach tury opisujemy przebieg tury kolejnych graczy według schematu

GRACZ <numer_porządkowy> (<rola>):

  Akcje: <lista\_akcji\_po_uzupełnieniu>

  Ruchy:

    <lista ruchów>

<lista_graczy>

lub

GRACZ <numer_porządkowy> (<rola>):

  MARTWY

Jeśli gracz został zabity.

Jeśli przed graczem leży dynamit dodatkowo w opisie gracza przed ruchami pojawia się nowe pole

GRACZ <numer_porządkowy> (<rola>):

  Akcje: <lista\_akcji\_po_uzupełnieniu>

  Dynamit: <WYBUCHŁ lub PRZECHODZI DALEJ>

  Ruchy:

    <lista ruchów>

Jeśli wybuch zabił gracza, komunikat przyjmuje formę:

GRACZ <numer_porządkowy> (<rola>):

  Akcje: <lista\_akcji\_po_uzupełnieniu>

  Dynamit: WYBUCHŁ

  Ruchy:

    MARTWY

Format listy akcji:

\[<nazwa\_akcji\_1>, <nazwa\_akcji\_2>, <nazwa\_akcji\_3>, <nazwa\_akcji\_4>, <nazwa\_akcji\_5>\]

Format ruchów:

<nazwa_akcji> <cel (opcjonalnie)>

Nazwy akcji:

 ULECZ, STRZEL, ZASIEG\_PLUS\_JEDEN, ZASIEG\_PLUS\_DWA, DYNAMIT

Format zakończenia:

 WYGRANA STRONA: <szeryf i pomocnicy lub bandyci>

lub

  REMIS - OSIĄGNIĘTO LIMIT TUR

* * *

[\[1\]](#ftnt_ref1) Strategia jest skuteczna w przypadku, gdy między bandytą a Szeryfem znajduje się inny bandyta i pomocnik. W tej sytuacji bandyta po zabiciu drugiego bandyty i pomocnika ma czyste konto, oszukując strategie zliczające.
