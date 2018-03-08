package zadania.zadanie2;

import zadania.IntBox;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.CyclicBarrier;

public class Współbieżny2 {

    private static final int N_KOLUMN = 100;

    private static List<IntBox> tab = Collections.synchronizedList(new ArrayList<IntBox>());
    private static int ileZrobiono = 0;

    private static final CyclicBarrier bariera = new CyclicBarrier(N_KOLUMN, new Runnable() {
        @Override
        public void run() {
            int suma = 0;
            for (int k = 0; k < N_KOLUMN; ++k) {
                suma += tab.get(k).getValue();
            }
            ++ileZrobiono;
            System.out.println(ileZrobiono + " " + suma);
        }
    });

    public static void main(final String[] args) {
        for (int k = 0; k < N_KOLUMN; ++k) {
            tab.add(new IntBox());
            new Thread(new Sumowator2(k, bariera, tab.get(k))).start();

        }
    }

}
