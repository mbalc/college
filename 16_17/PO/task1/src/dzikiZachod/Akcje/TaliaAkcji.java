package dzikiZachod.Akcje;

import dzikiZachod.Gra;

import java.util.Collections;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;

/**
 * **  **   *   *   ***   *   *   **  **
 * *                                   *
 * *    Created by Michał Balcerzak    *
 * *    Copyright by Wydział MIM UW    *
 * *                                   *
 * **  **   *   *   ***   *   *   **  **
 **/


public class TaliaAkcji {


    private final Queue <Akcja> pula;
    private final List <Akcja> doPotasowania;

    TaliaAkcji (List <Akcja> noweKarty) {
        doPotasowania = new LinkedList <>(noweKarty);
        pula = new LinkedList <>();
        potasuj();
    }

    private void potasuj () {
        Collections.shuffle(doPotasowania, Gra.losuj);
        pula.addAll(doPotasowania);
        doPotasowania.clear();
    }

    public Akcja dajAkcje () throws IllegalStateException {
        if (pula.isEmpty()) {
            potasuj();
        }
        if (pula.isEmpty()) {
            throw new IllegalStateException("W trakcie gry zabrakło kart akcji!");
        }
        return pula.poll();
    }

    public void dodaj (Akcja typ) {
        doPotasowania.add(typ);
    }

    public void dodaj (Akcja typ, int liczba) {
        for (int i = 0; i < liczba; ++i) {
            dodaj(typ);
        }
    }

}
