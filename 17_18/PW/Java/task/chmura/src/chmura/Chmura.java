package chmura;

import java.util.Collection;
import java.util.function.BiPredicate;


public class Chmura {

    private final Mapka uniwersum;

    public Chmura(BiPredicate<Integer, Integer> stan) {
        uniwersum = new Mapka(stan);
    }

    public Chmura() {
        this((a, b) -> false);
    }

    public Byt ustaw(int x, int y) throws InterruptedException {
        Miejsce docelowe = new Miejsce(x, y);
        synchronized (uniwersum) {
            while (uniwersum.czyZajeteMiejsce(docelowe)) {
                uniwersum.wait();
            }
            return uniwersum.ustaw(docelowe);
        }
    }

    public void przestaw(Collection<Byt> byty, int dx, int dy) throws NiebytException, InterruptedException {
        synchronized (uniwersum) {
            while (!uniwersum.czyMoznaPrzestawic(byty, dx, dy)) {
                uniwersum.wait();
            }
            uniwersum.przestaw(byty, dx, dy);
            uniwersum.notifyAll();
        }
    }

    public void kasuj(Byt byt) throws NiebytException {
        synchronized (uniwersum) {
            uniwersum.kasuj(byt);
            uniwersum.notifyAll();
        }
    }

    public int[] miejsce(Byt byt) {
        try {
            return uniwersum.dajWspolrzedne(byt).dajNormal();
        } catch (NiebytException e) {
           return null;
        }
    }
}
