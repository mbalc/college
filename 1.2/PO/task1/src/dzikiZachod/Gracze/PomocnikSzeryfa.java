package dzikiZachod.Gracze;

import dzikiZachod.Strategie.StrategiaPomocnikaSzeryfa;
import dzikiZachod.Strategie.StrategiaPomocnikaSzeryfaDomyslna;

/**
 * **  **   *   *   ***   *   *   **  **
 * *                                   *
 * *    Created by Michał Balcerzak    *
 * *    Copyright by Wydział MIM UW    *
 * *                                   *
 * **  **   *   *   ***   *   *   **  **
 **/


public class PomocnikSzeryfa extends Gracz {

    public PomocnikSzeryfa (StrategiaPomocnikaSzeryfa strategia) {
        super(strategia, generujLosowyLimitPunktowZycia());
    }

    public PomocnikSzeryfa () {
        this(new StrategiaPomocnikaSzeryfaDomyslna());
    }
}
