package dzikiZachod.Akcje;

import java.util.LinkedList;
import java.util.List;

/**
 * **  **   *   *   ***   *   *   **  **
 * *                                   *
 * *    Created by Michał Balcerzak    *
 * *    Copyright by Wydział MIM UW    *
 * *                                   *
 * **  **   *   *   ***   *   *   **  **
 **/


public class PulaAkcji {

    private final List <Akcja> zbiorKart;

    public PulaAkcji () {
        zbiorKart = new LinkedList <>();
    }

    public void dodaj (Akcja typ, int liczba) {
        for (int i = 0; i < liczba; ++i) {
            zbiorKart.add(typ);
        }
    }

    public TaliaAkcji dajTalieAkcji () {
        return new TaliaAkcji(zbiorKart);
    }

    @SuppressWarnings ("unused")
    public int size () {
        return zbiorKart.size();
    }
}
