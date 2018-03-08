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


public class WidokGraczy {
    @SuppressWarnings ("WeakerAccess")
    final List <Gracz> gracze;
    @SuppressWarnings ("WeakerAccess")
    final List <Integer> odlegloscOdPierwszegoGracza;

    WidokGraczy (List <Gracz> g, List <Integer> odl) {
        gracze = g;
        odlegloscOdPierwszegoGracza = odl;
    }

    public int odlegloscWPrawo (int g1, int g2) {
        List <Integer> tabOdl = odlegloscOdPierwszegoGracza;
        if (g1 <= g2) {
            return tabOdl.get(g2) - tabOdl.get(g1) + (gracze.get(g1).czyWGrze() ? 0 : -1);
        }
        int out = tabOdl.get(tabOdl.size() - 1) - tabOdl.get(g1) + tabOdl.get(g2) + (gracze.get(g1).czyWGrze() ? 0 : -1);
        if (gracze.get(0).czyWGrze() || gracze.get(tabOdl.size() - 1).czyWGrze()) out += 1;
        return out;
    }

    public int odlegloscMiedzyGraczami (int g1, int g2) {
        int o1 = odlegloscWPrawo(g1, g2);
        int o2 = odlegloscWPrawo(g2, g1);
        if (o1 < o2) return o1;
        return o2;
    }

    public boolean czyGraczNaPewnoBandyta (int indeks) {
        return gracze.get(indeks).czyBandyta();
    }

    public boolean czyGraczWGrze (int indeks) {
        return gracze.get(indeks).czyWGrze();
    }

    public int getPunktyZycia (int indeks) {
        return gracze.get(indeks).getPunktyZycia();
    }

    public int ileGraczy () {
        return gracze.size();
    }

}
