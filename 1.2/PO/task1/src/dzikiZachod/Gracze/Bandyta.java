package dzikiZachod.Gracze;

import dzikiZachod.Strategie.StrategiaBandyty;
import dzikiZachod.Strategie.StrategiaBandytyDomyslna;

/**
 * **  **   *   *   ***   *   *   **  **
 * *                                   *
 * *    Created by Michał Balcerzak    *
 * *    Copyright by Wydział MIM UW    *
 * *                                   *
 * **  **   *   *   ***   *   *   **  **
 **/


public class Bandyta extends Gracz implements Cloneable {

    @Override
    public boolean czyBandyta () {
        return true;
    }

    public Bandyta (StrategiaBandyty strategia) {
        super(strategia, generujLosowyLimitPunktowZycia());
    }

    public Bandyta () {
        this(new StrategiaBandytyDomyslna());
    }

}
