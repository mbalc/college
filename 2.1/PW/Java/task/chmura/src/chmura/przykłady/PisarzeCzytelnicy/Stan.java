package chmura.przykłady.PisarzeCzytelnicy;

import chmura.Byt;
import chmura.Chmura;

import java.util.Collections;

class Stan {
    static int iluPisze = 0, iluCzyta = 0, czekaCzyt = 0, czekaPis = 0;

    static Chmura chmura;
    static Byt mutex, czytelnicy, pisarze;

    Stan() throws InterruptedException {
        chmura = new Chmura((x, y) -> y < 0 || y > 2 || x < 0);
        mutex = chmura.ustaw(1, 2);
        czytelnicy = chmura.ustaw(0, 1);
        pisarze = chmura.ustaw(0, 0);
    }

    static private void metoda(Byt b, int przes) {
        try {
            chmura.przestaw(Collections.singletonList(b), przes, 0);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        ;
    }

    static void P(Byt b) {
        metoda(b, -1);
    }

    static void V(Byt b) {
        metoda(b, 1);
    }
}
