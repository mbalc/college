package zadania.zadanie4;

import zadania.IntBox;
import zadania.Macierz;
import zadania.Stałe;

import java.util.concurrent.BlockingQueue;

public class Sumowator4 implements Runnable {

    private static BlockingQueue<IntBox> queues[];
    private int ID_KOLUMNY;

    Sumowator4(int ID_KOLUMNY, BlockingQueue<IntBox> queues[]) {
        this.ID_KOLUMNY = ID_KOLUMNY;
        this.queues = queues;
    }

    @Override
    public void run() {
        try {
            for (int w = 0; w < Stałe.N_WIERSZY; ++w) {
                queues[w].put(new IntBox(Macierz.wartość(w, ID_KOLUMNY)));
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            System.err.println("Wykonanie przerwane");
        }
    }


}
