package dzikiZachod.Strategie;

import dzikiZachod.Gra;

/**
 * **  **   *   *   ***   *   *   **  **
 * *                                   *
 * *    Created by Michał Balcerzak    *
 * *    Copyright by Wydział MIM UW    *
 * *                                   *
 * **  **   *   *   ***   *   *   **  **
 **/


public abstract class StrategiaBandyty extends Strategia {

    private static final int ileLudziMiedzyMnaASzeryfemDoWypuszczeniaDynamitu = 3;

    @Override
    int czyStrzelic () {
        return Strategia.anulowanieAkcji;
    }

    int strzelDoSzeryfaGdyWZasiegu () {
        if (mojStan.getReka().ileStrzel() > 0) {
            if (widok.odlegloscMiedzyGraczami(mojStan.getIndeks(), Gra.getPozycjaSzeryfa()) <= mojStan.getZasieg()) {
                return Gra.getPozycjaSzeryfa();
            }
        }
        return StrategiaGracza.anulowanieAkcji;
    }

    @Override
    int czyRzucicDynamit () {
        if (mojStan.getReka().ileDynamit() > 0) {
            int ileOsobMiedzyMnaASzeryfem = widok.odlegloscWPrawo(mojStan.getIndeks(), Gra.getPozycjaSzeryfa()) - 1;
            if (ileOsobMiedzyMnaASzeryfem < ileLudziMiedzyMnaASzeryfemDoWypuszczeniaDynamitu) {
                return mojStan.getIndeks();
            }
        }
        return Strategia.anulowanieAkcji;
    }
}
