package zadania.zadanie1;

import zadania.IntBox;
import zadania.Macierz;

import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;

public class Sumowator1 implements Runnable {

    private static final int N_WIERSZY = 10;
    private CyclicBarrier bariera;
    private int ID_KOLUMNY;

    private IntBox komorka;

    Sumowator1(int ID_KOLUMNY, CyclicBarrier bariera, IntBox komorka) {
        this.ID_KOLUMNY = ID_KOLUMNY;
        this.bariera = bariera;
        this.komorka = komorka;

    }

    @Override
    public void run() {
        try {
            for (int w = 0; w < N_WIERSZY; ++w) {
                komorka.setValue(Macierz.wartość(w, ID_KOLUMNY));
                bariera.await();
            }
        } catch (InterruptedException | BrokenBarrierException e) {
            Thread.currentThread().interrupt();
            System.err.println("Wątek " + ID_KOLUMNY + " przerwany");
        }
    }


}
