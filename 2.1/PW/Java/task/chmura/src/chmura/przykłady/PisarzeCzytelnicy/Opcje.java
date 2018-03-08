package chmura.przykłady.PisarzeCzytelnicy;

class Opcje {
    static final int iluCzytelnikow = 8;
    static final int iluPisarzy = 3;

    // zakresy czas w milisekundach, jednorodne prawdopodobieństwo dla czasów z zakresu
    static final int[] czytelnikWlasneSprawy = new int[]{2000, 3000};
    static final int[] czytelnikCzytanie = new int[]{2000, 3000};
    static final int[] pisarzWlasneSprawy = new int[]{4000, 5000};
    static final int[] pisarzPisanie = new int[]{4000, 5000};
    static final double mnożnikCzasu = 1.0;
}
