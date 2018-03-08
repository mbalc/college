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


public class StrategiaSzeryfaDomyslna extends StrategiaSzeryfa {

    @Override
    int czyStrzelic () {
        if (mojStan.getReka().ileStrzel() > 0) {
            List <Integer> cele = zywiGraczeWZasieguPozaMna();
            List <Integer> atakujacyMnie = cele.stream()
                    .filter(x -> czyStrzelilDoSzeryfa[x])
                    .collect(Collectors.toList());
            if (atakujacyMnie.size() > 0) {
                return losujCelZListyIndeksow(atakujacyMnie);
            }
            return losujCelZListyIndeksow(cele);
        }
        return Strategia.anulowanieAkcji;
    }

}
