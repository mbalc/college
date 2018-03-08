class Fryzjer implements Runnable {


    private int strzyzenie = 300;
    private int strzyzenieMargines = 4000;

    private Dane dane() {
        return Dane.getInstance();
    }

    public Fryzjer() {
        new Thread(this, "Fryzjer").start();
    }

    @Override
    public void run() {
        while (true) {
            try {
                System.out.println("Fryzjer czeka na klienta");
                dane().klienci.acquire();
                dane().mutex.acquire();
                ++dane().wolneKrzesla;
                System.out.println("Fryzjer bierze nowego klienta             (pozostałe krzesła: " + dane().wolneKrzesla);
                dane().fryzjer.release();
                dane().mutex.release();
                System.out.println("Fryzjer rozpoczyna strzyżenie");
                Thread.sleep(dane().wykonaj(strzyzenie, strzyzenieMargines));
                System.out.println("Fryzjer kończy strzyc klienta");
                dane().fotel.release();
            } catch (InterruptedException e) {
                System.out.println("Przerwano!");
            }
        }


    }
}
