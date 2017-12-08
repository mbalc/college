package dzikiZachod.Widoki;

import dzikiZachod.Gracze.Gracz;

import java.util.List;

/**
 * **  **   *   *   ***   *   *   **  **
 * *                                   *
 * *    Created by Michał Balcerzak    *
 * *    Copyright by Wydział MIM UW    *
 * *                                   *
 * **  **   *   *   ***   *   *   **  **
 **/


public class WidokSzeryfa extends WidokGraczy {


    public WidokSzeryfa(List<Gracz> g, List<Integer> odl) {
        super(g, odl);
    }

    @Override
    public boolean czyGraczNaPewnoBandyta (int indeks) {
        if (!czyGraczWGrze(indeks)) super.czyGraczNaPewnoBandyta(indeks);
        return false;
    }

}
