package zadania.zadanie4;

import zadania.IntBox;
import zadania.Stałe;

import java.util.concurrent.BlockingQueue;

public class Wyliczator4 implements Runnable {


    private static BlockingQueue<IntBox> queues[];

    public Wyliczator4(BlockingQueue<IntBox> queues[]) {
        this.queues = queues;
    }

    @Override
    public void run() {
        for (int i = 0; i < Stałe.N_WIERSZY; ++i) {
            try {
                int suma = 0;
                for (int k = 0; k < Stałe.N_KOLUMN; ++k) {
                    suma += queues[i].take().getValue();
                }
                System.out.println((i+1) + " " + suma);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                System.err.println("Wykonanie przerwane");
            }
        }
    }

}
