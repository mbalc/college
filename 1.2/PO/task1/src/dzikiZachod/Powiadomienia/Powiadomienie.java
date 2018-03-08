package dzikiZachod.Powiadomienia;

/**
 * **  **   *   *   ***   *   *   **  **
 * *                                   *
 * *    Created by Michał Balcerzak    *
 * *    Copyright by Wydział MIM UW    *
 * *                                   *
 * **  **   *   *   ***   *   *   **  **
 **/


public class Powiadomienie extends Throwable {

    private static final int argumentAtrapa = -1;

    private final Wydarzenie rodzajPowiadomienia;

    private final int wykonawca; //lub numer tury!
    private final int odbiorca;

    public Powiadomienie (Wydarzenie w, int a, int b) {
        rodzajPowiadomienia = w;
        wykonawca = a;
        odbiorca = b;
    }

    public Powiadomienie (Wydarzenie w, int a) {
        this(w, a, argumentAtrapa);
    }

    public Powiadomienie (Wydarzenie w) {
        this(w, argumentAtrapa);
    }

    public Wydarzenie getRodzajPowiadomienia () {
        return rodzajPowiadomienia;
    }

    public int getWykonawca () {
        return wykonawca;
    }

    public int getOdbiorca () {
        return odbiorca;
    }
}
