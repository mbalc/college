package dzikiZachod.Gracze;

import dzikiZachod.Gra;

/**
 * **  **   *   *   ***   *   *   **  **
 * *                                   *
 * *    Created by Michał Balcerzak    *
 * *    Copyright by Wydział MIM UW    *
 * *                                   *
 * **  **   *   *   ***   *   *   **  **
 **/


public class StanGracza {

    private int punktyZycia;
    public final int limitPunktowZycia;

    private int zasieg;

    private boolean wGrze;

    private final int indeks;

    private final Reka reka;


    public int getPunktyZycia () {
        return punktyZycia;
    }

    public int getIndeks () {
        return indeks;
    }

    public Reka getReka () {
        return reka;
    }

    public int getZasieg () {
        return zasieg;
    }

    public boolean czyWGrze () {
        return wGrze;
    }

    StanGracza (int i, int limitZycia) {
        punktyZycia = limitZycia;
        limitPunktowZycia = limitZycia;
        wGrze = true;
        indeks = i;
        reka = new Reka();
        zasieg = Gra.poczatkowyZasiegGracza;
    }

    void otrzymajLeczenie (int limit) {
        if (punktyZycia + 1 <= limit) {
            ++punktyZycia;
        }
    }

    boolean otrzymajObrazenia () {
        --punktyZycia;
        if (punktyZycia <= 0) {
            wGrze = false;
            return true;
        }
        return false;
    }

    void otrzymajZwiekszenieZasiegu () {
        zasieg += 1;
    }
}
