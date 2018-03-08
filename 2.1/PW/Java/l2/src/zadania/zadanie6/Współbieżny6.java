package zadania.zadanie6;

import zadania.IntBox;
import zadania.Stałe;
import zadania.zadanie4.Sumowator4;
import zadania.zadanie4.Wyliczator4;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.*;

public class Współbieżny6 {


    private static ConcurrentMap<Integer, BlockingQueue<IntBox>> queues = new ConcurrentHashMap<>();
    private static final ExecutorService pulaWątków = Executors.newFixedThreadPool(Stałe.N_WĄTKÓW);

    public static void main(final String[] args) {

        for (int w = 0; w < Stałe.N_WIERSZY; ++w) {
            List<Future<Integer>> wyniki = new ArrayList<>();
            for (int k = 0; k < Stałe.N_KOLUMN; ++k) {
                wyniki.add(pulaWątków.submit(new LiczMacierz(w, k)));
            }
            new Thread(new Wyliczator6(w, wyniki)).start();
        }
    }

}
