package dzikiZachod.Strategie;

import dzikiZachod.Gra;
import dzikiZachod.Gracze.StanGracza;
import dzikiZachod.Powiadomienia.Powiadomienie;
import dzikiZachod.Powiadomienia.Wydarzenie;
import dzikiZachod.Widoki.WidokGraczy;

import java.util.Arrays;

/**
 * **  **   *   *   ***   *   *   **  **
 * *                                   *
 * *    Created by Michał Balcerzak    *
 * *    Copyright by Wydział MIM UW    *
 * *                                   *
 * **  **   *   *   ***   *   *   **  **
 **/


public abstract class StrategiaPomocnikaSzeryfa extends Strategia {


    int bilansOfiar[]; //+1 dla zabojcy pomocnika, -1 dla zabojcy bandyty
    boolean czyStrzelilDoSzeryfa[];

    private static final int ileLudziMiedzyMnaASzeryfemDoWypuszczeniaDynamitu = 3;
    private static final double czescBandytowDoWypuszczeniaDynamitu = 2. / 3.;

    @Override
    public void inicjalizuj (StanGracza stan, WidokGraczy w) {
        super.inicjalizuj(stan, w);
        bilansOfiar = new int[w.ileGraczy()];
        Arrays.fill(bilansOfiar, 0);
        czyStrzelilDoSzeryfa = new boolean[w.ileGraczy()];
        Arrays.fill(czyStrzelilDoSzeryfa, false);
    }

    @Override
    int czyUleczyc () {
        if (mojStan.getReka().ileUlecz() > 0) {
            if (widok.getPunktyZycia(Gra.getPozycjaSzeryfa()) < Gra.liczbaPunktowZyciaSzeryfa) {
                if (widok.odlegloscMiedzyGraczami(mojStan.getIndeks(), Gra.getPozycjaSzeryfa()) <= Gra.zasiegLeczenia) {
                    return Gra.getPozycjaSzeryfa();
                }
            }
        }
        return super.czyUleczyc();
    }

    @Override
    int czyRzucicDynamit () {
        if (mojStan.getReka().ileDynamit() > 0) {
            int ileOsobMiedzyMnaASzeryfem = widok.odlegloscWPrawo(mojStan.getIndeks(), Gra.getPozycjaSzeryfa());
            if (ileOsobMiedzyMnaASzeryfem > ileLudziMiedzyMnaASzeryfemDoWypuszczeniaDynamitu) {
                int liczbaPodejrzanychOBycieBandyta = 0;
                for (int i = (mojStan.getIndeks() + 1) % widok.ileGraczy(); i != Gra.getPozycjaSzeryfa(); i = (i + 1) % widok.ileGraczy()) {
                    if (widok.czyGraczWGrze(i) && (czyStrzelilDoSzeryfa[i] || bilansOfiar[i] > 0)) {
                        ++liczbaPodejrzanychOBycieBandyta;
                    }
                }
                if ((double)liczbaPodejrzanychOBycieBandyta / (double)ileOsobMiedzyMnaASzeryfem >= czescBandytowDoWypuszczeniaDynamitu) {
                    return mojStan.getIndeks();
                }
            }
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
        if (p.getRodzajPowiadomienia() == Wydarzenie.STRZAL) {
            if (p.getOdbiorca() == Gra.getPozycjaSzeryfa()) {
                czyStrzelilDoSzeryfa[p.getWykonawca()] = true;
            }
        }
        super.powiadom(p);
    }


}
