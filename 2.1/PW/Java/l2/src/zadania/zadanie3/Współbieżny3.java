package zadania.zadanie3;

import zadania.IntBox;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.CyclicBarrier;

public class Współbieżny3 {

    private static final int N_WIERSZY = 10;
    private static final int N_KOLUMN = 100;

    private static List<List<IntBox>> tab = Collections.synchronizedList(new ArrayList<List<IntBox>>());
    private static final CountDownLatch zatrzask[] =
            new CountDownLatch[N_WIERSZY];

    private static int ileZrobiono = 0;

    public static void main(final String[] args) {

        for (int w = 0; w < N_WIERSZY; ++w) {
            zatrzask[w] = new CountDownLatch(N_KOLUMN);
        }

        new Thread(new Runnable() {
            @Override
            public void run() {
                for (int i = 0; i < N_WIERSZY; ++i) {
                    try {
                        zatrzask[i].await();
                        int suma = 0;
                        for (int k = 0; k < N_KOLUMN; ++k) {
                            suma += tab.get(k).get(ileZrobiono).getValue();
                        }
                        ++ileZrobiono;
                        System.out.println(ileZrobiono + " " + suma);
                    } catch (InterruptedException e) {
                        Thread.currentThread().interrupt();
                        System.err.println("Wykonanie przerwane");
                    }
                }
            }
        }).start();

        for (int k = 0; k < N_KOLUMN; ++k) {
            tab.add(Collections.synchronizedList(new ArrayList<IntBox>()));
            for (int w = 0; w < N_WIERSZY; ++w) tab.get(k).add(new IntBox());
        }
        for (int k = 0; k < N_KOLUMN; ++k) {
            new Thread(new Sumowator3(k, tab.get(k), zatrzask)).start();
        }
    }

}
