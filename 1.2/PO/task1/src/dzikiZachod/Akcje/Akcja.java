package dzikiZachod.Akcje;

/**
 * **  **   *   *   ***   *   *   **  **
 * *                                   *
 * *    Created by Michał Balcerzak    *
 * *    Copyright by Wydział MIM UW    *
 * *                                   *
 * **  **   *   *   ***   *   *   **  **
 **/


public enum Akcja {
    ULECZ,
    STRZEL,
    ZASIEG_PLUS_JEDEN,
    ZASIEG_PLUS_DWA,
    DYNAMIT;


    @Override
    public String toString () {
        if (this == ULECZ) return "ULECZ";
        else if (this == STRZEL) return "STRZEL";
        else if (this == ZASIEG_PLUS_JEDEN) return "ZASIEG_PLUS_JEDEN";
        else if (this == ZASIEG_PLUS_DWA) return "ZASIEG_PLUS_DWA";
        else return "DYNAMIT";
    }
}
