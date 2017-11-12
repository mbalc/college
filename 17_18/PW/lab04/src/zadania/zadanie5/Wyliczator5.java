package zadania.zadanie5;

import zadania.IntBox;
import zadania.Stałe;

import java.util.concurrent.BlockingQueue;
import java.util.concurrent.ConcurrentMap;
import java.util.concurrent.LinkedBlockingQueue;

public class Wyliczator5 implements Runnable {

    private static ConcurrentMap<Integer, BlockingQueue<IntBox>> queues;

    public Wyliczator5(ConcurrentMap<Integer, BlockingQueue<IntBox>> queues) {
        this.queues = queues;
    }

    @Override
    public void run() {
        for (int i = 0; i < Stałe.N_WIERSZY; ++i) {
            try {
                queues.putIfAbsent(i, new LinkedBlockingQueue<IntBox>());
                final BlockingQueue<IntBox> queue = queues.get(i);

                int suma = 0;
                for (int k = 0; k < Stałe.N_KOLUMN; ++k) {
                    suma += queue.take().getValue();
                }
                queues.remove(i);
                System.out.println((i+1) + " " + suma);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                System.err.println("Wykonanie przerwane");
            }
        }
    }

}
