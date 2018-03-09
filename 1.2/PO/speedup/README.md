Szybkość programów w Javie
==========================

Proszę o zamieszczanie na tym forum wyników Państwa eksperymentów polegających na porównaniu szybkości programów w Javie i drugim, dowolnie wybranym języku. Na forum należy wpisać - każdy w nowym, osobnym wątku- następujące informacje:

*   nazwę porównywanego języka (np. _C++_),
*   środowisko (system operacyjny, Windows/Linux/..., wersja systemu, 32/64bity - np. pod Linuxem wynik polecenia _uname -mrs,_ który może wyglądać tak: _Linux 3.7.10-8 x86_64_),
*   wersję Javy (javac -version), ewentualne opcje kompilacji/wykonania (np. _javac:  1.7.0_45_),
*   wersję implementacji drugiego języka programowania (o ile dostępna),  ewentualne opcje kompilacji/wykonania (np. dla c++ wynik polecenia _c++ --version_, który może wyglądać tak: _c++ (PLD-Linux) 4.8.2 20131104 (release)_ ),
*   czas działania programu w Javie w ms (np. _Java: 500ms_),
*   czas działania programu w drugim języku w ms (np. _C++: 500ms_),
*   ewentualne uwagi.

Ponadto proszę o wgranie kodu źródłowego obu programów i (jeśli dotyczy) danych (jako spakowanego archiwum). Chodzi o możliwość odtworzenia wyników Państwa eksperymentu.

Proszę również  o przeprowadzenia eksperymentu z wyłączeniem kompilacji w JVMie. W standardowej JVM (zarówno pod Windows jak i pod Linuxem na students) działają następujące opcje:

*   `-X`   wyświetla dostępne opcje
*   `-Xprof`  wyświetla m.in. dane o tym czy/co zostało skopilowane
*   `-Xint` wymusza tryb pracy JVM bez kompilowania bytecodu

Zatem proszę przetestować swój program w Javie zarówno tak:

```java Program```

jak i tak:

```java -Xint -Xprof Program```

Algorytm nie musi być wyrafinowany. Na ile to możliwe należy zadbać o uruchomienie obu programów w wersji zoptymalizowanej czasowo i dodatkowo w Javie z `-Xint`. Zarówno algorytmy, jak i języki programowania, mogą się powtarzać (choć oczywiście zachęcamy do wybierania jak najbardziej różnorodnych).

Można podać kilka wyników (np. ten sam algorytm dla różnych rozmiarów danych). 

Po wpisaniu się na forum należy powiadomić prowadzącego zajęcia w grupie laboratoryjnej (żeby mógł dodać punkt), najwygodniej to zrobić podczas zajęć.

Termin oddawania (liczy się termin wpisania na forum, prowadzącemu wpisanie można zgłosić dużo później - podczas kolejnych zajęć): 22 IV 2017, 23:55.
