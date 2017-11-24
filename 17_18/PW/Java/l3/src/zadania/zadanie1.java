package zadania;

public class zadanie1 {
    public static void main(String[] args) {
        Worek<Integer> test = new ConcurrentWorek<>();

        for (int i = 0; i < 10; ++i) {
            final Integer blah = i % 3;
            new Thread(() -> {
                try {
                    for (int j = 0; j < 5; ++j) {
                        test.włóż(blah);
                        System.out.println("włóż " + blah);
                        Thread.sleep(1000 + (20*j) + (2 * blah));
                    }
                }
                catch (InterruptedException e) {
                    System.out.println("Przerwano");
                }
            }).start();

            new Thread(() -> {
                try {
                    for (int j = 0; j < 10; ++j) {
                        test.wyjmij(blah);
                        System.out.println("wyjmij " + blah);
                        Thread.sleep(200 + (2*j) + (20 * blah));
                    }
                }
                catch (InterruptedException e) {
                    System.out.println("Przerwano");
                }
            }).start();
        }
    }
}
