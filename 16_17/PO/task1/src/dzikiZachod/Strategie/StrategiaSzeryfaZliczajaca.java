package dzikiZachod.Strategie;

import dzikiZachod.Gracze.StanGracza;
import dzikiZachod.Powiadomienia.Powiadomienie;
import dzikiZachod.Powiadomienia.Wydarzenie;
import dzikiZachod.Widoki.WidokGraczy;

import java.util.Arrays;
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


public class StrategiaSzeryfaZliczajaca extends StrategiaSzeryfa {


    private int bilansOfiar[]; //+1 dla zabojcy pomocnika, -1 dla zabojcy bandyty

    @Override
    public void inicjalizuj (StanGracza stan, WidokGraczy w) {
        super.inicjalizuj(stan, w);
        bilansOfiar = new int[w.ileGraczy()];
        Arrays.fill(bilansOfiar, 0);
    }

    @Override
    int czyStrzelic () {
        if (mojStan.getReka().ileStrzel() > 0) {
            List<Integer> cele = zywiGraczeWZasieguPozaMna();
            List <Integer> strzelajacyDoSzeryfa = cele.stream()
                    .filter(x -> czyStrzelilDoSzeryfa[x])
                    .collect(Collectors.toList());
            if (strzelajacyDoSzeryfa.size() > 0) return losujCelZListyIndeksow(cele);
            List<Integer> zabojcyPomocnikow = cele.stream()
                    .filter(x -> bilansOfiar[x] > 1)
                    .collect(Collectors.toList());
            if (zabojcyPomocnikow.size() > 0) return losujCelZListyIndeksow(zabojcyPomocnikow);
        }
        return Strategia.anulowanieAkcji;
    }

    @Override
    public void powiadom (Powiadomienie p) {
        if (p.getRodzajPowiadomienia() == Wydarzenie.ZABOJSTWO) {
            if (widok.czyGraczNaPewnoBandyta(p.getOdbiorca())) {
                --bilansOfiar[p.getWykonawca()];
            } else {
                ++bilansOfiar[p.getWykonawca()];
            }
        }
        super.powiadom(p);
    }
}
