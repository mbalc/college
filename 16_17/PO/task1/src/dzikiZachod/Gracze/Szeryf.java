package dzikiZachod.Gracze;

import dzikiZachod.Gra;
import dzikiZachod.Strategie.StrategiaSzeryfa;
import dzikiZachod.Strategie.StrategiaSzeryfaDomyslna;

/**
 * **  **   *   *   ***   *   *   **  **
 * *                                   *
 * *    Created by Michał Balcerzak    *
 * *    Copyright by Wydział MIM UW    *
 * *                                   *
 * **  **   *   *   ***   *   *   **  **
 **/


public class Szeryf extends Gracz {

    @Override
    public boolean czySzeryf () {
        return true;
    }

    public Szeryf (StrategiaSzeryfa strategia) {
        super(strategia, Gra.liczbaPunktowZyciaSzeryfa);
    }

    public Szeryf () {
        this(new StrategiaSzeryfaDomyslna());
    }
}
