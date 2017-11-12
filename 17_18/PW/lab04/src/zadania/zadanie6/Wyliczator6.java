package zadania.zadanie6;

import java.util.List;
import java.util.concurrent.*;

public class Wyliczator6 implements Runnable {

    List<Future<Integer>> wyniki;

    Integer ID_WIERSZA;

    public Wyliczator6(Integer idWiersza, List<Future<Integer>> wyniki) {
        this.wyniki = wyniki;
        this.ID_WIERSZA = idWiersza;
    }

    @Override
    public void run() {
        try {
            int suma = 0;
            for (Future<Integer> wynik : wyniki) {
                suma += wynik.get();
            }
            System.out.println((ID_WIERSZA+1) + " " + suma);
        } catch (InterruptedException | ExecutionException e) {
            Thread.currentThread().interrupt();
            System.err.println("Wykonanie przerwane");
        }
    }

}
