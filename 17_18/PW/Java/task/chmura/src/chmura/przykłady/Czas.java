package chmura.przykłady;

import java.util.Random;

class Czas {

    private static Random rand = new Random();

    static long losowyCzas(int[] zakres, double mnożnikCzasu) {
        int roznica = zakres[1] - zakres[0] + 1;
        int los = rand.nextInt(roznica) + zakres[0];
        return (long)(los * mnożnikCzasu);
    }


}
