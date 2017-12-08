package dzikiZachod.Strategie;

import dzikiZachod.Gra;
import dzikiZachod.Gracze.StanGracza;
import dzikiZachod.Powiadomienia.Powiadomienie;
import dzikiZachod.Powiadomienia.Wydarzenie;
import dzikiZachod.Widoki.WidokGraczy;

import java.util.Collections;
import java.util.Comparator;
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


public class StrategiaBandytySprytna extends StrategiaBandyty {

    private boolean czyZabilemWTejTurzeBandyte;

    @Override
    public void inicjalizuj (StanGracza stan, WidokGraczy w) {
        super.inicjalizuj(stan, w);
        czyZabilemWTejTurzeBandyte = false;
    }

    @Override
    int czyStrzelic () {
        if (mojStan.getReka().ileStrzel() > 0) {
            int out = strzelDoSzeryfaGdyWZasiegu();
            if (out != Strategia.anulowanieAkcji) return out;

            if (!czyZabilemWTejTurzeBandyte) {
                List <Integer> bandyciKtorychMogeZabic = zywiGraczeWZasieguPozaMna().stream()
                        .filter(x -> widok.getPunktyZycia(x) <= mojStan.getReka().ileStrzel())
                        .filter(x -> widok.czyGraczNaPewnoBandyta(x))
                        .collect(Collectors.toList());
                Collections.shuffle(bandyciKtorychMogeZabic, Gra.losuj);
                bandyciKtorychMogeZabic.sort(Comparator.comparingInt(a -> widok.getPunktyZycia(a)));
            }

            List <Integer> cele = zywiGraczeWZasieguPozaMna().stream()
                    .filter(x -> !widok.czyGraczNaPewnoBandyta(x)) //nie-bandyci
                    .collect(Collectors.toList());
            return losujCelZListyIndeksow(cele);
        }
        return Strategia.anulowanieAkcji;
    }

    @Override
    public void powiadom (Powiadomienie p) {
        super.powiadom(p);
        if (p.getRodzajPowiadomienia() == Wydarzenie.POCZATEK_RUCHOW) {
            if (mojStan.getIndeks() == p.getWykonawca()) {
                czyZabilemWTejTurzeBandyte = false;
            }
        }
        if (p.getRodzajPowiadomienia() == Wydarzenie.ZABOJSTWO) {
            if (mojStan.getIndeks() == p.getWykonawca()) {
                if (widok.czyGraczNaPewnoBandyta(p.getOdbiorca())) {
                    czyZabilemWTejTurzeBandyte = true;
                }
            }
        }
    }
}
