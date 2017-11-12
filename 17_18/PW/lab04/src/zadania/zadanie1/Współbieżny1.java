package zadania.zadanie1;

import zadania.IntBox;

import java.util.concurrent.CyclicBarrier;

public class Współbieżny1 {

    private static final int N_KOLUMN = 100;

    private static IntBox tab[] = new IntBox[N_KOLUMN];
    private static int ileZrobiono = 0;

    private static final CyclicBarrier bariera = new CyclicBarrier(N_KOLUMN, new Runnable() {
        @Override
        public void run() {
            int suma = 0;
            for (int k = 0; k < N_KOLUMN; ++k) {
                suma += tab[k].getValue();
            }
            ++ileZrobiono;
            System.out.println(ileZrobiono + " " + suma);
        }
    });

    public static void main(final String[] args) {
        for (int k = 0; k < N_KOLUMN; ++k) {
            tab[k] = new IntBox();
            new Thread(new Sumowator1(k, bariera, tab[k])).start();

        }
    }

}
