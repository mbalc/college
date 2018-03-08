package dzikiZachod.Strategie;

import java.util.List;
import java.util.stream.Collectors;

/**
 * **  **   *   *   ***   *   *   **  **
 * *                                   *
 * *    Created by Michał Balcerzak    *
 * *    Copyright by Wydział MIM UW    *
 * *                                   *
 * **  **   *   *   ***   *   *   **  **
 **/


public class StrategiaPomocnikaSzeryfaZliczajaca extends StrategiaPomocnikaSzeryfa{

    @Override
    int czyStrzelic () {
        if (mojStan.getReka().ileStrzel() > 0) {
            List <Integer> cele = zywiGraczeWZasieguPozaMna();
            List <Integer> strzelajacyDoSzeryfa = cele.stream()
                    .filter(x -> czyStrzelilDoSzeryfa[x])
                    .collect(Collectors.toList());
            if (strzelajacyDoSzeryfa.size() > 0) return losujCelZListyIndeksow(cele);
            List <Integer> zabojcyPomocnikow = cele.stream()
                    .filter(x -> bilansOfiar[x] > 1)
                    .collect(Collectors.toList());
            if (zabojcyPomocnikow.size() > 0) return losujCelZListyIndeksow(zabojcyPomocnikow);
        }
        return Strategia.anulowanieAkcji;
    }

}
