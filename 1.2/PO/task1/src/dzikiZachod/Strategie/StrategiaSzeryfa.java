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


public abstract class StrategiaSzeryfa extends Strategia {

    boolean czyStrzelilDoSzeryfa[];

    @Override
    public void inicjalizuj (StanGracza stan, WidokGraczy w) {
        super.inicjalizuj(stan, w);
        czyStrzelilDoSzeryfa = new boolean [w.ileGraczy()];
        Arrays.fill(czyStrzelilDoSzeryfa, false);
    }

    @Override
    int czyRzucicDynamit () {
        return Strategia.anulowanieAkcji;
    }


    @Override
    public void powiadom (Powiadomienie p) {
        if (p.getRodzajPowiadomienia() == Wydarzenie.STRZAL) {
            if (p.getOdbiorca() == Gra.getPozycjaSzeryfa()) {
                czyStrzelilDoSzeryfa[p.getWykonawca()] = true;
            }
        }
    }
}
