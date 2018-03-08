package dzikiZachod.Strategie;

import dzikiZachod.Akcje.Akcja;
import dzikiZachod.Akcje.WykonanieAkcji;
import dzikiZachod.Gra;

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


public abstract class Strategia extends StrategiaGracza {

    int czyUleczyc () {
        if (mojStan.getReka().ileUlecz() > 0)
            if (mojStan.getPunktyZycia() < mojStan.limitPunktowZycia) return mojStan.getIndeks();
        return StrategiaGracza.anulowanieAkcji;
    }

    abstract int czyStrzelic ();

    abstract int czyRzucicDynamit ();

    List <Integer> osobyWZasieguPozaSzeryfem () {
        List <Integer> cele = zywiGraczeWZasieguPozaMna();
        cele = cele.stream()
                .filter(x -> x != Gra.getPozycjaSzeryfa())
                .collect(Collectors.toList());
        return cele;
    }

    @Override
    public WykonanieAkcji wykonajRuch () {
        int out = czyUleczyc();
        if (out != anulowanieAkcji) return new WykonanieAkcji(Akcja.ULECZ, out);
        if (mojStan.getReka().ileZasiegDwa() > 0) return new WykonanieAkcji(Akcja.ZASIEG_PLUS_DWA, mojStan.getIndeks());
        if (mojStan.getReka().ileZasiegJeden() > 0)
            return new WykonanieAkcji(Akcja.ZASIEG_PLUS_JEDEN, mojStan.getIndeks());
        out = czyStrzelic();
        if (out != anulowanieAkcji) return new WykonanieAkcji(Akcja.STRZEL, out);
        out = czyRzucicDynamit();
        if (out != anulowanieAkcji) return new WykonanieAkcji(Akcja.DYNAMIT, mojStan.getIndeks());
        return new WykonanieAkcji(Akcja.ULECZ, anulowanieAkcji);
    }
}
