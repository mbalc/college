package zadania.zadanie4;

import zadania.IntBox;
import zadania.Stałe;

import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

public class Współbieżny4 {


    private static BlockingQueue<IntBox> queues[] = new BlockingQueue[Stałe.N_WIERSZY];


    public static void main(final String[] args) {

        for (int w = 0; w < Stałe.N_WIERSZY; ++w) {
            queues[w] = new LinkedBlockingQueue<IntBox>(Stałe.N_KOLUMN + 10);
        }


        new Thread(new Wyliczator4(queues)).start();

        for (int k = 0; k < Stałe.N_KOLUMN; ++k) {
            new Thread(new Sumowator4(k, queues)).start();
        }
    }

}
