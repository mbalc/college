package dzikiZachod.Strategie;

/**
 * **  **   *   *   ***   *   *   **  **
 * *                                   *
 * *    Created by Michał Balcerzak    *
 * *    Copyright by Wydział MIM UW    *
 * *                                   *
 * **  **   *   *   ***   *   *   **  **
 **/


public class StrategiaBandytyCierpliwa extends StrategiaBandyty {

    @Override
    int czyStrzelic () {
        if (mojStan.getReka().ileStrzel() > 0) {
            return strzelDoSzeryfaGdyWZasiegu();
        }
        return Strategia.anulowanieAkcji;
    }

}
