class Klient implements Runnable {

    final int index;
    boolean zniesmaczony = false;

    private int zycieCodzienne = 3000;
    private int zycieCodzienneMargines = 100000;

    private Dane dane() {
        return Dane.getInstance();
    }


    public Klient(int index) {

        this.index = index;
        System.out.println("Klient " + index + " żyje własnym życiem");
        new Thread(this, "Klient" + index).start();

    }

    public int getIndex() {
        return index;
    }

    @Override
    public void run() {
        while (true) {
            try {
                Thread.sleep(dane().wykonaj(zycieCodzienne, zycieCodzienneMargines));
            } catch (InterruptedException e) {
                System.out.println("Przerwano!");
            }

            try {
                System.out.println("Klient " + index + " odwiedza fryzjera");
                dane().mutex.acquire();
                if (dane().wolneKrzesla == 0) {
                    System.out.println("Klient " + index + " zniesmaczony brakiem wolnych krzeseł opuszcza zakład");
                    dane().mutex.release();
                }
                else {
                    --dane().wolneKrzesla;
                    System.out.println("Klient nr " + index + " siada w poczekalni           (pozostałe krzesła: " + dane().wolneKrzesla);
                    dane().klienci.release();
                    dane().mutex.release();
                    dane().fryzjer.acquire();
                    System.out.println("Klient nr " + index + " siada na fotelu");
                    dane().fotel.acquire();
                    System.out.println("Klient nr " + index + " opuszcza zakład");
                }
            } catch (InterruptedException e) {
                System.out.println("Przerwano!");
            }
        }

    }
}
