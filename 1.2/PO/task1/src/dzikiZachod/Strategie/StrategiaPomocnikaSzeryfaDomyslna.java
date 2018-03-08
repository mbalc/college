package dzikiZachod.Strategie;


/**
 * **  **   *   *   ***   *   *   **  **
 * *                                   *
 * *    Created by Michał Balcerzak    *
 * *    Copyright by Wydział MIM UW    *
 * *                                   *
 * **  **   *   *   ***   *   *   **  **
 **/


public class StrategiaPomocnikaSzeryfaDomyslna extends StrategiaPomocnikaSzeryfa {

    @Override
    int czyStrzelic () {
        if (mojStan.getReka().ileStrzel() > 0) {
            return losujCelZListyIndeksow(osobyWZasieguPozaSzeryfem());
        }
        return Strategia.anulowanieAkcji;
    }
}
