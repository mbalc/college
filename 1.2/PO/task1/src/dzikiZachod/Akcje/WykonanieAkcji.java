package dzikiZachod.Akcje;

/**
 * **  **   *   *   ***   *   *   **  **
 * *                                   *
 * *    Created by Michał Balcerzak    *
 * *    Copyright by Wydział MIM UW    *
 * *                                   *
 * **  **   *   *   ***   *   *   **  **
 **/


public class WykonanieAkcji {

    private final Akcja akcja;
    private final int cel;

    public WykonanieAkcji (Akcja akcja, int cel) {
        this.akcja = akcja;
        this.cel = cel;
    }

    public Akcja getAkcja () {
        return akcja;
    }

    public int getCel () {
        return cel;
    }
}
