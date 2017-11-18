package zadania;

import java.util.Random;

class Zadanie implements Runnable {
    private final int grupa;
    private final Serwer serwer;

    private void własneSprawy() throws InterruptedException {
        System.out.printf("Wątek z grupy nr %02d zajmuje się sobą\n", grupa);
        Thread.sleep(new Random().nextInt(600));
        System.out.printf("Wątek z grupy nr %02d pyta serwera o zasób\n", grupa);
    }

    private void korzystam(int zasób) throws InterruptedException {
        System.out.printf("Wątek z grupy nr %02d zaczyna korzystać z zasobu %02d\n", grupa, zasób);
        Thread.sleep(new Random().nextInt(2000));
        System.out.printf("Wątek z grupy nr %02d kończy pracować na zasobie %02d\n", grupa, zasób);
    }

    public Zadanie (final int grupa, final Serwer serwer) {
        this.grupa = grupa;
        this.serwer = serwer;
    }

    @Override
    public void run() {
        try {
            while (true) {
                własneSprawy();
                int zasób = serwer.chcęKorzystać(grupa);
                korzystam(zasób);
                serwer.skończyłem(grupa, zasób);
            }
        }
        catch (InterruptedException e) {
            System.out.println("Przerwano");
        }
    }

}