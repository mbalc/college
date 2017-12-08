package dzikiZachod.Powiadomienia;

import java.util.ArrayList;
import java.util.List;

/**
 * **  **   *   *   ***   *   *   **  **
 * *                                   *
 * *    Created by Michał Balcerzak    *
 * *    Copyright by Wydział MIM UW    *
 * *                                   *
 * **  **   *   *   ***   *   *   **  **
 **/


public class Informator {

    private final List <ObserwatorGry> obserwujacy;

    public Informator () {
        obserwujacy = new ArrayList <>();
    }

    public void dodajObserwujacego (ObserwatorGry o) {
        obserwujacy.add(o);
    }

    public void powiadom (Powiadomienie p) {
        for (ObserwatorGry o : obserwujacy) {
            o.powiadom(p);
        }
    }

}
