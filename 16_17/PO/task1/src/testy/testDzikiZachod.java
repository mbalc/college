package testy;

import dzikiZachod.Akcje.Akcja;
import dzikiZachod.Akcje.PulaAkcji;
import dzikiZachod.Gra;
import dzikiZachod.Gracze.Bandyta;
import dzikiZachod.Gracze.Gracz;
import dzikiZachod.Gracze.PomocnikSzeryfa;
import dzikiZachod.Gracze.Szeryf;
import dzikiZachod.Strategie.StrategiaBandytyCierpliwa;
import dzikiZachod.Strategie.StrategiaBandytySprytna;
import dzikiZachod.Strategie.StrategiaPomocnikaSzeryfaZliczajaca;
import dzikiZachod.Strategie.StrategiaSzeryfaZliczajaca;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;

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


public class testDzikiZachod {
    @SuppressWarnings ("EmptyMethod")
    @Before
    public void setUp () throws Exception {
    }

    @SuppressWarnings ("EmptyMethod")
    @After
    public void tearDown () throws Exception {
    }

    @Test
    public void testDomyslny () {
        List <Gracz> gracze = new ArrayList <>();
        gracze.add(new Szeryf());
        for (int i = 0; i < 2; i++) gracze.add(new PomocnikSzeryfa());
        for (int i = 0; i < 3; i++) gracze.add(new Bandyta());

        PulaAkcji pulaAkcji = new PulaAkcji();
        pulaAkcji.dodaj(Akcja.ULECZ, 30);
        pulaAkcji.dodaj(Akcja.STRZEL, 60);
        pulaAkcji.dodaj(Akcja.ZASIEG_PLUS_JEDEN, 3);
        pulaAkcji.dodaj(Akcja.ZASIEG_PLUS_DWA, 1);
        pulaAkcji.dodaj(Akcja.DYNAMIT, 1);

        Gra gra = new Gra();
        gra.rozgrywka(gracze, pulaAkcji);
    }

    @Test
    public void testDuzoRozgrywek () {
        List <Gracz> gracze = new ArrayList <>();
        gracze.add(new Szeryf());
        gracze.add(new Bandyta());

        PulaAkcji pulaAkcji = new PulaAkcji();
        pulaAkcji.dodaj(Akcja.ULECZ, 30);
        pulaAkcji.dodaj(Akcja.STRZEL, 30);
        pulaAkcji.dodaj(Akcja.ZASIEG_PLUS_JEDEN, 10);
        pulaAkcji.dodaj(Akcja.ZASIEG_PLUS_DWA, 10);
        pulaAkcji.dodaj(Akcja.DYNAMIT, 1);

        Gra gra = new Gra();
        for (int i = 0; i < 9; ++i) gra.rozgrywka(gracze, pulaAkcji);
    }

    @Test
    public void testRoznorodnychStrategii () {
        List <Gracz> gracze = new ArrayList <>();
        gracze.add(new Szeryf(new StrategiaSzeryfaZliczajaca()));
        gracze.add(new Bandyta());
        gracze.add(new PomocnikSzeryfa());
        gracze.add(new Bandyta(new StrategiaBandytySprytna()));
        gracze.add(new PomocnikSzeryfa(new StrategiaPomocnikaSzeryfaZliczajaca()));
        gracze.add(new Bandyta(new StrategiaBandytyCierpliwa()));

        PulaAkcji pulaAkcji = new PulaAkcji();
        pulaAkcji.dodaj(Akcja.ULECZ, 30);
        pulaAkcji.dodaj(Akcja.STRZEL, 60);
        pulaAkcji.dodaj(Akcja.DYNAMIT, 1);

        Gra gra = new Gra();
        gra.rozgrywka(gracze, pulaAkcji);
    }
}
