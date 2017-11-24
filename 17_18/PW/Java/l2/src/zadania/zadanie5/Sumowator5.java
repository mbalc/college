package zadania.zadanie5;

import zadania.IntBox;
import zadania.Macierz;
import zadania.Stałe;

import java.util.concurrent.BlockingQueue;
import java.util.concurrent.ConcurrentMap;
import java.util.concurrent.LinkedBlockingQueue;

public class Sumowator5 implements Runnable {

    private static ConcurrentMap<Integer, BlockingQueue<IntBox>> queues;
    private int ID_KOLUMNY;

    Sumowator5(int ID_KOLUMNY, ConcurrentMap<Integer, BlockingQueue<IntBox>> queues) {
        this.ID_KOLUMNY = ID_KOLUMNY;
        this.queues = queues;

    }

    @Override
    public void run() {
        try {
            for (int w = 0; w < Stałe.N_WIERSZY; ++w) {
                queues.putIfAbsent(w, new LinkedBlockingQueue<IntBox>());
                final BlockingQueue<IntBox> queue = queues.get(w);
                queue.put(new IntBox(Macierz.wartość(w, ID_KOLUMNY)));
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            System.err.println("Wykonanie przerwane");
        }
    }


}
