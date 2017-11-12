package zadania.zadanie5;

import zadania.IntBox;
import zadania.Stałe;
import zadania.zadanie4.Sumowator4;
import zadania.zadanie4.Wyliczator4;

import java.util.concurrent.BlockingQueue;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentMap;
import java.util.concurrent.LinkedBlockingQueue;

public class Współbieżny5 {


    private static ConcurrentMap<Integer, BlockingQueue<IntBox>> queues = new ConcurrentHashMap<>();


    public static void main(final String[] args) {

        for (int k = 0; k < Stałe.N_KOLUMN; ++k) {
            new Thread(new Sumowator5(k, queues)).start();
        }

        new Thread(new Wyliczator5(queues)).start();
    }

}
