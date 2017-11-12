package zadania;

public class zadanie2 {
    public static void main(String[] args) {
        final Integer ileGrup = 10;
        final Integer ileZasobów = 4;
        Serwer test = new Serwer(ileGrup, ileZasobów);

        for (int i = 0; i < 29; ++i) {
            final Integer blah = i / 3;
            new Thread(new Zadanie(blah, test)).start();
        }
    }
}
