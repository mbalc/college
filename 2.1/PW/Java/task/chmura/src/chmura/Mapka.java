package chmura;

import java.util.Collection;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;
import java.util.function.BiPredicate;

class Mapka {

    private ConcurrentMap<Byt, Miejsce> przypisania;
    private BiPredicate<Integer, Integer> predykat;

    Mapka(BiPredicate<Integer, Integer> stan) {
        this.przypisania = new ConcurrentHashMap<>();
        this.predykat = stan;
    }

    private void zapewnijIstnienie(Byt b) throws NiebytException {
        if (!przypisania.containsKey(b)) throw new NiebytException();
    }

    void kasuj(Byt b) throws NiebytException {
        zapewnijIstnienie(b);
        przypisania.remove(b);
    }

    Miejsce dajWspolrzedne(Byt b) throws NiebytException {
        zapewnijIstnienie(b);
        return przypisania.get(b);
    }

    boolean czyZajeteMiejsce(Miejsce obj) {
        return przypisania.containsValue(obj) || obj.sprawdź(predykat);
    }

    boolean czyMoznaPrzestawic(Collection<Byt> byty, int dx, int dy) throws NiebytException {
        for (Byt byt : byty) {
            Miejsce m = dajWspolrzedne(byt); // zapewnia istnienie
            Miejsce cel = m.przesuń(dx, dy);
            if (czyZajeteMiejsce(cel) && !cel.equals(m)) return false; // zawsze pozwalamy na zerowe przesuniecie
        }
        return true;
    }

    Byt ustaw(Miejsce docelowe) {
        Byt byt = new Byt();
        przypisania.put(byt, docelowe);
        return byt;
    }

    void przestaw(Collection<Byt> byty, int dx, int dy) {
        for (Byt byt : byty) {
            Miejsce m = dajWspolrzedne(byt);
            przypisania.replace(byt, m.przesuń(dx, dy));
        }
    }
}
