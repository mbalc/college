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


public class StrategiaBandytyDomyslna extends StrategiaBandyty {

    @Override
    int czyStrzelic () {
        if (mojStan.getReka().ileStrzel() > 0) {
            int out = strzelDoSzeryfaGdyWZasiegu();
            if (out != Strategia.anulowanieAkcji) return out;
            List <Integer> cele = zywiGraczeWZasieguPozaMna().stream()
                    .filter(x -> !widok.czyGraczNaPewnoBandyta(x)) //nie-bandyci
                    .collect(Collectors.toList());
            return losujCelZListyIndeksow(cele);
        }
        return Strategia.anulowanieAkcji;
    }


}
