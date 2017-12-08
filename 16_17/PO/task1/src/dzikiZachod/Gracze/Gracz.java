package dzikiZachod.Gracze;

import dzikiZachod.Akcje.WykonanieAkcji;
import dzikiZachod.Gra;
import dzikiZachod.Powiadomienia.ObserwatorGry;
import dzikiZachod.Powiadomienia.Powiadomienie;
import dzikiZachod.Strategie.Strategia;
import dzikiZachod.Widoki.WidokGraczy;

/**
 * **  **   *   *   ***   *   *   **  **
 * *                                   *
 * *    Created by Michał Balcerzak    *
 * *    Copyright by Wydział MIM UW    *
 * *                                   *
 * **  **   *   *   ***   *   *   **  **
 **/


public abstract class Gracz extends ObserwatorGry {

    private final Strategia strategiaGracza;

    public final int limitPunktowZycia;

    private StanGracza stanGracza;

    static int generujLosowyLimitPunktowZycia () {
        int ziarno = Gra.losuj.nextInt(1 + Gra.gornyZakresPunktowZycia - Gra.dolnyZakresPunktowZycia);
        return ziarno + Gra.dolnyZakresPunktowZycia;
    }

    Gracz (Strategia strategia, int limitZycia) {
        strategiaGracza = strategia;
        limitPunktowZycia = limitZycia;
    }

    public void inicjalizujGracza (int indeks, WidokGraczy widok) {
        stanGracza = new StanGracza(indeks, limitPunktowZycia);
        strategiaGracza.inicjalizuj(stanGracza, widok);
    }


    public Reka getReka () {
        return stanGracza.getReka();
    }

    public int getPunktyZycia () {
        return stanGracza.getPunktyZycia();
    }

    public int getIndeks () {
        return stanGracza.getIndeks();
    }

    public int getZasieg () {
        return stanGracza.getZasieg();
    }
    public boolean czySzeryf () {
        return false;
    }

    public boolean czyBandyta () {
        return false;
    }

    public boolean czyWGrze () {
        return stanGracza.czyWGrze();
    }

    public void otrzymajLeczenie () {
        stanGracza.otrzymajLeczenie(limitPunktowZycia);
    }

    public void otrzymajZwiekszenieZasiegu () {
        stanGracza.otrzymajZwiekszenieZasiegu();
    }

    public boolean otrzymajObrazenia () {
        return stanGracza.otrzymajObrazenia();
    }

    public WykonanieAkcji wykonajRuch () {
        return strategiaGracza.wykonajRuch();
    }

    @Override
    public void powiadom (Powiadomienie p) {
        strategiaGracza.powiadom(p);
    }
}
