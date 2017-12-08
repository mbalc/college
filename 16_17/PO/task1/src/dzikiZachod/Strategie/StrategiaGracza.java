package dzikiZachod.Strategie;

import dzikiZachod.Akcje.WykonanieAkcji;
import dzikiZachod.Gra;
import dzikiZachod.Gracze.StanGracza;
import dzikiZachod.Powiadomienia.ObserwatorGry;
import dzikiZachod.Widoki.WidokGraczy;

import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

/**
 * **  **   *   *   ***   *   *   **  **
 * *                                   *
 * *    Created by Michał Balcerzak    *
 * *    Copyright by Wydział MIM UW    *
 * *                                   *
 * **  **   *   *   ***   *   *   **  **
 **/


public abstract class StrategiaGracza extends ObserwatorGry {

    public static final int anulowanieAkcji = -1;

    WidokGraczy widok;
    StanGracza mojStan;

    public void inicjalizuj (StanGracza stan, WidokGraczy w) {
        this.mojStan = stan;
        this.widok = w;
    }

    int losujCelZListyIndeksow (List<Integer>lista) {
        if (mojStan.getReka().ileStrzel() < 1 || lista.size() < 1) return Strategia.anulowanieAkcji;
        return lista.get(Gra.losuj.nextInt(lista.size()));
    }

    @SuppressWarnings ("WeakerAccess")
    List <Integer> graczeWZasiegu () {
        return IntStream.rangeClosed(0, widok.ileGraczy() - 1)
                .filter(x -> widok.odlegloscMiedzyGraczami(mojStan.getIndeks(), x) <= mojStan.getZasieg())
                .boxed().collect(Collectors.toList());
    }

    @SuppressWarnings ("WeakerAccess")
    List <Integer> graczeWZasieguPozaMna () {
        return graczeWZasiegu().stream()
                .filter(x -> x != mojStan.getIndeks())
                .collect(Collectors.toList());
    }

    @SuppressWarnings ("unused")
    List <Integer> zywiGraczeWZasiegu () {
        return graczeWZasiegu().stream()
                .filter(x -> widok.czyGraczWGrze(x))
                .collect(Collectors.toList());
    }

    List <Integer> zywiGraczeWZasieguPozaMna () {
        return graczeWZasieguPozaMna().stream()
                .filter(x -> widok.czyGraczWGrze(x))
                .collect(Collectors.toList());
    }

    @SuppressWarnings ("unused")
    public abstract WykonanieAkcji wykonajRuch ();

}