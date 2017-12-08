package dzikiZachod.Gracze;

import dzikiZachod.Akcje.Akcja;
import dzikiZachod.Gra;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

/**
 * **  **   *   *   ***   *   *   **  **
 * *                                   *
 * *    Created by Michał Balcerzak    *
 * *    Copyright by Wydział MIM UW    *
 * *                                   *
 * **  **   *   *   ***   *   *   **  **
 **/


public class Reka {

    private final List <Akcja> karty;
    private final int limitKart;

    Reka () {
        karty = new ArrayList <>();
        limitKart = Gra.limitKartWRece;
    }

    public int size () {
        return karty.size();
    }

    public int ileUlecz () {
        return karty.stream()
                .filter(k -> k == Akcja.ULECZ)
                .collect(Collectors.toList()).size();
    }

    public int ileStrzel () {
        return karty.stream()
                .filter(k -> k == Akcja.STRZEL)
                .collect(Collectors.toList()).size();
    }

    public int ileZasiegJeden () {
        return karty.stream()
                .filter(k -> k == Akcja.ZASIEG_PLUS_JEDEN)
                .collect(Collectors.toList()).size();
    }

    public int ileZasiegDwa () {
        return karty.stream()
                .filter(k -> k == Akcja.ZASIEG_PLUS_DWA)
                .collect(Collectors.toList()).size();
    }

    public int ileDynamit () {
        return karty.stream()
                .filter(k -> k == Akcja.DYNAMIT)
                .collect(Collectors.toList()).size();
    }

    @Override
    public String toString () {
        StringBuilder outBuilder = new StringBuilder("");
        if (size() > 0) {
            outBuilder.append(karty.get(0).toString());
            for (int i = 1; i < size(); ++i) {
                outBuilder.append(", ").append(karty.get(i).toString());
            }
        }
        return outBuilder.toString();
    }

    public void zwrocAkcje (Akcja a) throws IllegalArgumentException {
        if (!karty.remove(a)) throw new IllegalArgumentException("Nie posiadasz takiej karty akcji!");
    }

    public void przyjmijKarte (Akcja a) {
        if (size() < limitKart) karty.add(a);
    }

    public void odrzucWszystkieKarty () {
        karty.clear();
    }
}
