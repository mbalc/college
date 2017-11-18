package zadania.zadanie3;

import zadania.IntBox;
import zadania.Macierz;

import java.util.List;
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.CyclicBarrier;

public class Sumowator3 implements Runnable {

    private static final int N_WIERSZY = 10;
    private CountDownLatch zatrzask[];
    private int ID_KOLUMNY;

    private List<IntBox> wartosciWWierszu;

    Sumowator3(int ID_KOLUMNY, List<IntBox> wartosciWWierszu, CountDownLatch zatrzask[]) {
        this.ID_KOLUMNY = ID_KOLUMNY;
        this.wartosciWWierszu = wartosciWWierszu;
        this.zatrzask = zatrzask;

    }

    @Override
    public void run() {

        for (int w = 0; w < N_WIERSZY; ++w) {
            wartosciWWierszu.get(w).setValue(Macierz.wartość(w, ID_KOLUMNY));
            zatrzask[w].countDown();
        }

    }


}
