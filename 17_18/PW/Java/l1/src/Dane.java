import java.util.concurrent.Semaphore;

class Dane {

    public final int pojemnoscPoczekalni = 13;
    public int wolneKrzesla = pojemnoscPoczekalni;
    public final int liczbaKlientow = 35;
    public final double szybkoscSymulacji = 20.0;

    public Semaphore poczekalnia = new Semaphore(0);
    public Semaphore fotel = new Semaphore(0);
    public Semaphore fryzjer = new Semaphore(0);
    public Semaphore klienci = new Semaphore(0);
    public Semaphore mutex = new Semaphore(1);

    public long wykonaj(int minimalnyCzas, int margines) {
        return (long)( ( minimalnyCzas + ( margines * Math.random() ) ) * (1 / szybkoscSymulacji));
    }

    private static Dane instance = null;

    protected Dane() {
        // Exists only to defeat instantiation.
    }
    public static Dane getInstance() {
        if(instance == null) {
            instance = new Dane();
        }
        return instance;
    }

}
